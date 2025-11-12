#ifndef GAME
#define GAME

#include <memory>
#include <string>
#include <vector>

#include "gameManager.hpp"
#include "saveManager.hpp"
#include "gameState.hpp"
#include "gameGUI.hpp"

class Game {
    private:
        sf::RenderWindow window;
        std::unique_ptr<GameGUI> gameGUI;
        
        std::unique_ptr<GameManager> gameManager;
        SaveManager saveManager;
        GameState currentState;
        int currentLevel;
        bool gameRunning;

        void executeGameTurn();
        void handleEvents();
        void processInput(sf::Keyboard::Key key);
        void handleMainMenuInput(sf::Keyboard::Key key);
        void handlePlayingInput(sf::Keyboard::Key key);
        void handleUpgradeInput(sf::Keyboard::Key key);
        void handleGameOverInput(sf::Keyboard::Key key);
        void handleBuyingSpellInput(sf::Keyboard::Key key);
        void handleMouseClick(const sf::Vector2i& position);

        void startNewGame();
        void startNewLevel();
        
        void loadGame();
        void saveGame();

    public:
        Game();
        void run();
};

#endif