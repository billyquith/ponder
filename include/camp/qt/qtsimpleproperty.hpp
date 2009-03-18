// <insert copyright>

#ifndef CAMP_QT_QTSIMPLEPROPERTY_HPP
#define CAMP_QT_QTSIMPLEPROPERTY_HPP


#include <camp/qt/qtvariantconverter.hpp>
#include <camp/simpleproperty.hpp>
#include <camp/userobject.hpp>
#include <camp/value.hpp>
#include <QMetaProperty>
#include <QVariant>


namespace camp_ext
{
/**
 * \brief Specialization of camp::SimpleProperty implemented using a Qt property
 *
 * This class is instanciated and returned by QMapper<T>.
 *
 * \sa QMapper
 */
template <typename T>
class QtSimpleProperty : public camp::SimpleProperty
{
public:

    /**
     * \brief Construct the property from a QMetaProperty
     *
     * \param metaProperty Qt meta property
     */
    QtSimpleProperty(const QMetaProperty& metaProperty)
        : camp::SimpleProperty(metaProperty.name(), metaProperty.isEnumType() ? camp::enumType : QtVariantConverter::type(metaProperty.type()))
        , m_metaProperty(metaProperty)
    {
    }

    /**
     * \brief Get the value of the property for the given object
     *
     * \param object Object to read
     *
     * \return Current value of the property
     */
    virtual camp::Value getValue(const camp::UserObject& object) const
    {
        return QtVariantConverter::toValue(m_metaProperty.read(object.get<T*>()));
    }

    /**
     * \brief Set the value of the property for the given object
     *
     * \param object Object to write
     * \param value New value of the property
     */
    virtual void setValue(const camp::UserObject& object, const camp::Value& value) const
    {
        m_metaProperty.write(object.get<T*>(), QtVariantConverter::fromValue(value));
    }

    /**
     * \brief Check if the property is readable
     *
     * \return True if the property is readable, false otherwise
     */
    virtual bool isReadable() const
    {
        return m_metaProperty.isReadable();
    }

    /**
     * \brief Check if the property is writable
     *
     * \return True if the property is writable, false otherwise
     */
    virtual bool isWritable() const
    {
        return m_metaProperty.isWritable();
    }

private:

    QMetaProperty m_metaProperty; ///< Internal Qt property
};

} // namespace camp_ext


#endif // CAMP_QT_QTSIMPLEPROPERTY_HPP
