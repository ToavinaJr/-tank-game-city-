#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <QString>
#include <QColor>

class GameConfig {
public:
    static GameConfig& instance() {
        static GameConfig config;
        return config;
    }
    
    // Getters
    bool isSoundEnabled() const { return m_soundEnabled; }
    QColor getTankColor() const { return m_tankColor; }
    int getHighScore() const { return m_highScore; }
    int getMusicVolume() const { return m_musicVolume; }
    int getSfxVolume() const { return m_sfxVolume; }
    
    // Setters
    void setSoundEnabled(bool enabled) { m_soundEnabled = enabled; }
    void setTankColor(const QColor& color) { m_tankColor = color; }
    void setHighScore(int score) { m_highScore = score; }
    void setMusicVolume(int volume) { m_musicVolume = volume; }
    void setSfxVolume(int volume) { m_sfxVolume = volume; }
    
private:
    GameConfig() 
        : m_soundEnabled(true)
        , m_tankColor("#FFD700")
        , m_highScore(0)
        , m_musicVolume(50)
        , m_sfxVolume(70)
    {}
    
    bool m_soundEnabled;
    QColor m_tankColor;
    int m_highScore;
    int m_musicVolume;
    int m_sfxVolume;
};

#endif // GAMECONFIG_H