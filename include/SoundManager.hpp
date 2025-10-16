#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <QObject>
#include <QHash>
#include <QString>
#include <QSoundEffect>

class SoundManager : public QObject
{
    Q_OBJECT

public:
    explicit SoundManager(QObject* parent = nullptr);
    void initialize();
    void playSound(const QString& soundName);
    void setEnabled(bool enabled);
    void setVolume(int volume);

private:
    void loadSounds();

    bool m_enabled;
    int m_volume;
    QHash<QString, QSoundEffect*> m_sounds;
};

#endif // SOUNDMANAGER_HPP
