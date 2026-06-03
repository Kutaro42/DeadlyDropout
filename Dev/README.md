# Deadly Dropout — Itération 1

## Mise en route

1. Ouvrir `deadlydropout.pro` dans Qt Creator
2. Créer les sprites dans `assets/` (voir `assets.qrc`)
3. Décommenter `RESOURCES += assets.qrc` dans le `.pro` quand les assets sont prêts
4. **Build → Run** (le jeu tourne sans assets grâce aux fallbacks colorés)

## Contrôles

| Touche | Action |
|--------|--------|
| Z / ↑ | Monter |
| S / ↓ | Descendre |
| Q / ← | Gauche |
| D / → | Droite |
| Espace / J | Attaque katana |
| Échap | Quitter (après game over / victoire) |

## Ajouter un niveau

Dans `leveltemplate.h`, namespace `LevelData` :
1. Définir `inline constexpr LevelMatrix<ROWS, COLS> LEVEL_3 = {{ ... }};`
2. Créer `inline const LevelDescriptor LEVEL_3_DESC = makeDescriptor(3, "Niveau 3", LEVEL_3);`
3. Ajouter `&LEVEL_3_DESC` dans `ALL_LEVELS[]` et incrémenter `LEVEL_COUNT`

**Clés de la matrice :** 0=sol · 1=mur · 2=zombie · 3=sortie · 4=spawn joueur

## Structure MVC

```
Controller : AppController → GameController → LevelController
Model      : GameModel → PlayerModel + LevelModel → WallModel / ZombieStdModel / ExitDoorModel
View       : GameView (QGraphicsView) → PlayerView / ZombieStdView / WallView / ExitDoorView
```
