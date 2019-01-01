//
//  main.cpp
//  fileReadWriteHuffman
//
//  Created by Makayla Cresswell on 1/29/18.
//  Copyright © 2018 Kayla Cresswell. All rights reserved.

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <tuple>
#include <bitset>
#include <sys/stat.h>

/*
 * PBM file format: http://netpbm.sourceforge.net/doc/pbm.html
 *
 * must return false if the PBM file is invalid or cannot be read, and
 * true otherwise
 */
std::tuple<bool, int, int, std::vector<bool> *> read_pbm_file(std::string filename) {

    bool zero = false, one = true;
    int width = 0, height = 0;

    std::ifstream inFile;
    std::string p1;

    inFile.open(filename);

    //start to read file
    if(!inFile) {
        std::cout << "File not found. " << std::endl;
        return make_tuple(false, width, height, new std::vector<bool>());
    }
    //check file type -- should be magic number
    inFile >> p1;
    if (p1 != "P1") {
        std::cout << "FAIL: Not a valid PBM file. Cannot open :(" << "\n";
        return make_tuple(false, width, height, new std::vector<bool>());
    }

    else {
        inFile >> width >> height;
        int size = height * width;

        //start to read data
        std::vector<bool> *fileContents = new std::vector<bool>(size, 0);
        char characters[width];
        int j = 0;
        while(inFile.good() && j < size) {
            inFile >> characters;
            for (int i = 0; i < width; i++) {
                if ( j == size) {
                    break;
                } else if(characters[i] == '1') {
                    fileContents->at(j) = one;
                    j++;
                } else if(characters[i] == '0') {
                    fileContents->at(j) = zero;
                    j++;
                } else if(characters[i] != '\n' && characters[i] != ' ') {
                    std::cout << "Encountered unexpected character. Cannot process :(" << "\n";
                    return make_tuple(false, width, height, new std::vector<bool>());
                }
            }
        }

        inFile.close();
        return make_tuple(true, width, height, fileContents);
    }
}

std::tuple<bool, int, int, std::vector<uint8_t> *> read_ch_file(std::string filename) {
    /** get the file size, in order to properly initialize the array, and avoid
     * the push back operation.
     */
    struct stat st;
    int size;
    if(stat(filename.c_str(), &st) != 0) { size = 0; }
    size = st.st_size - 10;

    std::ifstream inFile (filename);

    /** get and validate magic number.
     *
     */
    char ch[2];

    inFile.read(ch, 2);
    if (ch[0] != 'c' && ch[1] != 'h') {
        return make_tuple(false, 0, 0, new std::vector<uint8_t>());
    }

    /** read height and width bytes, and assemble final 32 bit int values.
     *
     */
    char width[4];
    inFile.read(width, 4);

    unsigned int widthI = 0b00000000;

    widthI |= ((unsigned char)width[3] << 24);
    widthI |= ((unsigned char)width[2] << 16);
    widthI |= ((unsigned char)width[1] << 8);
    widthI |= ((unsigned char)width[0]);

    char height[4];
    inFile.read(height, 4);

    unsigned int heightI = 0b00000000;
    //added by Greg to read in the int value as 4 bytes in the oorder of an int (little endian)
    heightI |= ((unsigned char)height[3] << 24);
    heightI |= ((unsigned char)height[2] << 16);
    heightI |= ((unsigned char)height[1] << 8);
    heightI |= ((unsigned char)height[0]);

    std::cout << widthI << " ";
    std::cout << heightI << "\n";

    /** read the rest of the file.
     *
     */
    std::vector<uint8_t> *fileContents = new std::vector<uint8_t>(size);
    char character[size];
    inFile.read(character, size);
    int j = 0;
    for(int i = 0; i < size; i++) {
        fileContents->at(i) = character[i];
        j++;
    }
    std::cout << j;
    inFile.close();
    
    return make_tuple(true, widthI, heightI, fileContents);
}
