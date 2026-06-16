/**
 * @file levelregistry.cpp
 * @brief Implémentation de LevelRegistry.
 */

#include "levelregistry.h"
#include <QDebug>

LevelRegistry& LevelRegistry::instance() {
    static LevelRegistry reg;
    return reg;
}

void LevelRegistry::init(const QString& basePath) {
    m_dynamics.clear();
    m_descriptors.clear();

    // Pré-allouer pour éviter les réallocations intermédiaires.
    // IMPORTANT : si QVector réalloue, les pointeurs raw des LevelDescriptor
    // deviendraient invalides. On réserve donc avant tout append.
    m_dynamics.reserve(LEVEL_COUNT);

    for (int id = 1; id <= LEVEL_COUNT; ++id) {
        DynamicLevelDescriptor dld = MatrixExtractor::load(id, basePath);

        if (dld.rows == 0) {
            qCritical() << "[LevelRegistry] Impossible de charger le niveau" << id
                        << "depuis" << basePath
                        << "— le jeu risque de planter lors du chargement de ce niveau.";
        }

        m_dynamics.append(std::move(dld));
    }

    // Construire les LevelDescriptor après que tous les DynamicLevelDescriptor
    // sont stables en mémoire (plus d'append possible sur m_dynamics).
    m_descriptors.reserve(m_dynamics.size());
    for (const DynamicLevelDescriptor& dld : m_dynamics)
        m_descriptors.append(dld.asDescriptor());

}

int LevelRegistry::count() const {
    return m_descriptors.size();
}

const LevelDescriptor* LevelRegistry::descriptorAt(int index) const {
    if (index < 0 || index >= m_descriptors.size()) {
        qWarning() << "[LevelRegistry] Index hors bornes :" << index;
        return nullptr;
    }
    return &m_descriptors[index];
}
