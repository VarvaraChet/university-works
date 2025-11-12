#include "../headers/trapSpell.hpp"

#include "../headers/field.hpp"
#include "../headers/player.hpp"
#include "../headers/enemyManager.hpp"
#include "../headers/enemyBuilding.hpp"

void TrapSpell::isValidArguments(){
    if (damage <= 0 || range <= 0)
        throw std::invalid_argument("Incorrect spell arguments");
}

TrapSpell::TrapSpell(int damage, int range)
    : damage(damage), range(range) {
    isValidArguments();
}

std::string TrapSpell::getName() const{
    return "Trap spell";
}

std::string TrapSpell::getDescription() const{
    return "Places a trap that deals "+std::to_string(damage)+" damage when triggered";
}

std::string TrapSpell::getInfo() const{
    return "Damage: "+std::to_string(damage)+" | Range: "+std::to_string(range);
}

int TrapSpell::getRange() const {
    return range;
}

int TrapSpell::getDamage() const{
    return damage;
}

void TrapSpell::setRange(int delta){
    range += delta;
}

void TrapSpell::setDamage(int delta){
    damage += delta;
}

bool TrapSpell::cast(Player& player, Field& field, EnemyManager& enemyManager,
                    EnemyBuilding& enemyBuilding, std::pair<int, int> target, size_t improve) {
    std::pair <int, int> player_locate = player.getCoordinates();
    
    int dist = std::sqrt(std::pow(player_locate.first-target.first, 2)+std::pow(player_locate.second-target.second, 2));
    if (dist > range)
        return false;
    
    if (field.getCell(target)->getType() == cellType::OBSTACLE || enemyManager.getEnemyAtCoordinates(target) ||
        enemyBuilding.getCoordinates() == target || player.getCoordinates() == target)
        return false;
    
    Cell* cell = field.getCell(target);
    cell->setType(cellType::TRAP);
    cell->setDamage(damage+damage*improve);
    return true;
}

bool TrapSpell::cast(Player& player, Field& field, EnemyManager& enemyManager,
            EnemyBuilding& enemyBuilding, EnemyTower& enemyTower, AllyManager& allyManager, size_t improve) {
        return false;
}

bool TrapSpell::cast(Player& target, std::pair <int, int> locate){
    return false;
}

bool TrapSpell::cast(){
    return false;
}