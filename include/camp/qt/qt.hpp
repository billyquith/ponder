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


#ifndef CAMP_QT_QT_HPP
#define CAMP_QT_QT_HPP


/**
 * This file extends CAMP for the Qt library; it contains all the mappings needed to
 * make Qt work seamlessly with CAMP:
 * - QString is declared as a string type
 * - QVector and QList are declared as array types
 * - QMapper maps Qt properties to CAMP properties
 */

#include <camp/qt/qstring.hpp>
#include <camp/qt/qvector.hpp>
#include <camp/qt/qlist.hpp>
#include <camp/qt/qtmapper.hpp>


#endif // CAMP_QT_QT_HPP
