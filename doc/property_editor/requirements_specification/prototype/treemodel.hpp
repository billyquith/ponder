#ifndef TREEMODEL_HPP
#define TREEMODEL_HPP

#include <propertynode.hpp>
#include <visitor.hpp>

#include <camp/userobject.hpp>

#include <QAbstractItemModel>
#include <QHash>

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    TreeModel(QObject* parent = 0);

    virtual Qt::ItemFlags flags(const QModelIndex& index) const;

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;

    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex& index) const;

    PropertyNode* property(const QModelIndex& index) const;

    void setSubject(const camp::UserObject& subject = camp::UserObject::nothing);

    void accept(Visitor& visitor) const;

protected:
    void rebuild();

private:
    camp::UserObject m_subject;
    QHash<int, PropertyNode*> m_roots;
};

#endif // TREEMODEL_HPP

