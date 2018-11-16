//
//  bullets.hpp
//  asteroids
//
//  Created by Westley Kirkham on 9/19/17.
//
//

#ifndef bullets_hpp
#define bullets_hpp
#include <SFML/Graphics.hpp>
#include "ship.hpp"

/// Bullets are made when the user pushes space. Fire rate ~ 5/sec.
/// Their initialization is relative to the ship's current position.
class bullet{
    sf::RectangleShape rectangle;
    float distance;
    float speed; /// The bullet has its own speed irrespective of the ship's.

    sf::RectangleShape buildBullet(sf::Vector2f pos, int rot);
    
public:
    bullet(sf::Vector2f pos, int rot);
    sf::RectangleShape getRectangle();
    bullet();
    void move(int width, int height);
    int getDistance(); /// We destroy a bullet after it has traveled a certain distance.
    sf::Vector2f getPosition() const;
    int getRotation() const; /// Another get method replacement. The rotation is set to the direction the ship
    /// was facing when the bullet was shot.
    sf::Vector2f getSize() const;
    
    
    void drawBullet(sf::RenderWindow &window);
    
    /// Updates the position of a bullet for the current frame
    void updatePosition(int width, int height);
    
    /// Destroys bullet if it's traveled its distance
    static void destroyBullets(std::vector<bullet>& bullets);

    // adds bullets to the vector of bullets
    static void makeBullets(std::vector<bullet>& bullets, const ship& playerShip, bool hasSpray);

};

#endif /* bullets_hpp */
