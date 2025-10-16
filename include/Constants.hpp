#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

namespace GameConstants {
// Paramètres de grille
constexpr int GRID_WIDTH = 26;
constexpr int GRID_HEIGHT = 26;
constexpr int CELL_SIZE = 32;

// Paramètres de fenêtre
constexpr int GAME_AREA_WIDTH = GRID_WIDTH * CELL_SIZE;   // 832
constexpr int GAME_AREA_HEIGHT = GRID_HEIGHT * CELL_SIZE; // 832
constexpr int HUD_HEIGHT = 80;
constexpr int WINDOW_WIDTH = GAME_AREA_WIDTH;
constexpr int WINDOW_HEIGHT = GAME_AREA_HEIGHT + HUD_HEIGHT;

// Paramètres de jeu - VALEURS POUR MOUVEMENT FLUIDE
constexpr int PLAYER_SPEED = 2;          // Réduit à 2 pour mouvement plus fluide
constexpr int ENEMY_SPEED = 1;           // Réduit à 1 pour mouvement fluide
constexpr int BULLET_SPEED = 4;          // Réduit à 4 pour mouvement plus fluide
constexpr int MAX_PLAYER_HEALTH = 3;
constexpr int MAX_ENEMIES = 15;
constexpr int ACTIVE_ENEMIES = 3;

// Intervalles de timer (millisecondes) - OPTIMISÉ POUR FLUIDITÉ
constexpr int GAME_TICK_INTERVAL = 16;   // ~60 FPS pour animation fluide
constexpr int ENEMY_SPAWN_INTERVAL = 4000;
constexpr int ENEMY_SHOOT_INTERVAL = 2000;

// Score
constexpr int ENEMY_KILL_SCORE = 100;
constexpr int LEVEL_COMPLETE_BONUS = 1000;

// Clés de paramètres
const QString SETTING_SOUND_ENABLED = "sound_enabled";
const QString SETTING_TANK_COLOR = "tank_color";
const QString SETTING_HIGH_SCORE = "high_score";
const QString SETTING_MUSIC_VOLUME = "music_volume";
const QString SETTING_SFX_VOLUME = "sfx_volume";
}

namespace Colors {
// Couleurs par défaut - PLUS VISIBLES
const QString PLAYER_TANK_DEFAULT = "#FFD700";  // Or
const QString ENEMY_TANK = "#FF4444";           // Rouge
const QString BRICK_BLOCK = "#D2691E";          // Brun (briques)
const QString STEEL_BLOCK = "#708090";          // Gris acier
const QString WATER_BLOCK = "#4682B4";          // Bleu
const QString TREE_BLOCK = "#228B22";           // Vert forêt
const QString BASE = "#FF0000";                 // Rouge vif
const QString BULLET = "#FFFF00";               // Jaune brillant
const QString BACKGROUND = "#000000";           // Noir

// Couleurs de power-ups
const QString HEALTH_POWERUP = "#FF69B4";       // Rose
const QString BOMB_POWERUP = "#FF8C00";         // Orange
const QString SHIELD_POWERUP = "#00BFFF";       // Cyan
}

#endif // CONSTANTS_H
