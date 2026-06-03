#include "levelbuilder.h"
#include "wallmodel.h"
#include "wallview.h"
#include "zombiestdmodel.h"
#include "zombiestdview.h"
#include "exitdoormodel.h"
#include "exitdoorview.h"
#include "config.h"

void LevelBuilder::build(const LevelDescriptor& desc,
                          LevelModel&            levelModel,
                          QGraphicsScene*        scene)
{
    // Vide la scène et le modèle avant de reconstruire
    scene->clear();
    levelModel.clear();
    levelModel.setLevelId(desc.id);

    const float ts = static_cast<float>(Cfg::TILE_SIZE);

    // ─── Tuile de sol (fond de scène) ─────────────────────────
    int sceneW = desc.cols * Cfg::TILE_SIZE;
    int sceneH = desc.rows * Cfg::TILE_SIZE;
    auto* bg = scene->addRect(0, 0, sceneW, sceneH,
                               QPen(Qt::NoPen),
                               QBrush(QColor(40, 38, 32)));  // sol sombre
    bg->setZValue(Cfg::Z_FLOOR);

    // ─── Décodage de la matrice ───────────────────────────────
    for (int r = 0; r < desc.rows; ++r) {
        for (int c = 0; c < desc.cols; ++c) {

            TileType tile = static_cast<TileType>(desc.at(r, c));
            Vector2f pos(static_cast<float>(c) * ts,
                         static_cast<float>(r) * ts);

            switch (tile) {

                case TileType::WALL: {
                    auto* wm = new WallModel(pos, ts, ts);
                    auto* wv = new WallView(wm);
                    levelModel.addWall(wm);
                    scene->addItem(wv);
                    break;
                }

                case TileType::ZOMBIE_SPAWN: {
                    auto* zm = new ZombieStdModel(pos);
                    auto* zv = new ZombieStdView(zm);
                    levelModel.addEnemy(zm);
                    scene->addItem(zv);
                    break;
                }

                case TileType::EXIT_DOOR: {
                    auto* em = new ExitDoorModel(pos, ts * 2.0f, ts * 2.0f);
                    auto* ev = new ExitDoorView(em);
                    levelModel.setExitDoor(em);
                    scene->addItem(ev);
                    break;
                }

                case TileType::PLAYER_SPAWN:
                    levelModel.setPlayerSpawnPos(pos);
                    break;

                case TileType::FLOOR:
                default:
                    break;
            }
        }
    }

    // ─── Rectangle de scène ajusté au niveau ──────────────────
    scene->setSceneRect(0, 0, sceneW, sceneH);
}
