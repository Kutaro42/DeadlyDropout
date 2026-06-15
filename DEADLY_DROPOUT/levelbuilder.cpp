#include "levelbuilder.h"
#include "wallmodel.h"
#include "wallview.h"
#include "floormodel.h"
#include "floorview.h"
#include "zombiestdmodel.h"
#include "zombiestdview.h"
#include "zombielaunchermodel.h"
#include "zombielauncherview.h"
#include "exitdoormodel.h"
#include "exitdoorview.h"
#include "standarddoormodel.h"
#include "standarddoorview.h"
#include "config.h"
#include "bossmodel.h"
#include "bossview.h"
#include "bossdoormodel.h"
#include "bossdoorview.h"

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

    // ─── Décodage des trois matrices ──────────────────────────────────
    for (int r = 0; r < desc.rows; ++r) {
        for (int c = 0; c < desc.cols; ++c) {

            const float ts_f = static_cast<float>(Cfg::TILE_SIZE);
            Vector2f pos(static_cast<float>(c) * ts_f,
                         static_cast<float>(r) * ts_f);

            // ── Matrice sol (décorative, pas de collision) ────────────────
            if (desc.floorData) {
                int floorIdx = desc.floorAt(r, c);
                if (floorIdx > 0) {
                    auto* fm = new FloorModel(pos, ts_f, ts_f,
                                              floorIdx, desc.floorTilesetCols);
                    auto* fv = new FloorView(fm);
                    levelModel.addFloorTile(fm);
                    scene->addItem(fv);
                }
            }

            // ── Matrice murs ──────────────────────────────────────────────
            int wallIdx = desc.wallAt(r, c);
            if (wallIdx > 0) {
                auto* wm = new WallModel(pos, ts_f, ts_f,
                                         wallIdx, desc.tilesetCols);
                auto* wv = new WallView(wm);
                levelModel.addWall(wm);
                scene->addItem(wv);
            }

            // ── Matrice entités ───────────────────────────────────────────
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
            case TileType::STANDARD_DOOR: {
                // Porte sur une tuile simple (1×1), traversée horizontalement ou verticalement.
                // La largeur vaut 2 tuiles et la hauteur 3 tuile pour qu'elle bouche un couloir.
                auto* dm = new StandardDoorModel(pos, ts_f * 2.0f, ts_f*3.0f);
                auto* dv = new StandardDoorView(dm);
                levelModel.addStandardDoor(dm);
                scene->addItem(dv);
                break;
            }
            case TileType::PLAYER_SPAWN:
                levelModel.setPlayerSpawnPos(pos);
                break;
            case TileType::BOSS_SPAWN:{
                auto* BossM = new bossModel(pos, levelModel.getLevelId());
                auto* BossV = new BossView(BossM);
                levelModel.addEnemy(BossM);
                scene->addItem(BossV);
                break;
            }
            case TileType::BOSS_DOOR: {
                // La porte de boss occupe 2×1 tuiles (2 de large, 1 de haut)
                // pour boucher un couloir standard.
                auto* bdm = new BossDoorModel(pos, ts_f * 2.0f, ts_f);
                auto* bdv = new BossDoorView(bdm);
                levelModel.addBossDoor(bdm);
                scene->addItem(bdv);
                break;
            }
            default:
                break;
            }
        }
    }

    // ─── Rectangle de scène ajusté au niveau ──────────────────
    scene->setSceneRect(0, 0, sceneW, sceneH);
}
