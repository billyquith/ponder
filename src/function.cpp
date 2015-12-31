/****************************************************************************
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
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


#include <ponder/function.hpp>
#include <ponder/classvisitor.hpp>
#include <ponder/args.hpp>


namespace ponder
{
//-------------------------------------------------------------------------------------------------
Function::~Function()
{
}

//-------------------------------------------------------------------------------------------------
const std::string& Function::name() const
{
    return m_name;
}

//-------------------------------------------------------------------------------------------------
std::size_t Function::argCount() const
{
    return m_argTypes.size();
}

//-------------------------------------------------------------------------------------------------
Type Function::returnType() const
{
    return m_returnType;
}

//-------------------------------------------------------------------------------------------------
Type Function::argType(std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_argTypes.size())
        PONDER_ERROR(OutOfRange(index, m_argTypes.size()));

    return m_argTypes[index];
}

//-------------------------------------------------------------------------------------------------
bool Function::callable(const UserObject& object) const
{
    return m_callable.get(object);
}

//-------------------------------------------------------------------------------------------------
Value Function::call(const UserObject& object, const Args& args) const
{
    // Check if the function is callable
    if (!callable(object))
        PONDER_ERROR(ForbiddenCall(name()));

    // Check the number of arguments
    if (args.count() < m_argTypes.size())
        PONDER_ERROR(NotEnoughArguments(name(), args.count(), m_argTypes.size()));

    // Execute the function
    return execute(object, args);
}

//-------------------------------------------------------------------------------------------------
void Function::accept(ClassVisitor& visitor) const
{
    visitor.visit(*this);
}

//-------------------------------------------------------------------------------------------------
Function::Function(const std::string& name, Type returnType, const std::vector<Type>& argTypes)
    : m_name(name)
    , m_returnType(returnType)
    , m_argTypes(argTypes)
    , m_callable(true)
{
}

} // namespace ponder
