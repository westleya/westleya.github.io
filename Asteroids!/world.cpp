//
//  world.cpp
//  asteroids
//
//  Created by Westley Kirkham and Doug Garding on 9/18/17.
//
//

#include <stdlib.h> // to get random numbers
#include <cmath> // for the power and square root functions
#include <time.h> // uses this for the seed for the random number
#include <SFML/Graphics.hpp>
#include <vector> // for bullet and asteroids vectors
#include "world.hpp"
#include "ship.hpp"
#include "asteroid.hpp"
#include "collisions.hpp"
#include "PowerUp.hpp"
#include <string>
#include <iostream>

world::world(int w, int h){
    // Create seed for the random number generator
    srand (time(0));
    width = w;
    height = h;
    playerShip = ship::ship(20, width/2 - 10, height/2 - 10);
    game_lvl = 1;
    timeOfLastBullet = clock.getElapsedTime();
    
    // Create four level 3 asteroids for start of game
    asteroid::makeAsteroids(asteroids, game_lvl, height, width, playerShip.getPosition());
    
}


/**
 * RUNS THE GAME WINDOW AND GAME
 */
void world::runWorld(){
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(width, height), "---Asteroids---", sf::Style::Close, settings);
    
    sf::Font font;
    if (!font.loadFromFile("spaceAge.ttf"))
    {
        // error...
    }
    //
    // Title Screen
    //
    while (window.isOpen() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.clear(sf::Color(15, 12, 25));
        titleScreen(font, window);
        window.display();
    }
    
    
    //
    // START GAME LOOP!!
    //
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        
        // Clears the previous frame
        window.clear(sf::Color(15, 12, 25));
        
        
        
        ///////////////////////
        // 1 - PROCESS USER INPUT
        ///////////////////////
        
        playerShip.rotateRight();
        playerShip.rotateLeft();
        playerShip.thrusters(width, height);
        
        // Cheat to enter a power up into the game
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
            PowerUp::addPowerUp(powerUps);
        }
        
        
        sf::Time currentTime = clock.getElapsedTime();
        sf::Int32 msec = currentTime.asMilliseconds() - timeOfLastBullet.asMilliseconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && (msec > 200))
        {
            // If missile power up is active, create missiles instead of bullets
            //if(missile){
            //    Missile::makeMissiles(missiles, playerShip, asteroids);
            //}
            //else{
                bullet::makeBullets(bullets, playerShip, bulletSpray);
                timeOfLastBullet = clock.getElapsedTime();
            //}
        }
        
        // Cheat to enter a power up into the game
        currentTime = clock.getElapsedTime();
        msec = currentTime.asMilliseconds() - timeOfLastPower.asMilliseconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && msec > 1000){
            PowerUp::addPowerUp(powerUps);
            timeOfLastPower = clock.getElapsedTime();
        }
        
        
        ///////////////////////
        // 2 - UPDATE GAME STATE
        ///////////////////////
        
        // Update level
        if (asteroids.size() == 0)
        {
            game_lvl++;
            drawLevel(window, font, clock, game_lvl);
            playerShip.addlife();
            playerShip.decrimentLives(width, height);
            asteroid::makeAsteroids(asteroids, game_lvl, height, width, playerShip.getPosition());
            bullets.erase(bullets.begin(), bullets.end());
        }
        
        // Deactivate power ups if necessary
        for(int i = 0; i < powerUps.size(); i++ ){
            PowerUp temp = powerUps[i];
            
            // If the powerup is not active on a player, go to next power up
            if(temp.getIsOnField()){
                continue;
            }
            
            // If the power up time has reached its limit, remove the power completely
            sf::Time timeActive = clock.getElapsedTime() - temp.getStartTime();
            if(timeActive.asSeconds() >= temp.getTimeActive()){
                powerUps.erase(powerUps.begin() + i);
                i--;
                world::deactivatePowerup(temp);
            }
        }
        
        // Insert power up every so often
        if(static_cast<int>(currentTime.asMilliseconds()) % 10000 == 0){
            PowerUp::addPowerUp(powerUps);
        }
        
        
        // Detect ship power up collisions
        for(int i = 0; i < powerUps.size(); i++){
            if(collisions::shipPowerUp(playerShip, powerUps[i])){
                world::applyPowerUp(powerUps[i]);
            }
        }
        
        // Detect bullet asteroid collisions
        for(int i = 0; i < asteroids.size(); i++){
            for(int j = 0; j < bullets.size(); j++){
                //Check if each bullet and astroid collide
                if(collisions::bulletAsteroid(bullets[j], asteroids[i])){
                    
                    // Spawn smaller asteroids if destroyed asteroid was large enough
                    asteroid tempAst = asteroids[i];
                    int asteroidLevel = tempAst.getLevel();
                    if(asteroidLevel > 1){
                        asteroid::makeAsteroids(asteroids, game_lvl, asteroidLevel-1, tempAst.getPosition());
                    }
                    
                    bullets.erase(bullets.begin() + j);
                    asteroids.erase(asteroids.begin() + i);
                    if(i > 0){
                        i--;
                    }
                    j--;
                }
            }
        }
        
        // Detect ship asteroid collisions
        for (int i = 0; i < asteroids.size(); i++){
            // Check if asteroid has collided with ship
            if(collisions::shipAsteroid(playerShip, asteroids[i])){
                
                // If there's no shield power up, destroy the ship
                if(!shield){
                    playerShip.decrimentLives(width, height);
                    bullets.erase(bullets.begin(), bullets.end());
                    playerShip.shipReset(clock, asteroids, playerShip, width, height, window);
                }
                
                // Add code to destroy an asteroid if ship hits it?
            }
        }
        if (playerShip.livesLeft() == 0)
        {
            gameOver(window, font, playerShip);
            game_lvl = 1;
            bullets.erase(bullets.begin(), bullets.end());
            asteroids.erase(asteroids.begin(), asteroids.end());
            asteroid::makeAsteroids(asteroids, game_lvl, height, width, playerShip.getPosition());
        }
        
        // Update astroid locations
        for(auto &element : asteroids){
            element.updatePosition(width, height);
        }
        
        // Update bullet locations
        for(auto &element : bullets){
            element.updatePosition(width, height);
        }
        
        // Destroy old bullets
        bullet::destroyBullets(bullets);
        
        
        ///////////////////////
        // 3 - RENDER THE GAME
        ///////////////////////
        
        // Draw ship
        playerShip.drawShip(window);
        
        // Draw asteroids
        for(auto element : asteroids){
            element.drawAsteroid(window);
        }
        
        // Draw bullets
        for(auto element : bullets){
            element.drawBullet(window);
        }
        
        // Draw Powerups
        for(auto element : powerUps){
            // Check if it's still on the field
            if(element.getIsOnField()){
                element.drawPowerUp(window);
            }
        }
        
        // Draw lives
        playerShip.drawLives(window);
        
        // Updates the display
        window.display();
    }
    //
    // END GAME LOOP
    //
    
}

void world::titleScreen(sf::Font& font, sf::RenderWindow& window)
{
    sf::Text text1;
    sf::Text text2;
    
    
    text1.setFont(font);
    text1.setString("Asteroids");
    text1.setPosition(width / 4, height / 3);
    text1.setCharacterSize(96);
    text1.setFillColor(sf::Color::White);
    text1.setStyle(sf::Text::Bold | sf::Text::Underlined);
    
    text2.setFont(font);
    text2.setString("Push Space to Start");
    text2.setPosition(width / 3.5, height / 2.4);
    text2.setCharacterSize(36);
    text2.setFillColor(sf::Color::White);
    
    window.draw(text1);
    window.draw(text2);
    
    
}

void world::drawLevel(sf::RenderWindow& window, sf::Font& font, sf::Clock& clock, int game_lvl)
{
    sf::Text text;
    
    std::string lvl = "Level " + std::to_string(game_lvl);
    text.setFont(font);
    text.setPosition(width / 3, height / 2.5);
    text.setString( lvl);
    text.setCharacterSize(96);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);

    
    // clock.restart();
    sf::Time elapsed = clock.getElapsedTime();
    sf::Int32 msec1 = elapsed.asMilliseconds();
    sf::Int32 msec2 = elapsed.asMilliseconds();
    do {
        elapsed = clock.getElapsedTime();
        msec2 = elapsed.asMilliseconds();
        window.clear(sf::Color(15, 12, 25));
        window.draw(text);
        window.display();
    } while (msec2 - msec1 < 2000);

}

void world::gameOver(sf::RenderWindow& window, sf::Font& font, ship& playerShip)
{
    sf::Text text1;
    sf::Text text2;
    
    
    text1.setFont(font);
    text1.setString("Game Over");
    text1.setPosition(width / 4, height / 3);
    text1.setCharacterSize(108);
    text1.setFillColor(sf::Color::White);
    text1.setStyle(sf::Text::Bold);
    
    text2.setFont(font);
    text2.setString("Push Space to go again");
    text2.setPosition(width / 3.5, height / 2.4);
    text2.setCharacterSize(36);
    text2.setFillColor(sf::Color::White);
    
    window.draw(text1);
    window.draw(text2);
    do {
        window.clear(sf::Color(15, 12, 25));
        window.draw(text1);
        window.draw(text2);
        window.display();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return;
            }
        }
    } while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space));

    
    playerShip.decrimentLives(frameWidth, frameHeight);
    for(int i = 0; i < 4; i++)
    {
        playerShip.addlife();
    }
}

void world::applyPowerUp(PowerUp &power){
    if(power.getPowerType() == "shield"){
        shield = true;
        playerShip.setDrawShield(true);
    }
    else if(power.getPowerType() == "1up"){
        playerShip.incrementLives();
    }
    else if(power.getPowerType() == "spray"){
        bulletSpray = true;
    }
    else if(power.getPowerType() == "missile"){
        missile = true;
    }
    
    // Set the time this powerup became active
    power.setStartTime(clock.getElapsedTime());
    // Take the power up icon off the field
    power.setIsOnField(false);
}

void world::deactivatePowerup(PowerUp &power){
    if(power.getPowerType() == "shield"){
        shield = false;
        playerShip.setDrawShield(false);
    }
    else if(power.getPowerType() == "spray"){
        bulletSpray = false;
    }
    else if(power.getPowerType() == "missile"){
        missile = false;
    }
}

