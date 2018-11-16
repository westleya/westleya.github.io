//
//  powerUp.cpp
//  asteroids
//
//  Created by Doug on 9/21/17.
//


#include "PowerUp.hpp"
#include "world.hpp"


// Constructor
PowerUp::PowerUp(std::string power){
    type = power;
    shape = buildShape(type);
    isOnField = true;
    
    // Set the amount of time the power up will last (seconds)
    if(type == "shield"){
        timeActive = 10.0f;
    }
    else if(type == "1up"){
        timeActive = 0.0f;
    }
    else if(type=="spray"){
        timeActive = 10.0f;
    }
    else if(type=="missile"){
        timeActive = 10.0f;
    }
    
}


sf::ConvexShape PowerUp::buildShape(std::string type){
    
    sf::ConvexShape shape;
    
    if (type == "shield"){
        shape.setPosition(PowerUpPosition());
        shape.setPointCount(3);
        shape.setPoint(0, sf::Vector2f(0, 0));
        shape.setPoint(1, sf::Vector2f(15, 50));
        shape.setPoint(2, sf::Vector2f(-15, 50));
        shape.setOrigin(0, 25);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(4);
        shape.setOutlineColor(sf::Color(0, 178, 248));
    }
    
    if (type == "1up"){
        shape.setPosition(PowerUpPosition());
        shape.setPointCount(7);
        shape.setPoint(0, sf::Vector2f(0, 0));
        shape.setPoint(1, sf::Vector2f(15, 20));
        shape.setPoint(2, sf::Vector2f(5, 20));
        shape.setPoint(3, sf::Vector2f(5, 40));
        shape.setPoint(4, sf::Vector2f(-5, 40));
        shape.setPoint(5, sf::Vector2f(-5, 20));
        shape.setPoint(6, sf::Vector2f(-15, 20));
        shape.setOrigin(0, 25);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(4);
        shape.setOutlineColor(sf::Color(0, 246, 142));
    }
    
    if (type == "spray"){
        shape.setPosition(PowerUpPosition());
        shape.setPointCount(12);
        shape.setPoint(0, sf::Vector2f(0, 0));
        shape.setPoint(1, sf::Vector2f(3, 5));
        shape.setPoint(2, sf::Vector2f(6, 15));
        shape.setPoint(3, sf::Vector2f(8, 25));
        shape.setPoint(4, sf::Vector2f(8, 50));
        shape.setPoint(5, sf::Vector2f(-8, 50));
        shape.setPoint(6, sf::Vector2f(-8, 40));
        shape.setPoint(7, sf::Vector2f(8, 40));
        shape.setPoint(8, sf::Vector2f(-8, 40));
        shape.setPoint(9, sf::Vector2f(-8, 25));
        shape.setPoint(10, sf::Vector2f(-6, 15));
        shape.setPoint(11, sf::Vector2f(-3, 5));
        shape.setOrigin(0, 25);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(4);
        shape.setOutlineColor(sf::Color(225, 111, 127));
    }
    
    if (type == "missile"){
        shape.setPosition(PowerUpPosition());
        shape.setPointCount(12);
        shape.setPoint(0, sf::Vector2f(0, 0));
        shape.setPoint(1, sf::Vector2f(3, 5));
        shape.setPoint(2, sf::Vector2f(6, 15));
        shape.setPoint(3, sf::Vector2f(8, 25));
        shape.setPoint(4, sf::Vector2f(8, 50));
        shape.setPoint(5, sf::Vector2f(-8, 50));
        shape.setPoint(6, sf::Vector2f(-8, 40));
        shape.setPoint(7, sf::Vector2f(8, 40));
        shape.setPoint(8, sf::Vector2f(-8, 40));
        shape.setPoint(9, sf::Vector2f(-8, 25));
        shape.setPoint(10, sf::Vector2f(-6, 15));
        shape.setPoint(11, sf::Vector2f(-3, 5));
        shape.setOrigin(0, 25);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(4);
        shape.setOutlineColor(sf::Color(209, 153, 237));
    }
    
    return shape;
}

// Adds a power up to an array of power ups
void PowerUp::addPowerUp(std::vector<PowerUp> &list){
    
    int powerUp = rand() % 3;
    
    if(powerUp == 2){
        list.push_back(PowerUp("shield"));
    }
    else if(powerUp == 1){
        list.push_back(PowerUp("1up"));
    }
    else if(powerUp == 0){
        list.push_back(PowerUp("spray"));
    }
    
    
}

// Draws a power up to the window
void PowerUp::drawPowerUp(sf::RenderWindow &window){
    window.draw(shape);
}

// Returns the bounds of the object for collisions
sf::FloatRect PowerUp::getBounds(){
    return shape.getGlobalBounds();
}

// Gets the powerup type
std::string PowerUp::getPowerType() const{
    return type;
}

bool PowerUp::getIsOnField(){
    return isOnField;
}
void PowerUp::setIsOnField(bool b){
    isOnField = b;
}
sf::Time PowerUp::getStartTime(){
    return startTime;
}
void PowerUp::setStartTime(sf::Time t){
    startTime = t;
}
float PowerUp::getTimeActive(){
    return timeActive;
}



// Generates random starting point on map for the power up
sf::Vector2f PowerUp::PowerUpPosition(){
    
    int xPos = rand() % world::frameWidth;
    int yPos = rand() % world::frameHeight;
    
    sf::Vector2f startPosition(xPos, yPos);
    return startPosition;
}



/*
 COLORS:
 RED: (225, 111, 127)
 BLUE: (0, 178, 248)
 GREEN: (0, 246, 142)
 YELLOW: (250, 222, 137)
 PURPLE: (209, 153, 237)
 */
