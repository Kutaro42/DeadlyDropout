/**
 * @file player.h
 * @brief Classe expérimentale ou simplifiée pour représenter le joueur.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

/**
 * @class Player
 * @brief Modèle simplifié d'un joueur, héritant de Entity.
 * * Note : L'application utilise principalement `PlayerModel`. Cette classe encapsule la vitesse, l'or et les points de vie.
 */
class Player : public Entity {
private:
    int hp;       ///< Points de vie actuels du joueur.
    int maxHp;    ///< Points de vie maximum.
    int gold;     ///< Quantité de monnaie accumulée par le joueur.
    float speed;  ///< Vitesse de déplacement du joueur.

public:
    /**
     * @brief Constructeur initialisant les valeurs par défaut du joueur.
     * @param pos Position vectorielle initiale du joueur.
     */
    Player(Vector2f pos) : Entity(pos, 40.0f, 40.0f), hp(100), maxHp(100), gold(0), speed(150.0f) {}

    /**
     * @brief Met à jour la position du joueur en fonction de sa vitesse.
     * @param dt Temps écoulé (delta-time) en secondes depuis la dernière mise à jour.
     */
    void update(float dt) override {
        position = position + (velocity * speed * dt);
    }

    /**
     * @brief Applique des dégâts au joueur et gère la désactivation s'il n'a plus de vie.
     * @param amount Nombre de points de dégâts infligés.
     */
    void takeDamage(int amount) {
        hp -= amount;
        if (hp <= 0) {
            hp = 0;
            setActive(false);
        }
    }

    /**
     * @brief Ajoute un montant d'or à la réserve du joueur.
     * @param amount Nombre de pièces d'or obtenues.
     */
    void addGold(int amount) { gold += amount; }
    
    /**
     * @brief Récupère l'or actuel du joueur.
     * @return Quantité de monnaie détenue.
     */
    int getGold() const { return gold; }
    
    /**
     * @brief Récupère les points de vie actuels du joueur.
     * @return Points de vie.
     */
    int getHp() const { return hp; }
};

#endif // PLAYER_H