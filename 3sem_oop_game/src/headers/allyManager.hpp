#ifndef ALLY_MANAGER
#define ALLY_MANAGER

#include <vector>

#include "ally.hpp"

class AllyManager{
    private:
        std::vector <Ally*> allies;

    public:
        ~AllyManager();
        
        void addAlly(Ally* ally);
        void deleteAlly(Ally* ally);

        Ally* getAllyAtCoordinates(std::pair <int, int> locate) const;

        bool attackAlly(std::pair <int, int> locate, int damage);

        std::vector <Ally*> getAllies() const;
};

#endif