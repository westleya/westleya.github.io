//
//  collisions.hpp
//  asteroids
//
//  Created by Westley Kirkham on 9/19/17.
//
//

#ifndef collisions_hpp
#define collisions_hpp

#include <stdio.h>
#include "bullets.hpp"
#include "asteroid.hpp"
#include "ship.hpp"
#include "PowerUp.hpp"

/// Collisions are important for any shooter game.
/// Asteroids is no exception. Bullets can hit
/// asteroids, the ship can hit asteroids or
/// power-ups. Nothing else is taken into consideration.
class collisions{
public:
    // Detects a collision between an Asteroid and a Bullet
    static bool bulletAsteroid(bullet &b, asteroid &a);
    // Detects a collision between an Asteroid and a ship
    static bool shipAsteroid(ship &s, asteroid &a);
    // Detects a collision between a ship and a power up
    static bool shipPowerUp(ship &s, PowerUp &p);
};


#endif /* collisions_hpp */

