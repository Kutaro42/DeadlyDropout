/**
 * @file matrixextractor.cpp
 * @brief Implémentation de MatrixExtractor et DynamicLevelDescriptor.
 */

#include "matrixextractor.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

// ─── DynamicLevelDescriptor ───────────────────────────────────────

LevelDescriptor DynamicLevelDescriptor::asDescriptor() const {
    LevelDescriptor d;
    d.id               = id;
    d.name             = name;
    d.rows             = rows;
    d.cols             = cols;
    d.tilesetCols      = tilesetCols;
    d.floorTilesetCols = floorTilesetCols;

    // Les pointeurs raw sont valides tant que ce DynamicLevelDescriptor
    // reste en vie. Le LevelDescriptor ne possède PAS ces données.
    d.entityData = entityBuf.isEmpty() ? nullptr : entityBuf.constData();
    d.wallData   = wallBuf.isEmpty()   ? nullptr : wallBuf.constData();
    d.floorData  = floorBuf.isEmpty()  ? nullptr : floorBuf.constData();

    return d;
}

// ─── MatrixExtractor::readMatrixFile ─────────────────────────────

QVector<int> MatrixExtractor::readMatrixFile(const QString& filePath,
                                              int&           rows,
                                              int&           cols)
{
    rows = 0;
    cols = 0;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return {};
    }

    QTextStream in(&file);

    // ── Ligne d'en-tête : "<rows> <cols>" ─────────────────────────
    if (in.atEnd()) {
        return {};
    }
    in >> rows >> cols;
    if (rows <= 0 || cols <= 0) {
        return {};
    }

    // ── Données : rows lignes de cols entiers ──────────────────────
    QVector<int> buf;
    buf.reserve(rows * cols);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (in.atEnd()) {
                return {};
            }
            int val;
            in >> val;
            buf.append(val);
        }
    }

    return buf;
}

// ─── MatrixExtractor::load ────────────────────────────────────────

DynamicLevelDescriptor MatrixExtractor::load(int            levelId,
                                             const QString& basePath,
                                             int            tilesetCols,
                                             int            floorTilesetCols)
{
    DynamicLevelDescriptor dld;
    dld.id               = levelId;
    dld.name             = QString("Niveau %1").arg(levelId);
    dld.rows             = 0;
    dld.cols             = 0;
    dld.tilesetCols      = tilesetCols;
    dld.floorTilesetCols = floorTilesetCols;

    const QString dir = QString("%1/level_%2").arg(basePath).arg(levelId);

    // ── Lecture entités ───────────────────────────────────────────
    int rowsE = 0, colsE = 0;
    dld.entityBuf = readMatrixFile(dir + "/entities.txt", rowsE, colsE);
    if (dld.entityBuf.isEmpty()) {
        return dld; // rows == 0 → appelant peut détecter l'échec
    }

    // ── Lecture murs ──────────────────────────────────────────────
    int rowsW = 0, colsW = 0;
    dld.wallBuf = readMatrixFile(dir + "/walls.txt", rowsW, colsW);
    if (dld.wallBuf.isEmpty()) {
        return dld;
    }

    // ── Lecture sol ───────────────────────────────────────────────
    int rowsF = 0, colsF = 0;
    dld.floorBuf = readMatrixFile(dir + "/floor.txt", rowsF, colsF);
    // La matrice sol est optionnelle : si absente, floorBuf reste vide
    // et LevelBuilder ignorera la couche sol (floorData == nullptr).
    if (!dld.floorBuf.isEmpty() && (rowsF != rowsE || colsF != colsE)) {
        dld.floorBuf.clear();
    }

    // ── Validation cohérence entités / murs ───────────────────────
    if (rowsW != rowsE || colsW != colsE) {
        dld.entityBuf.clear();
        dld.wallBuf.clear();
        return dld;
    }

    dld.rows = rowsE;
    dld.cols = colsE;

    return dld;
}
