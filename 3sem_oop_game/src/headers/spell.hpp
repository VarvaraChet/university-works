#ifndef ISPELL
#define ISPELL

#include <cmath>
#include <utility>
#include <string>
#include <stdexcept>

class Field;
class Player;
class Ally;
class AllyManager;
class Enemy;
class EnemyManager;
class EnemyBuilding;
class EnemyTower;

enum class spellType{
    AREA_DAMAGE,
    DIRECT_DAMAGE,
    TRAP,
    ALLY,
    IMPROVE
};

class Spell{        
    public:
        virtual ~Spell() = default;

        virtual std::string getName() const = 0;
        virtual std::string getDescription() const = 0;
        virtual std::string getInfo() const = 0;

        virtual bool cast(Player& player, Field& field, EnemyManager& enemyManager,
            EnemyBuilding& enemyBuilding, std::pair <int, int> target, size_t improve) = 0;

        virtual bool cast(Player& player, Field& field, EnemyManager& enemyManager,
            EnemyBuilding& enemyBuilding, EnemyTower& enemyTower, AllyManager& allyManager, size_t improve) = 0;

        virtual bool cast(Player& target, std::pair <int, int> locate) = 0;

        virtual bool cast() = 0;
};

#endif