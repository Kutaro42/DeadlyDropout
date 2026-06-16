/**
 * @file config.h
 * @brief Global constants and configuration enumerations for the game Deadly Dropout.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <QRect>

/**
 * @namespace Cfg
 * @brief A namespace containing the game's numerical constants.
 */
namespace Cfg {

// ─── Window ──────────────────────────────────────────────────
constexpr int   WINDOW_W            = 960;    ///< Width of the game window in pixels.
constexpr int   WINDOW_H            = 650;    ///< The height of the game window in pixels.

// ─── Tile ───────────────────────────────────────────────────
constexpr int   TILE_SIZE           = 64;     ///< The size of a floor or wall tile in pixels.
constexpr int   ENTITY_SIZE         = 64;     ///< The size of an entity's sprite in pixels.

// ─── Player ───────────────────────────────────────────────────
constexpr float PLAYER_SPEED        = 300.0f; ///< The player's movement speed in pixels per second.
constexpr float PLAYER_SPEED_BOOSTED =1000.0f;///< Boosted player speed
constexpr int   PLAYER_MAX_HP       = 100;    ///< The player's maximum health points.

// ─── Katana ───────────────────────────────────────────────────
constexpr int   KATANA_DAMAGE       = 30;     ///< Base damage dealt by the katana.
constexpr float KATANA_SLASH_W      = 48.0f;  ///< Width of the SlashZone (perpendicular).
constexpr float KATANA_SLASH_D      = 40.0f;  ///< Depth of the SlashZone (range).
constexpr float KATANA_ANIM_DUR     = 0.2f;   ///< Duration of the attack animation in seconds.
constexpr float KATANA_COOLDOWN     = 0.5f;   ///< Cooldown time between attacks in seconds.

// ─── Standard zombie ──────────────────────────────────────────
constexpr int   ZOMBIE_MAX_HP       = 20;      ///< The zombie's maximum health points.
constexpr float ZOMBIE_SPEED        = 120.0f;  ///< The zombie's movement speed in pixels per second.
constexpr int   ZOMBIE_DMG_MIN      = 10;      ///< Minimum damage dealt by the zombie.
constexpr int   ZOMBIE_DMG_MAX      = 17;      ///< Maximum damage dealt by the zombie.
constexpr float ZOMBIE_ATK_CD       = 1.5f;    ///< Attack cooldown in seconds.
constexpr float ZOMBIE_DETECT_R     = 800.0f;  ///< The radius of the player's detection bubble.
constexpr int   ZOMBIE_GOLD_MIN     = 5;       ///< Minimum amount of gold dropped.
constexpr int   ZOMBIE_GOLD_MAX     = 10;      ///< Maximum amount of gold dropped.

// ─── Launcher zombie ───────────────────────────────────────────
constexpr int   LAUNCHER_MAX_HP     = 15;     ///< The zombie thrower's health points.
constexpr float LAUNCHER_SPEED      = 90.0f;  ///< Pursuit speed (exits firing range).
constexpr float LAUNCHER_DETECT_R   = 800.0f; ///< Player detection radius.
constexpr float LAUNCHER_SHOOT_R    = 320.0f; ///< Range: stay still underneath.
constexpr float LAUNCHER_SHOOT_CD   = 2.0f;   ///< Interval between two shots in seconds.
constexpr int   LAUNCHER_GOLD_MIN   = 7;      ///< Minimum gold dropped upon death.
constexpr int   LAUNCHER_GOLD_MAX   = 14;     ///< Maximum gold dropped upon death.

// ─── Gravat (projectile from the launcher) ──────────────────────────────
constexpr float RUBBLE_SPEED        = 450.0f; ///< Velocity of the debris in pixels per second.
constexpr int   RUBBLE_DAMAGE       = 30;     ///< Damage dealt by a rubble.
constexpr float RUBBLE_SIZE         = 16.0f;  ///< Size (side) of the rubble sprite in pixels.

// ─── Ball (player's gun) ──────────────────────────────────
constexpr float BULLET_SPEED        = 2000.0f; ///< Ball travel speed in pixels per second (GDD).
constexpr int   BULLET_DAMAGE       = 12;     ///< Base damage of a bullet (BD).
constexpr int   BULLET_BONUS_MIN    = 2;      ///< Minimum adaptive bonus (GDD).
constexpr int   BULLET_BONUS_MAX    = 5;      ///< Maximum adaptive bonus (GDD).
constexpr float BULLET_SIZE         = 8.0f;   ///< Size (side) of the ball sprite in pixels.
constexpr float BULLET_CRIT_CHANCE  = 0.02f;  ///< Critical hit chance (2%, GDD).
constexpr float BULLET_CRIT_MULT    = 2.0f;   ///< Critical damage multiplier (×2, GDD).

// ─── Pistol (player ranged weapon) ────────────────────────────
constexpr float GUN_SHOOT_CD        = 0.5f;   ///< Cooldown between shots in seconds (GDD).
constexpr int   GUN_MAG_SIZE        = 10;     ///< Default magazine size.
constexpr float GUN_OFFSET_X        = 20.0f;  ///< X-axis offset of the cannon relative to the player's centre.
constexpr float GUN_OFFSET_Y        = 10.0f;  ///< Y-axis offset of the cannon relative to the player's centre.

// ─── Game loop ────────────────────────────────────────────
constexpr int   TIMER_MS            = 16;     ///< Main timer interval (~60 FPS).16

// ─── Z-values (rendering layers) ──────────────────────────────
constexpr double Z_FLOOR            = 0.0;    ///< Display depth of the ground.
constexpr double Z_WALL             = 1.0;    ///< Wall display depth.
constexpr double Z_ENTITY           = 2.0;    ///< Display depth of features.
constexpr double Z_PLAYER           = 3.0;    ///< Player display depth.
constexpr double Z_SLASH            = 4.0;    ///< Display depth of the attack.
constexpr double Z_BOSS_ZONE        = 1.5;    ///< Depth of the boss's attack zone (between the ground and entities).

// ─── Enemies — transitional states ────────────────────
constexpr float ENEMY_HURT_DUR      = 0.3f;  ///< Duration (in seconds) of the HURT state before returning to the AI.
constexpr float ENEMY_DEAD_DUR      = 1.0f;  ///< Duration (in seconds) of the death animation before it is deactivated.
constexpr float ENEMY_CHASE_PERSIST = 4.0f; ///< The length of time (in seconds) for which the enemy continues to pursue the player after losing sight of them.


// ─── Boss ────────────────────────────────────────────
constexpr int   BOSSP1_MAX_HP       = 300;     ///< The boss's maximum health in Phase 1.
constexpr int   BOSSP2_MAX_HP       = 600;     ///< The boss's maximum health in Phase 2.
constexpr int   BOSSP3_MAX_HP       = 850;     ///< The boss's maximum health in Phase 3.
constexpr float BOSS_SPEED        = 90.0f;     ///< The zombie's movement speed in pixels per second.
constexpr int   BOSS_DMG            = 20;      ///< Damage dealt by the boss.
constexpr float BOSS_ATK_CD       = 1.5f;      ///< Attack cooldown in seconds.
constexpr float BOSS_DETECT_R     = 800.0f;    ///< The radius of the player's detection bubble.
constexpr int   BOSS_GOLD_MIN     = 300;       ///< Minimum amount of gold dropped.
constexpr int   BOSS_GOLD_MAX     = 400;       ///< Maximum amount of gold dropped.


// ─── Boss pattern 2 (area) ────────────────────────────────────
constexpr float BOSS_ZONE_CHARGE_CD2 = 1.5f;     ///< Time between two occurrences of pattern 2, level 3 (s).
constexpr float BOSS_ZONE_TIMER     = 1.1f;      ///< Time taken to charge before the area explodes (s).
constexpr float BOSS_ZONE_CHARGE_CD3 = 1.25f;    ///< Time between two occurrences of pattern 2, level 3 (s).
constexpr int   BOSS_ZONE_TILE_SIZE = 32;        ///< Size of a red zone tile (px).
constexpr int   BOSS_ZONE_DMG       = 70;        ///< Fixed damage dealt by the area-of-effect explosion (pattern 2).
constexpr int   BOSS_ZONE_SAFE_COUNT = 7;        ///< Number of safe tiles (holes in the area).
constexpr int   BOSS_ZONE_RADIUS     = 384;      ///< Radius in pixels around the boss for the attack area (6 tiles of 64px).

// ─── HUD ──────────────────────────────────────────────────────
constexpr int   HUD_MARGIN          = 12;     ///< Outer edge of the HUD.
constexpr int   HP_BAR_W            = 180;    ///< Width of the player's health bar.
constexpr int   HP_BAR_H            = 16;     ///< The height of the player's health bar.
constexpr int   BOSSBARX            = 300;    ///< Position X on the boss's health bar.
constexpr int   BOSSBARY            = 50;    ///< Position Y on the boss's health bar.
constexpr int   BOSS_HP_BAR_W       = 400;    ///< Width of the boss's health bar
constexpr int   BOSS_HP_BAR_H       = 20;    ///< Height of the boss's health bar

// ─── Floating pop-ups (damage / gold) ──────────────────────
constexpr double Z_POPUP         = 5.0;    ///< Above all else.
constexpr float  POPUP_DMG_DUR   = 1.0f;  ///< Pop-up damage duration (s).
constexpr float  POPUP_GOLD_DUR  = 1.4f;  ///< Duration of gold pop-up (seconds).
constexpr float  POPUP_RISE_PX   = 40.0f; ///< Vertical rise (px).
}

// ─── MARKET IMPROVEMENTS (damage / gold) ──────────────────────
constexpr float BONUS_MAX_HP = 20.0f;                 ///< Max HP upgrade.
constexpr float BONUS_GUN_MAX_DMG = 5.0f;             ///< Gun max damage upgrade.
constexpr float BONUS_KATANA_DMG = 10.0f;             ///< Katana max damage upgrade.
constexpr float BONUS_KATANA_RANGE = 15.0f;           ///< Katana range attack upgrade.
constexpr float BONUS_SPEED = 20.0f;                  ///< Walk speed upgrade.
constexpr float BONUS_CRIT_CHANCE =0.03;              ///< Crit chance  upgrade.
constexpr float BONUS_KATANA_COOLDOWN_MIN = 0.1f;     ///< Katana cooldown upgrade.
constexpr float BONUS_KATANA_COOLDOWN_MAX = 0.2f;     ///< Katana cooldown upgrade.
constexpr float BONUS_SHOOT_COOLDOWN_MIN = 0.1f;      ///< Fire cooldown upgrade.
constexpr float BONUS_SHOOT_COOLDOWN_MAX = 0.2f;      ///< Fire cooldown upgrade.
constexpr float BONUS_RESERVE = 10.0f;                ///< magazine size upgrade.

// ─── Enumerations ─────────────────────────────────────────────────

/** @brief  Management of the entity. */
enum class FacingDirection { UP, DOWN, LEFT, RIGHT };

/** @brief Machine states within the application's global state machine. */
enum class AppState        { MENU, PLAYING, GAME_OVER, VICTORY, PAUSED };

/** @brief Enemy AI states. */
enum class EnemyAIState    { IDLE, CHASING, ATTACKING, HURT, DEAD };

/** @brief Tile IDs for generating the level matrix. */
enum class TileType {
    FLOOR           = 0,    ///< Map floor.
    WALL            = 1,    ///< Map wall.
    ZOMBIE_SPAWN    = 2,    ///< Zombie spawn.
    EXIT_DOOR       = 3,    ///< The exit door opens when you defeat the boss
    PLAYER_SPAWN    = 4,    ///< Player spawn.
    LAUNCHER_SPAWN  = 5,    ///< Launcher zombie spawn.
    STANDARD_DOOR   = 6,    ///< Standard door that opens when touched by a player.
    BOSS_SPAWN      = 7,    ///< Boss spawn.
    BOSS_DOOR       = 8     ///< The door locks once you enter the boss area.
};

/**
 * @struct ZoneTile
 * @brief 32×32 tile in the boss's attack zone (pattern 2).
 */
struct ZoneTile {
    QRectF rect;    ///< Collision/rendering rectangle (32×32 px, scene coordinates).
    bool   isSafe;  ///< true = safe area (transparent), false = danger zone (red).
};

#endif // CONFIG_H
