//
//  HomingMissile.cpp
//  asteroids
//
//  Created by Doug on 9/21/17.
//

#include "HomingMissile.hpp"

Missile::Missile(sf::Vector2f position, asteroid &target){
    shape = buildFrame(position);
    speed = 0.5;
    pTargetAsteroid = &target;
}


sf::ConvexShape Missile::buildFrame(sf::Vector2f position){
    // Create the spaceship
    sf::ConvexShape missile;
    missile.setPosition(position.x, position.y);
    missile.setPointCount(3);
    missile.setPoint(0, sf::Vector2f(0, 0));
    missile.setPoint(1, sf::Vector2f(15, 50));
    missile.setPoint(2, sf::Vector2f(-15, 50));
    missile.setOrigin(0, 25);
    missile.setFillColor(sf::Color::Transparent);
    missile.setOutlineThickness(3);
    missile.setOutlineColor(sf::Color(209, 153, 237));
    
    return missile;
}

void Missile::makeMissiles(std::vector<Missile> &missiles, const ship &playerShip, std::vector<asteroid> &asteroids)
{
    missiles.push_back(Missile(playerShip.getPosition(), asteroids[0]));
}



