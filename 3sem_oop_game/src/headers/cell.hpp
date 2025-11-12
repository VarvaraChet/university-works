#ifndef CELL
#define CELL

#include <stdexcept>

enum class cellType{
    EMPTY,
    OBSTACLE,
    SLOW,
    TRAP
};

class Cell{
    private:
        int damage;
        cellType type;

    public:
        Cell();
        Cell(cellType type);

        int getDamage() const;
        cellType getType() const;

        void setDamage(int damage);
        void setType(cellType type);
};

#endif