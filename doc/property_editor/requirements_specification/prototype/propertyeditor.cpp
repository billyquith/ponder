
#include <propertyeditor.hpp>
#include <visitor.hpp>

#include <QHBoxLayout>

PropertyEditor::PropertyEditor(QWidget* parent)
    : QWidget(parent)
    , m_view(new QTreeView)
{
    m_view->setItemDelegate(&m_delegate);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(m_view);
    m_view->setModel(&m_model);
}

void PropertyEditor::setSubject(const camp::UserObject& subject)
{
    m_subject = subject;
    m_model.setSubject(m_subject);
    m_view->resizeColumnToContents(0);

    PersistentEditorVisitor v(m_view);
    m_model.accept(v);
}

