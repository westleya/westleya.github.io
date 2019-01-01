//
//  main.cpp
//  ch-decompress
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

//PROGRAM TAKES A FILENAME FROM THE COMMAND LINE AND WRITES TO A FILE THE UNHUFF/UNRLE (DECOMPRESSED FILE)
//argv[0] program name (ch-decompress)
int main(int argc, const char * argv[]) {
    
    if (argv[3] != NULL) {
        string debug = argv[3];
        if(debug == "true") {
            DEBUG_MODE = true;
        }
    }
    
    string filename(argv[1]); //argv[1] = input file
    cout << filename;
    tuple<bool, int, int, vector<bool> *> unrleTuple = unrle(filename);
    if(!get<0>(unrleTuple)) {
        cout << "Unrle was unsuccessful in ch-decompress.\n";
        return 0;
    }
    
    //getting width and height from tuple
    int width = get<1>(unrleTuple);
    int height = get<2>(unrleTuple);
    
    ofstream fileOut;
    fileOut.open(argv[2], ios::binary); //write to 2nd file (argv[2])
    fileOut << 'P' << '1' << '\n'; //magic # (2 bytes)
    fileOut << width << " " << height << '\n';
    if(DEBUG_MODE) { //printing for debugging
        cout << get<1>(unrleTuple) << " " << get<2>(unrleTuple) << '\n';
    }
    
    //get uncompressed data
    vector<bool> *runs = get<3>(unrleTuple);
    if(DEBUG_MODE) { //printing for debugging
        cout << "Vector size in ch-decompress: " << runs->size() << '\n';
    }
    
    int HtimesW = height * width;
    std::string run = "";
    if(DEBUG_MODE) { //printing for debugging
        cout << "Width: " << width << " Height: " << height << '\n';
        cout << "HtimesW: " << HtimesW << '\n';
    }
    for (int j = 0; j < height; j++) {
        
        for(int i = 0; i < width; i++) {
            
            if(runs->at(i + j * width)) {
                run += '1';
            }
            else {
                run += '0';
            }
            
            if(DEBUG_MODE) { //printing for debugging
                cout << "Runs in ch-decompress: " << run << '\n';
                cout << "COUNTER: " << i << '\n';
            }
        }
        
        run+= '\n';
        if ((j+1) % 100 == 0) {
            fileOut << run;
            run = "";
        }
    }
    fileOut << run;

    delete runs;
    fileOut.close();
    return 0;
}
