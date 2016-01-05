/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
**
****************************************************************************/


#ifndef PONDER_FUNCTION_HPP
#define PONDER_FUNCTION_HPP


#include <ponder/config.hpp>
#include <ponder/detail/getter.hpp>
#include <ponder/args.hpp>
#include <ponder/tagholder.hpp>
#include <ponder/type.hpp>
#include <ponder/value.hpp>
#include <string>
#include <vector>


namespace ponder
{
class Args;
class UserObject;
class ClassVisitor;

/**
 * \brief Abstract representation of a function
 *
 * Functions are members of metaclasses. Their main purpose is to be called; they also provide
 * detailed informations about their prototype.
 */
class PONDER_API Function : public TagHolder
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
     * \param args Arguments to pass to the function, for example "ponder::Args::empty"
     *
     * \return Value returned by the function call
     *
     * \throw ForbiddenCall the function is not callable
     * \throw NullObject object is invalid
     * \throw NotEnoughArguments too few arguments are provided
     * \throw BadArgument one of the arguments can't be converted to the requested type
     */
    Value call(const UserObject& object, const Args& args = Args::empty) const;

    // todo: pass args using variadic template:
    //    template <typename ...A>
    //    Value call(const UserObject& object, A... a) const
    //    {
    //        return call(object, callWithArgs(a...));
    //    }

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

} // namespace ponder


#endif // PONDER_FUNCTION_HPP
