#include "../headers/player.hpp"

void Player::isValidArguments(){
    if (maxHealth <= 0)
        throw std::invalid_argument("Incorrect max health");
    if (damage <= 0)
        throw std::invalid_argument("Incorrect damage");
}

Player::Player(int maxHealth, int damage, std::pair <int, int> locate, int hand_size, fightType fight=fightType::NEAR)
    : health(maxHealth), maxHealth(maxHealth), mana(0), damage(damage), fight(fight), canMove(moveType::CAN), locate(locate), points(0), hand(hand_size){
    
    isValidArguments();
}

int Player::getHealth() const{
    return health;
}

int Player::getMaxHealth() const{
    return maxHealth;
}

int Player::getDamage() const{
    return damage;
}

int Player::getPoints() const{
    return points;
}

int Player::getMana() const{
    return mana;
}

moveType Player::getMove() const{
    return canMove;
}

fightType Player::getFight() const{
    return fight;
}

PlayerHand Player::getHand() const{
    return hand;
}

std::pair <int, int> Player::getCoordinates() const{
    return locate;
}

void Player::changeFight(){
    if (fight == fightType::FAR)
        fight = fightType::NEAR;
    else
        fight = fightType::FAR;
}

void Player::setHealth(int delta){
    health += delta;
    health = std::min(health, maxHealth);
}

bool Player::isAlive() const{
    return (health > 0);
}

void Player::setMana(int delta){
    mana += delta;
}

void Player::addPoints(int cnt){
    points += cnt;
}

void Player::setMaxHealth(int new_health){
    maxHealth = new_health;
}

void Player::setDamage(int new_damage){
    damage = new_damage;
}

void Player::setMove(moveType move){
    canMove = move;
}

void Player::setPoints(int cnt_points){
    points = cnt_points;
}

void Player::setCoordinates(std::pair <int, int> new_locate){
    locate = new_locate;
}

bool Player::addSpell(spellType spell, int damage, int range){
    return hand.addSpell(spell, damage, range);
}

bool Player::removeSpell(size_t index){
    return hand.removeSpell(index);
}

void Player::clearHand(){
    hand.clear();
}

void Player::levelUpSpells(){
    hand.levelUp();
}

bool Player::cast(size_t spell_id, Field& field, EnemyManager& enemyManager, EnemyBuilding& enemyBuilding, std::pair <int, int> target){
    return hand.cast(spell_id, *this, field, enemyManager, enemyBuilding, target);
}

bool Player::cast(size_t spell_id, Field& field, EnemyManager& enemyManager, EnemyBuilding& enemyBuilding, EnemyTower& enemyTower, AllyManager& allyManager){
    return hand.cast(spell_id, *this, field, enemyManager, enemyBuilding, enemyTower, allyManager);
}

bool Player::cast(size_t spell_id){
    return hand.cast(spell_id);
}