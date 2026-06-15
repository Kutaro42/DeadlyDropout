/**
 * @file zombielauncherview.h
 * @brief Déclaration de la vue graphique du zombie lanceur.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef ZOMBIELAUNCHERVIEW_H
#define ZOMBIELAUNCHERVIEW_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QVector>
#include <QPixmap>
#include "zombielaunchermodel.h"
#include "rubbleview.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  ZombieLauncherView — représentation visuelle du zombie lanceur.
//
//  Spritesheets attendues (ressources Qt) :
//    :/images/Textures/Enemy/Launcher/Idle/{Down,Up,Left,Right}.png
//    :/images/Textures/Enemy/Launcher/Move/{Down,Up,Left,Right}.png
//  → 2 frames de 32×32 côte à côte, comme ZombieStdView.
//  Si absentes, un rectangle violet annoté "L" est utilisé.
//
//  Gestion des RubbleView :
//    À chaque tick, updateView() synchronise la liste des RubbleView
//    avec les RubbleModel créés par le modèle, puis appelle
//    pruneInactiveRubbles() pour libérer la mémoire.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class ZombieLauncherView
 * @brief Vue Qt du zombie lanceur, gérant aussi les vues des gravats qu'il tire.
 */
class ZombieLauncherView : public QGraphicsPixmapItem {
public:
    /**
     * @brief Constructeur.
     * @param model  Modèle source.
     * @param scene  Scène Qt dans laquelle ajouter les RubbleView.
     * @param parent Élément parent Qt.
     */
    explicit ZombieLauncherView(ZombieLauncherModel* model,
                                QGraphicsScene*      scene,
                                QGraphicsItem*       parent = nullptr);

    /**
     * @brief Met à jour l'animation du lanceur et les vues de ses gravats.
     * @param dt Delta-time en secondes.
     */
    void updateView(float dt);

private:
    ZombieLauncherModel* model;          ///< Modèle source.
    QGraphicsScene*      scene;          ///< Scène où injecter les nouvelles RubbleView.
    QVector<RubbleView*> rubbleViews;    ///< Vues des gravats actuellement en vol.

    // ─── Animation ────────────────────────────────────────────
    static constexpr int IDLE        = 0;
    static constexpr int WALK        = 1;
    static constexpr int HURT        = 2;  ///< Constante de l'état blessé.
    static constexpr int DEAD        = 3;  ///< Constante de l'état de mort.
    static constexpr int STATE_COUNT = 4;
    static constexpr int DIR_COUNT   = 4;
    static constexpr int FRAME_W     = 32; ///< Largeur d'une frame source (px).
    static constexpr int FRAME_H     = 32; ///< Hauteur d'une frame source (px).

    QVector<QPixmap> sprites[STATE_COUNT][DIR_COUNT]; ///< Frames par état × direction.

    int   currentFrame; ///< Index de frame courant.
    float frameTimer;   ///< Temps écoulé sur la frame courante.
    float animSpeed;    ///< Durée d'affichage d'une frame en secondes.
    float animSpeedDeath;
    int   currentDir;   ///< Direction courante (index 0-3).
    bool  deadAnimFinished; ///< True une fois que l'animation DEAD a été jouée entièrement.

    // ─── Helpers ──────────────────────────────────────────────
    static int             dirIdx(FacingDirection d);
    static FacingDirection velocityToDir(Vector2f vel);
    void                   loadSheet(QVector<QPixmap>& frames, const QString& path);
    QPixmap                makeFallback();
    void                   advanceFrames(float dt, const QVector<QPixmap>& frames);

    /**
     * @brief Synchronise rubbleViews avec les RubbleModel du modèle.
     * Crée une RubbleView pour chaque nouveau gravat, puis supprime
     * les vues des gravats devenus inactifs.
     */
    void syncRubbleViews();
};

#endif // ZOMBIELAUNCHERVIEW_H
