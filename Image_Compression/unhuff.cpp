//
//  unhuff.cpp
//
//
//  Created by Gregory Colledge on 1/27/18.
//
// ------------------- T E A M   R E L A S E ---------------------
#include "huff.h"
#include "pbmcompress-v1.h"
#include <assert.h>
#include <iostream>
#include <map>
#include <random> //for testing
#include <stdio.h>
#include <tuple>
#include <vector>

//#include
//"/Users/gregorycolledge/gcolledge/gcolledge/MSD_6015/cs6015-team2/ImageMainFunctions/pbmcompress-v1.h"
//#include
//"/Users/gregorycolledge/gcolledge/gcolledge/MSD_6015/cs6015-team2/ImageMainFunctions/huff.h"

/*
 * --------buildString Function-----------
 * build String goes through the ch data stream and builds string based on the
 * bits recieved. The string can be checked against the table.
 */
static bool buildString(std::vector<uint8_t> *ch_data, long i, std::string &rep,
                        bool &status) {
  uint8_t block;
  uint8_t bit;
  block = ch_data->at(i / 8); // the block is the current byte
  uint8_t mask = 0x01;
  bit = block >> (7 - (i % 8)) &
        mask; /*bit is the current bit of interest. This reads
               * the bits left to right. so 10101010 (as
               *printed byt the cout << rep line in the
               * fillVector function below) would read the 1
               * first, then ta 0 then a one and so on.
               */
  if (bit == 0) {
    rep += "0";
  } else if (bit == 1) {
    rep += "1";
  } else {
    std::cerr << "problem with bit read.\n";
    status = false;
    return false;
  }
  return true;
}

/*
 * --------FillVector Function-----------
 * This fills the return vector with the bytes according to the huff_table.
 */
void fillVector(std::vector<uint8_t> *RL_data, std::vector<uint8_t> *ch_data,
                std::map<std::string, uint8_t> table, bool status) {

  std::string rep = ""; // string representation
  for (long i = 0; i < ch_data->size() * 8; i++) {
    bool contLoop = buildString(ch_data, i, rep, status);
    if (!contLoop) {
      break;
    }
    if(DEBUG_MODE){
        std::cout << "rep: " << rep << "\n";//FEC
    }
    assert(rep.size() <
           256); // It is impossible to have a huff string bigger than 8 bytes.
    // check string against table
    auto found = table.find(rep);
    if (found != table.end()) { // the character has been found

      RL_data->push_back(found->second);
      if(DEBUG_MODE){
            std::cout << "Pushed " << found->second << " onto return data.\n";//FEC
      }
      rep = ""; // start new string representation
    }
  }
}

/*
 * --------TestVec Function-----------
 * This is a function to help test the unhuff function. It uses made up values
 * The vector pointer that is returned MUST be deleted at end of use!
 * THIS IS FOR TESTING ONLY!!!
 */
//std::vector<uint8_t> *testVec(int vals) {
//  std::vector<uint8_t> *result = new std::vector<uint8_t>{0x43};
//  for (int i = 0; i < vals; i++) {
//    uint8_t next = rand() % 256;
//    result->push_back(next);
//  }
//  return result;
//}

static std::map<std::string, uint8_t> prepTable() {
  std::map<std::string, uint8_t> table;
  for (int i = 0; i < 256;
       i++) { // 256 was chosen because of the array size. TODO: Check that this
              // is correct in all cases!!!
    table.insert(std::pair<std::string, uint8_t>(huff_table[i], i));
  }
  return table;
}

/*
 * This is the main Unhuff Function
 */
std::tuple<bool, int, int, std::vector<uint8_t> *>
unhuff(std::string filename) {

  if(DEBUG_MODE){
    std::cout << "-------------BEGIN UNHUFF------------\n";
  }
  // get data

//  std::tuple<bool, int, int, std::vector<uint8_t> *> input =
//      std::make_tuple(true, 4, 4, pt); // this will be the read_ch_file function
  
  std::tuple<bool, int, int, std::vector<uint8_t> *> input =
  read_ch_file(filename); // USE THIS FOR TEAM TESTING
  
  // unpack input
  bool status = std::get<0>(input);
  // check status
  if (status == false) {
    std::cerr << "Data came to unhuff with an error.\n";
    return input;
  }
  int columns = std::get<1>(input);
  int rows = std::get<2>(input);
  std::vector<uint8_t> *ch_data = std::get<3>(input);
  // Prepare the table
  std::map<std::string, uint8_t> table = prepTable();
  // create and fill return vector with the RunLength data.
  assert(status == true); // don't continue the computations if status is bad
  std::vector<uint8_t> *RL_data =
      new std::vector<uint8_t>; // RunLength data for returning.
  fillVector(RL_data, ch_data, table, status);

  delete ch_data;
  ch_data = NULL;
  std::tuple<bool, int, int, std::vector<uint8_t> *> result = {status, columns,
                                                               rows, RL_data};
  if(DEBUG_MODE){
    for (int i = 0; i < RL_data->size(); i++) {
      std::cout << std::bitset<8>(RL_data->at(i)) << "\n";
    }
    std::cout << "-------------END UNHUFF------------\n";
  }

  return result;
}

///* ----------------- MAIN: ONLY FOR TESTING MY unhuff as i build it
///----------------- */
// int main(int argc, const char * argv[]) {
//  // insert code here...
//  std::cout << "Hello, unhuff!\n";
////  std::tuple<bool, int, int, std::vector<uint8_t> *> testResult =
///unhuff("test.txt"); /  std::vector<uint8_t> * data = std::get<3>(testResult);
////  std::cout << "\nThis is the pointer" << data;
//  unhuff("test.txt");
//  return 0;
//}
