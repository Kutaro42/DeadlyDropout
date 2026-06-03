#ifndef LEVELBUILDER_H
#define LEVELBUILDER_H

#include "leveltemplate.h"
#include "levelmodel.h"
#include <QGraphicsScene>

// ═══════════════════════════════════════════════════════════════════
//  LevelBuilder — décode une matrice de niveau et instancie
//  les objets Model + les items de scène (View).
//
//  Utilisation :
//    LevelBuilder::build(LevelData::LEVEL_1_DESC, levelModel, scene);
//
//  Pour ajouter un niveau :
//    1. Définir une LevelMatrix<R,C> dans LevelData (leveltemplate.h)
//    2. Créer son LevelDescriptor avec makeDescriptor()
//    3. L'ajouter dans LevelData::ALL_LEVELS[]
// ═══════════════════════════════════════════════════════════════════
class LevelBuilder {
public:
    // Construit un niveau depuis un LevelDescriptor :
    //   - Remplit levelModel (WallModel, Enemy*, ExitDoorModel)
    //   - Ajoute WallView, ZombieStdView, ExitDoorView dans la scène
    static void build(const LevelDescriptor& desc,
                      LevelModel&            levelModel,
                      QGraphicsScene*        scene);

    // Surcharge template : évite de créer le descripteur manuellement
    template<int ROWS, int COLS>
    static void build(int id, const QString& name,
                      const LevelMatrix<ROWS, COLS>& matrix,
                      LevelModel&     levelModel,
                      QGraphicsScene* scene)
    {
        auto desc = makeDescriptor(id, name, matrix);
        build(desc, levelModel, scene);
    }
};

#endif // LEVELBUILDER_H
