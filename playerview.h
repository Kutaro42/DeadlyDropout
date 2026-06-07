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
//    Textures/Player/Idle/{Down,Up,Left,Right}.png   → 2 frames 36×36
//    Textures/Player/Move/{Down,Up,Left,Right}.png   → 2 frames 36×36
//    Textures/Player/Attack/{Down,Up,Left,Right}.png → 6 frames 128×128
//
//  Priorité d'animation : ATTACK > WALK > IDLE
//
//  Centrage : le sprite peut avoir une taille d'affichage différente selon
//  l'état (SPRITE_SIZE[état]) sans affecter la hitbox. setOffset() recentre
//  automatiquement le pixmap sur la hitbox (ENTITY_SIZE × ENTITY_SIZE).
//
//  Correction bug frames manquées :
//    L'animation d'attaque utilise ses propres timers (attackFrame,
//    attackFrameTimer, attackTotalTimer), indépendants de currentFrame et
//    frameTimer utilisés par IDLE/WALK. Le reset est déclenché une seule
//    fois à l'entrée de l'état ATTACK via le flag wasKatanaActive.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class PlayerView
 * @brief Classe graphique (QGraphicsPixmapItem) gérant les animations et l'affichage du joueur.
 *
 * Assure la lecture des sprites d'inactivité, de déplacement et d'attaque selon
 * la direction du joueur. Chaque état peut avoir un sprite de taille différente :
 * la boîte de collision reste inchangée (ENTITY_SIZE²), et setOffset() centre
 * visuellement le pixmap sur cette hitbox.
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

    // ─── États d'animation ────────────────────────────────────────
    static constexpr int IDLE   = 0; ///< Constante définissant l'état inactif.
    static constexpr int WALK   = 1; ///< Constante définissant l'état de marche.
    static constexpr int ATTACK = 2; ///< Constante définissant l'état de combat (katana).

    static constexpr int STATE_COUNT        = 3;  ///< Nombre total d'états gérés.
    static constexpr int DIR_COUNT          = 4;  ///< Nombre d'orientations gérées.
    static constexpr int ATTACK_FRAME_COUNT = 6;  ///< Nombre de frames constituant l'animation d'attaque.

    // ─── Dimensions source (pixels dans le fichier PNG) par état ──
    static constexpr int FRAME_W[STATE_COUNT] = { 36, 36, 128 }; ///< Largeur d'une frame source par état.
    static constexpr int FRAME_H[STATE_COUNT] = { 36, 36, 128 }; ///< Hauteur d'une frame source par état.

    // ─── Taille d'affichage cible (pixels à l'écran) par état ─────
    /**
     * @brief Taille du sprite rendu à l'écran pour chaque état.
     *
     * Peut différer de ENTITY_SIZE sans impacter la hitbox. Le sprite est
     * toujours centré sur la hitbox via applyCenter().
     */
    static constexpr int SPRITE_SIZE[STATE_COUNT] = {
        Cfg::ENTITY_SIZE,  ///< IDLE   : légèrement élargi.
        Cfg::ENTITY_SIZE,  ///< WALK   : légèrement élargi.
        Cfg::ENTITY_SIZE + 128, ///< ATTACK : très élargi pour le swing du katana.
    };

    QVector<QPixmap> sprites[STATE_COUNT][DIR_COUNT]; ///< Matrice de frames pour tous les états et directions.
    QPixmap          cachedFallback[DIR_COUNT];        ///< Fallbacks précalculés (évite QPainter à chaque tick).

    // ─── Timers IDLE / WALK (partagés, cycliques) ─────────────────
    int   currentFrame;  ///< Index de frame courant pour IDLE et WALK.
    float frameTimer;    ///< Temps écoulé sur la frame courante (IDLE/WALK).
    float animSpeed;     ///< Durée d'affichage d'une frame IDLE/WALK en secondes.

    // ─── Timers ATTACK (dédiés, remis à 0 à chaque nouvelle attaque) ──
    /**
     * @brief Index de la frame actuellement affichée pendant l'animation d'attaque.
     *
     * Indépendant de currentFrame : remis à 0 à chaque déclenchement d'attaque,
     * ce qui garantit que la frame 0 est toujours la première affichée.
     */
    int   attackFrame;

    /**
     * @brief Temps écoulé sur la frame d'attaque courante.
     *
     * Remis à 0 à chaque déclenchement d'attaque, indépendamment de frameTimer.
     */
    float attackFrameTimer;

    /**
     * @brief Durée totale écoulée depuis le début de l'animation d'attaque en cours.
     *
     * L'animation se joue jusqu'à attackTotalTimer >= ATTACK_FRAME_COUNT * attackAnimSpeed,
     * indépendamment de itsAnimTimer dans Katana. Cela évite la troncature de la
     * dernière frame causée par la désynchronisation entre le timer modèle et le dt
     * de la vue.
     */
    float attackTotalTimer;

    /**
     * @brief Durée d'affichage d'une frame d'attaque en secondes.
     *
     * Calculée comme Cfg::KATANA_ANIM_DUR / ATTACK_FRAME_COUNT.
     */
    float attackAnimSpeed;

    /**
     * @brief Mémorise l'état katana du tick précédent pour détecter le front montant.
     *
     * Passe à true uniquement quand isKatanaActive() bascule de false à true,
     * ce qui déclenche le reset des timers d'attaque (attackFrame, attackFrameTimer,
     * attackTotalTimer). Sans ce flag, un reset à chaque tick écraserait l'animation.
     */
    bool wasKatanaActive;

    /**
     * @brief Vaut true dès qu'une première attaque a été déclenchée.
     *
     * Empêche la condition (attackTotalTimer < attackFullDuration) de déclencher
     * l'animation au démarrage du jeu, où attackTotalTimer vaut 0 et serait
     * immédiatement inférieur à attackFullDuration sans que le joueur ait attaqué.
     */
    bool attackEverTriggered;

    // ─── Helpers ──────────────────────────────────────────────────
    /**
     * @brief Récupère l'index du tableau associé à la direction donnée.
     * @param d L'énumération FacingDirection.
     * @return Un entier correspondant à l'index de direction de la classe.
     */
    static int  dirIdx(FacingDirection d);

    /**
     * @brief Charge et découpe un spritesheet en plusieurs instances QPixmap.
     * @param frames     Conteneur dans lequel insérer les images découpées.
     * @param path       Chemin relatif (ou via .qrc) vers l'image PNG.
     * @param fw         Largeur de découpe unitaire dans le fichier source.
     * @param fh         Hauteur de découpe unitaire dans le fichier source.
     * @param targetSize Taille cible d'affichage à l'écran.
     */
    void        loadSheet(QVector<QPixmap>& frames, const QString& path,
                          int fw, int fh, int targetSize);

    /**
     * @brief Génère un carré coloré de substitution si un sprite n'a pas été trouvé.
     * @param dir La direction pointée par le carré pour simuler le regard.
     * @return Un objet QPixmap contenant le dessin géométrique provisoire.
     */
    QPixmap     makeFallback(FacingDirection dir);

    /**
     * @brief Gère l'incrémentation de l'animation pour les états IDLE et WALK.
     * @param dt     Delta-time en secondes.
     * @param frames La collection de frames associées à l'état en cours.
     * @param speed  Durée d'affichage d'une frame.
     */
    void        advanceFrames(float dt, const QVector<QPixmap>& frames, float speed);

    /**
     * @brief Centre le pixmap sur la hitbox en ajustant setOffset().
     *
     * setPos() reste toujours ancré sur le coin supérieur gauche de la hitbox.
     * Si le sprite est plus grand que ENTITY_SIZE, l'offset est négatif et le
     * fait déborder symétriquement sans affecter la boîte de collision.
     *
     * @param spriteSize Taille en pixels du pixmap actuellement affiché.
     */
    void        applyCenter(int spriteSize);
};

#endif // PLAYERVIEW_H
