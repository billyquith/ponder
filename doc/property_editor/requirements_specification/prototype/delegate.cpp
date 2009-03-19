
#include <delegate.hpp>
#include <treemodel.hpp>

Delegate::Delegate(QObject* parent)
    : QStyledItemDelegate(parent)
{

}

void Delegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QStyledItemDelegate::paint(painter, option, index);
}

QWidget* Delegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const TreeModel* model = static_cast<const TreeModel*>(index.model());
    PropertyNode* node = model->property(index);
    if (node && node->hasPersistentEditor(index.column()))
    {
        return node->persistentEditor(parent);
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void Delegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    const TreeModel* model = static_cast<const TreeModel*>(index.model());
    PropertyNode* node = model->property(index);
    if (node && node->hasPersistentEditor(index.column()))
        node->setEditorData(editor, index);
    else
        QStyledItemDelegate::setEditorData(editor, index);
}

void Delegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}

void Delegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    const TreeModel* treemodel = static_cast<const TreeModel*>(index.model());
    PropertyNode* node = treemodel->property(index);
    if (node && node->hasPersistentEditor(index.column()))
        node->setModelData(editor, model, index);
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}

