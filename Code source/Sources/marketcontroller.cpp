/**
 * @file marketcontroller.cpp
 * @brief Implementation of MarketController — applies purchased upgrades to the player.
 */

#include "marketcontroller.h"

MarketController::MarketController(QWidget* parentWidget, QObject* parent)
    : QObject(parent)
    , itsView(new MarketView(parentWidget))
{
    // Wire the view's buy signal to the controller slot so the view stays
    // free of business logic (gold validation, stat modification).
    connect(itsView, &MarketView::sigBuy, this, &MarketController::sltBuy);
}

MarketController::~MarketController() { delete itsView; }

void MarketController::open(PlayerModel* player) {
    itsPlayer = player;
    itsView->refresh(player); // Sync gold display and button enabled states before showing.
    itsView->exec(); // Blocking: returns when the player clicks "Continue".
}

void MarketController::sltBuy(int itemId) {
    if (!itsPlayer) return;
    // Reject purchase if insufficient gold (GDD: "if price > gold held, purchase refused").
    if (itsPlayer->getGold() < MarketView::ITEMS[itemId].cost) return;

    itsPlayer->addGold(-MarketView::ITEMS[itemId].cost);

    // Apply the upgrade that corresponds to the purchased item.
    // Each case maps 1-to-1 to a GDD power-up definition (Step 5).
    switch (itemId) {
    case 0: itsPlayer->setMaxHp(itsPlayer->getMaxHp() + BONUS_MAX_HP); itsPlayer->setHp(itsPlayer->getHp() + BONUS_MAX_HP); break;
    case 1: itsPlayer->getGun().setBaseDamage(itsPlayer->getGun().getBaseDamage() + BONUS_GUN_MAX_DMG); break;
    case 2: itsPlayer->setKatanaDamage(itsPlayer->getKatana().getDamage() + BONUS_KATANA_DMG); break;
    case 3: itsPlayer->setKatanaRange(itsPlayer->getKatana().getRange() + BONUS_KATANA_RANGE); break;
    case 4: itsPlayer->setSpeed(itsPlayer->getSpeed() + BONUS_SPEED); break;
    case 5: itsPlayer->getGun().setCritChance(itsPlayer->getGun().getCritChance() + BONUS_CRIT_CHANCE); break;
    case 6: itsPlayer->setKatanaCooldown(std::max(BONUS_KATANA_COOLDOWN_MIN,
                itsPlayer->getKatana().getCooldownRatio() - BONUS_KATANA_COOLDOWN_MAX)); break;
    case 7: itsPlayer->getGun().setShootCooldown(std::max(BONUS_SHOOT_COOLDOWN_MIN,
                itsPlayer->getGun().getShootCooldown() - BONUS_SHOOT_COOLDOWN_MAX)); break;
    case 8: itsPlayer->getGun().addReserve(BONUS_RESERVE); break;
    }

    // Refresh the dialog so gold totals and button states update immediately.
    itsView->refresh(itsPlayer);
}