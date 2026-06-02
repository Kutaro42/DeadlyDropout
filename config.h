#ifndef CONFIG_H
#define CONFIG_H

#include <QWidget>


#endif // CONFIG_H
enum EnemyState {Active, Inactive};
enum DirectionSprite { UP, DOWN, LEFT, RIGHT };
enum EnemyAIState { IDLE, CHASING, ATTACKING };
enum AppState { MENU, INGAME, PAUSE, GAMEOVER, VICTORY };
constexpr static QPoint SPAWN_COORDONATES = QPoint(0,0);
const DirectionSprite STARTPOS = RIGHT;
const int INITIAL_LIFEPOINT = 10;
const int  WINDOW_W = 960;
const int WINDOW_H = 650;
const int CAMERA_EDGE_MARGIN = 20;

const int TILE_SIZE = 32;
const int ENTITY_FRAME_SIZE = 64;
const float PLAYER_SPEED = 120.0f;
const int PLAYER_MAX_HP = 100;

const int KATANA_DAMAGE = 30;
const int KATANA_SLASH_W = 48;
const int KATANA_SLASH_D = 40;
const float KATANA_ANIM_DURATION = 0.2f;
const float KATANA_COOLDOWN = 0.5f;

const int ZOMBIE_MAX_HP = 20;
const float ZOMBIE_SPEED = 90.0f;
const int ZOMBIE_DAMAGE_MIN = 7;
const int ZOMBIE_DAMAGE_MAX = 12;
const float ZOMBIE_ATTACK_COOLDOWN = 1.5f;
const float ZOMBIE_DETECTION_RADIUS = 200.0f;
const int ZOMBIE_GOLD_DROP_MIN = 5;
const int ZOMBIE_GOLD_DROP_MAX = 10;

const int ROOM_PATTERN1_W = 480;
const int ROOM_PATTERN1_H = 370;
const int ROOM_PATTERN2_W = 700;
const int ROOM_PATTERN2_H = 580;
const int GAME_TIMER_INTERVAL_MS = 16;
