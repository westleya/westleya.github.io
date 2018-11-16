//
//  ship.hpp
//  asteroids
//
//  Created by Westley Kirkham and Doug Garding on 9/18/17.
//
//

#ifndef ship_hpp
#define ship_hpp
#include <SFML/Graphics.hpp>
#include "asteroid.hpp"

/// This describes the ship controlled by the player.
/// It includes a slightly more complex version of the
/// physics used by asteroids and bullets to move.
/// While thrusters are on (UP arrow is pressed) the
/// flame shows on the back of the ship. The player
/// begins the game with three lives. Upon collision
/// with an asteroid a life is lost and the ship is
/// placed back in the center of the screen.
class ship{
    sf::ConvexShape triangle; /// ship
    sf::ConvexShape flameShip; /// ship plus thruster
    sf::ConvexShape shield; /// shield for ship
    sf::Vector2f speed; /// contains initial (constant) velocity(Vo) in x and y directions
    int old_rotation; /// Necessary to save this in order to continue in the direction that the
    /// thrusters last pushed the ship.
    int lives;
    bool drawShield;
    
    sf::ConvexShape buildFrame(int xPos, int yPos);
    sf::ConvexShape buildFlameFrame();
    sf::ConvexShape buildShield();
    
public:
    
    ship();
    ship(int size, int xPos, int yPos);
    void addlife();
    int livesLeft();
    void rotateRight();
    void rotateLeft();
    void thrusters(int width, int height);
    sf::CircleShape getShip();
    sf::Vector2f getPosition() const; /// We replaced certain sf::methods with our own. Get being the most common.
    int rotationGet() const;
    void drawShip(sf::RenderWindow &window); /// We also made our own draw methods.
    void drawLives(sf::RenderWindow& window); /// The number of lives you currently have displays in the
    /// top left corner of the screen
    void decrimentLives(int width, int height); /// The width and height were necessary for us to be able to
    /// reset the ship as part of the death process.
    static void shipReset(sf::Clock& clock, std::vector<asteroid>& asteroids, ship& playerShip, int width, int height, sf::RenderWindow& window); /// This returns the ship to the center of the screen after dying.
    /// It continues to run until it's safe for the ship to reappear and until at least half a second has passed.
    
    // Returns the bounds of the object for collisions
    sf::FloatRect getBounds(); /// For power-ups only.

    void setDrawShield(bool b); /// Sets whether a shield will be drawn or not
    void incrementLives(); /// Increases the number of player lives
    
};


#endif /* ship_hpp */
