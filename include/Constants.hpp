#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

namespace GameConstants {
    // Grid settings
    constexpr int GRID_WIDTH = 26;
    constexpr int GRID_HEIGHT = 26;
    constexpr int CELL_SIZE = 32;
    
    // Window settings
    constexpr int GAME_AREA_WIDTH = GRID_WIDTH * CELL_SIZE;
    constexpr int GAME_AREA_HEIGHT = GRID_HEIGHT * CELL_SIZE;
    constexpr int HUD_HEIGHT = 80;
    constexpr int WINDOW_WIDTH = GAME_AREA_WIDTH;
    constexpr int WINDOW_HEIGHT = GAME_AREA_HEIGHT + HUD_HEIGHT;
    
    // Game settings
    constexpr int PLAYER_SPEED = 4;
    constexpr int ENEMY_SPEED = 2;
    constexpr int BULLET_SPEED = 8;
    constexpr int MAX_PLAYER_HEALTH = 3;
    constexpr int MAX_ENEMIES = 20;
    constexpr int ACTIVE_ENEMIES = 4;
    
    // Timer intervals (milliseconds)
    constexpr int GAME_TICK_INTERVAL = 16;
    constexpr int ENEMY_SPAWN_INTERVAL = 5000;
    constexpr int ENEMY_SHOOT_INTERVAL = 2000;
    
    // Scoring
    constexpr int ENEMY_KILL_SCORE = 100;
    constexpr int LEVEL_COMPLETE_BONUS = 1000;
    
    // Settings keys
    const QString SETTING_SOUND_ENABLED = "sound_enabled";
    const QString SETTING_TANK_COLOR = "tank_color";
    const QString SETTING_HIGH_SCORE = "high_score";
    const QString SETTING_MUSIC_VOLUME = "music_volume";
    const QString SETTING_SFX_VOLUME = "sfx_volume";
}

namespace Colors {
    // Default colors
    const QString PLAYER_TANK_DEFAULT = "#FFD700";
    const QString ENEMY_TANK = "#FF4444";
    const QString BRICK_BLOCK = "#D2691E";
    const QString STEEL_BLOCK = "#708090";
    const QString WATER_BLOCK = "#4682B4";
    const QString TREE_BLOCK = "#228B22";
    const QString BASE = "#FF0000";
    const QString BULLET = "#FFFF00";
    const QString BACKGROUND = "#000000";
    
    // Power-up colors
    const QString HEALTH_POWERUP = "#FF69B4";
    const QString BOMB_POWERUP = "#FF8C00";
    const QString SHIELD_POWERUP = "#00BFFF";
}

#endif // CONSTANTS_H