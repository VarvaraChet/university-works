#include "../headers/playerHand.hpp"

void PlayerHand::isValidArguments(){
    if (maxSize < 1)
        throw std::invalid_argument("Incorrect hand size");
}

PlayerHand::PlayerHand(size_t maxSize)
    : maxSize(maxSize), improve(0) {
    isValidArguments();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 100);
    
    spellType new_spell=(spellType)(dist(gen)%5);

    addSpell(new_spell, 10, 6);
}

PlayerHand::~PlayerHand(){
    while (spells.size())
        spells.pop_back();
}

bool PlayerHand::addSpell(spellType spell, int damage, int range){
    if (spells.size() >= maxSize)
        return false;

    Spell* new_spell;
    switch (spell){
        case spellType::AREA_DAMAGE:
            new_spell = new AreaDamageSpell(damage, range);
            break;

        case spellType::DIRECT_DAMAGE:
            new_spell = new DirectDamageSpell(damage, range);
            break;

        case spellType::TRAP:
            new_spell = new TrapSpell(damage, range);
            break;

        case spellType::ALLY:
            new_spell = new AllySpell(damage/2, range*4);
            break;

        case spellType::IMPROVE:
            new_spell = new ImproveSpell();
            break;
    }

    spells.push_back(new_spell);
    return true;
}

bool PlayerHand::removeSpell(size_t index){
    if (index >= spells.size())
        return false;

    spells.erase(spells.begin()+index);
    return true;
}

void PlayerHand::clear(){
    while (spells.size() > 0)
        spells.erase(spells.begin());
}

void PlayerHand::levelUp(){
    for (Spell* spell : spells){
        if (dynamic_cast<AreaDamageSpell*>(spell))
            dynamic_cast<AreaDamageSpell*>(spell)->setRange(dynamic_cast<AreaDamageSpell*>(spell)->getRange()+1);
        else if (dynamic_cast<DirectDamageSpell*>(spell))
            dynamic_cast<DirectDamageSpell*>(spell)->setRange(dynamic_cast<DirectDamageSpell*>(spell)->getRange()+1);
        else if (dynamic_cast<TrapSpell*>(spell))
            dynamic_cast<TrapSpell*>(spell)->setRange(dynamic_cast<TrapSpell*>(spell)->getRange()+1);
        else if (dynamic_cast<AllySpell*>(spell))
            dynamic_cast<AllySpell*>(spell)->setRange(dynamic_cast<AllySpell*>(spell)->getRange()+1);
    }
}

size_t PlayerHand::getSize() const{
    return spells.size();
}

size_t PlayerHand::getMaxSize() const{
    return maxSize;
}

bool PlayerHand::isFull() const{
    return spells.size() >= maxSize;
}

Spell* PlayerHand::getSpell(size_t index) const{
    if (index >= spells.size())
        return nullptr;

    return spells[index];
}

const std::vector<Spell*>& PlayerHand::getSpells() const{
    return spells;
}

bool PlayerHand::cast(size_t spell_id, Player& player, Field& field, EnemyManager& enemyManager, EnemyBuilding& enemyBuilding, std::pair <int, int> target){
    if (spell_id >= spells.size())
        return false;

    bool check = spells[spell_id]->cast(player, field, enemyManager, enemyBuilding, target, improve);
    if (check)
        improve = 0;

    return check;
}

bool PlayerHand::cast(size_t spell_id, Player& player, Field& field, EnemyManager& enemyManager, EnemyBuilding& enemyBuilding, EnemyTower& enemyTower, AllyManager& allyManager){
    if (spell_id >= spells.size())
        return false;

    bool check = spells[spell_id]->cast(player, field, enemyManager, enemyBuilding, enemyTower, allyManager, improve);
    if (check)
        improve = 0;

    return check;
}

bool PlayerHand::cast(size_t spell_id){
    if (spell_id >= spells.size())
        return false;

    bool check = spells[spell_id]->cast();
    if (check)
        improve++;

    return check;
}