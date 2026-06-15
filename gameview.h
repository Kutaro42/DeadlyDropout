/**
 * @file gameview.h
 * @brief Déclaration de la vue principale (QGraphicsView) du jeu in-game.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSet>
#include <QKeyEvent>
#include <QPainter>
#include <QVector>
#include <QPixmap>
#include <cmath>
#include "playerview.h"
#include "zombiestdview.h"
#include "zombielauncherview.h"
#include "bossview.h"
#include "exitdoorview.h"
#include "standarddoorview.h"
#include "gunview.h"
#include "gamemodel.h"
#include "config.h"
#include "levelcontroller.h"
#include <QGraphicsTextItem>
#include <QGraphicsOpacityEffect>
#include <QMouseEvent>
#include <QPushButton>
#include <QPointF>

/**
 * @class GameView
 * @brief Gère le rendu visuel du jeu, l'affichage du HUD, la caméra et capte les événements.
 */
class GameView : public QGraphicsView {
    Q_OBJECT

private:
    QGraphicsScene* scene;
    PlayerView*     playerView;
    GunView*        gunView;
    QSet<int>       pressedKeys;
    GameModel*      currentModel;
    QPointF         itsMouseViewPos; ///< Position souris en coordonnées viewport (ne dépend pas de la caméra).

    // ─── Boutons du menu pause ────────────────────────────────────
    QPushButton* pauseResumeButton = nullptr;
    QPushButton* pauseMenuButton   = nullptr;
    QPushButton* pauseMusicButton  = nullptr;
    QPushButton* pauseNextButton   = nullptr;
    bool         showingPause      = false;

public:
    explicit GameView(QWidget* parent = nullptr);
    ~GameView() override;

    QGraphicsScene* getScene()         const { return scene;            }
    QSet<int>       getPressedKeys()   const { return pressedKeys;      }
    PlayerView*     getPlayerView()    const { return playerView;       }
    QPointF         getMouseScenePos() const { return mapToScene(itsMouseViewPos.toPoint()); }

    void initWithModel(GameModel* model);
    void updateAllViews(float dt, GameModel* model, LevelController* levelCtrl = nullptr);
    void followPlayer(GameModel* model);
    void clearScene();
    void showOverlay(const QString& message);
    void showPauseOverlay();
    void hidePauseOverlay();

protected:
    void keyPressEvent(QKeyEvent* e)         override;
    void keyReleaseEvent(QKeyEvent* e)       override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event)  override;
    void drawForeground(QPainter* painter, const QRectF& rect) override;
    void wheelEvent(QWheelEvent *event)      override;

private:
    void drawHUD(QPainter* painter, GameModel* model);
    void drawHpBar(QPainter* p, int hp, int maxHp, int x, int y);
    void drawBossHpBar(QPainter* p, int hp, int maxHp, int x, int y);

    // ─── Popups flottants (dégâts / gold) ─────────────────────────
    struct PopupAnim {
        QGraphicsTextItem*      item;
        QGraphicsOpacityEffect* fx;
        float elapsed;
        float duration;
        float originY;
    };
    QVector<PopupAnim> activePopups;

    QString      overlayMessage;
    bool         showingOverlay;
    QPushButton* menuButton   = nullptr;
    QPushButton* replayButton = nullptr;

    // ─── Clignotement gold ────────────────────────────────────────
    int   lastGold;
    float goldFlashTimer;
    bool  goldFlashVisible;
    float goldFlashCycle;

    // ─── Clignotement clé ─────────────────────────────────────────
    bool  lastHasKey;
    float keyFlashTimer;
    bool  keyFlashVisible;
    float keyFlashCycle;

    // ─── Sprites cooldown HUD ─────────────────────────────────────
    QVector<QPixmap> gunCdFrames;    ///< 15 frames 26×26 cooldown pistolet.
    QVector<QPixmap> katanaCdFrames; ///< 21 frames 26×26 cooldown katana.

signals:
    void sig_rightClicked(QPointF scenePos);
    void sig_leftClicked(QPointF scenePos);
    void sig_escapePressed();
    void sigRestartRq();
    void sigQuitRq();
    void sigPauseMenu();
    void sigToggleMusic();
    void sigNextTrack();
};

#endif // GAMEVIEW_H
