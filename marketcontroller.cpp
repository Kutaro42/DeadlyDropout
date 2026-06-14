#include "marketcontroller.h"

MarketController::MarketController(QObject* parent) : QObject(parent) {
    itsView = new MarketView();
    connect(itsView, &MarketView::sigBuy,      this, &MarketController::sltBuy);
    connect(itsView, &MarketView::sigContinue, this, &MarketController::sigClosed);
}

MarketController::~MarketController() { delete itsView; }

void MarketController::open(PlayerModel* player) {
    itsPlayer = player;
    itsView->refresh(player);
    itsView->show();
}

void MarketController::sltBuy(int itemId) {
    if (!itsPlayer) return;

    // Coûts (même ordre que MarketView::ITEMS)
    const int costs[] = {450, 500, 700, 1000, 600, 720, 740, 670, 100};
    if (itsPlayer->getGold() < costs[itemId]) return;

    // Simuler soustraction de gold : addGold avec valeur négative
    itsPlayer->addGold(-costs[itemId]);

    switch (itemId) {
    case 0: itsPlayer->setMaxHp(itsPlayer->getMaxHp() + 20); break;
    case 1: itsPlayer->setKatanaDamage(itsPlayer->getKatana().getDamage()); // géré ci-dessous
        itsPlayer->getGun().setBaseDamage(itsPlayer->getGun().getBaseDamage() + 5); break;
    case 2: itsPlayer->setKatanaDamage(itsPlayer->getKatana().getDamage() + 10); break;
    case 3: itsPlayer->setKatanaRange(itsPlayer->getKatana().getRange() + 15); break;
    case 4: itsPlayer->setSpeed(itsPlayer->getSpeed() + 20.0f); break;
    case 5: itsPlayer->getGun().setCritChance(itsPlayer->getGun().getCritChance() + 0.03f); break;
    case 6: itsPlayer->setKatanaCooldown(std::max(0.1f, itsPlayer->getKatana().getCooldownRatio() - 0.2f)); break;//getCooldown()
    case 7: itsPlayer->getGun().setShootCooldown(std::max(0.1f, itsPlayer->getGun().getShootCooldown() - 0.2f)); break;
    case 8: itsPlayer->getGun().addReserve(10); break;
    }

    itsView->refresh(itsPlayer);
}