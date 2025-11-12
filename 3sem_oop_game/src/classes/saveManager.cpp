#include "../headers/saveManager.hpp"

namespace fs = std::filesystem;

SaveManager::SaveManager(){
    if (!fs::exists("saves")){
        fs::create_directory("saves");
    }
}

std::string SaveManager::getFullPath() const{
    return "saves/"+saveFileName;
}

void SaveManager::saveGame(const SaveData& data){
    std::string fullPath = getFullPath();
    
    try{
        std::ofstream file(fullPath);
        if (!file.is_open())
            throw SaveException("Cannot open file for writing: "+fullPath);
        
        json j = data.toJson();
        j["metadata"] = {
           {"version", "1.0"},
           {"timestamp", std::time(nullptr)},
           {"game", "Game"},
           {"type", "autosave"}
        };
        
        file << j.dump(4);

        if (!file.good())
            throw SaveException("Write operation failed for: "+fullPath); 

        file.close();
    }
    catch (const std::ofstream::failure& e){
        throw SaveException("File operation failed: "+std::string(e.what()));
    }
    catch (const json::exception& e){
        throw SaveException("JSON serialization error: "+std::string(e.what()));
    }
}

bool SaveManager::loadGame(SaveData& data){
    std::string fullPath = getFullPath();
    
    if (!fs::exists(fullPath)){
        std::cout << "Save file not found: " << fullPath << std::endl;
        return false;
    }
    
    try{
        std::ifstream file(fullPath);
        if (!file.is_open()){
            std::cout << "Cannot open save file: " << fullPath << std::endl;
            return false;
        }
        
        file.seekg(0, std::ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        
        if (fileSize == 0){
            std::cout << "Save file is empty: " << fullPath << std::endl;
            return false;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string jsonStr = buffer.str();
        
        if (jsonStr.empty()){
            std::cout << "Save file content is empty: " << fullPath << std::endl;
            return false;
        }
        
        json j = json::parse(jsonStr);
        
        if (!j.contains("metadata") || !j["metadata"].contains("game")){
            std::cout << "Invalid save file format: " << fullPath << std::endl;
            return false;
        }
        
        data = SaveData::fromJson(j);
        
        std::cout << "Game loaded from auto-save: " << fullPath << std::endl;
        return true;
        
    }
    catch (const std::ifstream::failure& e){
        std::cout << "File operation failed: " << e.what() << std::endl;
        return false;
    }
    catch (const json::parse_error& e){
        std::cout << "JSON parse error in save file: " << e.what() << std::endl;
        return false;
    }
    catch (const json::exception& e){
        std::cout << "JSON processing error: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception& e){
        std::cout << "Error loading save: " << e.what() << std::endl;
        return false;
    }
}