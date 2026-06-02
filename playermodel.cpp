#include "PlayerModel.h"
PlayerModel::PlayerModel(Vector2f startPos):
    Entity(startPos, 32.0f, 32.0f),
    lifePoints(PLAYER_MAX_HP),
    PlayerDirectionSprite(STARTPOS),
    gold(0),
    maxLP(PLAYER_MAX_HP),
    walkSpeed(PLAYER_SPEED),
    hasKey(false),
    direction(0.0f, 0.0f)
{
    // Si vous souhaitez forcer la position de config.h :
    this->position = SPAWN_COORDONATES;
    // Configuration des attributs spécifiques au joueur
    itsKatana.setDamage(KATANA_DAMAGE);
    itsKatana.setRange(KATANA_SLASH_W);
    itsKatana.setCooldown(KATANA_COOLDOWN);
}


PlayerModel::~PlayerModel() {

}

void PlayerModel::update(float dt) {
}

void PlayerModel::move(Vector2f dir, float dt) {
    float len = dir.length();
    if (len > 0) {
        // Normalisation du vecteur de direction
        float normX = dir.x / len;
        float normY = dir.y / len;

        // Calcul du déplacement théorique en float
        float moveX = normX * walkSpeed * dt;
        float moveY = normY * walkSpeed * dt;

        // Application du mouvement sur le QPoint via x(), y(), setX() et setY()
        this->position.setX(this->position.x() + static_cast<int>(std::round(moveX)));
        this->position.setY(this->position.y() + static_cast<int>(std::round(moveY)));

        // Mise à jour de l'enum de direction
        if (std::abs(dir.x) > std::abs(dir.y)) {
            PlayerDirectionSprite = (dir.x > 0) ? RIGHT : LEFT;
        } else {
            PlayerDirectionSprite = (dir.y > 0) ? DOWN : UP;
        }

        this->direction = dir;
    }
}

void PlayerModel::takeDamage(int amount) {
    lifePoints -= amount;
    if (lifePoints < 0) {
        lifePoints = 0;
    }
}

void PlayerModel::collectKey() {
    hasKey = true;
}

void PlayerModel::addGold(int amount) {
    gold += amount;
}

void PlayerModel::removeGold(int amount) {
    gold -= amount;
}

bool PlayerModel::isDead() {
    return lifePoints <= 0;
}

int PlayerModel::getHp() {
    return lifePoints;
}

int PlayerModel::getMaxHp() {
    return maxLP;
}

float PlayerModel::getSpeed() {
    return walkSpeed;
}

int PlayerModel::getGold() const {
    return gold;
}

bool PlayerModel::getHasKey() {
    return hasKey;
}

Vector2f PlayerModel::getDirection() {
    return direction;
}

void PlayerModel::setSpeed(int s) {
    walkSpeed = static_cast<float>(s);
}

void PlayerModel::setMaxHp(int mhp) {
    maxLP = mhp;
    if (lifePoints > maxLP) {
        lifePoints = maxLP;
    }
}

void PlayerModel::katanaAttack() {
    if (itsKatana.isReady()) {
        // Utilise la direction actuelle du joueur pour lancer l'attaque
        itsKatana.launchAttack(this->direction);
    }
}