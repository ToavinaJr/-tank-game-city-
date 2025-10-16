// SaveManager.h
#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QSettings>
#include <QString>
#include <QColor>

class SaveManager {
public:
    static SaveManager& instance() {
        static SaveManager manager;
        return manager;
    }
    
    void saveSettings();
    void loadSettings();
    
    void setSoundEnabled(bool enabled);
    bool getSoundEnabled() const;
    
    void setTankColor(const QColor& color);
    QColor getTankColor() const;
    
    void setHighScore(int score);
    int getHighScore() const;
    
    void setMusicVolume(int volume);
    int getMusicVolume() const;
    
    void setSfxVolume(int volume);
    int getSfxVolume() const;
    
private:
    SaveManager();
    QSettings m_settings;
};

#endif // SAVEMANAGER_H

// SoundManager.h
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QSoundEffect>
#include <QHash>
#include <QString>

class SoundManager : public QObject {
    Q_OBJECT
    
public:
    static SoundManager& instance() {
        static SoundManager manager;
        return manager;
    }
    
    void initialize();
    void playSound(const QString& soundName);
    void setEnabled(bool enabled);
    void setVolume(int volume);
    
private:
    SoundManager(QObject* parent = nullptr);
    void loadSounds();
    
    bool m_enabled;
    int m_volume;
    QHash<QString, QSoundEffect*> m_sounds;
};

#endif // SOUNDMANAGER_H