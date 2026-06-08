#include "levelbuilder.h"
#include "wallmodel.h"
#include "wallview.h"
#include "zombiestdmodel.h"
#include "zombiestdview.h"
#include "zombielaunchermodel.h"
#include "zombielauncherview.h"
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

    // ─── Tuile de sol (fond de scène) ─────────────────────────
    int sceneW = desc.cols * Cfg::TILE_SIZE;
    int sceneH = desc.rows * Cfg::TILE_SIZE;
    auto* bg = scene->addRect(0, 0, sceneW, sceneH,
                               QPen(Qt::NoPen),
                               QBrush(QColor(40, 38, 32)));  // sol sombre
    bg->setZValue(Cfg::Z_FLOOR);

    // ─── Décodage de la matrice ───────────────────────────────
    // ─── Décodage des deux matrices ───────────────────────────────
    for (int r = 0; r < desc.rows; ++r) {
        for (int c = 0; c < desc.cols; ++c) {

            const float ts_f = static_cast<float>(Cfg::TILE_SIZE);
            Vector2f pos(static_cast<float>(c) * ts_f,
                         static_cast<float>(r) * ts_f);

            // ── Matrice murs ──────────────────────────────────────
            int wallIdx = desc.wallAt(r, c);
            if (wallIdx > 0) {
                auto* wm = new WallModel(pos, ts_f, ts_f,
                                         wallIdx, desc.tilesetCols);
                auto* wv = new WallView(wm);
                levelModel.addWall(wm);
                scene->addItem(wv);
            }

            // ── Matrice entités ───────────────────────────────────
            TileType tile = static_cast<TileType>(desc.entityAt(r, c));
            switch (tile) {

            case TileType::ZOMBIE_SPAWN: {
                auto* zm = new ZombieStdModel(pos);
                auto* zv = new ZombieStdView(zm);
                levelModel.addEnemy(zm);
                scene->addItem(zv);
                break;
            }
            case TileType::LAUNCHER_SPAWN: {
                auto* lm = new ZombieLauncherModel(pos);
                auto* lv = new ZombieLauncherView(lm, scene);
                levelModel.addEnemy(lm);
                scene->addItem(lv);
                break;
            }
            case TileType::EXIT_DOOR: {
                auto* em = new ExitDoorModel(pos, ts_f * 2.0f, ts_f * 2.0f);
                auto* ev = new ExitDoorView(em);
                levelModel.setExitDoor(em);
                scene->addItem(ev);
                break;
            }
            case TileType::PLAYER_SPAWN:
                levelModel.setPlayerSpawnPos(pos);
                break;

            default:
                break;
            }
        }
    }

    // ─── Rectangle de scène ajusté au niveau ──────────────────
    scene->setSceneRect(0, 0, sceneW, sceneH);
}
