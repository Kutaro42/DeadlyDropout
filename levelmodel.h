#ifndef LEVELMODEL_H
#define LEVELMODEL_H

#include <vector>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "vector2f.h"

template <typename T>
class LevelModel {
private:
    std::vector<std::vector<T>> matrix;
    int tileSize;

public:
    LevelModel(int tileSize = 32) : tileSize(tileSize) {}

    void setMatrix(const std::vector<std::vector<T>>& newMatrix) {
        matrix = newMatrix;
    }

    // Fonction de décodage pour l'affichage et l'extraction des points d'apparition
    void decodeLevel(QGraphicsScene* scene, Vector2f& playerSpawnPos, std::vector<Vector2f>& zombieSpawnPositions) {
        if (matrix.empty()) return;

        // Création de textures par défaut (Sol et Murs)
        QPixmap wallPixmap(tileSize, tileSize);
        wallPixmap.fill(Qt::darkGray);
        QPixmap floorPixmap(tileSize, tileSize);
        floorPixmap.fill(Qt::lightGray);

        for (size_t r = 0; r < matrix.size(); ++r) {
            for (size_t c = 0; c < matrix[r].size(); ++c) {
                T cellValue = matrix[r][c];
                float posX = c * tileSize;
                float posY = r * tileSize;

                // Application du sol sous toutes les entités par défaut
                if (static_cast<int>(cellValue) != 1) {
                    QGraphicsPixmapItem* floorItem = new QGraphicsPixmapItem(floorPixmap);
                    floorItem->setPos(posX, posY);
                    floorItem->setZValue(-2);
                    scene->addItem(floorItem);
                }

                // Décodage des éléments de la matrice
                switch (static_cast<int>(cellValue)) {
                    case 1: { // Mur
                        QGraphicsPixmapItem* wallItem = new QGraphicsPixmapItem(wallPixmap);
                        wallItem->setPos(posX, posY);
                        wallItem->setZValue(-1);
                        scene->addItem(wallItem);
                        break;
                    }
                    case 2: // Position d'apparition Joueur
                        playerSpawnPos = Vector2f(posX + tileSize / 2.0f, posY + tileSize / 2.0f);
                        break;
                    case 3: // Position d'apparition Zombie
                        zombieSpawnPositions.push_back(Vector2f(posX + tileSize / 2.0f, posY + tileSize / 2.0f));
                        break;
                    default:
                        break;
                }
            }
        }
    }
};

#endif // LEVELMODEL_H