#include "library/treeitem.h"

/*
 * Just a word about how the TreeItem objects and TreeItemModels are used in general:
 * TreeItems are used by the TreeItemModel class to display tree
 * structures in the sidebar.
 *
 * The constructor has 4 arguments:
 * 1. argument represents a name shown in the sidebar view later on
 * 2. argument represents the absolute path of this tree item
 * 3. argument is a library feature object.
 *    This is necessary because in sidebar.cpp we hanlde 'activateChid' events
 * 4. the parent TreeItem object
 *    The constructor does not add this TreeItem object to the parent's child list
 *
 * In case of no arguments, the standard constructor creates a
 * root item that is not visible in the sidebar.
 *
 * Once the TreeItem objects are inserted to models, the models take care of their
 * deletion.
 *
 * Examples on how to use TreeItem and TreeItemModels can be found in
 * - playlistfeature.cpp
 * - cratefeature.cpp
 * - *feature.cpp
 */

TreeItem::TreeItem()
    : m_pFeature(nullptr),
      m_pParent(nullptr),
	  m_divider(false),
      m_bold(false),
	  m_trackCount(-1) {
}

TreeItem::TreeItem(
        LibraryFeature* pFeature,
        const QString& label,
        const QVariant& data)
    : m_pFeature(pFeature),
      m_pParent(nullptr),
      m_label(label),
      m_data(data),
	  m_divider(false),
      m_bold(false),
	  m_trackCount(-1) {
    DEBUG_ASSERT(m_pFeature != nullptr);
}

TreeItem::~TreeItem() {
    qDeleteAll(m_children);
}

bool TreeItem::isPlaylist() const {
    return (m_children.count() == 0);
}

bool TreeItem::isFolder() const {
	return (m_children.count() != 0);
}

int TreeItem::parentRow() const {
    if (m_pParent) {
        return m_pParent->m_children.indexOf(const_cast<TreeItem*>(this));
    } else {
        return kInvalidRow;
    }
}

TreeItem* TreeItem::child(int row) const {
    DEBUG_ASSERT(row >= 0);
    DEBUG_ASSERT(row < m_children.size());
    return m_children[row];
}

void TreeItem::appendChild(TreeItem* pChild) {
    DEBUG_ASSERT(feature() != nullptr);
    DEBUG_ASSERT(pChild != nullptr);
    DEBUG_ASSERT(pChild->feature() == feature());
    DEBUG_ASSERT(!pChild->hasParent());
    m_children.append(pChild);
    pChild->m_pParent = this;
}

void TreeItem::insertChild(TreeItem* pChild, int row) {
	DEBUG_ASSERT(row >= 0);
	DEBUG_ASSERT(row < m_children.size());
	DEBUG_ASSERT(feature() != nullptr);
	DEBUG_ASSERT(pChild != nullptr);
	DEBUG_ASSERT(pChild->feature() == feature());
	DEBUG_ASSERT(!pChild->hasParent());
	m_children.insert(row, pChild);
	pChild->m_pParent = this;
}

TreeItem* TreeItem::appendChild(
        const QString& label,
        const QVariant& data) {
    auto pNewChild = std::make_unique<TreeItem>(feature(), label, data);
    TreeItem* pChild = pNewChild.get();
    appendChild(pChild); // transfer ownership
    pNewChild.release(); // release ownership (afterwards)
    return pChild;
}

void TreeItem::removeChild(int row) {
    DEBUG_ASSERT(row >= 0);
    DEBUG_ASSERT(row < m_children.size());
    delete m_children.takeAt(row);
}

void TreeItem::insertChildren(QList<TreeItem*>& children, int row, int count) {
    DEBUG_ASSERT(feature() != nullptr);
    DEBUG_ASSERT(count >= 0);
    DEBUG_ASSERT(count <= children.size());
    DEBUG_ASSERT(row >= 0);
    DEBUG_ASSERT(row <= m_children.size());
    for (int counter = row; counter < count + row; ++counter) {
        DEBUG_ASSERT(!children.empty());
        TreeItem* pChild = children.takeFirst();
        insertChild(pChild, counter);
    }
}

void TreeItem::removeChildren(int row, int count) {
    DEBUG_ASSERT(count >= 0);
    DEBUG_ASSERT(count <= m_children.size());
    DEBUG_ASSERT(row >= 0);
    DEBUG_ASSERT(row <= (m_children.size() - count));
    qDeleteAll(m_children.begin() + row, m_children.begin() + (row + count));
    m_children.erase(m_children.begin() + row, m_children.begin() + (row + count));
}
