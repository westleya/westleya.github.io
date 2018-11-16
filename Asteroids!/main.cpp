
//
//  main.cpp
//  asteroids
//
//  Created by Westley Kirkham and Doug Garding on 9/18/17.
//
//
#include "world.hpp"
#include "SFML/Graphics.hpp"
#include "ship.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include "bullets.hpp"


int main(int argc, const char * argv[]){
    
    world game(1600, 1200);
    game.runWorld();
    
}
