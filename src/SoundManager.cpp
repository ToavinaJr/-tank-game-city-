// SaveManager.cpp
#include "../include/SaveManager.hpp"
#include "../include/Constants.hpp"
#include "../include/GameConfig.hpp"

SaveManager::SaveManager()
    : m_settings("TankBattleGame", "TankBattle")
{
    loadSettings();
}

void SaveManager::saveSettings() {
    auto& config = GameConfig::instance();
    
    m_settings.setValue(GameConstants::SETTING_SOUND_ENABLED, config.isSoundEnabled());
    m_settings.setValue(GameConstants::SETTING_TANK_COLOR, config.getTankColor().name());
    m_settings.setValue(GameConstants::SETTING_HIGH_SCORE, config.getHighScore());
    m_settings.setValue(GameConstants::SETTING_MUSIC_VOLUME, config.getMusicVolume());
    m_settings.setValue(GameConstants::SETTING_SFX_VOLUME, config.getSfxVolume());
    
    m_settings.sync();
}

void GameEngine::loadSettings() {
    auto& config = GameConfig::instance();
    
    config.setSoundEnabled(
        m_settings.value(GameConstants::SETTING_SOUND_ENABLED, true).toBool()
    );
    
    config.setTankColor(
        QColor(m_settings.value(GameConstants::SETTING_TANK_COLOR, 
                               Colors::PLAYER_TANK_DEFAULT).toString())
    );
    
    config.setHighScore(
        m_settings.value(GameConstants::SETTING_HIGH_SCORE, 0).toInt()
    );
    
    config.setMusicVolume(
        m_settings.value(GameConstants::SETTING_MUSIC_VOLUME, 50).toInt()
    );
    
    config.setSfxVolume(