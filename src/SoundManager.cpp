#include "../include/SoundManager.hpp"
#include "../include/SaveManager.hpp"
#include <QSoundEffect>
#include <QUrl>

SoundManager::SoundManager(QObject* parent)
    : QObject(parent), m_enabled(true), m_volume(50)
{
    loadSounds();
}

void SoundManager::initialize() {
    m_enabled = SaveManager::instance().getSoundEnabled();
    m_volume = SaveManager::instance().getSfxVolume();

    for (auto sound : m_sounds)
        sound->setVolume(m_enabled ? m_volume / 100.0f : 0.0f);
}

void SoundManager::loadSounds() {
    QStringList soundNames = {"shoot", "explosion", "powerup"};

    for (const QString& name : soundNames) {
        QSoundEffect* effect = new QSoundEffect(this);
        effect->setSource(QUrl::fromLocalFile(":/sounds/" + name + ".wav"));
        effect->setVolume(m_volume / 100.0f);
        m_sounds.insert(name, effect);
    }
}

void SoundManager::playSound(const QString& soundName) {
    if (!m_enabled) return;
    if (m_sounds.contains(soundName)) {
        m_sounds[soundName]->stop();
        m_sounds[soundName]->play();
    }
}

void SoundManager::setEnabled(bool enabled) {
    m_enabled = enabled;
    SaveManager::instance().setSoundEnabled(enabled);
    for (auto sound : m_sounds)
        sound->setVolume(enabled ? m_volume / 100.0f : 0.0f);
}

void SoundManager::setVolume(int volume) {
    m_volume = volume;
    SaveManager::instance().setSfxVolume(volume);
    for (auto sound : m_sounds)
        sound->setVolume(m_enabled ? m_volume / 100.0f : 0.0f);
}
