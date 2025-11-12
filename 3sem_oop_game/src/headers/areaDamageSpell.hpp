#ifndef AREA_DAMAGE_SPELL
#define AREA_DAMAGE_SPELL

#include "spell.hpp"

class AreaDamageSpell : public Spell{
    private:
        int damage;
        int range;

        void isValidArguments();
        
    public:
        AreaDamageSpell(int damage, int range);
    
        std::string getName() const override;
        std::string getDescription() const override;
        std::string getInfo() const override;
        
        int getRange() const;
        int getDamage() const;

        void setRange(int delta);
        void setDamage(int delta);
        
        bool cast(Player& player, Field& field, EnemyManager& enemyManager,
            EnemyBuilding& enemyBuilding, std::pair <int, int> target, size_t improve) override;

        bool cast(Player& player, Field& field, EnemyManager& enemyManager,
            EnemyBuilding& enemyBuilding, EnemyTower& enemyTower, AllyManager& allyManager, size_t improve) override;
            
        bool cast(Player& target, std::pair <int, int> locate) override;

        bool cast() override;
};

#endif