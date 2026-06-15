/**
 * @file zombiestdmodel.h
 * @brief Déclaration de la classe ZombieStdModel gérant les données et la logique du zombie.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef ZOMBIESTDMODEL_H
#define ZOMBIESTDMODEL_H

#include "enemy.h"

// ═══════════════════════════════════════════════════════════════════
//  ZombieStdModel — zombie standard (itération 1).
//  IA : IDLE → CHASING (détection) → ATTACKING (contact).
//  Stats (GDD) : 20 HP · 90 px/s · 7~12 dégâts · portée 400 px.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class ZombieStdModel
 * @brief Logique métier d'un ennemi "zombie standard" implémentant la classe parente Enemy.
 * * Équipé d'une machine à état simple (IDLE/CHASING) et pouvant infliger des dégâts aléatoires au joueur.
 */
class ZombieStdModel : public Enemy {
public:
    /**
     * @brief Constructeur configurant les caractéristiques du zombie.
     * * Assigne 20 HP, 90 px/s et une zone de détection selon le GDD.
     * @param pos Emplacement (X, Y) initial de l'apparition de l'ennemi.
     */
    explicit ZombieStdModel(Vector2f pos);

    /**
     * @brief Exécute la logique de l'IA (mise à jour état, gestion des temps de pause) à chaque tick.
     * @param dt Écart de temps en secondes.
     * @param playerPos Position du joueur servant de cible.
     */
    void update(float dt, Vector2f playerPos) override;
    
    /**
     * @brief S'exécute lors d'une collision joueur-ennemi, appliquant de 7 à 12 dégâts.
     * @param player Pointeur de modèle ciblant le joueur affecté par le coup.
     */
    void attack(PlayerModel* player) override;

private:
    /**
     * @brief Pilote l'approche rectiligne en direction du joueur, si celui-ci est détecté.
     * @param playerPos Coordonnées actuelles de la cible.
     * @param dt Delta-time pour normaliser le déplacement.
     */
    void chase(Vector2f playerPos, float dt);
    
    /**
     * @brief Analyse la distance avec le joueur pour faire basculer l'état interne de l'ennemi entre repos (IDLE) et traque (CHASING).
     * @param playerPos Coordonnées du joueur testées dans le rayon de détection.
     */
    void updateAIState(Vector2f playerPos);
};

#endif // ZOMBIESTDMODEL_H