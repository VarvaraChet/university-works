#include "../headers/allySpell.hpp"

#include "../headers/field.hpp"
#include "../headers/player.hpp"
#include "../headers/allyManager.hpp"
#include "../headers/enemyManager.hpp"
#include "../headers/enemyBuilding.hpp"
#include "../headers/enemyTower.hpp"

void AllySpell::isValidArguments(){
    if (damage <= 0 || range <= 0)
        throw std::invalid_argument("Incorrect spell arguments");
}

AllySpell::AllySpell(int damage, int range)
    : damage(damage), range(range) {
    isValidArguments();
}
    
std::string AllySpell::getName() const{
    return "Ally";
}

std::string AllySpell::getDescription() const{
    return "Call an ally";
}

std::string AllySpell::getInfo() const{
    return "Damage: "+std::to_string(damage)+" | Health: "+std::to_string(range);
}

int AllySpell::getRange() const{
    return range;
}

int AllySpell::getDamage() const{
    return damage;
}

void AllySpell::setRange(int delta){
    range += delta;
}

void AllySpell::setDamage(int delta){
    damage += delta;
}

bool AllySpell::cast(Player& player, Field& field, EnemyManager& enemyManager,
                    EnemyBuilding& enemyBuilding, std::pair<int, int> target, size_t improve) {
    return false;
}

std::pair <int, int> AllySpell::findCell(std::pair <int, int> locate, Field& field, EnemyManager& enemyManager,
            EnemyBuilding& enemyBuilding, EnemyTower& enemyTower, AllyManager& allyManager){

    for (int dx=-1; dx <= 1; dx++){
        for (int dy=-1; dy <= 1; dy++){
            int x=locate.first+dx;
            int y=locate.second+dy;
            std::pair <int, int> res_locate = std::make_pair(x, y);

            if (x >= 0 && x < field.getWidth() && y >= 0 && y < field.getHeight()){
                Cell* cell=field.getCell(res_locate);

                if ((cell->getType() == cellType::EMPTY || cell->getType() == cellType::SLOW) && !enemyManager.getEnemyAtCoordinates(res_locate)
                    && res_locate != enemyBuilding.getCoordinates() && res_locate != enemyTower.getCoordinates())
                        return res_locate;
            }
        }
    }
    return std::make_pair(-1, -1);
}

bool AllySpell::cast(Player& player, Field& field, EnemyManager& enemyManager,
            EnemyBuilding& enemyBuilding, EnemyTower& enemyTower, AllyManager& allyManager, size_t improve) {

    bool check = false;
    std::pair <int, int> player_locate = player.getCoordinates();
    
    for (int i=0; i <= improve; i++){
        std::pair <int, int> locate = findCell(player_locate, field, enemyManager, enemyBuilding, enemyTower, allyManager);
        if (locate != std::make_pair(-1, -1)){
            Ally* ally = new Ally(damage, range, locate);
            allyManager.addAlly(ally);
            check = true;
        }
    }
    
    return check;
}

bool AllySpell::cast(Player& target, std::pair <int, int> locate){
    return false;
}

bool AllySpell::cast(){
    return false;
}