#include "../headers/gameManager.hpp"

bool GameManager::validCell(std::pair <int, int> locate){
    if (locate.first < 0 || locate.first >= field.getWidth() || locate.second < 0 || locate.second >= field.getHeight())
        return false;
    if (field.getCell(locate)->getType() == cellType::OBSTACLE || enemyBuilding.getCoordinates() == locate || enemyTower.getCoordinates() == locate)
        return false;

    return true;
}

GameManager::GameManager() 
    : field(25, 20),
      player(150, 10, std::make_pair(0, 0), 6, fightType::NEAR),
      enemyBuilding(5, 0, 50, std::make_pair(field.getWidth()-1, field.getHeight()-1)),
      enemyTower(5, 3, std::make_pair(field.getWidth()/4, field.getHeight()/2)),
      currentLevel(1),
      moveCount(0),
      levelCompleted(false){

    field.getCell(std::make_pair(0, 0))->setType(cellType::EMPTY);
    
    std::pair <int, int> enemyPos ={field.getWidth()/2, field.getHeight()/2};
    Enemy* enemy = new Enemy(10, 50, enemyPos);
    enemyManager.addEnemy(enemy);
}

fightType GameManager::convertFightType(const std::string& fightTypeStr){
    if (fightTypeStr == "FAR")
        return fightType::FAR;

    return fightType::NEAR;
}

moveType GameManager::convertMove(const std::string& moveStr){
    if (moveStr == "CAN")
        return moveType::CAN;

    return moveType::NOT;
}

Player GameManager::getPlayer() const{
    return player;
}

AllyManager GameManager::getAllyManager() const{
    return allyManager;
}

EnemyManager GameManager::getEnemyManager() const{
    return enemyManager;
}

EnemyBuilding GameManager::getEnemyBuilding() const{
    return enemyBuilding;
}

EnemyTower GameManager::getEnemyTower() const{
    return enemyTower;
}

Field GameManager::getField() const{
    return field;
}

bool GameManager::movePlayer(direction direct){
    moveType canMove = player.getMove();
    std::pair <int, int> old_locate = player.getCoordinates();
    std::pair <int, int> new_locate = old_locate;

    if (canMove == moveType::NOT)
        return false;

    if (direct == direction::UP)
        new_locate.second--;
    if (direct == direction::DOWN)
        new_locate.second++;
    if (direct == direction::LEFT)
        new_locate.first--;
    if (direct == direction::RIGHT)
        new_locate.first++;

    if (!validCell(new_locate) || field.getCell(new_locate)->getType() == cellType::TRAP)
        return false;
    
    int point = enemyManager.attackEnemy(new_locate, player.getDamage());
    if (point){
        player.addPoints(point);

        if (point > 5)
            player.setMana(30);
    }
    else{
        player.setCoordinates(new_locate);
        if (field.getCell(new_locate)->getType() == cellType::SLOW)
            player.setMove(moveType::NOT);
    }

    moveCount++;
    return true;
}

void GameManager::moveAlly(Ally& ally, direction direct){
    std::pair <int, int> old_locate = ally.getCoordinates();
    std::pair <int, int> new_locate = old_locate;

    if (direct == direction::UP)
        new_locate.second--;
    if (direct == direction::DOWN)
        new_locate.second++;
    if (direct == direction::LEFT)
        new_locate.first--;
    if (direct == direction::RIGHT)
        new_locate.first++;

    if (!validCell(new_locate) || field.getCell(new_locate)->getType() == cellType::TRAP
        || allyManager.getAllyAtCoordinates(new_locate) || new_locate == player.getCoordinates())
        return;

    int mana = enemyManager.attackEnemy(new_locate, ally.getDamage());
    if (mana)
        player.setMana(mana);
    else
        ally.setCoordinates(new_locate);
}

void GameManager::moveEnemy(Enemy& enemy, direction direct){
    std::pair <int, int> old_locate = enemy.getCoordinates();
    std::pair <int, int> new_locate = old_locate;

    if (direct == direction::UP)
        new_locate.second--;
    if (direct == direction::DOWN)
        new_locate.second++;
    if (direct == direction::LEFT)
        new_locate.first--;
    if (direct == direction::RIGHT)
        new_locate.first++;

    if (!validCell(new_locate) || enemyManager.getEnemyAtCoordinates(new_locate))
        return;

    Cell* cell=field.getCell(new_locate);
    if (cell->getType() == cellType::TRAP){
        bool isAlive = enemy.loseHealth(cell->getDamage());
        if (!isAlive)
            enemyManager.deleteEnemy(&enemy);

        cell->setDamage(0);
        cell->setType(cellType::EMPTY);
    }

    std::pair <int, int> player_locate=player.getCoordinates();
    if (new_locate == player_locate)
        player.setHealth(-enemy.getDamage());
    else if (!allyManager.attackAlly(new_locate, enemy.getDamage()))
        enemy.setCoordinates(new_locate);
}

void GameManager::playerAttack(){
    int radius=std::min(field.getHeight(), field.getWidth()), damage=player.getDamage();

    if (player.getFight() == fightType::NEAR)
        radius /= 10;
    else{
        radius /= 5;
        damage /= 2;
    }

    std::pair <int, int> player_locate=player.getCoordinates();
    int x=player_locate.first, y=player_locate.second;
    for (int i=std::max(0, y-radius); i < std::min(field.getHeight(), y+radius+1); i++){
        for (int j=std::max(0, (int)(x-std::sqrt(std::pow(radius, 2)-std::pow((i-y), 2)))); j < std::min(field.getWidth(), (int)(x+std::sqrt(std::pow(radius, 2)-std::pow((i-y), 2))+1)); j++){
            int points = enemyManager.attackEnemy(std::make_pair(j, i), damage);
            player.addPoints(points);

            if (points > 5)
                player.setMana(30);
        }
    }

    moveCount++;
}

void GameManager::allyAction(){
    for (Ally* ally : allyManager.getAllies()){
        std::pair <int, int> ally_locate = ally->getCoordinates();
        std::pair <int, int> target = enemyManager.findNearEnemy(ally_locate);

        direction move_direct;
        int dx=target.first-ally_locate.first;
        int dy=target.second-ally_locate.second;

        if (abs(dx) >= abs(dy))
            move_direct = (dx > 0) ? direction::RIGHT : direction::LEFT;
        else
            move_direct = (dy > 0) ? direction::DOWN : direction::UP;

        moveAlly(*ally, move_direct);
    }
}

void GameManager::enemyAction(){
    std::pair <int, int> player_coord = player.getCoordinates();

    for (Enemy* enemy : enemyManager.getEnemies()){
        std::pair <int, int> enemy_coord = enemy->getCoordinates();

        direction move_direct;
        int dx=player_coord.first-enemy_coord.first;
        int dy=player_coord.second-enemy_coord.second;

        if (abs(dx) >= abs(dy))
            move_direct = (dx > 0) ? direction::RIGHT : direction::LEFT;
        else
            move_direct = (dy > 0) ? direction::DOWN : direction::UP;

        moveEnemy(*enemy, move_direct);
    }
}

endType GameManager::checkGameEnd(){
    if (!player.isAlive())
        return endType::LOSE;

    if (player.getPoints() >= 100+(currentLevel-1)*20 || !enemyBuilding.isExist()){
        levelCompleted = true;
        return endType::WIN;
    }

    return endType::CONTINUE;
}

void GameManager::spawnEnemy(int damage, int health){
    bool done=false;
    std::pair <int, int> locate = enemyBuilding.getCoordinates();
    int x=locate.first, y=locate.second;

    for (int dy=-1; dy <= 1 && !done; dy++){
        for (int dx=-1; dx <= 1 && !done; dx++){
            if ((dx != 0 || dy != 0) && x+dx >= 0 && x+dx < field.getWidth() && y+dy >= 0 && y+dy < field.getHeight()){
                std::pair <int, int> enemy_locate ={x+dx, y+dy};
                if (!(enemyManager.getEnemyAtCoordinates(enemy_locate)) && player.getCoordinates() != enemy_locate){
                    enemyBuilding.spawnEnemy(damage, health, enemy_locate, enemyManager);
                    done = true;
                }
            }
        }
    }
}

void GameManager::changePlayerMove(){
    if (player.getMove() == moveType::NOT)
        player.setMove(moveType::CAN);
    else
        player.setMove(moveType::NOT);
}

void GameManager::changeFight(){
    player.changeFight();
    moveCount++;

    player.setMove(moveType::CAN);
}

void GameManager::towerAction(){
    enemyTower.attackPlayer(player);
}

bool GameManager::castSpell(size_t spell_id, std::pair <int, int> target){
    bool cast_check = player.cast(spell_id, field, enemyManager, enemyBuilding, target);
    if (!cast_check)
        cast_check = player.cast(spell_id, field, enemyManager, enemyBuilding, enemyTower, allyManager);
    if (!cast_check)
        cast_check = player.cast(spell_id);

    if (cast_check){
        player.addPoints(10);
        player.removeSpell(spell_id);

        moveCount++;
        player.setMove(moveType::CAN);
    }
    
    return cast_check;
}

bool GameManager::addSpell(spellType spell){
    if (player.getMana() < 40)
        return false;
    
    bool add_check = player.addSpell(spell, 10+currentLevel*2, 5+currentLevel);
    if (add_check)
        player.setMana(-40);
    
    player.setMove(moveType::CAN);
    return add_check;
}

void GameManager::setupPlayerForLevel(int level){
    std::pair <int, int> startPosition ={0, 0};
    player.setCoordinates(startPosition);
    
    field.getCell(startPosition)->setType(cellType::EMPTY);
    
    player.setHealth(player.getMaxHealth());
    player.setPoints(0);
    player.levelUpSpells();
}

void GameManager::generateLevel(int level){
    if (level%2 != 0)
        field = Field(25, 20);
    else
        field = Field(23, 18);
    
    setupPlayerForLevel(level);
    
    allyManager = AllyManager();
    enemyManager = EnemyManager();
    
    std::pair <int, int> enemyPos ={field.getWidth()/2, field.getHeight()/2};
    Enemy* enemy = new Enemy(10+level*3, 50+level*10, enemyPos);
    enemyManager.addEnemy(enemy);
    field.getCell(enemyPos)->setType(cellType::EMPTY);

    enemyBuilding = EnemyBuilding(5, 0, 50+level*10, std::make_pair(field.getWidth()-1, field.getHeight()-1));
    enemyTower = EnemyTower(5+level, 3, std::make_pair(field.getWidth()/4, field.getHeight()/2));
}

void GameManager::removeHalfSpells(){
    PlayerHand hand = player.getHand();
    size_t spellCount = hand.getSize();
    if (spellCount <= 1)
        return;
    
    size_t spellsToRemove = spellCount/2;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::vector <size_t> indicesToRemove;
    while (indicesToRemove.size() < spellsToRemove){
        if (hand.getSize() > 0){
            std::uniform_int_distribution <size_t> dist(0, hand.getSize()-1);
            size_t index = dist(gen);

            if (std::find(indicesToRemove.begin(), indicesToRemove.end(), index) == indicesToRemove.end())
                indicesToRemove.push_back(index);
        }
    }
    
    std::sort(indicesToRemove.rbegin(), indicesToRemove.rend());
    for (size_t index : indicesToRemove)
        player.removeSpell(index);
}

void GameManager::nextLevel(int newLevel){
    currentLevel = newLevel;
    levelCompleted = false;
    moveCount = 0;
        
    removeHalfSpells();
    
    generateLevel(newLevel);
}

void GameManager::setCurrentLevel(int level){
    currentLevel = level;
}

void GameManager::upgradeRandomSpell(){
    PlayerHand hand = player.getHand();
    if (hand.getSize() == 0)
        return;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution <size_t> dist(0, hand.getSize()-1);
    size_t index = dist(gen);
    
    int attempts=0;
    Spell* spell = hand.getSpell(index);
    while (attempts < 10 && !spell || dynamic_cast<ImproveSpell*>(spell) != nullptr){
        dist = std::uniform_int_distribution <size_t>(0, hand.getSize()-1);
        index = dist(gen);

        spell = hand.getSpell(index);
        attempts++;
    }
    
    if (auto* damageSpell = dynamic_cast<DirectDamageSpell*>(spell))
        damageSpell->setDamage(5);
    else if (auto* areaSpell = dynamic_cast<AreaDamageSpell*>(spell))
        areaSpell->setDamage(3);
    else if (auto* trapSpell = dynamic_cast<TrapSpell*>(spell))
        trapSpell->setDamage(5);
    else if (auto* allySpell = dynamic_cast<AllySpell*>(spell))
        allySpell->setDamage(5);
}

bool GameManager::applyUpgrade(UpgradeType type){
    switch (type){
        case UpgradeType::HEALTH:
            player.setMaxHealth(player.getMaxHealth()+20);
            return true;
            
        case UpgradeType::DAMAGE:
            player.setDamage(player.getDamage()+5);
            return true;
            
        case UpgradeType::MANA:
            player.setMana(player.getMana()+30);
            return true;
            
        case UpgradeType::SPELL:
            upgradeRandomSpell();
            return true;
    }
    return false;
}

int GameManager::getCurrentLevel() const{
    return currentLevel;
}
int GameManager::getMoveCount() const{
    return moveCount;
}

bool GameManager::isLevelCompleted() const{
    return levelCompleted;
}

SaveData GameManager::getSaveData() const{
    SaveData data;
    
    data.level = currentLevel;
    data.moveCount = moveCount;
    data.levelCompleted = levelCompleted;

    data.playerHealth = player.getHealth();
    data.playerMaxHealth = player.getMaxHealth();
    data.playerDamage = player.getDamage();
    data.playerMana = player.getMana();
    data.playerPoints = player.getPoints();
    data.playerFightType = (player.getFight() == fightType::NEAR) ? "NEAR" : "FAR";
    data.playerMove = (player.getMove() == moveType::CAN) ? "CAN" : "NOT";
    data.playerPosition = player.getCoordinates();
    
    data.fieldWidth = field.getWidth();
    data.fieldHeight = field.getHeight();
    
    data.fieldCells.resize(data.fieldHeight);
    for (int y = 0; y < data.fieldHeight; y++){
        data.fieldCells[y].resize(data.fieldWidth);
        for (int x = 0; x < data.fieldWidth; x++){
            SaveData::CellData cellData;
            Cell* cell = field.getCell({x, y});
            
            switch (cell->getType()){
                case cellType::EMPTY:
                    cellData.type = "EMPTY";
                    break;
                case cellType::OBSTACLE:
                    cellData.type = "OBSTACLE";
                    break;
                case cellType::SLOW:
                    cellData.type = "SLOW";
                    break;
                case cellType::TRAP:
                    cellData.type = "TRAP";
                    break;
                default:
                    cellData.type = "EMPTY";
            }
            
            cellData.damage = cell->getDamage();
            data.fieldCells[y][x] = cellData;
        }
    }
    
    PlayerHand hand = player.getHand();
    const auto& spells = hand.getSpells();
    for (const auto& spell : spells){
        SaveData::SpellData spellData;
        
        if (dynamic_cast<AreaDamageSpell*>(spell)){
            spellData.type = "AREA_DAMAGE";
            spellData.damage = dynamic_cast<AreaDamageSpell*>(spell)->getDamage();
            spellData.range = dynamic_cast<AreaDamageSpell*>(spell)->getRange();
        }
        else if (dynamic_cast<DirectDamageSpell*>(spell)){
            spellData.type = "DIRECT_DAMAGE";
            spellData.damage = dynamic_cast<DirectDamageSpell*>(spell)->getDamage();
            spellData.range = dynamic_cast<DirectDamageSpell*>(spell)->getRange();
        }
        else if (dynamic_cast<TrapSpell*>(spell)){
            spellData.type = "TRAP";
            spellData.damage = dynamic_cast<TrapSpell*>(spell)->getDamage();
            spellData.range = dynamic_cast<TrapSpell*>(spell)->getRange();
        }
        else if (dynamic_cast<AllySpell*>(spell)){
            spellData.type = "ALLY";
            spellData.damage = dynamic_cast<AllySpell*>(spell)->getDamage();
            spellData.range = dynamic_cast<AllySpell*>(spell)->getRange();
        }
        else if (dynamic_cast<ImproveSpell*>(spell)){
            spellData.type = "IMPROVE";
        }
        
        data.playerSpells.push_back(spellData);
    }

    for (Enemy* enemy : enemyManager.getEnemies()){
        SaveData::EnemyData enemyData;
        enemyData.health = enemy->getHealth();
        enemyData.damage = enemy->getDamage();
        enemyData.position = enemy->getCoordinates();
        data.enemies.push_back(enemyData);
    }
    
    for (Ally* ally : allyManager.getAllies()){
        SaveData::AllyData allyData;
        allyData.health = ally->getHealth();
        allyData.damage = ally->getDamage();
        allyData.position = ally->getCoordinates();
        data.allies.push_back(allyData);
    }
    
    data.buildingPosition = enemyBuilding.getCoordinates();
    data.buildingHealth = enemyBuilding.getStrong();
    
    data.towerPosition = enemyTower.getCoordinates();
    
    return data;
}

void GameManager::loadFromSaveData(const SaveData& data){
    currentLevel = data.level;
    moveCount = data.moveCount;
    levelCompleted = data.levelCompleted;
    
    Field tempField(data.fieldWidth, data.fieldHeight);
    field = tempField;

    for (int y = 0; y < data.fieldHeight; y++){
        for (int x = 0; x < data.fieldWidth; x++){
            if (y < data.fieldCells.size() && x < data.fieldCells[y].size()){
                const auto& cellData = data.fieldCells[y][x];
                Cell* cell = field.getCell({x, y});
                
                cellType type = cellType::EMPTY;
                if (cellData.type == "OBSTACLE")
                    type = cellType::OBSTACLE;
                else if (cellData.type == "SLOW")
                    type = cellType::SLOW;
                else if (cellData.type == "TRAP")
                    type = cellType::TRAP;
                
                cell->setType(type);
                cell->setDamage(cellData.damage);
            }
        }
    }
    
    player = Player(data.playerMaxHealth, data.playerDamage, data.playerPosition, 6, convertFightType(data.playerFightType));
    player.setHealth(data.playerHealth-data.playerMaxHealth);
    player.setMana(data.playerMana);
    player.addPoints(data.playerPoints);
    player.setMove(convertMove(data.playerMove));

    player.clearHand();
    for (const auto& spellData : data.playerSpells){
        if (spellData.type == "AREA_DAMAGE")
            player.addSpell(spellType::AREA_DAMAGE, spellData.damage, spellData.range);
        else if (spellData.type == "DIRECT_DAMAGE")
            player.addSpell(spellType::DIRECT_DAMAGE, spellData.damage, spellData.range);
        else if (spellData.type == "TRAP")
            player.addSpell(spellType::TRAP, spellData.damage, spellData.range);
        else if (spellData.type == "ALLY")
            player.addSpell(spellType::ALLY, spellData.damage, spellData.range);
        else if (spellData.type == "IMPROVE")
            player.addSpell(spellType::IMPROVE, 0, 0);
    }
    
    enemyManager = EnemyManager();
    for (const auto& enemyData : data.enemies){
        Enemy* enemy = new Enemy(enemyData.damage, enemyData.health, enemyData.position);
        enemyManager.addEnemy(enemy);
    }
    
    allyManager = AllyManager();
    for (const auto& allyData : data.allies){
        Ally* ally = new Ally(allyData.damage, allyData.health, allyData.position);
        allyManager.addAlly(ally);
    }
    
    EnemyBuilding tempBuilding(5, 0, data.buildingHealth, data.buildingPosition);
    enemyBuilding = tempBuilding;
    
    EnemyTower tempTower(5+currentLevel, 3, data.towerPosition);
    enemyTower = tempTower;
}