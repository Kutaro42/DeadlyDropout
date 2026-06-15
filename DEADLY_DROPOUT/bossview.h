#ifndef BOSSVIEW_H
#define BOSSVIEW_H

#include <QGraphicsPixmapItem>
#include <QVector>
#include <QPixmap>
#include "bossmodel.h"
#include "config.h"

class BossView : public QGraphicsPixmapItem
{
public:
    BossView(bossModel* model, QGraphicsItem* parent = nullptr);
    ~BossView();

    void updateView(float dt);

private:
    bossModel* model;

    static constexpr int IDLE  = 0;         ///< Constante de l'état immobile.
    static constexpr int WALK  = 1;         ///< Constante de l'état en déplacement (Chasing).
    static constexpr int HURT  = 2;         ///< Constante de l'état blessé.
    static constexpr int DEAD  = 3;         ///< Constante de l'état de mort.
    static constexpr int STATE_COUNT = 4;   ///< Nombre total d'états d'animation gérés.
    static constexpr int DIR_COUNT   = 4;   ///< Nombre total de directions (DOWN, UP, LEFT, RIGHT).
    static constexpr int FRAME_W     = 48;  ///< Largeur unitaire d'une frame lue sur le spritesheet.
    static constexpr int FRAME_H     = 48;  ///< Hauteur unitaire d'une frame    lue sur le spritesheet.

    QVector<QPixmap> sprites[STATE_COUNT][DIR_COUNT];

    int   currentFrame; ///< Indice de l'image actuellement rendue.
    float frameTimer;   ///< Horloge accumulant le delta-time pour déclencher le changement de frame.
    float animSpeed;    ///< Délai d'attente (en s) avant de changer d'image de marche.
    float animSpeedDeath;
    int   currentDir;   ///< Index stockant la dernière direction calculée.
    bool  deadAnimFinished; ///< True une fois que l'animation DEAD a été jouée entièrement.

    // ─── Zone d'attaque (pattern 2) ───────────────────────────────
    QVector<QGraphicsRectItem*> zoneItems;  ///< Items Qt des tuiles de zone.
    bool zoneVisible;                       ///< true si les items sont dans la scène.

    void showZone();   ///< Crée et ajoute les items de zone à la scène.
    void hideZone();   ///< Retire et détruit les items de zone (safe contre double-free).


    /**
     * @brief Convertit l'énumération FacingDirection en index entier de tableau.
     * @param d Direction cardinale évaluée.
     * @return Index correspondant (0 = Bas, 1 = Haut, 2 = Gauche, 3 = Droite).
     */
    static int  dirIdx(FacingDirection d);

    /**
     * @brief Estime la direction cardinale dominante à partir d'un vecteur de déplacement cinématique.
     * @param vel Le vecteur de vitesse actuel.
     * @return L'orientation FacingDirection la plus probable.
     */
    static FacingDirection velocityToDir(Vector2f vel);

    /**
     * @brief Extrait et dimensionne les différentes frames d'une feuille de sprites horizontale.
     * @param frames Liste d'images (QVector) où stocker les découpes.
     * @param path Emplacement du fichier PNG dans les ressources Qt.
     */
    void        loadSheet(QVector<QPixmap>& frames, const QString& path);

    /**
     * @brief Construit un carré vert avec l'initiale "B" en cas de ressource manquante.
     * @return Une image générée dynamiquement au bon format.
     */
    QPixmap     makeFallback();

    /**
     * @brief Fait avancer le lecteur de frame de l'animation en cours.
     * @param dt Fraction de seconde écoulée.
     * @param frames Pile des images composant l'animation ciblée.
     */
    void        advanceFrames(float dt, const QVector<QPixmap>& frames);

};

#endif // BOSSVIEW_H
