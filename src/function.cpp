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


#include <camp/function.hpp>
#include <camp/classvisitor.hpp>
#include <camp/invalidindex.hpp>
#include <camp/invalidaccess.hpp>
#include <camp/invalidargument.hpp>


namespace camp
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
    if (index >= m_argTypes.size())
        CAMP_ERROR(InvalidIndex(index, m_argTypes.size()));

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
        CAMP_ERROR(InvalidAccess(m_name.c_str(), InvalidAccess::Call));

    // Check the arguments count
    if (args.count() < m_argTypes.size())
        CAMP_ERROR(InvalidArgument(noType, m_argTypes[args.count()], m_name.c_str(), args.count()));

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

} // namespace camp
