#ifndef DELEGATE_HPP
#define DELEGATE_HPP

#include <QStyledItemDelegate>

class Delegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
        Delegate(QObject* parent = 0);

        virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

        virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

        virtual void setEditorData(QWidget* editor, const QModelIndex& index) const;

        virtual void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;

        virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
};

#endif // DELEGATE_HPP

