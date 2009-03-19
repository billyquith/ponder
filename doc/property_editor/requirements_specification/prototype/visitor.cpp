#include <propertynode.hpp>
#include <visitor.hpp>

PersistentEditorVisitor::PersistentEditorVisitor(QAbstractItemView* view)
    : m_view(view)
{

}

void PersistentEditorVisitor::visit(PropertyNode* node, const QModelIndex& parent)
{
    if (!m_view)
        return;

    QAbstractItemModel* model = m_view->model();
    if (!model)
        return;

    if (node->hasPersistentEditor(0))
    {
        QModelIndex index = model->index(node->index(), 0, parent);
        m_view->openPersistentEditor(index);
    }

    if (node->hasPersistentEditor(1))
    {
        QModelIndex index = model->index(node->index(), 1, parent);
        m_view->openPersistentEditor(index);
    }

    node->accept(*this, model->index(node->index(), 0, parent));
}

