/**
 * @file config.h
 * @brief Constantes globales et énumérations de configuration du jeu Deadly Dropout.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef CONFIG_H
#define CONFIG_H

/**
 * @namespace Cfg
 * @brief Espace de nommage regroupant les constantes numériques du jeu.
 */
namespace Cfg {

    // ─── Fenêtre ──────────────────────────────────────────────────
    constexpr int   WINDOW_W            = 960;    ///< Largeur de la fenêtre de jeu en pixels.
    constexpr int   WINDOW_H            = 650;    ///< Hauteur de la fenêtre de jeu en pixels.

    // ─── Tuiles ───────────────────────────────────────────────────
    constexpr int   TILE_SIZE           = 64;     ///< Taille d'une tuile de sol ou de mur en pixels.
    constexpr int   ENTITY_SIZE         = 64;     ///< Taille du sprite d'une entité en pixels.

    // ─── Joueur ───────────────────────────────────────────────────
    constexpr float PLAYER_SPEED        = 300.0f; ///< Vitesse de déplacement du joueur en pixels/s.
    constexpr float PLAYER_SPEED_BOOSTED =1000.0f;///< Vitesse du joueur boosté
    constexpr int   PLAYER_MAX_HP       = 100;    ///< Points de vie maximum du joueur.

    // ─── Katana ───────────────────────────────────────────────────
    constexpr int   KATANA_DAMAGE       = 30;     ///< Dégâts de base infligés par le katana.
    constexpr float KATANA_SLASH_W      = 48.0f;  ///< Largeur de la SlashZone (perpendiculaire).
    constexpr float KATANA_SLASH_D      = 40.0f;  ///< Profondeur de la SlashZone (portée).
    constexpr float KATANA_ANIM_DUR     = 0.2f;   ///< Durée de l'animation d'attaque en secondes.
    constexpr float KATANA_COOLDOWN     = 0.5f;   ///< Temps de recharge entre les attaques en secondes.

    // ─── Zombie standard ──────────────────────────────────────────
    constexpr int   ZOMBIE_MAX_HP       = 20;     ///< Points de vie maximum du zombie.
    constexpr float ZOMBIE_SPEED        = 90.0f;  ///< Vitesse de déplacement du zombie en pixels/s.
    constexpr int   ZOMBIE_DMG_MIN      = 7;      ///< Dégâts minimum infligés par le zombie.
    constexpr int   ZOMBIE_DMG_MAX      = 12;     ///< Dégâts maximum infligés par le zombie.
    constexpr float ZOMBIE_ATK_CD       = 1.5f;   ///< Temps de recharge de l'attaque en secondes.
    constexpr float ZOMBIE_DETECT_R     = 400.0f; ///< Rayon de la bulle de détection du joueur.
    constexpr int   ZOMBIE_GOLD_MIN     = 5;      ///< Quantité minimum d'or lâchée.
    constexpr int   ZOMBIE_GOLD_MAX     = 10;     ///< Quantité maximum d'or lâchée.

    // ─── Zombie lanceur ───────────────────────────────────────────
    constexpr int   LAUNCHER_MAX_HP     = 15;     ///< Points de vie du zombie lanceur.
    constexpr float LAUNCHER_SPEED      = 90.0f;  ///< Vitesse de poursuite (sort de portée de tir).
    constexpr float LAUNCHER_DETECT_R   = 400.0f; ///< Rayon de détection du joueur.
    constexpr float LAUNCHER_SHOOT_R    = 320.0f; ///< Rayon de tir : reste immobile en dessous.
    constexpr float LAUNCHER_SHOOT_CD   = 2.0f;   ///< Intervalle entre deux tirs en secondes.
    constexpr int   LAUNCHER_GOLD_MIN   = 7;      ///< Or minimum lâché à la mort.
    constexpr int   LAUNCHER_GOLD_MAX   = 14;     ///< Or maximum lâché à la mort.

    // ─── Gravat (projectile lanceur) ──────────────────────────────
    constexpr float RUBBLE_SPEED        = 300.0f; ///< Vitesse de déplacement du gravat en pixels/s.
    constexpr int   RUBBLE_DAMAGE       = 10;     ///< Dégâts infligés par un gravat.
    constexpr float RUBBLE_SIZE         = 16.0f;  ///< Taille (côté) du sprite du gravat en pixels.

    // ─── Boucle de jeu ────────────────────────────────────────────
    constexpr int   TIMER_MS            = 16;     ///< Intervalle du timer principal (~60 FPS).

    // ─── Z-values (couches de rendu) ──────────────────────────────
    constexpr double Z_FLOOR            = 0.0;    ///< Profondeur d'affichage du sol.
    constexpr double Z_WALL             = 1.0;    ///< Profondeur d'affichage des murs.
    constexpr double Z_ENTITY           = 2.0;    ///< Profondeur d'affichage des entités.
    constexpr double Z_PLAYER           = 3.0;    ///< Profondeur d'affichage du joueur.
    constexpr double Z_SLASH            = 4.0;    ///< Profondeur d'affichage de l'attaque.

    // ─── Boss ────────────────────────────────────────────
    constexpr int   BOSSP1_MAX_HP       = 300;     ///< Points de vie maximum du boss en phase 1.
    constexpr int   BOSSP2_MAX_HP       = 300;     ///< Points de vie maximum du zombie.
    constexpr int   BOSSP3_MAX_HP       = 300;     ///< Points de vie maximum du zombie.
    constexpr float BOSS_SPEED        = 90.0f;  ///< Vitesse de déplacement du zombie en pixels/s.
    constexpr int   BOSS_DMG            = 20;      ///< Dégâts infligés par le boss.
    constexpr float BOSS_ATK_CD       = 1.5f;   ///< Temps de recharge de l'attaque en secondes.
    constexpr float BOSS_DETECT_R     = 400.0f; ///< Rayon de la bulle de détection du joueur.
    constexpr int   BOSS_GOLD_MIN     = 300;      ///< Quantité minimum d'or lâchée.
    constexpr int   BOSS_GOLD_MAX     = 400;     ///< Quantité maximum d'or lâchée.

    // ─── HUD ──────────────────────────────────────────────────────
    constexpr int   HUD_MARGIN          = 12;     ///< Marge extérieure du HUD.
    constexpr int   HP_BAR_W            = 180;    ///< Largeur de la barre de vie.
    constexpr int   HP_BAR_H            = 16;     ///< Hauteur de la barre de vie.
}

// ─── Énumérations ─────────────────────────────────────────────────

/** @brief Direction de l'entité. */
enum class FacingDirection { UP, DOWN, LEFT, RIGHT };

/** @brief États de la machine à états globale de l'application. */
enum class AppState        { MENU, PLAYING, GAME_OVER, VICTORY, PAUSED };

/** @brief États de l'IA ennemie. */
enum class EnemyAIState    { IDLE, CHASING, ATTACKING };

/** @brief Identifiants de tuiles pour la génération de la matrice de niveau. */
enum class TileType {
    FLOOR        = 0,
    WALL         = 1,
    ZOMBIE_SPAWN = 2,
    EXIT_DOOR    = 3,
    PLAYER_SPAWN = 4,
    LAUNCHER_SPAWN  = 5, ///< Spawn du zombie lanceur de gravats.
    STANDARD_DOOR   = 6,    ///< Porte standard ouvrable par contact du joueur.
    BOSS_SPAWN      = 7
};

#endif // CONFIG_H
