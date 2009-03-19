#ifndef PROPERTYEDITOR_HPP
#define PROPERTYEDITOR_HPP

#include <delegate.hpp>
#include <treemodel.hpp>

#include <camp/userobject.hpp>

#include <QTreeView>

class PropertyEditor : public QWidget
{
    Q_OBJECT

public:
        PropertyEditor(QWidget* parent = 0);

        void setSubject(const camp::UserObject& subject = camp::UserObject::nothing);

private:
        QTreeView* m_view;
        TreeModel m_model;
        camp::UserObject m_subject;
        Delegate m_delegate;
};

#endif // PROPERTYEDITOR_HPP

