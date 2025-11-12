#include "../headers/ally.hpp"

void Ally::isValidArguments(){
    if (health <= 0 || damage <= 0)
        throw std::invalid_argument("Incorrect ally arguments");
}

Ally::Ally(int damage, int health, std::pair <int, int> locate)
    : health(health), damage(damage), locate(locate){
    
    isValidArguments();
}

int Ally::getHealth() const{
    return health;
}

int Ally::getDamage() const{
    return damage;
}

std::pair <int, int> Ally::getCoordinates() const{
    return locate;
}

void Ally::setDamage(int delta){
    damage += delta;
}

bool Ally::loseHealth(int delta){
    health -= delta;
    return (health > 0);
}

void Ally::setCoordinates(std::pair <int, int> new_locate){
    locate = new_locate;
}