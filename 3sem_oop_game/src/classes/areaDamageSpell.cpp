#include "../headers/areaDamageSpell.hpp"

#include "../headers/field.hpp"
#include "../headers/player.hpp"
#include "../headers/enemyManager.hpp"
#include "../headers/enemyBuilding.hpp"

void AreaDamageSpell::isValidArguments(){
    if (damage <= 0 || range <= 0)
        throw std::invalid_argument("Incorrect spell arguments");
}

AreaDamageSpell::AreaDamageSpell(int damage, int range)
    : damage(damage), range(range) {
        isValidArguments();
}

std::string AreaDamageSpell::getName() const {
    return "Area damage";
}

std::string AreaDamageSpell::getDescription() const{
    return "Deals "+std::to_string(damage)+" damage in 2x2 area";
}

std::string AreaDamageSpell::getInfo() const{
    return "Damage: "+std::to_string(damage)+" | Range: "+std::to_string(range);
}

int AreaDamageSpell::getRange() const{
    return range;
}

int AreaDamageSpell::getDamage() const{
    return damage;
}

void AreaDamageSpell::setRange(int delta){
    range += delta;
}

void AreaDamageSpell::setDamage(int delta){
    damage += delta;
}

bool AreaDamageSpell::cast(Player& player, Field& field, EnemyManager& enemyManager,
            EnemyBuilding& enemyBuilding, std::pair <int, int> target, size_t improve) {
    std::pair <int, int> player_locate = player.getCoordinates();
    
    int dist = std::sqrt(std::pow(player_locate.first-target.first, 2)+std::pow(player_locate.second-target.second, 2));
    if (dist > range)
        return false;
    
    for (int dy = 0; dy < 2+improve; dy++) {
        for (int dx = 0; dx < 2+improve; dx++) {
            std::pair<int, int> area = {target.first+dx, target.second+dy};
            
            if (area.first >= 0 && area.first < field.getWidth() && area.second >= 0 && area.second < field.getHeight()) {
                Enemy* enemy = enemyManager.getEnemyAtCoordinates(area);
                if (enemy)
                    enemy->loseHealth(damage);
                
                if (enemyBuilding.getCoordinates() == area)
                    enemyBuilding.takeDamage(damage);
            }
        }
    }
    
    return true;
}

bool AreaDamageSpell::cast(Player& player, Field& field, EnemyManager& enemyManager,
            EnemyBuilding& enemyBuilding, EnemyTower& enemyTower, AllyManager& allyManager, size_t improve) {
        return false;
}

bool AreaDamageSpell::cast(Player& target, std::pair <int, int> locate) {
    return false;
}

bool AreaDamageSpell::cast(){
    return false;
}