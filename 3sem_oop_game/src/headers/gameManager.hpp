#ifndef GAME_MANAGER
#define GAME_MANAGER

#include <iostream>
#include <stdexcept>
#include <cmath>

#include "field.hpp"
#include "player.hpp"
#include "allyManager.hpp"
#include "enemyManager.hpp"
#include "enemyBuilding.hpp"
#include "enemyTower.hpp"

#include "saveManager.hpp"

enum class direction{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum class endType{
    LOSE,
    WIN,
    CONTINUE
};

enum class UpgradeType {
    HEALTH,
    DAMAGE,
    MANA,
    SPELL
};

class GameManager{
    private:
        Field field;
        Player player;
        AllyManager allyManager;
        EnemyManager enemyManager;
        EnemyBuilding enemyBuilding;
        EnemyTower enemyTower;

        int moveCount;
        int currentLevel;
        bool levelCompleted;

        bool validCell(std::pair <int, int> cell);

        void generateLevel(int level);
        void setupPlayerForLevel(int level);

        fightType convertFightType(const std::string& fightTypeStr);
        moveType convertMove(const std::string& moveStr);

    public:
        GameManager();

        Player getPlayer() const;
        AllyManager getAllyManager() const;
        EnemyManager getEnemyManager() const;
        EnemyBuilding getEnemyBuilding() const;
        EnemyTower getEnemyTower() const;
        Field getField() const;

        bool movePlayer(direction direct);
        void moveAlly(Ally& ally, direction direct);
        void moveEnemy(Enemy& enemy, direction direct);

        void playerAttack();
        void allyAction();
        void enemyAction();

        endType checkGameEnd();

        void spawnEnemy(int damage, int health);
        void changePlayerMove();
        void changeFight();
        void towerAction();

        bool castSpell(size_t spell_id, std::pair <int, int> target);
        bool addSpell(spellType spell);

        void nextLevel(int newLevel);
        void setCurrentLevel(int level);
        bool applyUpgrade(UpgradeType type);
        
        void removeHalfSpells();
        void upgradeRandomSpell();
        
        int getCurrentLevel() const;
        int getMoveCount() const;
        bool isLevelCompleted() const;
        
        SaveData getSaveData() const;
        void loadFromSaveData(const SaveData& data);
};

#endif