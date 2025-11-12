#ifndef ALLY_HPP
#define ALLY_HPP

#include <stdexcept>

class Ally{
    private:
        int health;
        int damage;

        std::pair <int, int> locate;

        void isValidArguments();

    public:
        Ally(int damage, int health, std::pair <int, int> locate);

        int getHealth() const;
        int getDamage() const;
        std::pair <int, int> getCoordinates() const;

        void setDamage(int delta);
        bool loseHealth(int delta);
        void setCoordinates(std::pair <int, int> new_locate);
};

#endif