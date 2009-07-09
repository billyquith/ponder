/****************************************************************************
**
** Copyright (C) 2009 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** Commercial Usage
** Licensees holding valid CAMP Commercial licenses may use this file in
** accordance with the CAMP Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and TECHNOGERMA Systems France.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3.txt included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at sales@technogerma.fr.
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
