/**
 * @file playerview.h
 * @brief Déclaration de la classe PlayerView gérant l'aspect visuel du joueur.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include <QGraphicsPixmapItem>
#include <QVector>
#include <QPixmap>
#include <QTransform>
#include "playermodel.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  PlayerView — représentation visuelle du joueur.
//
//  Spritesheets (prefix /images, ressources.qrc) :
//    Textures/Player/Idle/{Down,Up,Left,Right}.png   → 2 frames 36×36
//    Textures/Player/Move/{Down,Up,Left,Right}.png   → 2 frames 36×36
//    Textures/Player/Attack/{Down,Up,Left,Right}.png → 6 frames 128×128
//    Textures/Player/Slash/slash.png                 → N frames (sprite sheet
//                                                       horizontal, taille SLASH_FRAME_SRC²)
//
//  Priorité d'animation : ATTACK > WALK > IDLE
//
//  Effet de slash (SlashView) :
//    Un QGraphicsPixmapItem fils (itsSlashItem) est attaché à PlayerView.
//    Il affiche la spritesheet Slash/ pivotée à l'angle exact de l'attaque.
//    Il est caché quand le katana n'est pas en animation.
//
//  Centrage :
//    Le sprite peut avoir une taille d'affichage différente selon l'état
//    (SPRITE_SIZE[état]). setOffset() recentre le pixmap sur la hitbox.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class PlayerView
 * @brief Classe graphique gérant les animations du joueur et l'effet visuel du slash.
 */
class PlayerView : public QGraphicsPixmapItem {
public:
    explicit PlayerView(PlayerModel* model, QGraphicsItem* parent = nullptr);

    /**
     * @brief Met à jour la position, l'animation du joueur et l'effet de slash.
     * @param dt Delta-time en secondes.
     */
    void updateView(float dt);

private:
    PlayerModel* model;

    // ─── États d'animation joueur ─────────────────────────────────
    static constexpr int IDLE   = 0;
    static constexpr int WALK   = 1;
    static constexpr int ATTACK = 2;

    static constexpr int STATE_COUNT        = 3;
    static constexpr int DIR_COUNT          = 4;
    static constexpr int ATTACK_FRAME_COUNT = 6;

    static constexpr int FRAME_W[STATE_COUNT] = { 36, 36, 128 };
    static constexpr int FRAME_H[STATE_COUNT] = { 36, 36, 128 };
    static constexpr int SPRITE_SIZE[STATE_COUNT] = {
        Cfg::ENTITY_SIZE,
        Cfg::ENTITY_SIZE,
        Cfg::ENTITY_SIZE + 128,
    };

    QVector<QPixmap> sprites[STATE_COUNT][DIR_COUNT];
    QPixmap          cachedFallback[DIR_COUNT];

    // ─── Timers IDLE / WALK ───────────────────────────────────────
    int   currentFrame;
    float frameTimer;
    float animSpeed;

    // ─── Timers ATTACK ────────────────────────────────────────────
    int   attackFrame;
    float attackFrameTimer;
    float attackTotalTimer;
    float attackAnimSpeed;
    bool  wasKatanaActive;
    bool  attackEverTriggered;

    // ═══════════════════════════════════════════════════════════════
    //  Sprite d'effet de slash
    // ═══════════════════════════════════════════════════════════════

    /**
     * @brief Dimensions source de chaque frame dans la spritesheet slash.
     *        Adaptez si votre PNG a des frames d'une autre taille.
     */
    static constexpr int SLASH_FRAME_SRC  = 128; ///< Taille source (px) d'une frame slash.

    /**
     * @brief Taille d'affichage à l'écran du sprite de slash.
     *        Augmentez pour agrandir l'effet.
     */
    static constexpr int SLASH_DISPLAY    = 96;  ///< Taille d'affichage (px) du sprite slash.

    /**
     * @brief Nombre de frames dans la spritesheet de slash.
     *        À ajuster selon votre fichier PNG.
     */
    static constexpr int SLASH_FRAME_COUNT = 6;

    QVector<QPixmap>      itsSlashFrames;     ///< Frames de l'effet de slash (préchargées).
    QGraphicsPixmapItem*  itsSlashItem;       ///< Item fils affiché par-dessus le joueur.
    int                   itsSlashFrame;      ///< Index de frame courant de l'effet.
    float                 itsSlashFrameTimer; ///< Durée écoulée sur la frame courante.
    float                 itsSlashFrameSpeed; ///< Durée par frame = KATANA_ANIM_DUR / SLASH_FRAME_COUNT.

    // ─── Helpers ──────────────────────────────────────────────────
    static int  dirIdx(FacingDirection d);
    void        loadSheet(QVector<QPixmap>& frames, const QString& path,
                          int fw, int fh, int targetSize);
    QPixmap     makeFallback(FacingDirection dir);
    void        advanceFrames(float dt, const QVector<QPixmap>& frames, float speed);
    void        applyCenter(int spriteSize);

    // ─── Slash helpers ────────────────────────────────────────────

    /**
     * @brief Charge la spritesheet de slash depuis les ressources.
     *        Appelé une seule fois dans le constructeur.
     */
    void        loadSlashSheet();

    /**
     * @brief Met à jour l'item de slash (position, rotation, frame, visibilité).
     * @param dt Delta-time en secondes.
     */
    void        updateSlash(float dt);

    /**
     * @brief Positionne l'item de slash devant le joueur selon l'angle d'attaque.
     *        Le sprite est centré sur le bord de la hitbox dans la direction de l'attaque.
     */
    void        positionSlashItem();
};

#endif // PLAYERVIEW_H
