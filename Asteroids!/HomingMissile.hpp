//
//  HomingMissile.hpp
//  asteroids
//
//  Created by Doug on 9/21/17.
//

#ifndef HomingMissile_hpp
#define HomingMissile_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "asteroid.hpp"
#include "ship.hpp"

class Missile{
private:
    // Constructor
    Missile();
    Missile(sf::Vector2f position, asteroid &target);
    
    // Member variables
    sf::ConvexShape shape;
    float speed;
    asteroid* pTargetAsteroid;
    
    //helper method
    sf::ConvexShape buildFrame(sf::Vector2f position);

    
public:
    static void makeMissiles(std::vector<Missile> &missiles, const ship &playerShip, std::vector<asteroid> &asteroids);
};





#endif /* HomingMissile_hpp */
