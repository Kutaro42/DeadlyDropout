#include "marketcontroller.h"

MarketController::MarketController(QWidget* parentWidget, QObject* parent)
    : QObject(parent)
    , itsView(new MarketView(parentWidget))
{
    connect(itsView, &MarketView::sigBuy, this, &MarketController::sltBuy);
}

MarketController::~MarketController() { delete itsView; }

void MarketController::open(PlayerModel* player) {
    itsPlayer = player;
    itsView->refresh(player);
    itsView->exec(); // bloquant : retourne quand le joueur clique "Continuer"
}

void MarketController::sltBuy(int itemId) {
    if (!itsPlayer) return;
    if (itsPlayer->getGold() < MarketView::ITEMS[itemId].cost) return;
    itsPlayer->addGold(-MarketView::ITEMS[itemId].cost);

    switch (itemId) {
    case 0: itsPlayer->setMaxHp(itsPlayer->getMaxHp() + 20); break;
    case 1: itsPlayer->getGun().setBaseDamage(itsPlayer->getGun().getBaseDamage() + 5); break;
    case 2: itsPlayer->setKatanaDamage(itsPlayer->getKatana().getDamage() + 10); break;
    case 3: itsPlayer->setKatanaRange(itsPlayer->getKatana().getRange() + 15); break;
    case 4: itsPlayer->setSpeed(itsPlayer->getSpeed() + 20.0f); break;
    case 5: itsPlayer->getGun().setCritChance(itsPlayer->getGun().getCritChance() + 0.03f); break;
    case 6: itsPlayer->setKatanaCooldown(std::max(0.1f, itsPlayer->getKatana().getCooldownRatio() - 0.2f)); break;
    case 7: itsPlayer->getGun().setShootCooldown(std::max(0.1f, itsPlayer->getGun().getShootCooldown() - 0.2f)); break;
    case 8: itsPlayer->getGun().addReserve(10); break;
    }
    itsView->refresh(itsPlayer);
}
