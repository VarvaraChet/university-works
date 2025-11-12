#ifndef HAND
#define HAND

#include <stdexcept>
#include <vector>
#include <random>

#include "spell.hpp"
#include "areaDamageSpell.hpp"
#include "directDamageSpell.hpp"
#include "trapSpell.hpp"
#include "allySpell.hpp"
#include "improveSpell.hpp"

class Field;
class Player;
class EnemyManager;
class EnemyBuilding;
class EnemyTower;
class AllyManager;

class PlayerHand{
    private:
        std::vector <Spell*> spells;
        size_t maxSize;

        size_t improve;

        void isValidArguments();

    public:
        PlayerHand(size_t maxSize);
        ~PlayerHand();

        bool addSpell(spellType spell, int damage, int range);
        bool removeSpell(size_t index);
        void clear();
        void levelUp();
        
        size_t getSize() const;
        size_t getMaxSize() const;
        bool isFull() const;
        
        Spell* getSpell(size_t index) const;
        const std::vector<Spell*>& getSpells() const;

        bool cast(size_t spell_id, Player& player, Field& field, EnemyManager& enemyManager, EnemyBuilding& enemyBuilding, std::pair <int, int> target);
        bool cast(size_t spell_id, Player& player, Field& field, EnemyManager& enemyManager, EnemyBuilding& enemyBuilding, EnemyTower& enemyTower, AllyManager& allyManager);
        bool cast(size_t spell_id);
};

#endif