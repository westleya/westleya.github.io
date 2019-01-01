//
//  main.cpp
//  dump-runs
//
//  Created by Sydney Sturgeon on 1/30/18.
//  Copyright © 2018 Sydney Sturgeon. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <bitset>

#include "../pbmcompress-v1.h"

bool DEBUG_MODE = false;

using namespace std;

//PROGRAM TAKES A FILENAME FROM THE COMMAND LINE AND WRITES TO A FILE THE RLE RUNS
//argv[0] program name (dump-runs)
int main(int argc, const char * argv[]) {
    if (argv[3] != NULL) {
        string debug = argv[3];
        if(debug == "true") {
            DEBUG_MODE = true;
        }
    }

    string filename(argv[1]); //argv[1] = input file
    tuple<bool, int, int, vector<uint8_t> *> rleTuple = rle(filename);
    if(DEBUG_MODE) { //printing for debugging
        if(get<0>(rleTuple) == false) {
            cout << "RLE was unsuccessful in dump-runs.\n";
            return 0;
        } else {
            cout << "RLE was successful in dump-runs.\n";
        }
    }
    
    ofstream fileOut;
    fileOut.open(argv[2], ios::binary); //write to 2nd file (argv[2])
    fileOut << 'c' << 'h'; //magic # (2 bytes)
    
    fileOut << (unsigned char)(get<1>(rleTuple) & 0xFF); //width = get<1>(huffTuple)
    fileOut << (unsigned char)(get<1>(rleTuple) >> 8 & 0xFF);
    fileOut << (unsigned char)(get<1>(rleTuple) >> 16 & 0xFF);
    fileOut << (unsigned char)(get<1>(rleTuple) >> 24 & 0xFF);
    
    fileOut << (unsigned char)(get<2>(rleTuple) & 0xFF); //height = get<2>(huffTuple)
    fileOut << (unsigned char)(get<2>(rleTuple) >> 8 & 0xFF);
    fileOut << (unsigned char)(get<2>(rleTuple) >> 16 & 0xFF);
    fileOut << (unsigned char)(get<2>(rleTuple) >> 24 & 0xFF);
    
    vector<uint8_t> *runs = get<3>(rleTuple);
    if (DEBUG_MODE) { //printing for debugging
        cout << "Vector size in dump-runs: " << runs->size() << endl;
    }
    int counter = 0;
    int HtimesW = get<1>(rleTuple) * get<2>(rleTuple);
    if(DEBUG_MODE) { //printing for debugging
        cout << "Width: " << get<1>(rleTuple) << " Height: " << get<2>(rleTuple) << endl;
        cout << "HtimesW: " << HtimesW << endl;
    }
    for(auto run : *runs) {
        if (counter == HtimesW) {
            fileOut.close();
            break;
        }
        fileOut << run;
        if(DEBUG_MODE) { //printing for debugging
            bitset<8>runBits = run;
            cout << "Runs in dump-runs: " << runBits << endl;
        }
        counter++;
    }
    fileOut.close();
    return 0;
}
