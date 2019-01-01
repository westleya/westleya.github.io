//
//  read-files.h
//  ImageMainFunctions
//
//  Created by Sydney Sturgeon on 1/30/18.
//  Copyright © 2018 Sydney Sturgeon. All rights reserved.
//

#ifndef read_files_h
#define read_files_h

/*
 * PBM file format: http://netpbm.sourceforge.net/doc/pbm.html
 *
 * must return false if the PBM file is invalid or cannot be read, and
 * true otherwise
 */
std::tuple<bool, int, int, std::vector<bool> *> read_pbm_file(std::string filename);

std::tuple<bool, int, int, std::vector<uint8_t> *> read_ch_file(std::string filename);

#endif /* read_files_h */
