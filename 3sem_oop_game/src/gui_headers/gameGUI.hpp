#ifndef GAME_GUI
#define GAME_GUI

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>

#include "gameManager.hpp"

#include "../headers/saveManager.hpp"
#include "gameState.hpp"
#include "textureManager.hpp"
#include "enemyInfoWindow.hpp"

class GameGUI {
    private:
        TextureManager textureManager;
        EnemyInfoWindow enemyInfoWindow;
        
        sf::Font font;
        sf::Text pointsText;
        sf::Text healthText;
        sf::Text manaText;
        sf::Text levelText;
        sf::Text messageText;
        
        static const int CELL_SIZE = 40;
        static const int SIDEBAR_WIDTH = 350;

        void loadTextures();
        void setupUI();

        void renderMainMenu(sf::RenderWindow& window);
        void renderPlaying(sf::RenderWindow& window, const GameState& state, const GameManager* gameManager, int currentLevel);
        void renderLevelComplete(sf::RenderWindow& window);
        void renderGameOver(sf::RenderWindow& window);
        void renderUpgrading(sf::RenderWindow& window);
        void renderBuyingSpell(sf::RenderWindow& window);
        
        void renderField(sf::RenderWindow& window, const GameManager* gameManager);
        void renderSidebar(sf::RenderWindow& window, const GameManager* gameManager, int currentLevel);
        void renderSpells(sf::RenderWindow& window, const GameManager* gameManager, float startY);
        void renderAttackRadius(sf::RenderWindow& window, const GameManager* gameManager);
        void renderSpellRadius(sf::RenderWindow& window, const GameManager* gameManager);
        void renderEnemyInfo(sf::RenderWindow& window, const GameState& state);
        void renderMenuOptions(sf::RenderWindow& window, const std::vector<std::string>& options, float startY);

    public:
        GameGUI();
        void render(sf::RenderWindow& window, const GameState& state, const GameManager* gameManager, int currentLevel);
};

#endif