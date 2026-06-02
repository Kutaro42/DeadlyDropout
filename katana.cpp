#include "katana.h"
#include <QTimer>
#include "Vector2f.h"

int katana::getDamage() {
    return itsDamage;
}

int katana::getRange() {
    return itsRange;
}

float katana::getCooldown() {
    // Cast explicite car itsRange est un int et la fonction retourne un float
    return itsCooldown;
}

bool katana::isReady() {
    return itsReady;
}

void katana::setDamage(int d) {
    itsDamage = d;
}

void katana::setRange(int r) {
    itsRange = r;
}

void katana::setCooldown(float c) {
    itsCooldown = c;
}

void katana::launchAttack(Vector2f mouseWorldPos) {

}