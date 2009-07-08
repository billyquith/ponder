

#ifndef CAMP_QT_QTMAPPER_HPP
#define CAMP_QT_QTMAPPER_HPP


#include <camp/qt/qtsimpleproperty.hpp>
#include <camp/function.hpp>
#include <QMetaObject>


namespace camp_ext
{
/**
 * \brief Mapper for Qt properties
 *
 * This class tells CAMP how to build metaproperties and metafunctions from Qt's metatypes
 */
template <typename T>
class QtMapper
{
public:

    /**
     * \brief Return the number of properties to map
     *
     * \return Number of properties
     */
    static std::size_t propertyCount()
    {
        int count = 0;
        for (int i = 0; i < T::staticMetaObject.propertyCount(); ++i)
        {
            // Discard incompatible types
            if (filter(T::staticMetaObject.property(i)))
                count++;
        }

        return count;
    }

    /**
     * \brief Map a property
     *
     * \param index Index of the property to map
     *
     * \return index-th property mapped to a camp::Property
     */
    static camp::Property* property(std::size_t index)
    {
        int prop = 0;
        int count = 0;
        for (prop = 0; count <= static_cast<int>(index); ++prop)
        {
            // Discard incompatible types
            if (filter(T::staticMetaObject.property(prop)))
                count++;
        }

        return new QtSimpleProperty<T>(T::staticMetaObject.property(prop - 1));
    }

    /**
     * \brief Return the number of functions to map
     *
     * \return Number of functions
     */
    static std::size_t functionCount()
    {
        return 0;
    }

    /**
     * \brief Map a function
     *
     * \param index Index of the function to map
     *
     * \return index-th function mapped to a camp::function
     */
    static camp::Function* function(std::size_t index)
    {
        return 0;
    }

private:

    /**
     * \brief Check if a property is mappable to CAMP
     *
     * \param property Property to check
     *
     * \return True if the property can be bound to CAMP
     */
    static bool filter(const QMetaProperty& property)
    {
        return QtVariantConverter::type(property.type()) != camp::noType;
    }
};

} // namespace camp_ext


#endif // CAMP_QT_QTMAPPER_HPP
