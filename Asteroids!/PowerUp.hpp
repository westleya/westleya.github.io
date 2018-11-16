//
//  powerUp.hpp
//  asteroids
//
//  Created by Doug on 9/21/17.
//

#ifndef powerUp_hpp
#define powerUp_hpp
#include <stdio.h>
#include <SFML/Graphics.hpp>

///The game randomly generates power-ups every so often.
class PowerUp {
private:
    /// Shape of the power up to be collected by ship
    sf::ConvexShape shape;
    /// Specifies which powerup
    std::string type;
    // Tracks if the power up is on the field
    bool isOnField;
    // Tracks how much time the power up has left
    sf::Time startTime;
    // How long the power up will last (in seconds)
    float timeActive;
    
    /// Creates the convexShape object
    sf::ConvexShape buildShape(std::string type);
    /// Creates the convexShape object
    sf::ConvexShape buildActiveShape(std::string type);
    /// Generates random starting point on map for the power up
    sf::Vector2f PowerUpPosition();
    
public:
    /// Default constructor
    PowerUp(std::string power);
    /// Draws the power up to the screen
    void drawPowerUp(sf::RenderWindow &window);
    
    // Adds a power up to an array of power ups
    static void addPowerUp(std::vector<PowerUp> &list);
    
    /// Returns the bounds of the object for collisions
    sf::FloatRect getBounds();
    bool getIsOnField();
    void setIsOnField(bool b);
    sf::Time getStartTime();
    void setStartTime(sf::Time t);
    float getTimeActive();
    
    // Gets the powerup type
    std::string getPowerType() const;
};

#endif /* powerUp_hpp */
