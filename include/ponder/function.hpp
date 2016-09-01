/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2016 Billy Quith.
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


namespace ponder {
    
class Args;
class UserObject;
class ClassVisitor;
    
/**
 * \brief Abstract representation of a function
 *
 * Functions are members of metaclasses. Their purpose is to provide detailed information
 * about their prototype.
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
    IdRef name() const;
    
    FunctionType classification() const { return m_funcType; }

    /**
     * \brief Get the type of variable returned by the function
     *
     * \return Type of the result of the function
     */
    ValueType returnType() const;

    /**
     * \brief Get the number of parameters of the function
     *
     * \return Total number of parameters taken by the function
     */
    virtual std::size_t paramCount() const = 0;

    /**
     * \brief Get the type of an parameter given by its index
     *
     * \param index Index of the parameter
     *
     * \return Type of the index-th parameter
     *
     * \throw OutOfRange index is out of range
     */
    virtual ValueType paramType(std::size_t index) const = 0;

    /**
     * \brief Accept the visitation of a ClassVisitor
     *
     * \param visitor Visitor to accept
     */
    virtual void accept(ClassVisitor& visitor) const;

protected:

    /**
     * \brief Construct the function from its description
     *
     * \param name Name of the function
     * \param returnType Type of the function result
     * \param paramTypes Types of the function parameters (empty array by default)
     *
     * \return Value returned by the function call
     */
    Function(IdRef name) : m_name(name) {}
    
    Function(const Function&) = delete;

    Id m_name; ///< Name of the function
    FunctionType m_funcType;
    ValueType m_returnType; ///< Return type
    
};

} // namespace ponder


#endif // PONDER_FUNCTION_HPP
