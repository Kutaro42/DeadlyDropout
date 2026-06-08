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
#include "playerview.h"
#include "zombiestdview.h"
#include "exitdoorview.h"
#include "gamemodel.h"
#include "config.h"
#include <QMouseEvent>
#include <QPushButton>
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

/**
 * @class GameView
 * @brief Gère le rendu visuel du jeu, l'affichage du HUD, la caméra et capte les événements.
 */
class GameView : public QGraphicsView {
    Q_OBJECT

private:
    QGraphicsScene* scene;          ///< Scène Qt contenant les éléments graphiques 2D.
    PlayerView* playerView;     ///< Représentation graphique du joueur.
    QSet<int>       pressedKeys;    ///< Ensemble des touches clavier actuellement enfoncées.
    GameModel* currentModel;   ///< Pointeur vers le modèle (utile pour le dessin du HUD).

public:
    /**
     * @brief Constructeur de la vue de jeu.
     */
    explicit GameView(QWidget* parent = nullptr);
    ~GameView() override;

    QGraphicsScene* getScene()        const { return scene;       }
    QSet<int>       getPressedKeys()  const { return pressedKeys; }
    PlayerView* getPlayerView()   const { return playerView;  }

    /**
     * @brief Injecte les dépendances graphiques à partir du modèle de données (Création du joueur).
     * @param model Le modèle contenant les informations à afficher.
     */
    void initWithModel(GameModel* model);

    /**
     * @brief Actualise toutes les entités visuelles attachées à la scène.
     * @param dt Delta-time en secondes.
     * @param model Modèle source de la mise à jour.
     */
    void updateAllViews(float dt, GameModel* model);

    /**
     * @brief Centre le décalage de la caméra sur les coordonnées du joueur.
     * @param model Modèle permettant de trouver le joueur.
     */
    void followPlayer(GameModel* model);

    /**
     * @brief Purge la scène de tous ses items visuels.
     */
    void clearScene();

    /**
     * @brief Affiche un écran de transition ou de fin.
     * @param message Texte superposé à afficher (ex: "Game Over").
     */
    void showOverlay(const QString& message);

protected:
    void keyPressEvent(QKeyEvent* e)   override;
    void keyReleaseEvent(QKeyEvent* e) override;
    void mousePressEvent(QMouseEvent* event) override;

    /**
     * @brief Méthode Qt surchargée pour dessiner par-dessus la scène (Affichage tête haute - HUD).
     */
    void drawForeground(QPainter* painter, const QRectF& rect) override;

private:
    void drawHUD(QPainter* painter, GameModel* model);
    void drawHpBar(QPainter* p, int hp, int maxHp, int x, int y);

    QString overlayMessage; ///< Message tampon de fin de partie.
    bool    showingOverlay; ///< Déclencheur visuel de l'overlay sombre.
    QPushButton* menuButton = nullptr;    ///< Bouton pour retourner à l'écran de menu.
    QPushButton* replayButton = nullptr; ///< Bouton pour relancer directement la partie.

signals:
    /** @brief Émis lors d'un clic droit capté dans la zone de jeu. */
    void sig_rightClicked();
    
    /** @brief Émis lors d'un clic gauche capté dans la zone de jeu. */
    void sig_leftClicked();

    void sigRestartRq();

    void sigQuitRq();
};

#endif // GAMEVIEW_H
