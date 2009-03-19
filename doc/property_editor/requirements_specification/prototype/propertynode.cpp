#include <propertynode.hpp>

#include <camp/class.hpp>
#include <camp/qt/qt.hpp>

#include <QSpinBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QApplication>

PropertyNode::PropertyNode(const camp::Property& property, int index, PropertyNode* parent)
    : m_parent(parent)
    , m_property(property)
    , m_index(index)
{
    buildTree();
}

PropertyNode::PropertyNode(const camp::Property& property, int index, const camp::UserObject& subject)
    : m_parent(0)
    , m_property(property)
    , m_index(index)
    , m_subject(subject)
{
    buildTree();
}

PropertyNode::~PropertyNode()
{
    foreach(PropertyNode* node, m_children)
        delete node;
}

PropertyNode* PropertyNode::parent() const
{
    return m_parent;
}

int PropertyNode::childCount() const
{
    return m_children.size();
}

PropertyNode* PropertyNode::child(int index)
{
    return m_children.value(index);
}

int PropertyNode::index() const
{
    return m_index;
}

Qt::ItemFlags PropertyNode::flags(int column) const
{
    if (column == 0)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if (column == 1)
    {
        Qt::ItemFlags f = Qt::ItemIsSelectable;

        if (m_property.writable(subject()) && m_property.type() != camp::arrayType)
            f |= Qt::ItemIsEnabled | Qt::ItemIsEditable;

        return f;
    }

    return Qt::NoItemFlags;
}

QVariant PropertyNode::data(int column, int role) const
{
    switch (role)
    {
        case Qt::DisplayRole:
            if (column == 0)
                return QString::fromStdString(m_property.name());

            if (column == 1)
            {
                if (!m_property.readable(subject()))
                    return QVariant();

                if (m_property.type() == camp::arrayType)
                    return QString("size: %1").arg(static_cast<const camp::ArrayProperty&>(m_property).size(subject()));

                if (m_property.type() != camp::userType)
                    return camp_ext::QtVariantConverter::fromValue(m_property.get(subject()));
            }
            break;

         case Qt::EditRole:
            if (column == 0)
                return QString::fromStdString(m_property.name());

            if (column == 1)
            {
                if (m_property.readable(subject()))
                    return data(column, Qt::DisplayRole);

                switch (m_property.type())
                {
                    case camp::noType:
                        return QVariant();
                    case camp::boolType:
                        return false;
                    case camp::intType:
                        return 0;
                    case camp::realType:
                        return 0.0;
                    case camp::stringType:
                        return QString();
                    case camp::enumType:
                        return QVariant();
                    case camp::arrayType:
                        return QVariant();
                    case camp::userType:
                        return QVariant();
                    default:
                        return QVariant();
                }
            }
            break;
    }

    return QVariant();
}

bool PropertyNode::setData(int column, const QVariant& value, int role)
{
    switch (role)
    {
        case Qt::EditRole:
            if (column == 0)
                return false;

            if (column == 1)
            {
                if (!m_property.writable(subject()))
                    return false;

                if (m_property.type() == camp::userType)
                    return false;

                m_property.set(subject(), camp_ext::QtVariantConverter::toValue(value));

                return true;
            }
            break;
    }

    return false;
}

const camp::UserObject& PropertyNode::subject() const
{
    if (m_subject == camp::UserObject::nothing && m_parent)
        m_subject = m_parent->childSubject();

    return m_subject;
}

camp::UserObject PropertyNode::childSubject() const
{
    return m_property.get(subject()).to<camp::UserObject>();
}

bool PropertyNode::hasPersistentEditor(int column) const
{
    return (column == 0) && (m_property.type() == camp::arrayType);
}

QWidget* PropertyNode::persistentEditor(QWidget* parent) const
{
    QWidget* w = new QWidget(parent);
    w->setAutoFillBackground(true);
    w->setFocusPolicy(Qt::StrongFocus);

    QLayout* l = new QHBoxLayout(w);
    QSpinBox* sb = new QSpinBox;
    sb->setObjectName("spinbox");
    l->addWidget(sb);

    QPushButton* left = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_ArrowLeft), "");
    l->addWidget(left);
    QObject::connect(left, SIGNAL(clicked()), sb, SLOT(stepDown()));

    QPushButton* right = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_ArrowRight), "");
    l->addWidget(right);
    QObject::connect(right, SIGNAL(clicked()), sb, SLOT(stepUp()));

    return w;
}

void PropertyNode::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QSpinBox* spinbox = editor->findChild<QSpinBox*>("spinbox");

    int size = static_cast<const camp::ArrayProperty&>(m_property).size(subject());
    spinbox->setRange(0, size - 1);
    spinbox->setValue(0);
    spinbox->setEnabled(size > 0);
}

void PropertyNode::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index)
{

}

void PropertyNode::accept(Visitor& visitor, const QModelIndex& index) const
{
    foreach (PropertyNode* node, m_children)
        visitor.visit(node, index);
}

void PropertyNode::buildTree()
{
    if (m_property.type() == camp::userType)
    {
        const camp::UserProperty& userProp = static_cast<const camp::UserProperty&>(m_property);
        const camp::Class& metaclass = userProp.getClass();
        for (int i = 0; i< metaclass.propertyCount(); ++i)
            m_children.insert(i, new PropertyNode(metaclass.property(i), i, this));
    }
}

