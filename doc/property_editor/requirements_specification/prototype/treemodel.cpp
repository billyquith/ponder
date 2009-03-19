#include <treemodel.hpp>

#include <camp/class.hpp>
#include <camp/property.hpp>

TreeModel::TreeModel(QObject* parent)
    : QAbstractItemModel(parent)
{

}

Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return static_cast<PropertyNode*>(index.internalPointer())->flags(index.column());
}

QVariant TreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    return static_cast<PropertyNode*>(index.internalPointer())->data(index.column(), role);
}

bool TreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
        return false;

    if (static_cast<PropertyNode*>(index.internalPointer())->setData(index.column(), value, role))
    {
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (orientation)
    {
        case Qt::Horizontal:
            switch (section)
            {
                case 0:
                    if (role == Qt::DisplayRole)
                        return tr("Property");
                case 1:
                    if (role == Qt::DisplayRole)
                        return tr("Value");
            }

        case Qt::Vertical:
            break;
    }

    return QVariant();
}

int TreeModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid()
            ? static_cast<PropertyNode*>(parent.internalPointer())->childCount()
            : m_roots.size();
}

int TreeModel::columnCount(const QModelIndex& parent) const
{
    return 2;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (row >= rowCount(parent) || column >= columnCount(parent))
        return QModelIndex();

    if (!parent.isValid())
    {
        PropertyNode* node = m_roots.value(row);
        return createIndex(row, column, static_cast<void*>(node));
    }
    else
    {
        PropertyNode* parentNode = static_cast<PropertyNode*>(parent.internalPointer());
        return createIndex(row, column, parentNode->child(row));
    }

    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    PropertyNode* node = static_cast<PropertyNode*>(index.internalPointer());
    PropertyNode* parentNode = node->parent();

    if (!parentNode)
        return QModelIndex();

    return createIndex(parentNode->index(), 0, parentNode);
}

PropertyNode* TreeModel::property(const QModelIndex& index) const
{
    if (!index.isValid())
        return 0;

    if (!index.parent().isValid())
        return m_roots.value(index.row());

    return property(index.parent())->child(index.row());
}

void TreeModel::setSubject(const camp::UserObject& subject)
{
    m_subject = subject;

    rebuild();
}

void TreeModel::accept(Visitor& visitor) const
{
    foreach (PropertyNode* node, m_roots)
        visitor.visit(node, QModelIndex());
}

void TreeModel::rebuild()
{
    foreach(PropertyNode* node, m_roots)
        delete node;
    m_roots.clear();

    if (m_subject != camp::UserObject::nothing)
    {
        const camp::Class& metaclass = m_subject.getClass();

        for (int i = 0; i < metaclass.propertyCount(); ++i)
            m_roots.insert(i, new PropertyNode(metaclass.property(i), i, m_subject));
    }

    reset();
}

