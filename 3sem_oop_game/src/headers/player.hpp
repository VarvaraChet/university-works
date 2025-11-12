#ifndef PLAYER
#define PLAYER

#include <algorithm>
#include <stdexcept>

#include "playerHand.hpp"

class Field;
class EnemyManager;
class EnemyBuilding;
class EnemyTower;
class AllyManager;

enum class fightType{
    NEAR,
    FAR
};

enum class moveType{
    CAN,
    NOT
};

class Player{
    private:
        int health;
        int maxHealth;
        int mana;

        int damage;
        fightType fight;
        
        moveType canMove;
        std::pair <int, int> locate;

        int points;
        PlayerHand hand;

        void isValidArguments();

    public:
        Player(int maxHealth, int damage, std::pair <int, int> locate, int hand_size, fightType fight);

        int getHealth() const;
        int getMaxHealth() const;
        int getDamage() const;
        int getPoints() const;
        int getMana() const;
        moveType getMove() const;
        fightType getFight() const;
        PlayerHand getHand() const; 
        std::pair <int, int> getCoordinates() const;

        void changeFight();
        void setHealth(int delta);
        bool isAlive() const;
        void setMana(int delta);
        void addPoints(int cnt);
        void setMaxHealth(int new_health);
        void setDamage(int new_damage);
        void setMove(moveType move);
        void setPoints(int cnt_points);
        void setCoordinates(std::pair <int, int> new_locate);

        bool addSpell(spellType spell, int damage, int range);
        bool removeSpell(size_t index);
        void clearHand();
        void levelUpSpells();

        bool cast(size_t spell_id, Field& field, EnemyManager& enemyManager, EnemyBuilding& enemyBuilding, std::pair <int, int> target);
        bool cast(size_t spell_id, Field& field, EnemyManager& enemyManager, EnemyBuilding& enemyBuilding, EnemyTower& enemyTower, AllyManager& allyManager);
        bool cast(size_t spell_id);
};

#endif