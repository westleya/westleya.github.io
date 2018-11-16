//
//  asteroid.cpp
//  asteroids
//
//  Created by Westley Kirkham on 9/19/17.
//
//

#include "asteroid.hpp"
#include "ship.hpp"
#include <cmath>
#include <stdlib.h>
#include <time.h> // uses this for the seed for the random number
#define PI 3.14159265

asteroid::asteroid(){
    
}

// Constructs an asteroid of size s (size can be 1, 2, or 3)
asteroid::asteroid(int lvl, sf::Vector2f position, int dir, float sp){
    level = lvl;
    speed = sp;
    
    if (level == 3)
        size = 60;
    else if (level == 2)
        size = 40;
    else
        size = 20;

    buildFrame(position.x, position.y, dir);
}

// Creates the frame/shape of an astroid
void asteroid::buildFrame(int xPos, int yPos, int dir){
    // Initialize the rock
    circle = sf::CircleShape();
    
    // Set the direction of the rock
    circle.setRotation(dir);
    
    // Set the position of the rock
    circle.setPosition(xPos, yPos);

    // Set the size of the rock
    circle.setRadius(size);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(2);
    circle.setOrigin(size/2, size/2);
    
}

// Updates the asteroid's position
void asteroid::updatePosition(int w, int h){
    
    // Keeps the asteroid within the bounds of the game/screen
    if (getPosition().x >= 1600)
    {
        circle.setPosition(0, getPosition().y);
    }
    else if (getPosition().x <= 0)
    {
        circle.setPosition(w - 1, getPosition().y);
    }
    if (getPosition().y >= 1200)
    {
        circle.setPosition(getPosition().x, 0);
    }
    else if (getPosition().y <= 0)
    {
        circle.setPosition(getPosition().x, h - 1);
    }
    
    float move_x = -cos(getRotation() * PI / 180 ) * speed;
    float move_y = sin(getRotation() * PI / 180 ) * speed;
    circle.move(move_x, move_y);
}

// Populates vector of asteroids with asteroids
void asteroid::makeAsteroids(std::vector<asteroid>& asteroids, int game_lvl, int ast_lvl, sf::Vector2f startPos){
    for (int i = 0; i < 2; i++){
        int direction = rand() % 359 + 1;
        float speed_mod = (rand() % 200) / 115.0 + .3;
        asteroids.push_back(asteroid(ast_lvl, startPos, direction, 0.25 * speed_mod));
    }
}

// Populates vector of asteroids with asteroids
void asteroid::makeAsteroids(std::vector<asteroid>& asteroids, int game_lvl, int height, int width, sf::Vector2f shipPos)
{
    for (int i = 0; i < 2 + game_lvl; i++){
        int direction = rand() % 359 + 1;
        sf::Vector2f  startPos = asteroidStartPosition(height, width, shipPos);
        float speed_mod = (rand() % 200) / 115.0 + .3;
        asteroids.push_back(asteroid(3, startPos, direction, 0.25 * speed_mod));
    }
}




/**
 * HELPER METHOD - RANDOM ASTEROID STARTING POSITION
 * Gets two random points within the game window. These two points will be at least
 * 400 pixels away from the position of the ship. This is to prevent an unfair scenario
 * where asteroids spawn so close to the ship that there is no fair opportunity to avoid them.
 */
sf::Vector2f asteroid::asteroidStartPosition(int width, int height, sf::Vector2f shipPos){
    
    // (x, y) values within world dimensions, but 200px away from ship:
    int xPos = 0;
    int yPos = 0;
    int distance = 0;
    do{
        xPos = rand() % width;
        yPos = rand() % height;
        
        // Distance formula to calculate distance between this coordinate and the ship coordinate
        distance = sqrt(pow((xPos - shipPos.x), 2) + pow((yPos - shipPos.y), 2));
        
    } while (distance <= 200);
    
    sf::Vector2f startPosition(xPos, yPos);
    return startPosition;
}



// Draws an astroid to the window
void asteroid::drawAsteroid(sf::RenderWindow &window){
    window.draw(circle);
}

// Gets the asteroid's size
int asteroid::getSize(){
    return size;
}

sf::Vector2f asteroid::getPosition(){
    return circle.getPosition();
}

int asteroid::getRotation()
{
    return circle.getRotation();
}

int asteroid::getRadius()
{
    return circle.getRadius();
}

int asteroid::getLevel(){
    return level;
}
