/**
 * @file marketview.h
 * @brief Declaration of MarketView, the BDE Market upgrade shop dialog.
 * @author Alexandre AUBIN, Iaviantsoa RASOLOFOHARIVELO, Babacar DIOP, Léo BOUCARD
 * @date June 2026
 */

#ifndef MARKETVIEW_H
#define MARKETVIEW_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include "playermodel.h"

/**
 * @class MarketView
 * @brief Modal dialog presenting the upgrade shop between levels (GDD Step 5).
 *
 * The view is kept intentionally dumb: it displays item names, costs and
 * descriptions, disables buttons the player cannot afford, and emits sigBuy
 * when a button is clicked.  All balance values and stat changes live in
 * MarketController, not here, so the GDD balancing can be adjusted without
 * touching the UI code.
 *
 * ITEMS is a static table so it can be shared between MarketView (display)
 * and MarketController (cost lookup) without duplicating data.
 */
class MarketView : public QDialog {
    Q_OBJECT
public:
    /**
     * @brief Constructs the market dialog and lays out all item buttons.
     * @param parent Qt widget parent (nullptr = top-level window).
     */
    explicit MarketView(QWidget* parent = nullptr);

    /**
     * @brief Refreshes the gold label and enables/disables buttons based on current gold.
     *
     * Called by MarketController after each purchase so the UI stays in sync
     * with the player model without the view holding a permanent player pointer.
     *
     * @param player The current player (read-only access to gold balance).
     */
    void refresh(PlayerModel* player);

signals:
    /**
     * @brief Emitted when the player clicks a buy button.
     *
     * MarketController listens to this signal to validate gold and apply the upgrade,
     * keeping all business logic out of the view layer.
     *
     * @param itemId Index into ITEMS identifying which upgrade was requested.
     */
    void sigBuy(int itemId);

private:
    QLabel* goldLabel; ///< Displays the player's current gold balance.

    /// Internal pairing of a button widget with its item index.
    struct ItemBtn { QPushButton* btn; int id; };
    QVector<ItemBtn> itemBtns; ///< All purchasable item buttons, iterated in refresh().

public:
    /**
     * @struct ItemDef
     * @brief Descriptor for a single market item (id, display name, cost, description).
     *
     * Public and static so MarketController can look up costs without reimplementing
     * the item list, ensuring a single source of truth for prices.
     */
    struct ItemDef { int id; QString name; int cost; QString desc; };

    /// Ordered list of all purchasable upgrades.  Order defines button layout (row-major, 3 columns).
    static const QVector<ItemDef> ITEMS;
};

#endif