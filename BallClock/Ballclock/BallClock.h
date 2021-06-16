//
//  BallClock.h
//  Ballclock
//
//  Created by Westley Kirkham on 6/15/21.
//

#ifndef BallClock_h
#define BallClock_h
#include <queue>
using namespace std;

 
void startBallClock(int);

struct BallClock{
    int HalfDaysPast = 0;
    
    struct Ball{
        int IdentificationNumber;
    } ball;
    
    struct Minutes{
        int Max = 4;
        deque<Ball> queue; //used for push and pop capabilities
    } minutes;

    struct Fives{
        int Max = 11;
        deque<Ball> queue;
    } fives;

    struct Hours{
        int Max = 11;
        deque<Ball> queue;
    } hours;

    struct Unused{
        deque<Ball> queue;
    } unused;

    struct FirstOrder{
        deque<Ball> queue;
    } firstOrder;

    BallClock(int numberOfBalls){
        
        for(int i = 1; i <= numberOfBalls; i++){
            
            //Create Ball
            Ball CurrentBall;
            CurrentBall.IdentificationNumber = i;

            //Add to queues
            unused.queue.push_back(CurrentBall);
            firstOrder.queue.push_back(CurrentBall);

        }
        
    };
};
#endif /* BallClock_h */
