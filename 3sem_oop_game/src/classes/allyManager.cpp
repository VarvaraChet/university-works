#include "../headers/allyManager.hpp"

AllyManager::~AllyManager(){
    while (allies.size())
        allies.pop_back();
}

void AllyManager::addAlly(Ally* ally){
    allies.push_back(ally);
}

void AllyManager::deleteAlly(Ally* ally){
    for (int i=0; i < allies.size(); i++){
        if (allies[i] == ally){
            delete ally;
            allies.erase(allies.begin()+i);
            return;
        }
    }
}

Ally* AllyManager::getAllyAtCoordinates(std::pair <int, int> locate) const{
    for (int i=0; i < allies.size(); i++){
        if (allies[i]->getCoordinates() == locate)
            return allies[i];
    }

    return nullptr;
}

bool AllyManager::attackAlly(std::pair <int, int> locate, int damage){
    for (int i=0; i < allies.size(); i++){
        Ally* ally = allies[i];

        if (ally->getCoordinates() == locate){

            bool alive = ally->loseHealth(damage);
            if (!alive){
                delete ally;
                allies.erase(allies.begin()+i);
            }

            return true;
        }
    }
    return false;
}

std::vector <Ally*> AllyManager::getAllies() const{ 
    return allies;
}