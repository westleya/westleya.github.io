//
//  world.hpp
//  asteroids
//
//  Created by Westley Kirkham and Doug Garding on 9/18/17.
//
//

#ifndef world_hpp
#define world_hpp

#include "SFML/Graphics.hpp"
#include <stdio.h>
#include "ship.hpp"
#include "asteroid.hpp"
#include "bullets.hpp"
#include "collisions.hpp"
#include "PowerUp.hpp"
#include "HomingMissile.hpp"
#include <unordered_map>


/// World class. This is where most of the game happens.
/// It also contains all the other classes. There are
/// multiple screens: a title screen, level screens, and
/// a Game Over screen. The font used for the screens is
/// Space Age.
class world{
private:
    // Width and height of window/game
    int width; /// Width of screen
    int height; /// Height of screen
    int game_lvl;
    ship playerShip;
    sf::Clock clock;
    sf::Time timeOfLastBullet;
    sf::Time timeOfLastPower;
    std::vector<asteroid> asteroids;
    std::vector<bullet> bullets;
    std::vector<Missile> missiles;
    std::vector<PowerUp> powerUps; // Stores the powerups that are on the field
    
    // Power up variables
    bool shield;
    bool bulletSpray;
    bool missile;
    
    void applyPowerUp(PowerUp &power);
    void deactivatePowerup(PowerUp &power);
    
public:
    // static ints that represent the width and height
    const static int frameWidth = 1600; /// These are because we got tired of including width and height as parameters
    const static int frameHeight = 1200; /// But they came so late in the game that we never got to use them.
    
    world(int w, int h);
    void titleScreen(sf::Font& font, sf::RenderWindow& window); /// Title Screen
    
    void runWorld();
    void drawBullets(std::vector<bullet>& bullets, sf::RenderWindow& window);
    void drawLevel(sf::RenderWindow& window, sf::Font& font, sf::Clock& clock, int game_lvl); /// Level Screen
    void gameOver(sf::RenderWindow& window, sf::Font& font, ship& playerShip); /// Game Over Screen


};

#endif /* world_hpp */
