/**
 * @file playerview.cpp
 * @brief Implémentation des mécanismes de dessin et d'animation pour le joueur.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "playerview.h"
#include <QPainter>
#include <QFont>

// ─── Définition des tableaux de constantes statiques ────────────────────────
constexpr int PlayerView::FRAME_W[PlayerView::STATE_COUNT];
constexpr int PlayerView::FRAME_H[PlayerView::STATE_COUNT];
constexpr int PlayerView::SPRITE_SIZE[PlayerView::STATE_COUNT];

// ─── Correspondance FacingDirection → index tableau ─────────────────────────
int PlayerView::dirIdx(FacingDirection d) {
    switch (d) {
    case FacingDirection::DOWN:  return 0;
    case FacingDirection::UP:    return 1;
    case FacingDirection::LEFT:  return 2;
    case FacingDirection::RIGHT: return 3;
    }
    return 0;
}

// ─── Constructeur ────────────────────────────────────────────────────────────
PlayerView::PlayerView(PlayerModel* m, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
    , model(m)
    , currentFrame(0)
    , frameTimer(0.0f)
    , animSpeed(0.15f)
    , attackFrame(0)
    , attackFrameTimer(0.0f)
    , attackTotalTimer(0.0f)
    , attackAnimSpeed(Cfg::KATANA_ANIM_DUR / ATTACK_FRAME_COUNT)
    , wasKatanaActive(false)
    , attackEverTriggered(false)
{
    const char* dirNames[DIR_COUNT] = { "Down", "Up", "Left", "Right" };
    for (int d = 0; d < DIR_COUNT; ++d) {
        loadSheet(sprites[IDLE][d],
                  QString(":/images/Textures/Player/Idle/%1.png").arg(dirNames[d]),
                  FRAME_W[IDLE], FRAME_H[IDLE], SPRITE_SIZE[IDLE]);
        loadSheet(sprites[WALK][d],
                  QString(":/images/Textures/Player/Move/%1.png").arg(dirNames[d]),
                  FRAME_W[WALK], FRAME_H[WALK], SPRITE_SIZE[WALK]);
        loadSheet(sprites[ATTACK][d],
                  QString(":/images/Textures/Player/Attack/%1.png").arg(dirNames[d]),
                  FRAME_W[ATTACK], FRAME_H[ATTACK], SPRITE_SIZE[ATTACK]);
    }

    setZValue(Cfg::Z_PLAYER);
    setPos(model->getPosition().toQPointF());

    // Précalcul des fallbacks pour toutes les directions (évite QPainter à chaque tick)
    const FacingDirection dirs[DIR_COUNT] = {
        FacingDirection::DOWN, FacingDirection::UP,
        FacingDirection::LEFT, FacingDirection::RIGHT
    };
    for (int d = 0; d < DIR_COUNT; ++d)
        cachedFallback[d] = makeFallback(dirs[d]);

    int d = dirIdx(model->getFacing());
    if (!sprites[IDLE][d].isEmpty()) {
        setPixmap(sprites[IDLE][d][0]);
        applyCenter(SPRITE_SIZE[IDLE]);
    } else {
        setPixmap(cachedFallback[d]);
        applyCenter(Cfg::ENTITY_SIZE);
    }
}

// ─── Chargement d'un spritesheet (frames horizontales) ───────────────────────
void PlayerView::loadSheet(QVector<QPixmap>& frames, const QString& path,
                           int fw, int fh, int targetSize)
{
    QPixmap sheet(path);
    if (sheet.isNull()) {
        qWarning("[PlayerView] Introuvable : %s", qPrintable(path));
        return;
    }
    int count = sheet.width() / fw;
    for (int i = 0; i < count; ++i) {
        QPixmap raw = sheet.copy(i * fw, 0, fw, fh);
        frames.append(raw.scaled(targetSize, targetSize,
                                 Qt::IgnoreAspectRatio,
                                 Qt::FastTransformation));
    }
}

// ─── Fallback visuel ─────────────────────────────────────────────────────────
QPixmap PlayerView::makeFallback(FacingDirection dir) {
    QPixmap pm(Cfg::ENTITY_SIZE, Cfg::ENTITY_SIZE);
    pm.fill(QColor(40, 80, 180));
    QPainter p(&pm);
    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 9, QFont::Bold));
    const char* arrow = "▼";
    switch (dir) {
    case FacingDirection::UP:    arrow = "▲"; break;
    case FacingDirection::LEFT:  arrow = "◀"; break;
    case FacingDirection::RIGHT: arrow = "▶"; break;
    default: break;
    }
    p.drawText(pm.rect(), Qt::AlignCenter, QString("P\n") + arrow);
    return pm;
}

// ─── Centrage du sprite sur la hitbox ────────────────────────────────────────
void PlayerView::applyCenter(int spriteSize) {
    float offset = (Cfg::ENTITY_SIZE - spriteSize) / 2.0f;
    setOffset(offset, offset);
}

// ─── Avance une frame IDLE/WALK et l'affiche ─────────────────────────────────
void PlayerView::advanceFrames(float dt, const QVector<QPixmap>& frames, float speed)
{
    if (frames.isEmpty()) return;

    if (currentFrame >= frames.size()) {
        currentFrame = 0;
        frameTimer   = 0.0f;
    }

    frameTimer += dt;
    if (frameTimer >= speed) {
        frameTimer   = 0.0f;
        currentFrame = (currentFrame + 1) % frames.size();
    }
    setPixmap(frames[currentFrame]);
}

// ─── Mise à jour chaque tick ──────────────────────────────────────────────────
// Priorité : ATTACK > WALK > IDLE
void PlayerView::updateView(float dt) {
    if (!model) return;

    setPos(model->getPosition().toQPointF());
    int d = dirIdx(model->getFacing());

    const bool katanaActive = model->isKatanaActive();

    // ── Détection du front montant : entrée dans l'état ATTACK ────────────────
    // Reset des timers d'attaque UNE SEULE FOIS au déclenchement.
    if (katanaActive && !wasKatanaActive) {
        attackFrame          = 0;
        attackFrameTimer     = 0.0f;
        attackTotalTimer     = 0.0f;
        attackEverTriggered  = true;
    }
    wasKatanaActive = katanaActive;

    const float attackFullDuration = ATTACK_FRAME_COUNT * attackAnimSpeed;

    // L'animation d'attaque est active si :
    //   - le modèle signale une attaque en cours (isKatanaActive), OU
    //   - la vue n'a pas encore fini de jouer toutes ses frames (attackTotalTimer)
    // Le second critère n'est évalué qu'après une première attaque (attackEverTriggered),
    // ce qui évite de déclencher l'animation au démarrage (attackTotalTimer=0 < duration).
    const bool playingAttack = katanaActive
                            || (attackEverTriggered && attackTotalTimer < attackFullDuration);

    if (playingAttack) {
        // ── Animation d'attaque ───────────────────────────────────
        const QVector<QPixmap>& atk = sprites[ATTACK][d];
        if (!atk.isEmpty()) {
            attackTotalTimer += dt;
            attackFrameTimer += dt;
            if (attackFrameTimer >= attackAnimSpeed) {
                attackFrameTimer = 0.0f;
                // Bloque sur la dernière frame plutôt que de wrapper,
                // pour ne pas revenir à la frame 0 en fin d'animation.
                if (attackFrame < ATTACK_FRAME_COUNT - 1)
                    ++attackFrame;
            }
            int safeFrame = qMin(attackFrame, atk.size() - 1);
            setPixmap(atk[safeFrame]);
            applyCenter(SPRITE_SIZE[ATTACK]);
        } else {
            setPixmap(cachedFallback[d]);
            applyCenter(Cfg::ENTITY_SIZE);
        }

    } else if (model->isMoving()) {
        // ── Animation marche ──────────────────────────────────────
        if (currentFrame >= sprites[WALK][d].size()) {
            currentFrame = 0;
            frameTimer   = 0.0f;
        }
        const QVector<QPixmap>& walk = sprites[WALK][d];
        if (!walk.isEmpty()) {
            advanceFrames(dt, walk, animSpeed);
            applyCenter(SPRITE_SIZE[WALK]);
        } else {
            setPixmap(cachedFallback[d]);
            applyCenter(Cfg::ENTITY_SIZE);
        }

    } else {
        // ── Animation idle ────────────────────────────────────────
        const QVector<QPixmap>& idle = sprites[IDLE][d];
        if (!idle.isEmpty()) {
            advanceFrames(dt, idle, animSpeed);
            applyCenter(SPRITE_SIZE[IDLE]);
        } else {
            setPixmap(cachedFallback[d]);
            applyCenter(Cfg::ENTITY_SIZE);
        }
    }
}
