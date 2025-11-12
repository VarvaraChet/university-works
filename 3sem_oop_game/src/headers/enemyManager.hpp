#ifndef ENEMY_MANAGER
#define ENEMY_MANAGER

#include <vector>

#include "enemy.hpp"

class EnemyManager{
    private:
        std::vector <Enemy*> enemys;

    public:
        ~EnemyManager();

        void addEnemy(Enemy* enemy);
        void deleteEnemy(Enemy* enemy);

        Enemy* getEnemyAtCoordinates(std::pair <int, int> locate) const;

        int attackEnemy(std::pair <int, int> locate, int damage);

        std::vector<Enemy*> getEnemies() const;
        std::pair <int, int> findNearEnemy(std::pair <int, int> locate);
};

#endif