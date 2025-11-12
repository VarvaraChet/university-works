#ifndef SAVE_MANAGER
#define SAVE_MANAGER

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include <filesystem>

#include "../libs/json.hpp"

using json = nlohmann::json;

struct SaveData{
    int level = 1;
    int moveCount = 0;
    bool levelCompleted;
    int savedState;

    int playerHealth = 0;
    int playerMaxHealth = 0;
    int playerDamage = 0;
    int playerMana = 0;
    int playerPoints = 0;
    std::string playerFightType = "NEAR";
    std::string playerMove = "CAN";
    std::pair <int, int> playerPosition = {0, 0};
    
    int fieldWidth = 0;
    int fieldHeight = 0;
    
    struct CellData{
        std::string type = "EMPTY";
        int damage = 0;
        
        json toJson() const{
            return {
               {"type", type},
               {"damage", damage}
            };
        }
        
        static CellData fromJson(const json& j){
            CellData data;
            data.type = j.value("type", "EMPTY");
            data.damage = j.value("damage", 0);
            return data;
        }
    };
    std::vector <std::vector <CellData>> fieldCells;
    
    struct SpellData{
        std::string type = "DIRECT_DAMAGE";
        int damage = 0;
        int range = 0;
        
        json toJson() const{
            return {
               {"type", type},
               {"damage", damage},
                {"range", range}
            };
        }
        
        static SpellData fromJson(const json& j){
            SpellData data;
            data.type = j.value("type", "DIRECT_DAMAGE");
            data.damage = j.value("damage", 0);
            data.range = j.value("range", 0);
            return data;
        }
    };
    std::vector <SpellData> playerSpells;

    struct EnemyData{
        int health = 0;
        int damage = 0;
        std::pair <int, int> position ={0, 0};
        
        json toJson() const{
            return{
               {"health", health},
               {"damage", damage},
               {"position",{position.first, position.second}}
            };
        }
        
        static EnemyData fromJson(const json& j){
            EnemyData data;
            data.health = j.value("health", 0);
            data.damage = j.value("damage", 0);
            auto pos = j.value("position", std::vector <int>{0, 0});
            if (pos.size() == 2)
                data.position ={pos[0], pos[1]};

            return data;
        }
    };
    std::vector <EnemyData> enemies;
    
    struct AllyData{
        int health = 0;
        int damage = 0;
        std::pair <int, int> position ={0, 0};
        
        json toJson() const{
            return {
               {"health", health},
               {"damage", damage},
               {"position",{position.first, position.second}}
            };
        }
        
        static AllyData fromJson(const json& j){
            AllyData data;
            data.health = j.value("health", 0);
            data.damage = j.value("damage", 0);
            auto pos = j.value("position", std::vector <int>{0, 0});
            if (pos.size() == 2)
                data.position ={pos[0], pos[1]};
                
            return data;
        }
    };
    std::vector <AllyData> allies;
    
    std::pair <int, int> buildingPosition = {0, 0};
    int buildingHealth = 0;
    std::pair <int, int> towerPosition = {0, 0};
    
    json toJson() const{
        json j;
        j["level"] = level;
        j["moveCount"] = moveCount;
        j["levelCompleted"] = levelCompleted;
        j["savedState"] = savedState;
        j["player"] ={
           {"health", playerHealth},
           {"maxHealth", playerMaxHealth},
           {"damage", playerDamage},
           {"mana", playerMana},
           {"points", playerPoints},
           {"fightType", playerFightType},
           {"move", playerMove},
           {"position",{playerPosition.first, playerPosition.second}}
        };
        j["field"] ={
           {"width", fieldWidth},
           {"height", fieldHeight}
        };
        
        json fieldJson = json::array();
        for (int y = 0; y < fieldHeight; y++){
            json rowJson = json::array();
            for (int x = 0; x < fieldWidth; x++){
                if (y < fieldCells.size() && x < fieldCells[y].size())
                    rowJson.push_back(fieldCells[y][x].toJson());
                else
                    rowJson.push_back(CellData().toJson());
            }
            fieldJson.push_back(rowJson);
        }
        j["fieldCells"] = fieldJson;
        
        json spellsJson = json::array();
        for (const auto& spell : playerSpells)
            spellsJson.push_back(spell.toJson());
        j["playerSpells"] = spellsJson;

        json enemiesJson = json::array();
        for (const auto& enemy : enemies)
            enemiesJson.push_back(enemy.toJson());
        j["enemies"] = enemiesJson;
        
        json alliesJson = json::array();
        for (const auto& ally : allies)
            alliesJson.push_back(ally.toJson());
        j["allies"] = alliesJson;
        
        j["building"] ={
           {"position",{buildingPosition.first, buildingPosition.second}},
           {"health", buildingHealth}
        };

        j["tower"] ={
           {"position",{towerPosition.first, towerPosition.second}}
        };
        
        return j;
    }
    
    static SaveData fromJson(const json& j){
        SaveData data;
        
        try{
            data.level = j.value("level", 1);
            data.moveCount = j.value("moveCount", 0);
            data.levelCompleted = j.value("levelCompleted", false);
            data.savedState = j.value("savedState", 0);
            
            if (j.contains("player")){
                auto playerJson = j["player"];
                data.playerHealth = playerJson.value("health", 0);
                data.playerMaxHealth = playerJson.value("maxHealth", 0);
                data.playerDamage = playerJson.value("damage", 0);
                data.playerMana = playerJson.value("mana", 0);
                data.playerPoints = playerJson.value("points", 0);
                data.playerFightType = playerJson.value("fightType", "NEAR");
                data.playerMove = playerJson.value("move", "CAN");
                auto playerPos = playerJson.value("position", std::vector <int>{0, 0});
                if (playerPos.size() == 2)
                    data.playerPosition ={playerPos[0], playerPos[1]};
            }
            
            if (j.contains("field")){
                auto fieldJson = j["field"];
                data.fieldWidth = fieldJson.value("width", 25);
                data.fieldHeight = fieldJson.value("height", 20);
            }
            
            if (j.contains("fieldCells")){
                data.fieldCells.resize(data.fieldHeight);
                for (int y = 0; y < data.fieldHeight; y++){
                    data.fieldCells[y].resize(data.fieldWidth);
                    if (y < j["fieldCells"].size()){
                        auto rowJson = j["fieldCells"][y];
                        for (int x = 0; x < data.fieldWidth; x++){
                            if (x < rowJson.size())
                                data.fieldCells[y][x] = CellData::fromJson(rowJson[x]);
                        }
                    }
                }
            }
            
            if (j.contains("playerSpells")){
                for (const auto& spellJson : j["playerSpells"])
                    data.playerSpells.push_back(SpellData::fromJson(spellJson));
            }

            if (j.contains("enemies")){
                for (const auto& enemyJson : j["enemies"])
                    data.enemies.push_back(EnemyData::fromJson(enemyJson));
            }
            
            if (j.contains("allies")){
                for (const auto& allyJson : j["allies"])
                    data.allies.push_back(AllyData::fromJson(allyJson));
            }
            
            if (j.contains("building")){
                auto buildingJson = j["building"];
                data.buildingHealth = buildingJson.value("health", 0);
                auto buildingPos = buildingJson.value("position", std::vector <int>{0, 0});
                if (buildingPos.size() == 2)
                    data.buildingPosition ={buildingPos[0], buildingPos[1]};
            }
            
            if (j.contains("tower")){
                auto towerJson = j["tower"];
                auto towerPos = towerJson.value("position", std::vector <int>{0, 0});
                if (towerPos.size() == 2)
                    data.towerPosition ={towerPos[0], towerPos[1]};
            }
            
        }
        catch (const json::exception& e){
            throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
        }
        
        return data;
    }
};

class SaveException : public std::runtime_error{
    public:
        explicit SaveException(const std::string& message) 
            : std::runtime_error("Save Error: " + message){}
};

class SaveManager{
    private:
        std::string saveFileName = "autosave.json";

        std::string getFullPath() const;

    public:
        SaveManager();
        
        void saveGame(const SaveData& data);
        bool loadGame(SaveData& data);
};

#endif