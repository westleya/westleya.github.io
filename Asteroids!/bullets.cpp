//
//  bullets.cpp
//  asteroids
//
//  Created by Westley Kirkham on 9/19/17.
//
//
#include "bullets.hpp"
#include <cmath>
#define PI 3.14159265


bullet::bullet(sf::Vector2f pos, int rot)
{
    rectangle = buildBullet(pos, rot);
    speed = 1.3;
    distance = 0;
}

sf::RectangleShape bullet::buildBullet(sf::Vector2f pos, int rot)
{
    sf::RectangleShape rectangle(sf::Vector2f(5, 15));
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setPosition(pos.x, pos.y);
    rectangle.setOrigin(2, 25);
    rectangle.setRotation(rot);
    return rectangle;
}

sf::RectangleShape bullet::getRectangle()
{
    return rectangle;
}

void bullet::updatePosition(int width, int height){
    
    if (getPosition().x >= width)
    {
        rectangle.setPosition(0, getPosition().y);
    }
    else if (getPosition().x <= 0)
    {
        rectangle.setPosition(width -1, getPosition().y);
    }
    if (getPosition().y >= height)
    {
        rectangle.setPosition(getPosition().x, 0);
        
    }
    else if (getPosition().y <= 0)
    {
        rectangle.setPosition(getPosition().x, height - 1);
    }
    
    float y_update = -cos(getRotation() * PI / 180 ) * speed;
    float x_update = sin(getRotation() * PI / 180 ) * speed;
    rectangle.move(x_update, y_update);
    distance +=  speed;
}


void bullet::makeBullets(std::vector<bullet>& bullets, const ship& playerShip, bool hasSpray)
{
    // If the spray power is active, need to make extra bullets
    if(hasSpray){
        bullet rightBullet(playerShip.getPosition(), playerShip.rotationGet() + 10);
        bullet leftBullet(playerShip.getPosition(), playerShip.rotationGet() - 10);
        bullets.push_back(rightBullet);
        bullets.push_back(leftBullet);
    }
    
    bullets.push_back(bullet(playerShip.getPosition(), playerShip.rotationGet()));
}


// Destroys bullet if it's traveled its distance
void bullet::destroyBullets(std::vector<bullet>& bullets){
    for(int i = 0; i < bullets.size(); i++){
        if (bullets[i].getDistance() > 600 )
        {
            bullets.erase (bullets.begin() + i);
            i--;
        }
    }
}


void bullet::drawBullet(sf::RenderWindow& window)
{
    window.draw(rectangle);
}





int bullet::getDistance()
{
    return distance;
}

sf::Vector2f bullet::getPosition() const
{
    return rectangle.getPosition();
}

int bullet::getRotation() const
{
    return rectangle.getRotation();
}

sf::Vector2f bullet::getSize() const
{
    return rectangle.getSize();
}



