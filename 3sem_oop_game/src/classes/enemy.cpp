#include "../headers/enemy.hpp"

void Enemy::isValidArguments(){
    if (health <= 0 || damage <= 0)
        throw std::invalid_argument("Incorrect enemy arguments");
}

Enemy::Enemy(int damage, int health, std::pair <int, int> locate)
    : health(health), damage(damage), hits(0), locate(locate){
    
    isValidArguments();
}

int Enemy::getHealth() const{
    return health;
}

int Enemy::getDamage() const{
    return damage;
}

int Enemy::getHits() const{
    return hits;
}

std::pair <int, int> Enemy::getCoordinates() const{
    return locate;
}

void Enemy::setDamage(int delta){
    damage += delta;
}

void Enemy::addHit(){
    hits++;
}

bool Enemy::loseHealth(int delta){
    health -= delta;
    return (health > 0);
}

void Enemy::setCoordinates(std::pair <int, int> new_locate){
    locate = new_locate;
}