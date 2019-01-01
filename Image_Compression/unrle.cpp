//
//  unrle.cpp
//  ImageMainFunctions
//
//Work done by Mason West
//

/* This method calls the unhuff method and uses the tuple that is output to
 * uncompress the data back to an vector of bools that is then returned in a new tuple
 */

#include "pbmcompress-v1.h"

#include <iostream>
#include <vector>
#include <stdio.h>

/*
 * This is a helper method called in teh readByte() method. It takes a bit, a number of times to repeat the bit,
 * and a pointer to a vector in which it should write the data.
 */
void writeByte(bool bit, int repeat, std::vector<bool> *vector) {
  //TED TWEAK: resize vector first, use operator[], faster than push_back
  int baseVectSize = vector->size();
    repeat += baseVectSize;
  vector->resize(repeat);
  for (int i = baseVectSize; i < vector->size(); ++i) {
    vector->at(i) = bit;
  }
}


/*
 * This is a helper method used in unrle(). This method also calls the writeByte() helper method to write
 * the data. This method shifts the byte to right by 7 to get the single bit representing the repeated bool
 * and masks with 0111 1111 to return how many times that value needs to be repeated.
 */
void readByte(uint8_t byte, std::vector<bool> *vector) {
    //shift right 7 to get the type
    int value = byte >> 7;
    //mask with 01111111 to get number 0x7F hex
    int numberOfTimes = (byte & 0x7f);
    //write byte
    writeByte(value, numberOfTimes, vector);
//    return vector;
}

/*
 * unrle calls the unhuff to get a tuple with a vector of uint8's to decompress.
 * This method calls readByte() and returns a vector of bools.
 */
std::tuple<bool, int, int, std::vector<bool> *> unrle(std::string filename) {
    if(DEBUG_MODE == true) {
        std::cout << "*** UNRLE STARTING ***\n";
    }
    //variables to set with data from the tuple
    bool success;
    int width, height;
    std::vector<uint8_t> *unHuffVect;
    
    //call unhuff to get data to work with and unpack the data with tie
    std::tie(success, width, height, unHuffVect) = unhuff(filename);
    
    //error handling for if unhuff failed, aka bool in tuple is false
    if(!success) {
        if(DEBUG_MODE == true) {
            std::cout << "*** UNRLE Had Failure Bool Passed Via Tuple ***\n";
        }
        return std::make_tuple(false, 0, 0, (std::vector<bool> *)0);
    }
    
    //vector to write boolean data uncompressed from unhuff
    auto *imageBoolsVect = new std::vector<bool>;
    
    //loop through the unhuff data vector to pull each byte
    for(int i = 0; i < unHuffVect->size(); i++) {
        readByte(unHuffVect->at(i), imageBoolsVect);
    }
    //make tuple to return from the method
    delete(unHuffVect);
    if(DEBUG_MODE == true) {
        std::cout << "*** UNRLE ENDING ***\n";
        for (auto bools: *imageBoolsVect) {
            std::cout << bools;
        }
    }
    
    
    return std::make_tuple(true, width, height, imageBoolsVect);;
}
