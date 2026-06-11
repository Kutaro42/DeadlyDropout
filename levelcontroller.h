/**
 * @file levelcontroller.h
 * @brief Déclaration du LevelController, le moteur logique des niveaux.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef LEVELCONTROLLER_H
#define LEVELCONTROLLER_H

#include "levelmodel.h"
#include "playermodel.h"
#include "zombielaunchermodel.h"
#include <QRectF>

/**
 * @class LevelController
 * @brief Orchestre la logique physique et les règles métier d'un niveau.
 * * Ce contrôleur gère la mise à jour de l'IA ennemie, la résolution des collisions AABB,
 * les interactions de combat (SlashZone), la distribution du butin (gold) et la
 * validation des conditions de victoire.
 * * @version 1.0
 */
class LevelController {
public:
    /**
     * @brief Constructeur par défaut.
     */
    LevelController();

    /**
     * @brief Met à jour toute la logique du niveau.
     * * Appelé à chaque itération de la boucle principale par le GameController.
     * * @param dt Temps écoulé (Delta-time) depuis la dernière frame.
     * @param player Référence au modèle du joueur.
     * @param level Référence au modèle du niveau en cours.
     */
    void update(float dt, PlayerModel& player, LevelModel& level);

    bool levelJustCompleted; ///< Drapeau mis à vrai lorsque le niveau est terminé et réussi.

private:
    // ─── Sous-étapes de mise à jour ───────────────────────────
    void updateEnemies(float dt, PlayerModel& player, LevelModel& level); ///< Met à jour l'IA des ennemis.
    void distributeGold(PlayerModel& player, LevelModel& level);          ///< Distribue l'or des ennemis morts.
    void resolvePlayerWallCollisions(PlayerModel& player, LevelModel& level); ///< Gère les collisions joueur/murs.
    void resolveEnemyWallCollisions(LevelModel& level);                   ///< Gère les collisions ennemis/murs.
    void checkPlayerEnemyCollisions(PlayerModel& player, LevelModel& level);  ///< Vérifie si les ennemis touchent le joueur.
    void checkSlashZoneHits(PlayerModel& player, LevelModel& level);      ///< Applique les dégâts du katana aux ennemis.
    void checkExitCondition(PlayerModel& player, LevelModel& level);      ///< Vérifie si la porte de sortie doit s'ouvrir.
    void checkAndResolvePlayerEnemyCollisions(PlayerModel& player, LevelModel& level); ///< Attaque + séparation physique joueur/ennemis en une passe.
    void resolvePlayerVsEnemyWalls(PlayerModel& player, LevelModel& level);            ///< Bloque le joueur contre les ennemis (ennemis = obstacles solides).
    void resolveEnemyEnemyCollisions(LevelModel& level);                               ///< Empêche les ennemis de se chevaucher entre eux.
    void checkRubbleCollisions(PlayerModel& player, LevelModel& level);                ///< Détecte les impacts des gravats sur le joueur et les murs.
    void checkBulletCollisions(PlayerModel& player, LevelModel& level);
    void checkStandardDoorInteractions(PlayerModel& player, LevelModel& level);        ///< Ouvre les portes standards au contact du joueur et résout leur collision.
    void checkBossDoorInteractions(PlayerModel& player, LevelModel& level);             ///< Ouvre les portes de boss à la mort du boss et résout leur collision.

    /**
     * @brief Résout une collision AABB (Axis-Aligned Bounding Box).
     * * Repousse l'entité mobile en dehors de l'obstacle en modifiant directement sa position.
     * * @param mover L'entité mobile (Joueur ou Ennemi).
     * @param obstacle Le rectangle de collision de l'obstacle (Mur).
     */
    void resolveAABB(Entity& mover, const QRectF& obstacle);
};

#endif // LEVELCONTROLLER_H
