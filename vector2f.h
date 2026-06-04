/**
 * @file vector2f.h
 * @brief Déclaration de la classe mathématique Vector2f pour la géométrie 2D du jeu.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef VECTOR2F_H
#define VECTOR2F_H

#include <cmath>
#include <QPointF>

/**
 * @class Vector2f
 * @brief Classe mathématique décrivant des vecteurs en 2 dimensions (position, vitesse).
 * * Facilite les calculs physiques du projet de manière indépendante avant conversion vers les objets graphiques Qt.
 */
class Vector2f {
public:
    float x; ///< Coordonnée sur l'axe des abscisses.
    float y; ///< Coordonnée sur l'axe des ordonnées.

    /**
     * @brief Constructeur avec des coordonnées initiales.
     * @param x Position horizontale (0 par défaut).
     * @param y Position verticale (0 par défaut).
     */
    Vector2f(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    /**
     * @brief Calcule la magnitude (longueur) totale du vecteur euclidien.
     * @return La distance séparant le vecteur de son point d'origine (0,0).
     */
    float length() const {
        return std::sqrt(x * x + y * y);
    }

    /**
     * @brief Crée et retourne une version normalisée du vecteur.
     * * Assure que le vecteur conserve sa direction d'origine avec une magnitude équivalente à 1.
     * @return Le vecteur unitaire normalisé, ou (0,0) si le vecteur initial est nul.
     */
    Vector2f normalized() const {
        float len = length();
        if (len > 0) return Vector2f(x / len, y / len);
        return Vector2f(0.0f, 0.0f);
    }

    // ─── Surcharges des opérateurs arithmétiques ─────────────────────
    
    /**
     * @brief Additionne deux vecteurs ensemble.
     */
    Vector2f operator+(const Vector2f& v) const { return Vector2f(x + v.x, y + v.y); }
    
    /**
     * @brief Soustrait un vecteur à celui-ci.
     */
    Vector2f operator-(const Vector2f& v) const { return Vector2f(x - v.x, y - v.y); }
    
    /**
     * @brief Applique une multiplication scalaire au vecteur.
     */
    Vector2f operator*(float s) const { return Vector2f(x * s, y * s); }
    
    // Compatibilité Qt
    /**
     * @brief Convertit ce vecteur en un point QPointF compréhensible par le framework Qt.
     * @return Une instance de point 2D flottant issue des coordonnées du vecteur.
     */
    QPointF toQPointF() const { return QPointF(x, y); }
};

#endif // VECTOR2F_H