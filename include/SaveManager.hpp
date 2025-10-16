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

    // --- Sound Enabled ---
    void setSoundEnabled(bool enabled);
    bool getSoundEnabled() const;

    // --- Tank Color ---
    void setTankColor(const QColor& color);
    QColor getTankColor() const;

    // --- High Score ---
    void setHighScore(int score);
    int getHighScore() const;

    // --- Music Volume ---
    void setMusicVolume(int volume);
    int getMusicVolume() const;

    // --- SFX Volume ---
    void setSfxVolume(int volume);
    int getSfxVolume() const;

    // --- Save/Load Settings ---
    void saveSettings();
    void loadSettings();

private:
    explicit SaveManager(QObject* parent = nullptr);
    SaveManager(const SaveManager&) = delete;
    SaveManager& operator=(const SaveManager&) = delete;

    QSettings m_settings;
};

#endif // SAVEMANAGER_HPP
