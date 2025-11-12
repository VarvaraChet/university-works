#ifndef ENEMY_TOWER
#define ENEMY_TOWER

#include <stdexcept>
#include <utility>

#include "player.hpp"
#include "directDamageSpell.hpp"

class EnemyTower{
    private:
        std::pair <int, int> locate;
        
        bool attack;
        DirectDamageSpell spell;

    public:
        EnemyTower(int damage, int range, std::pair <int, int> locate);

        void attackPlayer(Player& player);

        int getRange() const;
        std::pair <int, int> getCoordinates() const;
};

#endif