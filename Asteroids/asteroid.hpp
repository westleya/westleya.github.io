//
//  asteroid.hpp
//  asteroids
//
//  Created by Westley Kirkham on 9/19/17.
//
//

#ifndef asteroid_hpp
#define asteroid_hpp

#include <stdio.h>
#include "SFML/Graphics.hpp"

/// The asteroid is made here and their initial position has to be a certain distance from the playerShip.
class asteroid{
private:
    int size;
    int level; /// There are three levels of asteroids, just as there are three sizes.
    float speed; /// The base asteroid speed is constant but we have a rand() operation that changes that somewhat.
    sf::CircleShape circle;
    
    
    /// Creates the frame/shape of an astroid
    void buildFrame(int xPos, int yPos, int dir);
    /// Gets an initial starting position that is at least 50 away from the ship
    sf::Vector2f getStartPosition();
    /// Generates a random starting position
    sf::Vector2f asteroidStartPosition();
    /// Creates a starting position that is X pixels away from the ship
    static sf::Vector2f asteroidStartPosition(int height, int width, sf::Vector2f shipPos);
    
public:
    // asteroid Constructor
    asteroid();
    asteroid(int size, sf::Vector2f position, int direction, float speed);
    /// Draws an astroid to the window
    void drawAsteroid(sf::RenderWindow &window);
    /// Updates the asteroid's position
    void updatePosition(int w, int h);
    /// Gets the asteroid's position
    sf::Vector2f getPosition();
    /// Gets the asteroid's size
    int getSize();
    /// Gets the asteroid's rotation
    int getRotation();
    int getRadius();
    int getLevel();
    
    /// Populates a vector of asteroids
    static void makeAsteroids(std::vector<asteroid>& asteroids, int game_lvl, int ast_lvl, sf::Vector2f startPos);
    // Populates a vector of asteroids
    static void makeAsteroids(std::vector<asteroid>& asteroids, int game_lvl, int height, int width, sf::Vector2f shipPos);
    
};

#endif /* asteroid_hpp */
