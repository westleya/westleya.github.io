//
//  main.cpp
//  Ballclock
//
//  Created by Westley Kirkham on 6/15/21.
//

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "BallClock.h"
using namespace std;

//./Ballclock ##
// ## can be any number between 27 and 127

int main(int argc, const char * argv[]) {
    
    if (argc < 2 ) {
        cout << "Error! Execution of the form ./a.out ## expected.\n";
        cout << "Did not receive the number of balls with which to operate the clock.\n";
        return 1;
    }
    int numberOfBalls;
    try{
        numberOfBalls = stoi(argv[1]);
    }
    catch(...){
        cerr << "Please provide a number between 27 and 127\n";
        cerr << "for the number of balls used in the Ball Clock.\n";
        return 1;
    }
    
    if(numberOfBalls < 27 || numberOfBalls > 127){
        cout << "Please provide a number between 27 and 127\n";
        cout << "for the number of balls used in the Ball Clock.\n";
        return 1;
    }
    
    //Start keeping track of the program runtime
    auto start = chrono::system_clock::now();
    
    //The actual ball clock program
    startBallClock(numberOfBalls);
    
    //The program has finished running, what is its runtime?
    auto stop = chrono::system_clock::now();
    long elapsed = chrono::duration_cast<chrono::milliseconds>(stop - start).count();
    
    cout << "Program runtime: " << elapsed << " ms\n";
}
