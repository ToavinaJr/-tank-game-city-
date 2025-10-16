// GameScene.cpp
#include "../include/GameScene.hpp"
#include "../include/Constants.hpp"

GameScene::GameScene(QObject* parent)
    : QObject(parent)
    , m_sceneBounds(0, 0, GameConstants::GAME_AREA_WIDTH, 
                    GameConstants::GAME_AREA_HEIGHT)
{
}