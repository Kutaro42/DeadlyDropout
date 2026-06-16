/**
 * @file marketcontroller.h
 * @brief Declaration of MarketController, managing the between-level upgrade shop.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef MARKETCONTROLLER_H
#define MARKETCONTROLLER_H

#include <QObject>
#include <algorithm>
#include "marketview.h"
#include "playermodel.h"
#include "config.h"

/**
 * @class MarketController
 * @brief Controller for the BDE Market upgrade shop (GDD Step 5 — Market rules).
 *
 * The market is a modal interaction: the game loop is stopped, the player buys
 * upgrades, then the loop resumes.  Using QDialog::exec() inside open() achieves
 * this without a separate application state, keeping GameController's state machine
 * simpler.
 *
 * All upgrade bonus values are defined as named constants below so that balancing
 * changes require edits in one place rather than scattered magic numbers.
 */
class MarketController : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Constructs MarketController and creates the owned MarketView.
     * @param parentWidget Qt widget parent for the dialog (nullptr = top-level).
     * @param parent       Qt object parent.
     */
    explicit MarketController(QWidget* parentWidget, QObject* parent = nullptr);
    ~MarketController();

    /**
     * @brief Opens the market modal and blocks until the player clicks "Continue".
     *
     * GameController calls this between levels after stopping the game timer.
     * The blocking nature means the caller does not need an async callback —
     * execution simply resumes after the player is done shopping.
     *
     * @param player The player whose stats will be upgraded and whose gold will be deducted.
     */
    void open(PlayerModel* player);

    /**
     * @brief Returns the managed MarketView (for embedding or testing).
     * @return Pointer to MarketView.
     */
    MarketView* getView() const { return itsView; }

private slots:
    /**
     * @brief Applies the selected upgrade to the player when a buy button is clicked.
     *
     * Validates the player's gold balance before applying any stat change.
     * Each case corresponds to one GDD power-up (LifePowerUp, BulletPowerUp, etc.).
     *
     * @param itemId Index into MarketView::ITEMS identifying the purchased upgrade.
     */
    void sltBuy(int itemId);

private:
    MarketView*  itsView;             ///< Owned dialog view.
    PlayerModel* itsPlayer = nullptr; ///< Non-owning pointer to the active player.
    

signals:
    /// @brief Emitted when the market dialog is closed (player clicked "Continue").
    void sigClosed();
};

#endif