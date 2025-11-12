#ifndef ENEMY_INFO_WINDOW
#define ENEMY_INFO_WINDOW

#include <SFML/Graphics.hpp>
#include <sstream>

#include "../headers/enemy.hpp"

class EnemyInfoWindow {
private:
    sf::RectangleShape background;
    sf::Text healthText;
    sf::Text hitsText;
    sf::Text damageText;
    sf::Font font;
    bool visible;

public:
    EnemyInfoWindow();
    void setup();
    void show(const Enemy& enemy, const sf::Vector2f& position);
    void hide();
    bool isVisible() const;
    void draw(sf::RenderWindow& window);
};

#endif