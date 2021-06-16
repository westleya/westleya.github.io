//
//  BallClock.cpp
//  Ballclock
//
//  Created by Westley Kirkham on 6/15/21.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include "BallClock.h"
using namespace std;

//Empties an active queue (minutes, fives, hours) of all its balls
//and returns them to the inactive (unused) queue
static void returnBallsToUnusedQueue(deque<BallClock::Ball> &queue,
                                     struct BallClock &ballClock) {
    while(!queue.empty()){
        BallClock::Ball ballToReturn = queue.front();
        queue.pop_front();
        ballClock.unused.queue.push_back(ballToReturn);
    }
}

//Compares the balls in the unused and firstorder queues
//to see if the order matches.
static void CheckOrder(struct BallClock &ballClock, bool &ReturnedToOriginalOrder, int numberOfBalls) {
   
    //Desired outcome
    ReturnedToOriginalOrder = true;
    
    for(int i = 0; i < numberOfBalls; i++){
        if(ballClock.unused.queue[i].IdentificationNumber !=
           ballClock.firstOrder.queue[i].IdentificationNumber){
            //Desired ourcome not found.
            ReturnedToOriginalOrder = false;
            break;
        }
    }
}

//Runs the Ball Clock program
static void runBallClock(struct BallClock &ballClock, bool &ReturnedToOriginalOrder, int numberOfBalls) {
    
    //Cascade ball into the clock
    BallClock::Ball currentBall = ballClock.unused.queue.front();
    ballClock.unused.queue.pop_front();
    
    //Check for empty spots
    if((int) ballClock.minutes.queue.size() < ballClock.minutes.Max){
        ballClock.minutes.queue.push_front(currentBall);
    }
    else if ((int) ballClock.fives.queue.size() < ballClock.fives.Max){
        //five minutes have passed, empty the minutes queue
        returnBallsToUnusedQueue(ballClock.minutes.queue, ballClock);
        
        ballClock.fives.queue.push_front(currentBall);
    }
    else if ((int) ballClock.hours.queue.size() < ballClock.hours.Max){
        //an hour has passed, empty the minutes and fives queues
        returnBallsToUnusedQueue(ballClock.minutes.queue, ballClock);
        returnBallsToUnusedQueue(ballClock.fives.queue, ballClock);
        
        ballClock.hours.queue.push_front(currentBall);
    }
    else {
        //Twelve hours have past
        ballClock.HalfDaysPast++;
        
        //All the balls need to return
        //to the unused queue.
        returnBallsToUnusedQueue(ballClock.minutes.queue, ballClock);
        returnBallsToUnusedQueue(ballClock.fives.queue, ballClock);
        returnBallsToUnusedQueue(ballClock.hours.queue, ballClock);
        
        //The cascading ball returns as well
        ballClock.unused.queue.push_back(currentBall);
        
        //This is the time to check the order of the balls in unused
        //since all are present.
        CheckOrder(ballClock, ReturnedToOriginalOrder, numberOfBalls);
    }
}

//Starts the Ball Clock
void startBallClock(int numberOfBalls){
    
    //Create Ball Clock
    struct BallClock ballClock = BallClock(numberOfBalls);
    bool ReturnedToOriginalOrder = false;
    
    //Run Ball Clock
    while(!ReturnedToOriginalOrder){
        runBallClock(ballClock, ReturnedToOriginalOrder, numberOfBalls);
    }
    
    cout << "Number of balls: " << numberOfBalls << "\n";
    cout << "Days Required: " << ballClock.HalfDaysPast / 2 << "\n";
    
}
