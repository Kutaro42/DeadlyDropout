/**
 * @file playerview.cpp
 * @brief Implementation of drawing and animation mechanisms for the player.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "playerview.h"
#include <QPainter>

// ─── Definition of static constant arrays ───────────────────────────────────
constexpr int PlayerView::FRAME_W[PlayerView::STATE_COUNT];
constexpr int PlayerView::FRAME_H[PlayerView::STATE_COUNT];
constexpr int PlayerView::SPRITE_SIZE[PlayerView::STATE_COUNT];

// ─── FacingDirection → table index mapping ──────────────────────────────────
int PlayerView::dirIdx(FacingDirection d) {
    switch (d) {
    case FacingDirection::DOWN:  return 0;
    case FacingDirection::UP:    return 1;
    case FacingDirection::LEFT:  return 2;
    case FacingDirection::RIGHT: return 3;
    }
    return 0;
}

// ─── Constructor ─────────────────────────────────────────────────────────────
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
    // ── Slash ────────────────────────────────────────────────────────────────
    , itsSlashItem(new QGraphicsPixmapItem(this))  // child of PlayerView
    , itsSlashFrame(0)
    , itsSlashFrameTimer(0.0f)
    , itsSlashFrameSpeed(Cfg::KATANA_ANIM_DUR / SLASH_FRAME_COUNT)
    , itsSlashDone(false)
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

    /* Generates fallbacks for all directions in one pass,
    so they are not recreated every tick if the PNGs are missing. */
    const FacingDirection dirs[DIR_COUNT] = {
        FacingDirection::DOWN, FacingDirection::UP,
        FacingDirection::LEFT, FacingDirection::RIGHT
    };
    for (int d = 0; d < DIR_COUNT; ++d)
        cachedFallback[d] = makeFallback(dirs[d]);

    /* Displays the initial frame (idle facing down) before the first tick. */
    int d = dirIdx(model->getFacing());
    if (!sprites[IDLE][d].isEmpty()) {
        setPixmap(sprites[IDLE][d][0]);
        applyCenter(SPRITE_SIZE[IDLE]);
    } else {
        setPixmap(cachedFallback[d]);
        applyCenter(Cfg::ENTITY_SIZE);
    }

    // ── Slash sprite initialization ───────────────────────────────────────────
    loadSlashSheet();
    itsSlashItem->setZValue(Cfg::Z_SLASH);
    /* The rotation pivot must be the center of the sprite.
    Without this, setRotation() orbits around the top-left corner and
    visually offsets the sprite on every attack. */
    itsSlashItem->setTransformOriginPoint(SLASH_DISPLAY / 2.0, SLASH_DISPLAY / 2.0);
    itsSlashItem->hide();
}

// ─── Spritesheet loading (horizontal frames) ─────────────────────────────────
void PlayerView::loadSheet(QVector<QPixmap>& frames, const QString& path,
                           int fw, int fh, int targetSize)
{
    QPixmap sheet(path);
    if (sheet.isNull()) {
        qWarning("[PlayerView] Introuvable : %s", qPrintable(path));
        return;
    }
    /* Slices each frame from left to right; the count is inferred from the total width
    so it adapts automatically if an artist adds frames without changing the code. */
    int count = sheet.width() / fw;
    for (int i = 0; i < count; ++i) {
        QPixmap raw = sheet.copy(i * fw, 0, fw, fh);
        frames.append(raw.scaled(targetSize, targetSize,
                                 Qt::IgnoreAspectRatio,
                                 Qt::FastTransformation));
    }
}

// ─── Slash effect spritesheet loading ────────────────────────────────────────
void PlayerView::loadSlashSheet()
{
    // Slash spritesheet path: horizontal frames, square.
    // If the resource is missing, a procedural fallback is generated.
    const QString path = ":/images/Textures/Player/Slash/slash.png";
    QPixmap sheet(path);

    if (!sheet.isNull()) {
        // The spritesheet may contain more or fewer frames than SLASH_FRAME_COUNT.
        // We load as many frames as available, up to SLASH_FRAME_COUNT.
        int available = sheet.width() / SLASH_FRAME_SRC;
        int toLoad    = std::min(available, SLASH_FRAME_COUNT);
        for (int i = 0; i < toLoad; ++i) {
            QPixmap raw = sheet.copy(i * SLASH_FRAME_SRC, 0, SLASH_FRAME_SRC, SLASH_FRAME_SRC);
            itsSlashFrames.append(raw.scaled(SLASH_DISPLAY, SLASH_DISPLAY,
                                             Qt::IgnoreAspectRatio,
                                             Qt::FastTransformation));
        }
    }

    // ── Procedural fallback if the spritesheet is missing ────────────────────
    // Generates SLASH_FRAME_COUNT frames representing a growing arc,
    // giving an acceptable result without any external PNG resource.
    if (itsSlashFrames.isEmpty()) {
        for (int i = 0; i < SLASH_FRAME_COUNT; ++i) {
            QPixmap pm(SLASH_DISPLAY, SLASH_DISPLAY);
            pm.fill(Qt::transparent);
            QPainter p(&pm);
            p.setRenderHint(QPainter::Antialiasing, true);

            // Progress from 0 to 1 across frames
            float t      = static_cast<float>(i + 1) / SLASH_FRAME_COUNT;
            int   alpha  = static_cast<int>(220 * (1.0f - t * 0.6f)); // fade at end of anim
            int   thick  = static_cast<int>(3.0f + 4.0f * t);         // thickening

            // Yellow-orange arc simulating a blade flash
            QPen pen(QColor(255, 200, 60, alpha), thick, Qt::SolidLine, Qt::RoundCap);
            p.setPen(pen);

            // The arc starts on the right (0°) and sweeps downward (increasing direction)
            // over a portion proportional to t, simulating the sword swing.
            int margin = 8;
            QRect arcRect(margin, margin, SLASH_DISPLAY - 2*margin, SLASH_DISPLAY - 2*margin);
            // drawArc: angles in 1/16°; we draw an arc from 0° to t*180°
            int startAngle = 0;
            int spanAngle  = static_cast<int>(t * 180.0f * 16);
            p.drawArc(arcRect, startAngle, spanAngle);

            // White central highlight for the blade edge effect
            QPen white(QColor(255, 255, 255, alpha / 2), 1);
            p.setPen(white);
            p.drawArc(arcRect.adjusted(6,6,-6,-6), startAngle, spanAngle);

            itsSlashFrames.append(pm);
        }
    }
}

// ─── Visual fallback ─────────────────────────────────────────────────────────
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

// ─── Centering the sprite on the hitbox ──────────────────────────────────────
void PlayerView::applyCenter(int spriteSize) {
    float offset = (Cfg::ENTITY_SIZE - spriteSize) / 2.0f;
    setOffset(offset, offset);
}

// ─── Advance one IDLE/WALK frame ─────────────────────────────────────────────
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

// ─── Slash sprite update ──────────────────────────────────────────────────────

void PlayerView::positionSlashItem()
{
    if (itsSlashFrames.isEmpty()) return;

    float angle = model->getAttackAngle();   // radians (atan2: 0=right, π/2=down)
    float cosA  = std::cos(angle);
    float sinA  = std::sin(angle);

    // Distance of the slash sprite center from the player center.
    // Placed at (halfDiag + SLASH_DISPLAY/2) in the attack direction
    // so it appears visually in front of the blade, not overlapping the character.
    float hw      = Cfg::ENTITY_SIZE / 2.0f;
    float hh      = Cfg::ENTITY_SIZE / 2.0f;
    float halfDiag = std::sqrt(hw * hw + hh * hh);
    float dist    = halfDiag + SLASH_DISPLAY * 0.35f;

    // Local coordinates (relative to the PlayerView origin, top-left corner of the hitbox)
    float lx = hw + cosA * dist - SLASH_DISPLAY / 2.0f;
    float ly = hh + sinA * dist - SLASH_DISPLAY / 2.0f;

    itsSlashItem->setPos(lx, ly);

    // Rotation: QGraphicsItem::setRotation() expects degrees.
    // The atan2 angle points right at 0 rad; we convert to degrees.
    qreal deg = static_cast<qreal>(angle) * 180.0 / M_PI;
    itsSlashItem->setRotation(deg);
}

void PlayerView::updateSlash(float dt)
{
    // No attack has occurred yet: nothing to display
    if (!attackEverTriggered) {
        itsSlashItem->hide();
        return;
    }

    const bool katanaActive = model->isKatanaActive();

    if (!itsSlashDone && !itsSlashFrames.isEmpty()) {
        positionSlashItem();

        itsSlashFrameTimer += dt;
        if (itsSlashFrameTimer >= itsSlashFrameSpeed) {
            itsSlashFrameTimer = 0.0f;
            if (itsSlashFrame < SLASH_FRAME_COUNT - 1) {
                ++itsSlashFrame;
            } else {
                itsSlashDone = true;
                itsSlashItem->hide();
                return;
            }
        }
        int safeFrame = qMin(itsSlashFrame, itsSlashFrames.size() - 1);
        itsSlashItem->setPixmap(itsSlashFrames[safeFrame]);
        itsSlashItem->show();
    } else {
        itsSlashItem->hide();
    }
}

// ─── Main update every tick ───────────────────────────────────────────────────
void PlayerView::updateView(float dt) {
    if (!model) return;

    setPos(model->getPosition().toQPointF());
    int d = dirIdx(model->getFacing());

    const bool katanaActive = model->isKatanaActive();

    // ── Attack rising-edge detection ──────────────────────────────────────────
    // wasKatanaActive is updated HERE, once per tick.
    // All resets (player AND slash) are grouped in this block to guarantee
    // they are triggered at exactly the same tick, before any call to updateSlash().
    if (katanaActive && !wasKatanaActive) {
        attackFrame          = 0;
        attackFrameTimer     = 0.0f;
        attackTotalTimer     = 0.0f;
        attackEverTriggered  = true;
        // Slash reset: same rising edge, same tick
        itsSlashFrame        = 0;
        itsSlashFrameTimer   = 0.0f;
        itsSlashDone        = false;
    }
    wasKatanaActive = katanaActive;

    const float attackFullDuration = ATTACK_FRAME_COUNT * attackAnimSpeed;
    const bool  playingAttack      = katanaActive
                                  || (attackEverTriggered && attackTotalTimer < attackFullDuration);

    if (playingAttack) {
        const QVector<QPixmap>& atk = sprites[ATTACK][d];
        if (!atk.isEmpty()) {
            attackTotalTimer += dt;
            attackFrameTimer += dt;
            if (attackFrameTimer >= attackAnimSpeed) {
                attackFrameTimer = 0.0f;
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
        const QVector<QPixmap>& idle = sprites[IDLE][d];
        if (!idle.isEmpty()) {
            advanceFrames(dt, idle, animSpeed);
            applyCenter(SPRITE_SIZE[IDLE]);
        } else {
            setPixmap(cachedFallback[d]);
            applyCenter(Cfg::ENTITY_SIZE);
        }
    }

    // ── Slash effect update (after updating wasKatanaActive) ──────────────────
    updateSlash(dt);
}
