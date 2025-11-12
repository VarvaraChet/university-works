#ifndef GAME_STATE
#define GAME_STATE

#include "enemy.hpp"
#include <string>

struct GameState{
    enum State{
        MAIN_MENU,
        PLAYING,
        LEVEL_COMPLETE,
        GAME_OVER,
        UPGRADING,
        BUYING_SPELL
    };
    
    State state;
    
    bool showingAttackRadius;
    bool castingSpell;
    bool showingEnemyInfo;
    bool showingSpellRadius;

    int selectedSpellIndex;
    std::pair <int, int> spellTarget;

    Enemy* selectedEnemy;
    
    GameState(State initialState = MAIN_MENU) 
        : state(initialState),
          showingAttackRadius(false),
          castingSpell(false),
          showingEnemyInfo(false),
          showingSpellRadius(false),
          selectedSpellIndex(-1),
          selectedEnemy(nullptr) {}
};

#endif