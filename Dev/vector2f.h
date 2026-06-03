#ifndef VECTOR2F_H
#define VECTOR2F_H

#include <cmath>
#include <QPointF>

// Classe spécialisée pour les vecteurs de position et de vitesse
class Vector2f {
public:
    float x;
    float y;

    Vector2f(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2f normalized() const {
        float len = length();
        if (len > 0) return Vector2f(x / len, y / len);
        return Vector2f(0.0f, 0.0f);
    }

    Vector2f operator+(const Vector2f& v) const { return Vector2f(x + v.x, y + v.y); }
    Vector2f operator-(const Vector2f& v) const { return Vector2f(x - v.x, y - v.y); }
    Vector2f operator*(float s) const { return Vector2f(x * s, y * s); }
    
    // Compatibilité Qt
    QPointF toQPointF() const { return QPointF(x, y); }
};

#endif // VECTOR2F_H