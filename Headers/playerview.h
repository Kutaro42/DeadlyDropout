/**
 * @file playerview.h
 * @brief Declaration of the PlayerView class, which manages the player's visual appearance.
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
//  PlayerView — visual representation of the player.
//
//  Spritesheets (prefix /images, ressources.qrc):
//    Textures/Player/Idle/{Down,Up,Left,Right}.png   → 2 frames 36×36
//    Textures/Player/Move/{Down,Up,Left,Right}.png   → 2 frames 36×36
//    Textures/Player/Attack/{Down,Up,Left,Right}.png → 6 frames 128×128
//    Textures/Player/Slash/slash.png                 → N frames (horizontal
//                                                       sprite sheet, size SLASH_FRAME_SRC²)
//
//  Animation priority: ATTACK > WALK > IDLE
//
//  Slash effect (SlashView):
//    A child QGraphicsPixmapItem (itsSlashItem) is attached to PlayerView.
//    It displays the Slash/ spritesheet rotated to the exact attack angle.
//    It is hidden when the katana is not animating.
//
//  Centering:
//    The sprite may have a different display size depending on the state
//    (SPRITE_SIZE[state]). setOffset() re-centers the pixmap on the hitbox.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class PlayerView
 * @brief Graphics class managing the player's animations and the slash visual effect.
 */
class PlayerView : public QGraphicsPixmapItem {
public:
    explicit PlayerView(PlayerModel* model, QGraphicsItem* parent = nullptr);

    /**
     * @brief Updates the player's position, animation and slash effect.
     * @param dt Delta-time in seconds.
     */
    void updateView(float dt);

private:
    PlayerModel* model;

    // ---- Player animation states -----------------------------------------
    static constexpr int IDLE   = 0;
    static constexpr int WALK   = 1;
    static constexpr int ATTACK = 2;

    static constexpr int STATE_COUNT        = 3;
    static constexpr int DIR_COUNT          = 4;
    static constexpr int ATTACK_FRAME_COUNT = 4;

    static constexpr int FRAME_W[STATE_COUNT] = { 18, 18, 32 };
    static constexpr int FRAME_H[STATE_COUNT] = { 18, 18, 32 };
    static constexpr int SPRITE_SIZE[STATE_COUNT] = {
        Cfg::ENTITY_SIZE,
        Cfg::ENTITY_SIZE,
        Cfg::ENTITY_SIZE + 50,
    };

    QVector<QPixmap> sprites[STATE_COUNT][DIR_COUNT];
    QPixmap          cachedFallback[DIR_COUNT];

    // ---- IDLE / WALK timers -----------------------------------------
    int   currentFrame;
    float frameTimer;
    float animSpeed;

    // ---- ATTACK timers -----------------------------------------
    int   attackFrame;
    float attackFrameTimer;
    float attackTotalTimer;
    float attackAnimSpeed;
    bool  wasKatanaActive;
    bool  attackEverTriggered;

    // ═══════════════════════════════════════════════════════════════
    //  Slash effect sprite
    // ═══════════════════════════════════════════════════════════════

    /**
     * @brief Source dimensions of each frame in the slash spritesheet.
     *        Adjust if your PNG has frames of a different size.
     */
    static constexpr int SLASH_FRAME_SRC  = 128; ///< Source size (px) of one slash frame.

    /**
     * @brief On-screen display size of the slash sprite.
     *        Increase to enlarge the effect.
     */
    static constexpr int SLASH_DISPLAY    = 128;  ///< Display size (px) of the slash sprite.

    /**
     * @brief Number of frames in the slash spritesheet.
     *        Adjust according to your PNG file.
     */
    static constexpr int SLASH_FRAME_COUNT = 3;

    QVector<QPixmap>      itsSlashFrames;     ///< Slash effect frames (pre-loaded).
    QGraphicsPixmapItem*  itsSlashItem;       ///< Child item displayed on top of the player.
    int                   itsSlashFrame;      ///< Current frame index of the effect.
    float                 itsSlashFrameTimer; ///< Time elapsed on the current frame.
    float                 itsSlashFrameSpeed; ///< Duration per frame = KATANA_ANIM_DUR / SLASH_FRAME_COUNT.
    bool itsSlashDone;                        ///< Slash finished, prevents freezing on the last frame.

    // ─── Helpers ──────────────────────────────────────────────────
   /** @brief Converts a FacingDirection to a table index (0=Down,1=Up,2=Left,3=Right).
       @param d Cardinal direction.
   */
    static int dirIdx(FacingDirection d);

    /**
     * @brief Slices a horizontal spritesheet and scales each frame to targetSize.
     * @param frames     Target vector to store the frames.
     * @param path       Qt resource path.
     * @param fw/fh      Dimensions of one frame in the source PNG.
     * @param targetSize Desired display size.
     */
    void loadSheet(QVector<QPixmap>& frames, const QString& path,
                   int fw, int fh, int targetSize);

    /** @brief Generates an annotated colored sprite if the PNG resource is missing.
    @param dir Direction of the sprite to generate.
     */
    QPixmap makeFallback(FacingDirection dir);

    /**
     * @brief Advances one frame in an IDLE or WALK animation according to the timer.
     * @param dt     Delta-time in seconds.
     * @param frames Frames of the current animation.
     * @param speed  Duration per frame (s).
     */
    void advanceFrames(float dt, const QVector<QPixmap>& frames, float speed);

    /** @brief Re-centers the pixmap on the player's hitbox according to the active sprite size.
    @param spriteSize Size of the active sprite.
     */
    void applyCenter(int spriteSize);

    // ─── Slash helpers ────────────────────────────────────────────

    /**
     * @brief Loads the slash spritesheet from the resources.
     *        Called only once in the constructor.
     */
    void        loadSlashSheet();

    /**
     * @brief Updates the slash item (position, rotation, frame, visibility).
     * @param dt Delta-time in seconds.
     */
    void        updateSlash(float dt);

    /**
     * @brief Positions the slash item in front of the player according to the attack angle.
     *        The sprite is centered on the hitbox edge in the direction of the attack.
     */
    void        positionSlashItem();
};

#endif // PLAYERVIEW_H
