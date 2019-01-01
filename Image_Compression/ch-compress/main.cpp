//
//  main.cpp
//  ch-compress
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

//PROGRAM TAKES A FILENAME FROM THE COMMAND LINE AND WRITES TO A FILE THE RLE/HUFF ENCODING (COMPRESSED FILE)
//argv[0] program name (ch-compress)
int main(int argc, const char * argv[]) {
    if (argv[3] != NULL) {
        string debug = argv[3];
        if(debug == "true") {
            DEBUG_MODE = true;
        }
    }
    
    string filename(argv[1]); //first file = argv[1]
    tuple<bool, int, int, vector<uint8_t> *> huffTuple = huff(filename);
    if(get<0>(huffTuple) == false) {
        cout << "Huff was unsuccessful in ch-compress.\n";
        return 0;
        
    }
    int width = get<1>(huffTuple);
    int height = get<2>(huffTuple);
    ofstream fileOut;
    fileOut.open(argv[2], ios::binary); //write to 2nd file (argv[2])
    fileOut << 'c' << 'h'; //magic # (2 bytes)
    
    fileOut << (unsigned char)(width & 0xFF); //width = get<1>(huffTuple)
    fileOut << (unsigned char)(width >> 8 & 0xFF);
    fileOut << (unsigned char)(width >> 16 & 0xFF);
    fileOut << (unsigned char)(width >> 24 & 0xFF);
    
    fileOut << (unsigned char)(height & 0xFF); //height = get<2>(huffTuple)
    fileOut << (unsigned char)(height >> 8 & 0xFF);
    fileOut << (unsigned char)(height >> 16 & 0xFF);
    fileOut << (unsigned char)(height >> 24 & 0xFF);

    vector<uint8_t> *runs = get<3>(huffTuple);
    if(DEBUG_MODE) { //printing for debugging
        cout << "Vector size in ch-compress: " << runs->size() << '\n';
    }
    int counter = 0;
    int HtimesW = height * width;
    if (DEBUG_MODE) { //printing for debugging
        cout << "Width: " << width << " Height: " << height << '\n';
        cout << "HtimesW: " << HtimesW << '\n';
    }
    for(auto run : *runs) {
        if (counter > HtimesW) {
            fileOut.close();
            break;
        }
        fileOut << run;
        if(DEBUG_MODE) { //printing for debugging
            bitset<8>runBits = run;
            cout << "Huff in ch-compress: " << runBits << '\n';
        }
        counter++;
    }
    fileOut.close();
    return 0;
}
