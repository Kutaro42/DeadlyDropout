/**
 * @file playerview.cpp
 * @brief Implémentation des mécanismes de dessin et d'animation pour le joueur.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date Juin 2026
 */

#include "playerview.h"
#include <QPainter>

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
    // ── Slash ────────────────────────────────────────────────────────────────
    , itsSlashItem(new QGraphicsPixmapItem(this))  // fils de PlayerView
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

    // ── Initialisation du sprite de slash ────────────────────────────────────
    loadSlashSheet();
    itsSlashItem->setZValue(Cfg::Z_SLASH);
    // Le pivot de rotation doit être le centre du sprite, pas son coin sup-gauche
    // (comportement par défaut de Qt). Sans ça, setRotation() orbite autour du
    // coin sup-gauche et décale visuellement le sprite à chaque attaque.
    itsSlashItem->setTransformOriginPoint(SLASH_DISPLAY / 2.0, SLASH_DISPLAY / 2.0);
    itsSlashItem->hide();
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

// ─── Chargement de la spritesheet d'effet de slash ───────────────────────────
void PlayerView::loadSlashSheet()
{
    // Chemin de la spritesheet de slash : frames horizontales, carrées.
    // Si la ressource est absente, un fallback procédural est généré.
    const QString path = ":/images/Textures/Player/Slash/slash.png";
    QPixmap sheet(path);

    if (!sheet.isNull()) {
        // La spritesheet peut contenir plus ou moins de frames que SLASH_FRAME_COUNT.
        // On charge autant de frames que possible, jusqu'à SLASH_FRAME_COUNT.
        int available = sheet.width() / SLASH_FRAME_SRC;
        int toLoad    = std::min(available, SLASH_FRAME_COUNT);
        for (int i = 0; i < toLoad; ++i) {
            QPixmap raw = sheet.copy(i * SLASH_FRAME_SRC, 0, SLASH_FRAME_SRC, SLASH_FRAME_SRC);
            itsSlashFrames.append(raw.scaled(SLASH_DISPLAY, SLASH_DISPLAY,
                                             Qt::IgnoreAspectRatio,
                                             Qt::FastTransformation));
        }
    }

    // ── Fallback procédural si la spritesheet est absente ────────────────────
    // Génère SLASH_FRAME_COUNT frames représentant un arc de cercle croissant,
    // ce qui donne un rendu acceptable sans aucune ressource PNG externe.
    if (itsSlashFrames.isEmpty()) {
        for (int i = 0; i < SLASH_FRAME_COUNT; ++i) {
            QPixmap pm(SLASH_DISPLAY, SLASH_DISPLAY);
            pm.fill(Qt::transparent);
            QPainter p(&pm);
            p.setRenderHint(QPainter::Antialiasing, true);

            // Progression de 0 à 1 au fil des frames
            float t      = static_cast<float>(i + 1) / SLASH_FRAME_COUNT;
            int   alpha  = static_cast<int>(220 * (1.0f - t * 0.6f)); // fondu en fin d'anim
            int   thick  = static_cast<int>(3.0f + 4.0f * t);         // épaississement

            // Arc jaune-orangé simulant un éclat de lame
            QPen pen(QColor(255, 200, 60, alpha), thick, Qt::SolidLine, Qt::RoundCap);
            p.setPen(pen);

            // L'arc part de la droite (0°) et balaie vers le bas (sens croissant)
            // sur une portion proportionnelle à t, pour simuler le mouvement du sabre.
            int margin = 8;
            QRect arcRect(margin, margin, SLASH_DISPLAY - 2*margin, SLASH_DISPLAY - 2*margin);
            // drawArc : angles en 1/16° ; on trace un arc de 0° à t*180°
            int startAngle = 0;
            int spanAngle  = static_cast<int>(t * 180.0f * 16);
            p.drawArc(arcRect, startAngle, spanAngle);

            // Reflet blanc central pour l'effet tranchant
            QPen white(QColor(255, 255, 255, alpha / 2), 1);
            p.setPen(white);
            p.drawArc(arcRect.adjusted(6,6,-6,-6), startAngle, spanAngle);

            itsSlashFrames.append(pm);
        }
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

// ─── Avance une frame IDLE/WALK ──────────────────────────────────────────────
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

// ─── Mise à jour du sprite de slash ──────────────────────────────────────────

void PlayerView::positionSlashItem()
{
    if (itsSlashFrames.isEmpty()) return;

    float angle = model->getAttackAngle();   // radians (atan2 : 0=droite, π/2=bas)
    float cosA  = std::cos(angle);
    float sinA  = std::sin(angle);

    // Distance du centre du sprite de slash depuis le centre du joueur.
    // On le place à (halfDiag + SLASH_DISPLAY/2) dans la direction de l'attaque
    // pour qu'il soit visuellement devant la lame, pas superposé au personnage.
    float hw      = Cfg::ENTITY_SIZE / 2.0f;
    float hh      = Cfg::ENTITY_SIZE / 2.0f;
    float halfDiag = std::sqrt(hw * hw + hh * hh);
    float dist    = halfDiag + SLASH_DISPLAY * 0.35f;

    // Coordonnées locales (relatives à l'origine de PlayerView, coin sup-gauche de la hitbox)
    float lx = hw + cosA * dist - SLASH_DISPLAY / 2.0f;
    float ly = hh + sinA * dist - SLASH_DISPLAY / 2.0f;

    itsSlashItem->setPos(lx, ly);

    // Rotation : QGraphicsItem::setRotation() attend des degrés.
    // L'angle atan2 pointe vers la droite pour 0 rad ; on convertit en degrés.
    qreal deg = static_cast<qreal>(angle) * 180.0 / M_PI;
    itsSlashItem->setRotation(deg);
}

void PlayerView::updateSlash(float dt)
{
    // Aucune attaque n'a encore eu lieu : rien à afficher
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

// ─── Mise à jour principale chaque tick ──────────────────────────────────────
void PlayerView::updateView(float dt) {
    if (!model) return;

    setPos(model->getPosition().toQPointF());
    int d = dirIdx(model->getFacing());

    const bool katanaActive = model->isKatanaActive();

    // ── Détection du front montant d'attaque ──────────────────────────────────
    // wasKatanaActive est mis à jour ICI, une seule fois par tick.
    // Tous les resets (joueur ET slash) sont groupés dans ce bloc pour garantir
    // qu'ils sont déclenchés exactement au même tick, avant tout appel à updateSlash().
    if (katanaActive && !wasKatanaActive) {
        attackFrame          = 0;
        attackFrameTimer     = 0.0f;
        attackTotalTimer     = 0.0f;
        attackEverTriggered  = true;
        // Reset slash : même front montant, même tick
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

    // ── Mise à jour de l'effet de slash (après avoir mis à jour wasKatanaActive) ─
    updateSlash(dt);
}
