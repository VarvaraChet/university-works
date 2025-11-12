#include "../headers/enemyManager.hpp"

EnemyManager::~EnemyManager(){
    while (enemys.size())
        enemys.pop_back();
}

void EnemyManager::addEnemy(Enemy* enemy){
    enemys.push_back(enemy);
}

void EnemyManager::deleteEnemy(Enemy* enemy){
    for (int i=0; i < enemys.size(); i++){
        if (enemys[i] == enemy){
            delete enemy;
            enemys.erase(enemys.begin()+i);
            return;
        }
    }
}

Enemy* EnemyManager::getEnemyAtCoordinates(std::pair <int, int> locate) const{
    for (int i=0; i < enemys.size(); i++){
        if (enemys[i]->getCoordinates() == locate)
            return enemys[i];
    }

    return nullptr;
}

int EnemyManager::attackEnemy(std::pair <int, int> locate, int damage){
    int point=0;

    for (int i=0; i < enemys.size(); i++){
        Enemy* enemy = enemys[i];

        if (enemy->getCoordinates() == locate){
            enemy->addHit();
            point += 5;

            bool alive = enemy->loseHealth(damage);
            if (!alive){
                point += 20/enemy->getHits();

                delete enemy;
                enemys.erase(enemys.begin()+i);
            }

            break;
        }
    }

    return point;
}

std::vector<Enemy*> EnemyManager::getEnemies() const{ 
    return enemys;
}

std::pair <int, int> EnemyManager::findNearEnemy(std::pair <int, int> locate){
    int min_dist=100;
    std::pair <int, int> result = locate;

    for (Enemy* enemy : enemys){
        std::pair <int, int> enemy_locate = enemy->getCoordinates();

        int dist = std::abs(locate.first-enemy_locate.first)+std::abs(locate.second-enemy_locate.second);
        if (dist < min_dist){
            min_dist = dist;
            result = enemy_locate;
        }
    }

    return result;
}