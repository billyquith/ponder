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
#ifndef PONDER_VALUEVISITOR_HPP
#define PONDER_VALUEVISITOR_HPP

namespace ponder {
    
/**
 * \brief Base class for writing custom Value visitors
 *
 * A value visitor acts like compile-time dispatchers which automatically
 * calls the function which matches the actual type of the stored value.
 * This is a more direct and straight-forward approach than using a runtime switch,
 * based on value.kind() and then converting to the proper type.
 * It also gives access to enum and user objects, which can give useful informations with
 * no knowledge about the actual C++ class or enum.
 *
 * The template parameter T is the type returned by the visitor.
 *
 * To handle one of the possible types of the value, just write the corresponding 
 * \c operator() function. Here is the list of the mapping between Ponder types and their 
 * corresponding C++ types:
 *
 * \li ponder::ValueKind::None --> ponder::NoType
 * \li ponder::ValueKind::Boolean --> bool
 * \li ponder::ValueKind::Integer --> long
 * \li ponder::ValueKind::Real --> double
 * \li ponder::ValueKind::String --> ponder::String
 * \li ponder::ValueKind::Enum --> ponder::EnumObject
 * \li ponder::ValueKind::User --> ponder::UserObject
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
 *     PropertyEditor* operator()(IdRef value)
 *     {
 *         return new StringEditor(value);
 *     }
 * 
 *     PropertyEditor* operator()(const ponder::EnumObject& value)
 *     {
 *         return new EnumEditor(value);
 *     }
 * 
 *     PropertyEditor* operator()(const ponder::UserObject& value)
 *     {
 *         return new UserEditor(value);
 *     }
 * };
 * 
 * ponder::Value value(5.4);
 * PropertyEditor* editor = value.visit(EditorFactory());
 * \endcode
 */
template <typename T = void>
class ValueVisitor
{
public:
    using result_type = T; //!< Type of value visited.
};

} // namespace ponder

#endif // PONDER_VALUEVISITOR_HPP
