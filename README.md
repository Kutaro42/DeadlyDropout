\# Deadly Dropout



> Roguelike 2D Vue axionométrique IUT La Rochelle, BUT1 Groupe B13, 2025-2026  

> Alexandre AUBIN · Iaviantsoa RASOLOFOHARIVELO · Babacar DIOP · Léo BOUCARD



\---



\## Mise en route



1\. Obtenir depuis le dépôt Git de la dernière version avec : 

la commande `git clone https://forge.iut-larochelle.fr/2025-2026-but-info1-super_sae/td_b/tp_b1/b13/deadly-dropout.git` :

ouvrer un terminal à l'endroit où vous souhaiteriez installer le jeu et lancer cette commande.



2\. \*\*Rendez-vous dans le dossier nouvellement créé "deadly-dropout/" puis dans le dossier "Executable (Application)/" dans lequel vous pourrez retrouver l'executable correspondant à votre système d'exploitation dans le dossier dédié.\*\*



3\. \*\*Double-cliquer sur l'executable, et profiter du jeu !\*\*



\---



\## Contrôles



| Touche | Action |

|--------|--------|

| Z / ↑ | Monter |

| S / ↓ | Descendre |

| Q / ← | Gauche |

| D / → | Droite |

| Clic gauche | Tirer (pistolet) |

| Clic droit / Espace | Attaque katana |

| R | Recharger |

| Échap | Quitter (depuis l'écran de game over ou de victoire) |



\---



\## Mécaniques principales



\### Joueur

\- Vitesse de déplacement : \*\*300 px/s\*\* (modifiable par le SpeedPowerUp)

\- Déplacement sur 4 axes + diagonales (ZQSD / touches directionnelles)

\- Fenêtre de jeu : \*\*960 × 650 px\*\*



\### Katana

\- Dégâts de base : \*\*30\*\* par coup

\- Portée de base : \*\*20 px\*\* (zone de slash autour du joueur)

\- Attaque déclenchée à l'angle exact du curseur souris



\### Pistolet

\- Dégâts de base : \*\*12\*\* par balle + bonus aléatoire de 2 à 5 pts

\- Taux critique de base : \*\*2 %\*\* (×2 dégâts)

\- Semi-automatique : cooldown de \*\*0,5 s\*\* entre chaque tir

\- Chargeur : \*\*10 balles\*\* ; recharge manuelle avec \*\*R\*\*

\- Munitions achetées par lot de 10 au shop



\### Ennemis



| Type | PV | Vitesse | Dégâts | Portée détection |

|------|----|---------|--------|-----------------|

| Zombie standard | 20 | 120 px/s | 10–17 (cd 1,5 s) | 800 px (diamètre) |

| Zombie lanceur | 15 | 90 px/s | projectile 450 px/s | 800 px |

| Boss (LV1) | 300 | — | 20 (cd 1,5 s) + attaque de zone 70 | — |

| Boss (LV2) | 600 | — | idem, charge zone 1,5 s | — |

| Boss (LV3) | 850 | — | idem, charge zone 1,1 s | — |



\*\*États IA :\*\* `IDLE` → `CHASING` → `ATTACKING` → `HURT` (0,3 s) → `DEAD` (1,0 s)



\### Loot \& Gold

\- Gold de base à la mort de chaque ennemi + bonus aléatoire de \*\*5 à 10 Gold\*\*

\- La clé est lâchée à la mort du boss ; affichage `KEY: Found` dans le HUD

\- La zone de sortie téléporte au niveau suivant uniquement si le joueur possède la clé



\### Shop (BDE)

Accessible automatiquement à la fin de chaque niveau (distributeur) :



| PowerUp | Effet | Coût |

|---------|-------|------|

| LifePowerUp | +20 PV max | 450 G |

| BulletPowerUp | +5 dégâts balles | 500 G |

| KatanaPowerUp | +10 dégâts katana | 700 G |

| KatanaRangePowerUp | +15 px portée katana | 1 000 G |

| SpeedPowerUp | +20 px/s vitesse | 600 G |

| GoldPowerUp | +4 % gain gold/ennemi | 650 G |

| CritPowerUp | +3 % chance critique | 720 G |

| AttackSpeedPowerUp | −0,2 s cooldown katana | 740 G |

| ReloadPowerUp | −0,2 s cooldown pistolet | 670 G |



\---



\## Niveaux



| | LV1 | LV2 | LV3 |

|--|-----|-----|-----|

| Salles | 9 | 12 | 17 |

| Ennemis | 17 | 28 | 38 |

| Boss (PV) | 300 | 600 | 850 |



\### Ajouter un niveau



Dans `leveltemplate.h`, namespace `LevelData` :



1\. Définir la matrice :

```cpp

inline constexpr LevelMatrix<ROWS, COLS> LEVEL\_4 = {{ ... }};

```

2\. Créer le descripteur :

```cpp

inline const LevelDescriptor LEVEL\_4\_DESC = makeDescriptor(4, "Niveau 4", LEVEL\_4);

```

3\. Ajouter `\&LEVEL\_4\_DESC` dans `ALL\_LEVELS\[]` et incrémenter `LEVEL\_COUNT`



\*\*Clés de la matrice :\*\* `0` = sol · `1` = mur · `2` = zombie · `3` = sortie · `4` = spawn joueur



\---



\## Structure MVC



```

Controller : AppController → GameController → LevelController

Model      : GameModel → PlayerModel + LevelModel

&#x20;                         ├── WallModel

&#x20;                         ├── ZombieStdModel

&#x20;                         ├── ZombieLauncherModel  (+ RubbleModel)

&#x20;                         ├── BossModel

&#x20;                         ├── StandardDoorModel

&#x20;                         └── ExitDoorModel

View       : GameView (QGraphicsView)

&#x20;                         ├── PlayerView           (+ SlashView)

&#x20;                         ├── ZombieStdView

&#x20;                         ├── ZombieLauncherView   (+ RubbleView)

&#x20;                         ├── BossView

&#x20;                         ├── StandardDoorView

&#x20;                         └── WallView / ExitDoorView

```



\---



\## Itérations



\*\*Itération 1\*\* — Joueur + katana dans une salle avec 1 ennemi. Zone de test de portée (agro/katana). Transition vers le niveau suivant. 1 compétence activable/désactivable (touche U). Drop de clé.



\*\*Itération 2\*\* — Ajout du pistolet (clic gauche, munitions infinies avec recharge et cooldown). Salles avec plusieurs ennemis. Boss de fin de zone (pattern 1 uniquement).



\*\*Itération 3\*\* — 3 niveaux complets, tous les ennemis, tous les boss (P1/P2/P3), shop complet avec tous les objets achetables, munitions limitées, retrait du mode de test (touche U).



