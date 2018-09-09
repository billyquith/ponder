/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2018 Nick Trout.
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

#pragma once
#ifndef PONDER_FUNCTION_HPP
#define PONDER_FUNCTION_HPP

#include <ponder/config.hpp>
#include <ponder/detail/getter.hpp>
#include <ponder/args.hpp>
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
class PONDER_API Function : public Type
{
    PONDER__NON_COPYABLE(Function);
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
    IdReturn name() const;
    
   /**
    * \brief Get the kind of function represented here
    *
    * \return Kind of the function
    */
    FunctionKind kind() const { return m_funcType; }

    /**
     * \brief Get the type of variable returned by the function
     *
     * \return Type of the result of the function
     */
    ValueKind returnType() const;
    
    /**
     * \brief Get the kind of return policy this function uses
     *
     * \return Kind of return policy enum
     */
    policy::ReturnKind returnPolicy() const { return m_returnPolicy; }

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
    virtual ValueKind paramType(std::size_t index) const = 0;

    /**
     * \brief Accept the visitation of a ClassVisitor
     *
     * \param visitor Visitor to accept
     */
    virtual void accept(ClassVisitor& visitor) const;
    
   /**
    * \brief Get the per-function uses data (internal)
    *
    * \note This data is private to the uses module that created it.
    *
    * \return Opaque data pointer
    */
    const void* getUsesData() const {return m_usesData;}
    
protected:

    // FunctionImpl inherits from this and constructs.
    Function(IdRef name) : m_name(name) {}

    Id m_name;                          // Name of the function
    FunctionKind m_funcType;            // Kind of function
    ValueKind m_returnType;             // Runtime return type
    policy::ReturnKind m_returnPolicy;  // Return policy
    const void *m_usesData;
};
    
} // namespace ponder

#endif // PONDER_FUNCTION_HPP
