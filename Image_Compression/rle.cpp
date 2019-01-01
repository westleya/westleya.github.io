
//
//  pbmcompress-v1.cpp
//  pbmCompressor
//
//  Created by William Aldrich on 1/26/18.
//  Copyright © 2018 William Aldrich. All rights reserved.
//

#include "pbmcompress-v1.h"
#include <iostream>

struct rleStruct {
    bool type;
    int count;
};

void addToVector(rleStruct &thisStruct, std::vector<uint8_t> * rleAdd) {
    uint8_t toAdd = 0;
    if (thisStruct.type) {
        //shift 1 over to the far left
        toAdd |= 0x01 << 7;
    }
    //Say what is going on in loop with example or a couple sentences
    while (thisStruct.count > 127) {
        //or it with 127
        toAdd = toAdd | 127;
        rleAdd->push_back(toAdd);
        //subtract 127 from the total
        thisStruct.count -= 128;
        //shift right then left to make sure the first bit is what it should be
        toAdd = toAdd >> 7 << 7;
    }
    toAdd |= thisStruct.count;
    rleAdd->push_back(toAdd);
}

/*
 * a run is 8 bits; the high-order bit is the value and the next 7 bits are the
 * length of the run.
 *
 * IMPORTANT: since runs of length zero are uninteresting, the run is
 * one longer than the value.
 *
 */
std::tuple<bool, int, int, std::vector<uint8_t> *> rle(std::string filename) {
    
    if (DEBUG_MODE) {
        std::cout << "\n----------STARTING RLE FUNCTION----------\n";
    }
    bool status;
    int width, height;
    std::vector<bool> * booleanInfo;
    
    std::tie(status, width, height, booleanInfo) = read_pbm_file(filename);
    
    //if the first bit is false immediately return false
    if (status == false) {
        if (DEBUG_MODE) {
            std::cout << "----------ENDING RLE FUNCTION WITH FALSE BOOLEAN----------\n";
        }
        return std::make_tuple(false, 0, 0, (std::vector<uint8_t>*)0);
    }
    
    std::vector<uint8_t>* rle = new std::vector<uint8_t>;
    
    rleStruct currentValue;
    //get the first boolean value
    currentValue.type = booleanInfo->front();
    //needs to be -1 so when it hits it again count goes to 0 stating there is one there
    currentValue.count = -1;
    
    if (DEBUG_MODE) {
        std::cout << "PRINTING VALUES FROM read_pbm_file()\n";
    }
    
    for(bool value: *booleanInfo) {
        if (DEBUG_MODE) {
            std::cout << value << " ";
        }
        if (value != currentValue.type) {
            addToVector(currentValue, rle);
            currentValue.type = value;
            currentValue.count = 0;
            //continue the loop
            continue;
        }
        currentValue.count++;
    }
    
    //add last value to vector
    addToVector(currentValue, rle);
    
    if (DEBUG_MODE) {
        std::cout << "\nPRINTING VALUES IN RLE END OF FUNCTION\n";
        for (int i = 0; i < rle->size(); i++) {
            uint8_t hey = rle->at(i);
            std::bitset<8> blah = hey;
            std::cout << blah << "\n";
        }
        std::cout << "SIZE OF VECTOR\n" << rle->size();
    }
    
    delete(booleanInfo);
    if (DEBUG_MODE) {
        std::cout << "----------ENDING RLE FUNCTION WITH SUCCESS----------\n";
    }
    return std::make_tuple(status, width, height, rle);
}


