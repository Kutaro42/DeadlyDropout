/**
 * @file zombiestdview.h
 * @brief Déclaration de la classe ZombieStdView gérant l'apparence visuelle du zombie standard.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef ZOMBIESTDVIEW_H
#define ZOMBIESTDVIEW_H

#include <QGraphicsPixmapItem>
#include <QVector>
#include <QPixmap>
#include "zombiestdmodel.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  ZombieStdView — représentation visuelle du zombie standard.
//
//  Spritesheets (prefix /images, ressources.qrc) :
//    Textures/Enemy/Standard/Idle/{Down,Up,Left,Right}.png → 2 frames 32×32
//    Textures/Enemy/Standard/Move/{Down,Up,Left,Right}.png → 2 frames 32×32
//
//  Direction déduite du vecteur velocity du modèle.
//  Fallback : rectangle vert annoté "Z" si sprite absent.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class ZombieStdView
 * @brief Composant graphique (Vue) du zombie standard.
 * * Hérite de QGraphicsPixmapItem pour l'affichage Qt. Gère les animations de marche et de repos basées sur l'état de l'IA (Idle/Chasing).
 */
class ZombieStdView : public QGraphicsPixmapItem {
public:
    /**
     * @brief Constructeur de la vue du zombie.
     * @param model Pointeur vers les données logiques du zombie (ZombieStdModel).
     * @param parent Élément graphique parent.
     */
    explicit ZombieStdView(ZombieStdModel* model, QGraphicsItem* parent = nullptr);
    
    /**
     * @brief Met à jour la position de l'image et l'animation courante.
     * @param dt Delta-time en secondes écoulé depuis le dernier cycle.
     */
    void updateView(float dt);

private:
    ZombieStdModel* model; ///< Référence vers le modèle du zombie gérant les données d'état (position, vitesse).

    static constexpr int IDLE  = 0;         ///< Constante de l'état immobile.
    static constexpr int WALK  = 1;         ///< Constante de l'état en déplacement (Chasing).
    static constexpr int HURT  = 2;         ///< Constante de l'état blessé.
    static constexpr int DEAD  = 3;         ///< Constante de l'état de mort.
    static constexpr int STATE_COUNT = 4;   ///< Nombre total d'états d'animation gérés.
    static constexpr int DIR_COUNT   = 4;   ///< Nombre total de directions (DOWN, UP, LEFT, RIGHT).
    static constexpr int FRAME_W     = 32;  ///< Largeur unitaire d'une frame lue sur le spritesheet.
    static constexpr int FRAME_H     = 32;  ///< Hauteur unitaire d'une frame lue sur le spritesheet.

    QVector<QPixmap> sprites[STATE_COUNT][DIR_COUNT]; ///< Matrice 2D des sprites (état x direction).

    int   currentFrame; ///< Indice de l'image actuellement rendue.
    float frameTimer;   ///< Horloge accumulant le delta-time pour déclencher le changement de frame.
    float animSpeed;    ///< Délai d'attente (en s) avant de changer d'image de marche.
    float animSpeedDeath;
    int   currentDir;   ///< Index stockant la dernière direction calculée.
    bool  deadAnimFinished; ///< True une fois que l'animation DEAD a été jouée entièrement.

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
     * @brief Construit un carré vert avec l'initiale "Z" en cas de ressource manquante.
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

#endif // ZOMBIESTDVIEW_H
