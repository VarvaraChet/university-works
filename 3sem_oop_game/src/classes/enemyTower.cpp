#include "../headers/enemyTower.hpp"

EnemyTower::EnemyTower(int damage, int range, std::pair <int, int> locate)
    : locate(locate), attack(false), spell(damage, range) {}

void EnemyTower::attackPlayer(Player& player){
    std::pair <int, int> player_locate = player.getCoordinates();
    int dist = std::sqrt(std::pow(player_locate.first-locate.first, 2)+std::pow(player_locate.second-locate.second, 2));

    if (!attack)
        attack = spell.cast(player, locate);
    else if (dist >= spell.getRange())
        attack = false;
}

int EnemyTower::getRange() const{
    return spell.getRange();
}

std::pair <int, int> EnemyTower::getCoordinates() const{
    return locate;
}