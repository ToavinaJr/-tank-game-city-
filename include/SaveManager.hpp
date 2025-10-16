#ifndef SAVEMANAGER_HPP
#define SAVEMANAGER_HPP

#include <QObject>
#include <QColor>
#include <QSettings>

class SaveManager : public QObject
{
    Q_OBJECT

public:
    static SaveManager& instance()
    {
        static SaveManager _instance;
        return _instance;
    }

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

    void saveSettings();
    void loadSettings();

private:
    explicit SaveManager(QObject* parent = nullptr);
    SaveManager(const SaveManager&) = delete;
    SaveManager& operator=(const SaveManager&) = delete;

    QSettings m_settings;
};

#endif // SAVEMANAGER_HPP
