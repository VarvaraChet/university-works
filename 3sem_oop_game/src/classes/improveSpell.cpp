#include "../headers/improveSpell.hpp"

std::string ImproveSpell::getName() const{
    return "Improve";
}

std::string ImproveSpell::getDescription() const{
    return "Improve next spell";
}

std::string ImproveSpell::getInfo() const{
    return "";
}

bool ImproveSpell::cast(Player& player, Field& field, EnemyManager& enemyManager,
    EnemyBuilding& enemyBuilding, std::pair <int, int> target, size_t improve){
    return false;
}

bool ImproveSpell::cast(Player& player, Field& field, EnemyManager& enemyManager,
    EnemyBuilding& enemyBuilding, EnemyTower& enemyTower, AllyManager& allyManager, size_t improve){
    return false;
}

bool ImproveSpell::cast(Player& target, std::pair <int, int> locate){
    return false;
}

bool ImproveSpell::cast(){
    return true;
}