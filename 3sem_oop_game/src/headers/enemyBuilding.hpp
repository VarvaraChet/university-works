#ifndef ENEMY_BUILDING
#define ENEMY_BUILDING

#include <stdexcept>

#include "player.hpp"
#include "field.hpp"
#include "enemyManager.hpp"

class EnemyBuilding{
    private:
        int break_time;
        int start;
        int strong;
        std::pair <int, int> locate;

        void isValidArguments();

    public:
        EnemyBuilding(int break_time, int move_cnt, int strong, std::pair <int, int> locate);

        int getBreak() const;
        int getStart() const;
        int getStrong() const;
        std::pair <int, int> getCoordinates() const;

        bool isExist() const;
        void takeDamage(int damage);

        void spawnEnemy(int damage, int health, std::pair <int, int> enemy_locate, EnemyManager& enemyManager);
};

#endif