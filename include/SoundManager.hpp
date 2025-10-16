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