#ifndef PROPERTYNODE_HPP
#define PROPERTYNODE_HPP

#include <visitor.hpp>

#include <camp/property.hpp>
#include <camp/userobject.hpp>

#include <QHash>
#include <QVariant>

class PropertyNode
{
public:

    PropertyNode(const camp::Property& property, int index, PropertyNode* parent = 0);
    PropertyNode(const camp::Property& property, int index, const camp::UserObject& subject);
    virtual ~PropertyNode();
    
    PropertyNode* parent() const;
    int childCount() const;
    PropertyNode* child(int index);
    int index() const;

    Qt::ItemFlags flags(int column) const;
    QVariant data(int column, int role = Qt::DisplayRole) const;
    bool setData(int column, const QVariant& value, int role = Qt::EditRole);

    const camp::UserObject& subject() const;
    camp::UserObject childSubject() const;

    bool hasPersistentEditor(int column) const;
    QWidget* persistentEditor(QWidget* parent) const;

    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index);

    void accept(Visitor& visitor, const QModelIndex& index) const;

private:
    void buildTree();

private:
    PropertyNode* m_parent;
    QHash<int, PropertyNode*> m_children;

    const camp::Property& m_property;
    int m_index;

    mutable camp::UserObject m_subject;
};

#endif // PROPERTYNODE_HPP

