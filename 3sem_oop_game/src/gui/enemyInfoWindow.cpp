#include "../gui_headers/enemyInfoWindow.hpp"

EnemyInfoWindow::EnemyInfoWindow() : visible(false) {
    background.setSize(sf::Vector2f(200, 100));
    background.setFillColor(sf::Color(50, 50, 50, 200));
    background.setOutlineThickness(2);
    background.setOutlineColor(sf::Color::White);
}

void EnemyInfoWindow::setup() {
    font.loadFromFile("resources/font.ttf");
    
    healthText.setFont(font);
    healthText.setCharacterSize(16);
    healthText.setFillColor(sf::Color::Green);
    
    hitsText.setFont(font);
    hitsText.setCharacterSize(16);
    hitsText.setFillColor(sf::Color::Yellow);
    
    damageText.setFont(font);
    damageText.setCharacterSize(16);
    damageText.setFillColor(sf::Color::Red);
}

void EnemyInfoWindow::show(const Enemy& enemy, const sf::Vector2f& position) {
    std::stringstream healthStream, hitsStream, damageStream;
    healthStream << "Health: " << enemy.getHealth();
    hitsStream << "Hits: " << enemy.getHits();
    damageStream << "Damage: " << enemy.getDamage();
    
    healthText.setString(healthStream.str());
    hitsText.setString(hitsStream.str());
    damageText.setString(damageStream.str());
    
    background.setPosition(position);
    healthText.setPosition(position.x+10, position.y+10);
    hitsText.setPosition(position.x+10, position.y+40);
    damageText.setPosition(position.x+10, position.y+70);
    
    visible = true;
}

void EnemyInfoWindow::hide() {
    visible = false;
}

bool EnemyInfoWindow::isVisible() const {
    return visible;
}

void EnemyInfoWindow::draw(sf::RenderWindow& window) {
    if (visible) {
        window.draw(background);
        window.draw(healthText);
        window.draw(hitsText);
        window.draw(damageText);
    }
}