#include "../headers/directDamageSpell.hpp"

#include "../headers/field.hpp"
#include "../headers/player.hpp"
#include "../headers/enemyManager.hpp"
#include "../headers/enemyBuilding.hpp"

void DirectDamageSpell::isValidArguments(){
    if (damage <= 0 || range <= 0)
        throw std::invalid_argument("Incorrect spell arguments");
}

DirectDamageSpell::DirectDamageSpell(int damage, int range)
    : damage(damage), range(range) {
    isValidArguments();
}

std::string DirectDamageSpell::getName() const{
    return "Direct Damage";
}

std::string DirectDamageSpell::getDescription() const{
    return "Deals "+std::to_string(damage)+" damage to a single target";
}

std::string DirectDamageSpell::getInfo() const{
    return "Damage: "+std::to_string(damage)+" | Range: "+std::to_string(range);
}

int DirectDamageSpell::getRange() const{
    return range;
}

int DirectDamageSpell::getDamage() const{
    return damage;
}

void DirectDamageSpell::setRange(int delta){
    range += delta;
}

void DirectDamageSpell::setDamage(int delta){
    damage += delta;
}

bool DirectDamageSpell::cast(Player& player, Field& field, EnemyManager& enemyManager,
            EnemyBuilding& enemyBuilding, std::pair <int, int> target, size_t improve) {
    std::pair <int, int> player_locate = player.getCoordinates();
    
    int dist = std::sqrt(std::pow(player_locate.first-target.first, 2)+std::pow(player_locate.second-target.second, 2));
    if (dist > range+improve)
        return false;
    
    Enemy* enemy = enemyManager.getEnemyAtCoordinates(target);
    if (enemy){
        enemy->loseHealth(damage);
        return true;
    }
    
    if (enemyBuilding.getCoordinates() == target){
        enemyBuilding.takeDamage(damage);
        return true;
    }
    
    return false;
}

bool DirectDamageSpell::cast(Player& player, Field& field, EnemyManager& enemyManager,
            EnemyBuilding& enemyBuilding, EnemyTower& enemyTower, AllyManager& allyManager, size_t improve) {
        return false;
}

bool DirectDamageSpell::cast(Player& target, std::pair <int, int> locate){
    std::pair <int, int> player_locate = target.getCoordinates();
    int dist = std::sqrt(std::pow(player_locate.first-locate.first, 2)+std::pow(player_locate.second-locate.second, 2));
    if (dist >= range)
        return false;

    target.setHealth(-damage);
    return true;
}

bool DirectDamageSpell::cast(){
    return false;
}