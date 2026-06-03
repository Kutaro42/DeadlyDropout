#ifndef CONFIG_H
#define CONFIG_H

// ═══════════════════════════════════════════════════════════════════
//  config.h — Constantes globales & énumérations (Deadly Dropout)
//  Toute valeur numérique du jeu doit être définie ici.
// ═══════════════════════════════════════════════════════════════════

namespace Cfg {

    // ─── Fenêtre ──────────────────────────────────────────────────
    constexpr int   WINDOW_W            = 960;
    constexpr int   WINDOW_H            = 650;

    // ─── Tuiles ───────────────────────────────────────────────────
    constexpr int   TILE_SIZE           = 32;   // px par tuile de sol/mur
    constexpr int   ENTITY_SIZE         = 64;   // taille sprite entité (px)

    // ─── Joueur ───────────────────────────────────────────────────
    constexpr float PLAYER_SPEED        = 120.0f;  // px/s
    constexpr int   PLAYER_MAX_HP       = 100;

    // ─── Katana ───────────────────────────────────────────────────
    constexpr int   KATANA_DAMAGE       = 30;
    constexpr float KATANA_SLASH_W      = 48.0f;  // largeur SlashZone (perpendiculaire)
    constexpr float KATANA_SLASH_D      = 40.0f;  // profondeur SlashZone (portée)
    constexpr float KATANA_ANIM_DUR     = 0.2f;   // durée animation tranche (s)
    constexpr float KATANA_COOLDOWN     = 0.5f;   // cooldown entre attaques (s)

    // ─── Zombie standard ──────────────────────────────────────────
    constexpr int   ZOMBIE_MAX_HP       = 20;
    constexpr float ZOMBIE_SPEED        = 90.0f;
    constexpr int   ZOMBIE_DMG_MIN      = 7;
    constexpr int   ZOMBIE_DMG_MAX      = 12;
    constexpr float ZOMBIE_ATK_CD       = 1.5f;
    constexpr float ZOMBIE_DETECT_R     = 400.0f;
    constexpr int   ZOMBIE_GOLD_MIN     = 5;
    constexpr int   ZOMBIE_GOLD_MAX     = 10;

    // ─── Boucle de jeu ────────────────────────────────────────────
    constexpr int   TIMER_MS            = 16;     // ~60 FPS

    // ─── Z-values (couches de rendu) ──────────────────────────────
    constexpr double Z_FLOOR            = 0.0;
    constexpr double Z_WALL             = 1.0;
    constexpr double Z_ENTITY           = 2.0;
    constexpr double Z_PLAYER           = 3.0;
    constexpr double Z_SLASH            = 4.0;

    // ─── HUD ──────────────────────────────────────────────────────
    constexpr int   HUD_MARGIN          = 12;
    constexpr int   HP_BAR_W            = 180;
    constexpr int   HP_BAR_H            = 16;
}

// ─── Énumérations ─────────────────────────────────────────────────
enum class FacingDirection { UP, DOWN, LEFT, RIGHT };
enum class AppState        { MENU, PLAYING, GAME_OVER, VICTORY };
enum class EnemyAIState    { IDLE, CHASING, ATTACKING };

// Identifiants de tuiles pour la matrice de niveau
enum class TileType {
    FLOOR        = 0,
    WALL         = 1,
    ZOMBIE_SPAWN = 2,
    EXIT_DOOR    = 3,
    PLAYER_SPAWN = 4
};

#endif // CONFIG_H
