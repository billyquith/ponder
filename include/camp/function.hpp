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


#ifndef CAMP_FUNCTION_HPP
#define CAMP_FUNCTION_HPP


#include <camp/config.hpp>
#include <camp/detail/getter.hpp>
#include <camp/args.hpp>
#include <camp/tagholder.hpp>
#include <camp/type.hpp>
#include <camp/value.hpp>
#include <string>
#include <vector>


namespace camp
{
template <typename T> class ClassBuilder;
class UserObject;
class ClassVisitor;

/**
 * \brief Abstract representation of a function
 *
 * Functions are members of metaclasses. Their main purpose is to be called; they also provide
 * detailed informations about their prototype.
 */
class CAMP_API Function : public TagHolder
{
public:

    /**
     * \brief Destructor
     */
    virtual ~Function();

    /**
     * \brief Get the name of the function
     *
     * \return Name of the function
     */
    const std::string& name() const;

    /**
     * \brief Get the number of arguments of the function
     *
     * \return Total number of arguments taken by the function
     */
    std::size_t argCount() const;

    /**
     * \brief Get the type of variable returned by the function
     *
     * \return Type of the result of the function
     */
    Type returnType() const;

    /**
     * \brief Get the type of an argument given by its index
     *
     * \param index Index of the argument
     *
     * \return Type of the index-th argument
     *
     * \throw OutOfRange index is out of range
     */
    Type argType(std::size_t index) const;

    /**
     * \brief Check if the function is currently callable for a given object
     *
     * \param object Object
     *
     * \return True if the function can be called, false otherwise
     *
     * \throw NullObject object is invalid
     */
    bool callable(const UserObject& object) const;

    /**
     * \brief Call the function
     *
     * \param object Object
     * \param args Arguments to pass to the function (empty list by default)
     *
     * \return Value returned by the function call
     *
     * \throw ForbiddenCall the function is not callable
     * \throw NullObject object is invalid
     * \throw NotEnoughArguments too few arguments are provided
     * \throw BadArgument one of the arguments can't be converted to the requested type
     */
    Value call(const UserObject& object, const Args& args = Args::empty) const;

    /**
     * \brief Accept the visitation of a ClassVisitor
     *
     * \param visitor Visitor to accept
     */
    virtual void accept(ClassVisitor& visitor) const;

protected:

    template <typename T> friend class ClassBuilder;

    /**
     * \brief Construct the function from its description
     *
     * \param name Name of the function
     * \param returnType Type of the function result
     * \param argTypes Types of the function arguments (empty array by default)
     *
     * \return Value returned by the function call
     */
    Function(const std::string& name, Type returnType, const std::vector<Type>& argTypes = std::vector<Type>());

    /**
     * \brief Do the actual call
     *
     * This function is a pure virtual which has to be implemented in derived classes.
     *
     * \param object Object
     * \param args Arguments to pass to the function
     *
     * \return Value returned by the function call
     *
     * \throw NullObject object is invalid
     * \throw BadArgument one of the arguments can't be converted to the requested type
     */
    virtual Value execute(const UserObject& object, const Args& args) const = 0;

private:

    std::string m_name; ///< Name of the function
    Type m_returnType; ///< Return type
    std::vector<Type> m_argTypes; ///< Type of all the function arguments
    detail::Getter<bool> m_callable; ///< Accessor to get the callable state of the function
};

} // namespace camp


#endif // CAMP_FUNCTION_HPP
