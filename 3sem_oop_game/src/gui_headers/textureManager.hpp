#ifndef TEXTURE_MANAGER
#define TEXTURE_MANAGER

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <stdexcept>

class TextureManager {
private:
    std::map<std::string, sf::Texture> textures;

public:
    TextureManager();
    void loadTexture(const std::string& name, const std::string& filename);
    sf::Texture& getTexture(const std::string& name);
    bool hasTexture(const std::string& name) const;
};

#endif