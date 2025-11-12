#ifndef FIELD
#define FIELD

#include <vector>
#include <stdexcept>
#include <random>

#include "cell.hpp"

class Field{
    private:
        int width;
        int height;
        std::vector <std::vector <Cell*>> grid;

        void filloutGrid();
        void isValidArguments();

    public:
        Field(int width, int height);
        Field(const Field& other_field);
        Field(Field&& other_field);
        ~Field();

        Field& operator=(const Field& other_field);
        Field& operator=(Field&& otherField);

        int getWidth() const;
        int getHeight() const;
        Cell* getCell(std::pair <int, int> coordinates) const;
};

#endif