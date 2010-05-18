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


#ifndef CAMP_ERRORS_HPP
#define CAMP_ERRORS_HPP


#include <camp/error.hpp>
#include <camp/type.hpp>


namespace camp
{
class Class;

/**
 * \brief Error thrown when providing a metavalue whose type is incompatible with what's expected
 */
class CAMP_API BadType : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param provided Provided type
     * \param expected Expected type
     */
    BadType(Type provided, Type expected);

protected:

    /**
     * \brief Constructor for derived classes
     *
     * \param message Description of the error
     */
    BadType(const std::string& message);

    /**
     * \brief Get the string name of a CAMP type
     *
     * \param type Type to convert
     *
     * \return Name of the provided type
     */
    static std::string typeName(Type type);
};

/**
 * \brief Error thrown when providing a function argument which is incompatible with what the function expects
 */
class CAMP_API BadArgument : public BadType
{
public:

    /**
     * \brief Constructor
     *
     * \param provided Provided type
     * \param expected Expected type
     * \param index Index of the argument in the function prototype
     * \param functionName Name of the function
     */
    BadArgument(Type provided, Type expected, std::size_t index, const std::string& functionName);
};

/**
 * \brief Error thrown when a declaring a metaclass that already exists
 */
class CAMP_API ClassAlreadyCreated : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param name Name of the class
     * \param type Identifier of the C++ type
     */
    ClassAlreadyCreated(const std::string& name, const std::string& type);
};

/**
 * \brief Error thrown when a metaclass couldn't be found (either by its name or its id)
 */
class CAMP_API ClassNotFound : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param name Name of the requested class
     */
    ClassNotFound(const std::string& name);
};

/**
 * \brief Error thrown when trying to convert an object to a class that is not a base nor a derived
 */
class CAMP_API ClassUnrelated : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param sourceClass Name of the source class
     * \param requestedClass Name of the requested class
     */
    ClassUnrelated(const std::string& sourceClass, const std::string& requestedClass);
};

/**
 * \brief Error thrown when no constructor was found to match the provided list of arguments
 */
class CAMP_API ConstructorNotFound : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param className Name of the owner class
     */
    ConstructorNotFound(const std::string& className);
};

/**
 * \brief Error thrown when a declaring a metaenum that already exists
 */
class CAMP_API EnumAlreadyCreated : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param name Name of the enum
     * \param type Identifier of the C++ type
     */
    EnumAlreadyCreated(const std::string& name, const std::string& type);
};

/**
 * \brief Error thrown when the value of a metaenum couldn't be found by its name
 */
class CAMP_API EnumNameNotFound : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param name Name of the requested metaenum member
     * \param enumName Name of the owner metaenum
     */
    EnumNameNotFound(const std::string& name, const std::string& enumName);
};

/**
 * \brief Error thrown when a metaenum couldn't be found (either by its name or its id)
 */
class CAMP_API EnumNotFound : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param name Name of the requested enum
     */
    EnumNotFound(const std::string& name);
};

/**
 * \brief Error thrown when a value in a metaenum couldn't be found
 */
class CAMP_API EnumValueNotFound : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param value Value of the requested metaenum member
     * \param enumName Name of the owner metaenum
     */
    EnumValueNotFound(long value, const std::string& enumName);
};

/**
 * \brief Error thrown when calling a function that is not callable
 */
class CAMP_API ForbiddenCall : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param functionName Name of the function
     */
    ForbiddenCall(const std::string& functionName);
};

/**
 * \brief Error thrown when trying to read a property that is not readable
 */
class CAMP_API ForbiddenRead : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param propertyName Name of the property
     */
    ForbiddenRead(const std::string& propertyName);
};

/**
 * \brief Error thrown when trying to write a function that is not writable
 */
class CAMP_API ForbiddenWrite : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param propertyName Name of the property
     */
    ForbiddenWrite(const std::string& propertyName);
};

/**
 * \brief Error thrown when a function can't be found in a metaclass (by its name)
 */
class CAMP_API FunctionNotFound : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param name Name of the requested function
     * \param className Name of the owner metaclass
     */
    FunctionNotFound(const std::string& name, const std::string& className);
};

/**
 * \brief Error thrown when a declaring a metaclass that already exists
 */
class CAMP_API NotEnoughArguments : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param functionName Name of the function
     * \param provided Number of arguments provided
     * \param expected Number of arguments expected
     */
    NotEnoughArguments(const std::string& functionName, std::size_t provided, std::size_t expected);
};

/**
 * \brief Error thrown when trying to use an empty metaobject
 */
class CAMP_API NullObject : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param objectClass Metaclass of the object (may be null if object has no class)
     */
    NullObject(const Class* objectClass);
};

/**
 * \brief Error thrown when using an index which is out of bounds
 */
class CAMP_API OutOfRange : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param index Invalid index
     * \param size Allowed size
     */
    OutOfRange(std::size_t index, std::size_t size);
};

/**
 * \brief Error thrown when a property can't be found in a metaclass (by its name)
 */
class CAMP_API PropertyNotFound : public Error
{
public:

    /**
     * \brief Constructor
     *
     * \param name Name of the requested property
     * \param className Name of the owner metaclass
     */
    PropertyNotFound(const std::string& name, const std::string& className);
};

} // namespace camp


#endif // CAMP_ERRORS_HPP
