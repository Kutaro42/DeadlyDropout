#include "marketview.h"
#include <QVBoxLayout>
#include <QGridLayout>

const QVector<MarketView::ItemDef> MarketView::ITEMS = {
    {0, "LifePowerUp",        450,  "+20 HP max"},
    {1, "BulletPowerUp",      500,  "+5 dégâts balle"},
    {2, "KatanaPowerUp",      700,  "+10 dégâts katana"},
    {3, "KatanaRangePowerUp", 1000, "+15px portée katana"},
    {4, "SpeedPowerUp",       600,  "+20 px/s vitesse"},
    {5, "CritPowerUp",        720,  "+3% chance critique"},
    {6, "AttackSpeedPowerUp", 740,  "-0.2s cooldown katana"},
    {7, "ReloadPowerUp",      670,  "-0.2s cooldown gun"},
    {8, "Ammo x10",           100,  "+10 munitions réserve"},
    };

MarketView::MarketView(QWidget* parent) : QDialog(parent) {
    setWindowTitle("BDE Market");
    setFixedSize(960, 650);
    setModal(true);
    setStyleSheet("background-color:#0d0d0d;");

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(40, 20, 40, 20);
    layout->setSpacing(10);

    auto* title = new QLabel("=== BDE MARKET ===", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color:#ffcc00;font-size:24px;font-weight:bold;");
    layout->addWidget(title);

    goldLabel = new QLabel("GOLD : 0", this);
    goldLabel->setAlignment(Qt::AlignCenter);
    goldLabel->setStyleSheet("color:#ffd700;font-size:16px;");
    layout->addWidget(goldLabel);

    auto* grid = new QGridLayout();
    grid->setSpacing(10);
    for (int i = 0; i < ITEMS.size(); ++i) {
        const ItemDef& item = ITEMS[i];
        auto* btn = new QPushButton(
            QString("[%1G] %2\n%3").arg(item.cost).arg(item.name).arg(item.desc), this);
        btn->setFixedHeight(60);
        btn->setStyleSheet(
            "QPushButton{background:#1a1a2e;color:#00ffcc;font-size:12px;"
            "border:1px solid #00ffcc;border-radius:6px;}"
            "QPushButton:hover{background:#00ffcc;color:#000;}"
            "QPushButton:disabled{background:#111;color:#444;border-color:#333;}");
        int id = item.id;
        connect(btn, &QPushButton::clicked, this, [this, id]{ emit sigBuy(id); });
        grid->addWidget(btn, i / 3, i % 3);
        itemBtns.append({btn, id});
    }
    layout->addLayout(grid);

    auto* continueBtn = new QPushButton("► Continuer", this);
    continueBtn->setFixedHeight(50);
    continueBtn->setStyleSheet(
        "QPushButton{background:#1a1a1a;color:#ff3366;font-size:16px;font-weight:bold;"
        "border:2px solid #ff3366;border-radius:8px;}"
        "QPushButton:hover{background:#ff3366;color:#000;}");
    connect(continueBtn, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(continueBtn);
}

void MarketView::refresh(PlayerModel* player) {
    if (!player) return;
    goldLabel->setText(QString("GOLD : %1").arg(player->getGold()));
    for (auto& ib : itemBtns)
        ib.btn->setEnabled(player->getGold() >= ITEMS[ib.id].cost);
}
