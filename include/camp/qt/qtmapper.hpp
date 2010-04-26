/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** CAMP is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** CAMP is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with CAMP.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/


#ifndef CAMP_QT_QTMAPPER_HPP
#define CAMP_QT_QTMAPPER_HPP


#include <camp/qt/qtsimpleproperty.hpp>
#include <camp/qt/qtfunction.hpp>
#include <camp/function.hpp>
#include <QMetaObject>
#include <QMetaMethod>


namespace camp_ext
{
/**
 * \brief Mapper for Qt properties
 *
 * This class tells CAMP how to build metaproperties and metafunctions from Qt's metatypes.
 *
 * This mapper is not complete due to some limitations of either CAMP or Qt:
 * \li enum properties are not mapped using camp::EnumProperty, but camp::SimpleProperty
 * \li arrays and user defined classes are not supported for meta-properties
 * \li arrays, user defined enums and classes are not supported for meta-functions
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
        int count = 0;
        for (int i = 0; i < T::staticMetaObject.methodCount(); ++i)
        {
            // Discard incompatible types
            if (filter(T::staticMetaObject.method(i)))
                count++;
        }

        return count;
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
        int func = 0;
        int count = 0;
        for (func = 0; count <= static_cast<int>(index); ++func)
        {
            // Discard incompatible types
            if (filter(T::staticMetaObject.method(func)))
                count++;
        }

        return new QtFunction<T>(T::staticMetaObject.method(func - 1));
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
        return QtHelper::type(property.type()) != camp::noType
               || property.isEnumType();
    }

    /**
     * \brief Check if a function is mappable to CAMP
     *
     * \param function Function to check
     *
     * \return True if the function can be bound to CAMP
     */
    static bool filter(const QMetaMethod& function)
    {
        // Check return type
        if ((QtHelper::type(function.typeName()) == camp::noType)
            && QMetaType::type(function.typeName()) != QMetaType::Void) // void maps to camp::noType but is a valid return type
        {
            return false;
        }

        // Check parameters types
        QList<QByteArray> args = function.parameterTypes();
        Q_FOREACH(QByteArray arg, args)
        {
            if (QtHelper::type(arg) == camp::noType)
                return false;
        }

        return true;
    }
};

} // namespace camp_ext


#endif // CAMP_QT_QTMAPPER_HPP
