#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <QAbstractItemView>

class PropertyNode;

class Visitor
{
public:
    virtual void visit(PropertyNode* node, const QModelIndex& parent) {}
};

class PersistentEditorVisitor : public Visitor
{
public:
    PersistentEditorVisitor(QAbstractItemView* view);
    virtual void visit(PropertyNode* node, const QModelIndex& parent);

private:
    QAbstractItemView* m_view;
};

#endif // VISITOR_HPP

