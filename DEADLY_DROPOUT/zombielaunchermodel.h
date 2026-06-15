/**
 * @file zombielaunchermodel.h
 * @brief Déclaration du modèle du zombie lanceur de gravats.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef ZOMBIELAUNCHERMODEL_H
#define ZOMBIELAUNCHERMODEL_H

#include "enemy.h"
#include "rubblemodel.h"
#include <QVector>

// ═══════════════════════════════════════════════════════════════════
//  ZombieLauncherModel — zombie lanceur (itération 2).
//
//  IA :
//    IDLE     → aucune action (hors détection)
//    CHASING  → le joueur est détecté MAIS hors portée de tir :
//               l'ennemi le poursuit pour se rapprocher
//    ATTACKING→ le joueur est à portée de tir :
//               l'ennemi reste IMMOBILE et tire un gravat
//               toutes les LAUNCHER_SHOOT_CD secondes
//
//  Propriété des projectiles :
//    Les RubbleModel créés sont stockés dans un vecteur interne.
//    Le LevelController les récupère via getRubbles() pour les mettre
//    à jour et détecter leurs collisions avec le joueur/les murs.
// ═══════════════════════════════════════════════════════════════════

/**
 * @class ZombieLauncherModel
 * @brief Ennemi à distance qui tire des gravats sur le joueur.
 */
class ZombieLauncherModel : public Enemy {
public:
    /**
     * @brief Constructeur.
     * @param pos Position de spawn dans le niveau.
     */
    explicit ZombieLauncherModel(Vector2f pos);
    ~ZombieLauncherModel() override {
        for (RubbleModel* r : rubbles) delete r;
        rubbles.clear();
    }

    // ─── Interface Enemy ──────────────────────────────────────
    /**
     * @brief Met à jour l'IA et les projectiles à chaque tick.
     * @param dt        Delta-time en secondes.
     * @param playerPos Position actuelle du joueur.
     */
    void update(float dt, Vector2f playerPos) override;

    /**
     * @brief Attaque de contact (non utilisée en pratique : l'ennemi tire à distance).
     * @param player Joueur cible.
     */
    void attack(PlayerModel* player) override;

    // ─── Accès aux projectiles ────────────────────────────────
    /**
     * @brief Retourne la liste des gravats actifs tirés par cet ennemi.
     * @return Référence vers le vecteur de RubbleModel*.
     */
    QVector<RubbleModel*>& getRubbles() { return rubbles; }

    /**
     * @brief Supprime de la liste les gravats qui ne sont plus actifs.
     * Doit être appelé par LevelController après traitement des collisions.
     */
    void pruneInactiveRubbles();

private:
    QVector<RubbleModel*> rubbles;  ///< Gravats en vol tirés par cet ennemi.
    float shootTimer;               ///< Temps restant avant le prochain tir autorisé.

    // ─── Sous-méthodes IA ─────────────────────────────────────
    /**
     * @brief Calcule la distance au joueur et met à jour l'état IA.
     * @param playerPos Position du joueur.
     * @return Distance euclidienne entre l'ennemi et le joueur.
     */
    float updateAIState(Vector2f playerPos);

    /**
     * @brief Déplace l'ennemi vers le joueur.
     * @param playerPos Cible.
     * @param dt        Delta-time.
     */
    void chase(Vector2f playerPos, float dt);

    /**
     * @brief Crée un nouveau RubbleModel dirigé vers le joueur.
     * @param playerPos Cible du tir.
     */
    void shoot(Vector2f playerPos);
};

#endif // ZOMBIELAUNCHERMODEL_H
