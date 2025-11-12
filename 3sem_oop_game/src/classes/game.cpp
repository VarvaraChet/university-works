#include "../headers/game.hpp"

Game::Game() 
    : window(sf::VideoMode(25*40+350, 20*40), "Game"),
      currentState(),
      currentLevel(1),
      gameRunning(true) {
    
    gameManager = std::make_unique <GameManager>();

    currentState.state = GameState::MAIN_MENU;
    gameGUI = std::make_unique<GameGUI>();
}

void Game::run(){
    while (window.isOpen() && gameRunning){        
        handleEvents();
        
        if (currentState.state == GameState::PLAYING && gameManager){
            endType end = gameManager->checkGameEnd();
            if (end == endType::WIN)
                currentState.state = GameState::LEVEL_COMPLETE;
            else if (end == endType::LOSE)
                currentState.state = GameState::GAME_OVER;
        }
        
        window.clear(sf::Color::Black);
        gameGUI->render(window, currentState, gameManager.get(), currentLevel);
        window.display();
    }
}

void Game::executeGameTurn() {
    if (!gameManager)
        return;
    
    gameManager->allyAction();

    gameManager->enemyAction();
    
    EnemyBuilding enemyBuilding = gameManager->getEnemyBuilding();
    if ((gameManager->getMoveCount()-enemyBuilding.getStart())%enemyBuilding.getBreak() == 0)
        gameManager->spawnEnemy(10+currentLevel*3, 50+currentLevel*10);

    gameManager->towerAction();
    
    endType end = gameManager->checkGameEnd();
    if (end == endType::WIN)
        currentState.state = GameState::LEVEL_COMPLETE;
    else if (end == endType::LOSE)
        currentState.state = GameState::GAME_OVER;
}

void Game::handleEvents(){
    sf::Event event;
    while (window.pollEvent(event)){
        if (event.type == sf::Event::Closed)
            window.close();

        else if (event.type == sf::Event::KeyPressed)
            processInput(event.key.code);

        else if (event.type == sf::Event::MouseButtonPressed){
            if (event.mouseButton.button == sf::Mouse::Left)
                handleMouseClick(sf::Mouse::getPosition(window));
        }
    }
}

void Game::processInput(sf::Keyboard::Key key){
    switch (currentState.state){
        case GameState::MAIN_MENU:
            handleMainMenuInput(key);
            break;

        case GameState::PLAYING:
            handlePlayingInput(key);
            break;

        case GameState::LEVEL_COMPLETE:
            if (key == sf::Keyboard::Num1)
                currentState.state = GameState::UPGRADING;
            else if (key == sf::Keyboard::Num2)
                currentState.state = GameState::MAIN_MENU;
            else if (key == sf::Keyboard::S)
                saveGame();
            break;

        case GameState::UPGRADING:
            handleUpgradeInput(key);
            break;

        case GameState::GAME_OVER:
            handleGameOverInput(key);
            break;

        case GameState::BUYING_SPELL:
            handleBuyingSpellInput(key);
            break;
    }
}

void Game::handleMainMenuInput(sf::Keyboard::Key key){
    if (key == sf::Keyboard::Num1){
        startNewGame();
        currentState.state = GameState::PLAYING;
    }
    else if (key == sf::Keyboard::Num2)
        loadGame();
    else if (key == sf::Keyboard::Num3)
        gameRunning = false;
}

void Game::handlePlayingInput(sf::Keyboard::Key key){
    if (!gameManager)
        return;
    
    bool checkAction = false;

    switch (key){
        case sf::Keyboard::Up:
            checkAction = gameManager->movePlayer(direction::UP);
            break;

        case sf::Keyboard::Down:
            checkAction = gameManager->movePlayer(direction::DOWN);
            break;

        case sf::Keyboard::Left:
            checkAction = gameManager->movePlayer(direction::LEFT);
            break;

        case sf::Keyboard::Right:
            checkAction = gameManager->movePlayer(direction::RIGHT);
            break;
            
        case sf::Keyboard::F:
            gameManager->changeFight();
            checkAction = true;
            break;
            
        case sf::Keyboard::A:
            currentState.showingAttackRadius = true;
            break;
            
        case sf::Keyboard::B:
            currentState.state = GameState::BUYING_SPELL;
            break;
            
        case sf::Keyboard::S:
            saveGame();
            break;
            
        case sf::Keyboard::Num1:
        case sf::Keyboard::Num2:
        case sf::Keyboard::Num3:
        case sf::Keyboard::Num4:
        case sf::Keyboard::Num5:
        case sf::Keyboard::Num6:
            if (currentState.castingSpell){
                currentState.castingSpell = false;
                currentState.selectedSpellIndex = -1;
            }
            else{
                int spellIndex = key-sf::Keyboard::Num1;
                PlayerHand hand = gameManager->getPlayer().getHand();
                if (spellIndex < hand.getSize()){
                    currentState.castingSpell = true;
                    currentState.selectedSpellIndex = spellIndex;
                    currentState.showingSpellRadius = true;
                }
            }
            break;
            
        case sf::Keyboard::Escape:
            currentState.showingAttackRadius = false;
            currentState.castingSpell = false;
            currentState.showingSpellRadius = false;
            currentState.selectedSpellIndex = -1;
            break;
    }

    if (checkAction && currentState.state == GameState::PLAYING)
        executeGameTurn();
}

void Game::handleMouseClick(const sf::Vector2i& position){
    if (currentState.state != GameState::PLAYING || !gameManager)
        return;
    
    int gridX = position.x/40;
    int gridY = position.y/40;
    std::pair <int, int> gridPos ={gridX, gridY};
    
    if (currentState.showingAttackRadius){
        gameManager->playerAttack();
        currentState.showingAttackRadius = false;

        executeGameTurn();
        
    }
    else if (currentState.castingSpell && currentState.showingSpellRadius){
        int spellIndex = currentState.selectedSpellIndex;
        bool success = gameManager->castSpell(spellIndex, gridPos);
        if (success){
            currentState.castingSpell = false;
            currentState.showingSpellRadius = false;
            currentState.selectedSpellIndex = -1;
            
            executeGameTurn();
        }
    }
    else{
        Enemy* enemy = gameManager->getEnemyManager().getEnemyAtCoordinates(gridPos);
        if (enemy){
            currentState.selectedEnemy = enemy;
            currentState.showingEnemyInfo = true;
        }
        else
            currentState.showingEnemyInfo = false;
    }
}

void Game::handleUpgradeInput(sf::Keyboard::Key key){
    if (!gameManager)
        return;
    
    if (key == sf::Keyboard::S)
        saveGame();

    if (key >= sf::Keyboard::Num1 && key <= sf::Keyboard::Num4){
        int choice = key-sf::Keyboard::Num1;
        if (gameManager->applyUpgrade(static_cast<UpgradeType>(choice))){
            startNewLevel();
            currentState.state = GameState::PLAYING;
        }
    }
}

void Game::handleGameOverInput(sf::Keyboard::Key key){
    if (key == sf::Keyboard::Num1)
        currentState.state = GameState::MAIN_MENU;
    else if (key == sf::Keyboard::Num2)
        gameRunning = false;
}

void Game::handleBuyingSpellInput(sf::Keyboard::Key key){
    if (!gameManager)
        return;
    
    if (key >= sf::Keyboard::Num1 && key <= sf::Keyboard::Num5){
        spellType spell = static_cast<spellType>(key-sf::Keyboard::Num1);
        if (gameManager->addSpell(spell))
            currentState.state = GameState::PLAYING;

        executeGameTurn();
    }
    else if (key == sf::Keyboard::Escape)
        currentState.state = GameState::PLAYING;
}

void Game::startNewGame(){
    try{
        gameManager = std::make_unique<GameManager>();
        currentLevel = 1;
    }
    catch (const std::exception& e){
        std::cout << "Game buil error: " << e.what() << std::endl;
    }
}

void Game::startNewLevel(){
    if (!gameManager)
        return;

    currentLevel++;
    gameManager->nextLevel(currentLevel);
}

void Game::loadGame(){
    SaveData data;
    if (saveManager.loadGame(data)){
        try{
            gameManager->loadFromSaveData(data);

            currentLevel = data.level;
            gameManager->setCurrentLevel(currentLevel);
            currentState.state = static_cast<GameState::State>(data.savedState);
        }
        catch (const std::exception& e){
            std::cout << "Load error: " << e.what() << std::endl;
            startNewGame();
            currentState.state = GameState::PLAYING;
        }
    }
    else{
        startNewGame();
        currentState.state = GameState::PLAYING;
    }
}

void Game::saveGame(){
    if (!gameManager)
        return;
    
    try{
        SaveData data = gameManager->getSaveData();
        data.savedState = static_cast<int>(currentState.state);
        
        saveManager.saveGame(data);
    }
    catch (const SaveException& e){
        std::cout << "Save error: " << e.what() << std::endl;
    }
}
