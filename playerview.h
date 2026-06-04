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
#include "playermodel.h"
#include "config.h"

// ═══════════════════════════════════════════════════════════════════
//  PlayerView — représentation visuelle du joueur.
//
//  Spritesheets (prefix /images, ressources.qrc) :
//    Textures/Player/Idle/{Down,Up,Left,Right}.png   → 2 frames 32×32
//    Textures/Player/Move/{Down,Up,Left,Right}.png   → 2 frames 32×32
//    Textures/Player/Attack/{Down,Up,Left,Right}.png → 3 frames 32×32
//
//  Priorité d'animation : ATTACK > WALK > IDLE
// ═══════════════════════════════════════════════════════════════════

/**
 * @class PlayerView
 * @brief Classe graphique (QGraphicsPixmapItem) gérant les animations et l'affichage du joueur.
 * * Assure la lecture des sprites d'inactivité, de déplacement et d'attaque selon la direction du joueur.
 */
class PlayerView : public QGraphicsPixmapItem {
public:
    /**
     * @brief Constructeur de la vue joueur.
     * @param model Pointeur vers le modèle de données du joueur pour synchroniser l'affichage.
     * @param parent Élément graphique parent.
     */
    explicit PlayerView(PlayerModel* model, QGraphicsItem* parent = nullptr);
    
    /**
     * @brief Met à jour la position graphique et change de frame d'animation si nécessaire.
     * @param dt Delta-time en secondes écoulé depuis la dernière mise à jour.
     */
    void updateView(float dt);

private:
    PlayerModel* model; ///< Pointeur vers le modèle fournissant les données de position et d'état.

    // États d'animation
    static constexpr int IDLE   = 0; ///< Constante définissant l'état inactif.
    static constexpr int WALK   = 1; ///< Constante définissant l'état de marche.
    static constexpr int ATTACK = 2; ///< Constante définissant l'état de combat (katana).

    static constexpr int STATE_COUNT        = 3;  ///< Nombre total d'états gérés.
    static constexpr int DIR_COUNT          = 4;  ///< Nombre d'orientations gérées.
    static constexpr int FRAME_W            = 32; ///< Largeur en pixels d'une frame lue depuis le fichier source.
    static constexpr int FRAME_H            = 32; ///< Hauteur en pixels d'une frame lue depuis le fichier source.
    static constexpr int ATTACK_FRAME_COUNT = 3;  ///< Nombre de frames constituant l'animation d'attaque.

    QVector<QPixmap> sprites[STATE_COUNT][DIR_COUNT]; ///< Matrice de tableaux de frames contenant tous les sprites chargés.

    int   currentFrame;      ///< Index de la frame actuelle affichée.
    float frameTimer;        ///< Timer mesurant le temps d'exposition de la frame courante.
    float animSpeed;         ///< Durée d'affichage (en secondes) d'une frame de marche/repos.
    float attackAnimSpeed;   ///< Durée d'affichage d'une frame d'attaque (souvent plus courte que la marche).

    /**
     * @brief Récupère l'index du tableau associé à la direction donnée.
     * @param d L'énumération FacingDirection.
     * @return Un entier correspondant à l'index de direction de la classe.
     */
    static int  dirIdx(FacingDirection d);
    
    /**
     * @brief Charge et découpe un spritesheet en plusieurs instances QPixmap.
     * @param frames Conteneur dans lequel insérer les images découpées.
     * @param path Chemin relatif (ou via .qrc) vers l'image PNG.
     * @param fw Largeur de découpe unitaire.
     * @param fh Hauteur de découpe unitaire.
     */
    void        loadSheet(QVector<QPixmap>& frames, const QString& path, int fw, int fh);
    
    /**
     * @brief Génère un carré coloré de substitution si un sprite n'a pas été trouvé.
     * @param dir La direction pointée par le carré pour simuler le regard.
     * @return Un objet QPixmap contenant le dessin géométrique provisoire.
     */
    QPixmap     makeFallback(FacingDirection dir);
    
    /**
     * @brief Gère l'incrémentation de la chronologie de l'animation.
     * @param dt Delta-time en secondes.
     * @param frames La collection de frames associées à l'état en cours.
     * @param speed La vitesse requise pour passer à l'image suivante.
     */
    void        advanceFrames(float dt, const QVector<QPixmap>& frames, float speed);
};

#endif // PLAYERVIEW_H