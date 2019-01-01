//
//  main.cpp
//  ch-display
//
//  Created by Sydney Sturgeon on 1/30/18.
//  Copyright © 2018 Sydney Sturgeon. All rights reserved.
//

#include <iostream>
#include <string>
#include "CImg.h"

bool DEBUG_MODE = false;

using namespace std;
using namespace cimg_library;

//PROGRAM TAKES A FILENAME FROM THE COMMAND LINE AND DISPLAYS THE IMAGE
//argv[0] program name (ch-display)
int main(int argc, const char * argv[]) {
    if (argv[2] != NULL) {
        string debug = argv[2];
        if(debug == "true") {
            DEBUG_MODE = true;
        }
    }
    string filename = argv[1];
    const char* file = filename.c_str();
    CImg<unsigned char> img(file); //displays the input image file
    img.display(file); //displays the file's name in the display window
    return 0;
}
