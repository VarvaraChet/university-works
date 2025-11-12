#include "../headers/field.hpp"

void Field::filloutGrid(){
    for (int i=0; i < height; i++){
        for (int j=0; j < width; j++){
            cellType type=cellType::EMPTY;
            if ((i+j+i*j)%5 == 0){
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<int> dist(0, 100);
                int rand = dist(gen);

                type = (cellType)(rand%3);
            }
            
            grid[i][j] = new Cell(type);
        }
    }
}

void Field::isValidArguments(){
    if (width < 10  || height < 10 || width > 25 || height > 25)
        throw std::invalid_argument("Incorrect field size");
}

Field::Field(int width, int height){
    this->width = width;
    this->height = height;

    isValidArguments();
    
    grid.resize(height, std::vector <Cell*>(width));

    filloutGrid();
}

Field::Field(const Field& other_field) {   
    width = other_field.getWidth();
    height = other_field.getHeight();

    grid.resize(height, std::vector <Cell*>(width, nullptr));

    for (int i=0; i < height; i++){
        for (int j=0; j < width; j++){
            Cell* cell=other_field.getCell(std::make_pair(j, i));
            if (cell)
                grid[i][j] = new Cell(cell->getType());
        }
    }
}

Field::Field(Field&& other_field) {
    if (this != &other_field){
        width = other_field.getWidth();
        height = other_field.getHeight();

        grid = std::move(other_field.grid);

        other_field.width = 0;
        other_field.height = 0;
    }
}

Field::~Field(){
    for (int i=0; i < height; i++){
        for (int j=0; j < width; j++){
            if (grid[i][j])
                delete grid[i][j];

            grid[i][j] = nullptr;
        }
    }
    width = 0; height = 0;
}

Field& Field::operator=(const Field& other_field){
    Field temp(other_field);
    
    std::swap(width, temp.width);
    std::swap(height, temp.height);
    std::swap(grid, temp.grid);
    
    return *this;
}

Field& Field::operator=(Field&& other_field){
    if (this != &other_field){
        for (int i=0; i < height; i++){
            for (int j=0; j < width; j++){
                if (grid[i][j])
                    delete grid[i][j];
                
                grid[i][j] = nullptr;
            }
        }

        width = other_field.getWidth();
        height = other_field.getHeight();

        grid = std::move(other_field.grid);

        other_field.width = 0;
        other_field.height = 0;
    }

    return *this;
}

int Field::getWidth() const{
    return width;
}

int Field::getHeight() const{
    return height;
}

Cell* Field::getCell(std::pair <int, int> coordinates) const {
    if (coordinates.first < 0 || coordinates.first >= width || coordinates.second < 0 || coordinates.second >= height)
        throw std::invalid_argument("Incorrect cell coordinates");
    return grid[coordinates.second][coordinates.first];
}