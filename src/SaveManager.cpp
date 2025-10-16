#include "../include/SaveManager.hpp"
#include <QColor>

SaveManager::SaveManager(QObject* parent)
    : QObject(parent)
    , m_settings("MyCompany", "TankBattleGame")
{
}

// --- Settings ---
void SaveManager::saveSettings()
{
    m_settings.sync();
}

void SaveManager::loadSettings()
{

}

// --- Sound Enabled ---
void SaveManager::setSoundEnabled(bool enabled)
{
    m_settings.setValue("sound/enabled", enabled);
}

bool SaveManager::getSoundEnabled() const
{
    return m_settings.value("sound/enabled", true).toBool();
}

// --- Tank Color ---
void SaveManager::setTankColor(const QColor& color)
{
    m_settings.setValue("tank/color", color.name());
}

QColor SaveManager::getTankColor() const
{
    return QColor(m_settings.value("tank/color", "#00FF00").toString());
}

// --- High Score ---
void SaveManager::setHighScore(int score)
{
    m_settings.setValue("game/highscore", score);
}

int SaveManager::getHighScore() const
{
    return m_settings.value("game/highscore", 0).toInt();
}

// --- Music Volume ---
void SaveManager::setMusicVolume(int volume)
{
    m_settings.setValue("audio/musicVolume", volume);
}

int SaveManager::getMusicVolume() const
{
    return m_settings.value("audio/musicVolume", 50).toInt();
}

// --- SFX Volume ---
void SaveManager::setSfxVolume(int volume)
{
    m_settings.setValue("audio/sfxVolume", volume);
}

int SaveManager::getSfxVolume() const
{
    return m_settings.value("audio/sfxVolume", 50).toInt();
}
