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


#ifndef CAMP_VALUEVISITOR_HPP
#define CAMP_VALUEVISITOR_HPP


#include <boost/variant/static_visitor.hpp>


namespace camp
{
/**
 * \brief Base class for writing custom Value visitors
 *
 * A value visitor acts like compile-time dispatchers which automatically
 * calls the function which matches the actual type of the stored value.
 * This is a more direct and straight-forward approach than using a runtime switch,
 * based on value.type() and then converting to the proper type.
 * It also gives access to enum and user objects, which can give useful informations with
 * no knowledge about the actual C++ class or enum.
 *
 * The template parameter T is the type returned by the visitor.
 *
 * To handle one of the possible types of the value, just write the corresponding \c operator() function.
 * Here is the list of the mapping between CAMP types and their corresponding C++ types:
 *
 * \li camp::noType --> camp::NoType
 * \li camp::boolType --> bool
 * \li camp::intType --> long
 * \li camp::realType --> double
 * \li camp::stringType --> std::string
 * \li camp::enumType --> camp::EnumObject
 * \li camp::userType --> camp::UserObject
 *
 * Here an example of a unary visitor which creates an editor for the value based on its type
 * \code
 * struct EditorFactory : public ValueVisitor<PropertyEditor*>
 * {
 *     PropertyEditor* operator()(bool value)
 *     {
 *         return new BooleanEditor(value);
 *     }
 * 
 *     PropertyEditor* operator()(long value)
 *     {
 *         return new IntegerEditor(value);
 *     }
 * 
 *     PropertyEditor* operator()(double value)
 *     {
 *         return new RealEditor(value);
 *     }
 * 
 *     PropertyEditor* operator()(const std::string& value)
 *     {
 *         return new StringEditor(value);
 *     }
 * 
 *     PropertyEditor* operator()(const camp::EnumObject& value)
 *     {
 *         return new EnumEditor(value);
 *     }
 * 
 *     PropertyEditor* operator()(const camp::UserObject& value)
 *     {
 *         return new UserEditor(value);
 *     }
 * };
 * 
 * camp::Value value(5.4);
 * PropertyEditor* editor = value.visit(EditorFactory());
 * \endcode
 */
template <typename T = void>
class ValueVisitor : public boost::static_visitor<T>
{

};

} // namespace camp


#endif // CAMP_VALUEVISITOR_HPP
