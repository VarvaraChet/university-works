#ifndef ENEMY
#define ENEMY

#include <stdexcept>

class Enemy{
    private:
        int health;
        int damage;
        int hits;

        std::pair <int, int> locate;

        void isValidArguments();

    public:
        Enemy(int damage, int health, std::pair <int, int> locate);

        int getHealth() const;
        int getDamage() const;
        int getHits() const;
        std::pair <int, int> getCoordinates() const;

        void setDamage(int delta);
        void addHit();
        bool loseHealth(int delta);
        void setCoordinates(std::pair <int, int> new_locate);
};

#endif