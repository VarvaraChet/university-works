#include "../headers/enemyBuilding.hpp"

void EnemyBuilding::isValidArguments(){
    if (break_time <= 0 || strong <= 0)
        throw std::invalid_argument("Incorrect argument for enemy building");
}

EnemyBuilding::EnemyBuilding(int break_time, int move_cnt, int strong, std::pair <int, int> locate) 
    : break_time(break_time), start(move_cnt), strong(strong), locate(locate){
    
    isValidArguments();
}

int EnemyBuilding::getBreak() const{
    return break_time;
}

int EnemyBuilding::getStart() const{
    return start;
}

int EnemyBuilding::getStrong() const{
    return strong;
}

std::pair <int, int> EnemyBuilding::getCoordinates() const{
    return locate;
}

void EnemyBuilding::takeDamage(int damage){
    strong -= damage;
}

bool EnemyBuilding::isExist() const{
    return (strong > 0);
}

void EnemyBuilding::spawnEnemy(int damage, int health, std::pair <int, int> enemy_locate, EnemyManager& enemyManager){
    if (isExist()){
        Enemy* enemy = new Enemy(damage, health, enemy_locate);
        enemyManager.addEnemy(enemy);
    }
}