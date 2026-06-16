/**
 * @file vector2f.h
 * @brief Declaration of the Vector2f math class for the game's 2D geometry.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#ifndef VECTOR2F_H
#define VECTOR2F_H

#include <cmath>
#include <QPointF>

/**
 * @class Vector2f
 * @brief Mathematical class describing 2D vectors (position, velocity).
 * * Facilitates the project's physics calculations independently before conversion to Qt graphics objects.
 */
class Vector2f {
public:
    float x; ///< Coordinate on the horizontal axis.
    float y; ///< Coordinate on the vertical axis.

    /**
     * @brief Constructor with initial coordinates.
     * @param x Horizontal position (0 by default).
     * @param y Vertical position (0 by default).
     */
    Vector2f(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    /**
     * @brief Computes the magnitude (length) of the Euclidean vector.
     * @return The distance from the vector to its origin point (0,0).
     */
    float length() const {
        return std::sqrt(x * x + y * y);
    }

    /**
     * @brief Creates and returns a normalized version of the vector.
     * * Ensures the vector keeps its original direction with a magnitude of 1.
     * @return The normalized unit vector, or (0,0) if the original vector is zero.
     */
    Vector2f normalized() const {
        float len = length();
        if (len > 0) return Vector2f(x / len, y / len);
        return Vector2f(0.0f, 0.0f);
    }

    // ─── Arithmetic operator overloads ─────────────────────
    
    /**
     * @brief Adds two vectors together.
     */
    Vector2f operator+(const Vector2f& v) const { return Vector2f(x + v.x, y + v.y); }
    
    /**
     * @brief Subtracts a vector from this one.
     */
    Vector2f operator-(const Vector2f& v) const { return Vector2f(x - v.x, y - v.y); }
    
    /**
     * @brief Applies a scalar multiplication to the vector.
     */
    Vector2f operator*(float s) const { return Vector2f(x * s, y * s); }
    
    // Qt compatibility
    /**
     * @brief Converts this vector to a QPointF understood by the Qt framework.
     * @return A 2D floating-point instance built from the vector's coordinates.
     */
    QPointF toQPointF() const { return QPointF(x, y); }
};

#endif // VECTOR2F_H
