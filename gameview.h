#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSet>
#include <QKeyEvent>
#include <QPainter>
#include "playerview.h"
#include "zombiestdview.h"
#include "exitdoorview.h"
#include "gamemodel.h"
#include "config.h"
#include <QMouseEvent>

// ═══════════════════════════════════════════════════════════════════
//  GameView — vue principale du jeu (couche View, MVC).
//  Hérite QGraphicsView ; possède et affiche la QGraphicsScene.
//
//  Responsabilités :
//    - Capture des touches clavier (QSet<int> pressedKeys)
//    - Synchronisation des vues à chaque tick (updateAllViews)
//    - Caméra centrée sur le joueur (followPlayer)
//    - Affichage du HUD en surimpression (drawForeground override)
// ═══════════════════════════════════════════════════════════════════
class GameView : public QGraphicsView {
    Q_OBJECT

private:
    QGraphicsScene* scene;
    PlayerView*     playerView;
    QSet<int>       pressedKeys;
    GameModel*      currentModel;   // référence faible pour drawForeground

public:
    explicit GameView(QWidget* parent = nullptr);
    ~GameView() override;

    QGraphicsScene* getScene()        const { return scene;       }
    QSet<int>       getPressedKeys()  const { return pressedKeys; }
    PlayerView*     getPlayerView()   const { return playerView;  }

    // Initialise le PlayerView une fois le modèle chargé
    void initWithModel(GameModel* model);

    // Synchronise toutes les vues (position, animation) — appelé chaque tick
    void updateAllViews(float dt, GameModel* model);

    // Centre la caméra sur le joueur
    void followPlayer(GameModel* model);

    // Remet la scène à zéro (appelé avant de charger un nouveau niveau)
    void clearScene();

    // Affiche un message de fin en overlay (GameOver / Victory)
    void showOverlay(const QString& message);

protected:
    void keyPressEvent(QKeyEvent* e)   override;
    void keyReleaseEvent(QKeyEvent* e) override;
    void mousePressEvent(QMouseEvent* event) override;

    // HUD dessiné en coordonnées écran (indépendant du zoom caméra)
    void drawForeground(QPainter* painter, const QRectF& rect) override;

private:
    void drawHUD(QPainter* painter, GameModel* model);
    void drawHpBar(QPainter* p, int hp, int maxHp, int x, int y);

    QString overlayMessage;
    bool    showingOverlay;
signals:
    void sig_rightClicked();//signal clic droit pour la souris
    void sig_leftClicked();//signal clic gauche pour la souris
};

#endif // GAMEVIEW_H
