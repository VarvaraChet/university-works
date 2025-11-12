#include "../headers/cell.hpp"

Cell::Cell() : type(cellType::EMPTY){}

Cell::Cell(cellType type) : type(type){}

int Cell::getDamage() const{
    return damage;
}

cellType Cell::getType() const{
    return type;
}

void Cell::setDamage(int damage){
    this->damage = damage;
}

void Cell::setType(cellType type){
    this->type = type;
}