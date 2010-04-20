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



namespace camp
{
//-------------------------------------------------------------------------------------------------
inline std::size_t enumCount()
{
    return detail::EnumManager::instance().count();
}

//-------------------------------------------------------------------------------------------------
inline const Enum& enumByIndex(std::size_t index)
{
    return detail::EnumManager::instance().getByIndex(index);
}

//-------------------------------------------------------------------------------------------------
inline const Enum& enumByName(const std::string& name)
{
    return detail::EnumManager::instance().getByName(name);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
const Enum& enumByObject(T)
{
    return detail::EnumManager::instance().getById(detail::typeId<T>());
}

//-------------------------------------------------------------------------------------------------
template <typename T>
const Enum& enumByType()
{
    return detail::EnumManager::instance().getById(detail::typeId<T>());
}

//-------------------------------------------------------------------------------------------------
template <typename T>
const Enum* enumByTypeSafe()
{
    return detail::EnumManager::instance().getByIdSafe(detail::safeTypeId<T>());
}

} // namespace camp
