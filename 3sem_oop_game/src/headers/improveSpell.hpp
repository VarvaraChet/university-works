#ifndef IMPROVE_SPELL
#define IMPROVE_SPELL

#include "spell.hpp"

class ImproveSpell : public Spell{
    public:
        std::string getName() const override;
        std::string getDescription() const override;
        std::string getInfo() const override;

        bool cast(Player& player, Field& field, EnemyManager& enemyManager,
            EnemyBuilding& enemyBuilding, std::pair <int, int> target, size_t improve) override;

        bool cast(Player& player, Field& field, EnemyManager& enemyManager,
            EnemyBuilding& enemyBuilding, EnemyTower& enemyTower, AllyManager& allyManager, size_t improve) override;

        bool cast(Player& target, std::pair <int, int> locate) override;

        bool cast() override;
};

#endif