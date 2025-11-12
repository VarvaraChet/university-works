#include "../gui_headers/textureManager.hpp"

TextureManager::TextureManager() {}

void TextureManager::loadTexture(const std::string& name, const std::string& filename) {
    sf::Texture texture;
    if (!texture.loadFromFile(filename)) {
        throw std::runtime_error("Failed to load texture: " + filename);
    }
    textures[name] = texture;
}

sf::Texture& TextureManager::getTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it == textures.end()) {
        throw std::runtime_error("Texture not found: " + name);
    }
    return it->second;
}

bool TextureManager::hasTexture(const std::string& name) const {
    return textures.find(name) != textures.end();
}