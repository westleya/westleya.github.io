//
//  collisions.cpp
//  asteroids
//
//  Created by Westley Kirkham on 9/19/17.
//
//

#include "collisions.hpp"
#include <cmath> // for the power and square root functions



// Check if bullet collided with an asteroid
bool collisions::bulletAsteroid(bullet &b, asteroid &a){
    
    // get the position of the bullet and asteroid
    sf::Vector2f bPosition = b.getPosition();
    sf::Vector2f aPosition = a.getPosition();
    
    // Calculate distance between the two using distance formula
    int distance = sqrt(pow((bPosition.x - aPosition.x), 2) + pow((bPosition.y - aPosition.y), 2));
    
    return (distance <= (a.getRadius() + b.getSize().x));
}


// Check if ship has collided with an asteroid
bool collisions::shipAsteroid(ship &s, asteroid &a){
    
    // get the position of the ship and asteroid
    sf::Vector2f sPosition = s.getPosition();
    sf::Vector2f aPosition = a.getPosition();
    
    // Calculate distance between the two using distance formula
    int distance = sqrt(pow((sPosition.x - aPosition.x), 2) + pow((sPosition.y - aPosition.y), 2));
    
    return (distance <= (a.getRadius() + 20));
}

// Check if ship has collided with a power up
bool collisions::shipPowerUp(ship &ship, PowerUp &powerUp){
    return (powerUp.getBounds().intersects(ship.getBounds()));
}
