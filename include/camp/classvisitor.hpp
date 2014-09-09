/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
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


#ifndef CAMP_CLASSVISITOR_HPP
#define CAMP_CLASSVISITOR_HPP


#include <camp/config.hpp>


namespace camp
{
class Property;
class SimpleProperty;
class ArrayProperty;
class EnumProperty;
class UserProperty;
class Function;

/**
 * \brief Base class for writing custom Class visitors
 *
 * A ClassVisitor, when applied to a Class, will be notified for each member property and function of
 * the target class. To receive this notification for a specific type T of property or function,
 * you have to override the \c visit(T) function.
 *
 * The benefit of such visitation is that you directly get the actual type of the
 * property or function, and can handle it safely without having to use dangerous downcasts.
 *
 * Another benefit of this method is that you can easily filter the properties and functions
 * according to their C++ type: if you don't want to handle a specific type of property or function,
 * just don't override the corresponding \c visit function.
 *
 * Here an example of a visitor which prints the contents of a class:
 * \code
 * class MyClassVisitor : public camp::ClassVisitor
 * {
 *     void visit(const camp::SimpleProperty& property)
 *     {
 *         std::cout << "Simple property: " << property.name() << std::endl;
 *     }
 * 
 *     void visit(const camp::ArrayProperty& property)
 *     {
 *         std::cout << "Array property: " << property.name() << " - "
 *                   << "dynamic:" << property.dynamic() << std::endl;
 *     }
 * 
 *     void visit(const camp::EnumProperty& property)
 *     {
 *         std::cout << "Enum property: " << property.name() << " - "
 *                   << "owner enum:" << property.getEnum().name() << std::endl;
 *     }
 * 
 *     void visit(const camp::UserProperty& property)
 *     {
 *         std::cout << "User property: " << property.name() << " - "
 *                   << "owner class:" << property.getClass().name() << std::endl;
 *     }
 * 
 *     void visit(const camp::Function& function)
 *     {
 *         std::cout << "Function: " << function.name() << " - "
 *                   << "number of arguments:" << function.argCount() << std::endl;
 *     }
 * };
 * 
 * const camp::Class& metaclass = camp::classByName("MyClass");
 * MyClassVisitor visitor;
 * metaclass.visit(visitor);
 * \endcode
 */
class CAMP_API ClassVisitor
{
public:

    /**
     * \brief Destructor
     */
    virtual ~ClassVisitor();

    /**
     * \brief Visit any kind of property
     *
     * This function is a generic callback which can be used
     * to receive all property types which are not handled with
     * their derived type.
     *
     * \param property Property which is being visited
     */
    virtual void visit(const Property& property);

    /**
     * \brief Visit a simple property
     *
     * \param property Property which is being visited
     */
    virtual void visit(const SimpleProperty& property);

    /**
     * \brief Visit an array property
     *
     * \param property Property which is being visited
     */
    virtual void visit(const ArrayProperty& property);

    /**
     * \brief Visit an enum property
     *
     * \param property Property which is being visited
     */
    virtual void visit(const EnumProperty& property);

    /**
     * \brief Visit a user property
     *
     * \param property Property which is being visited
     */
    virtual void visit(const UserProperty& property);

    /**
     * \brief Visit a function
     *
     * \param function Function which is being visited
     */
    virtual void visit(const Function& function);

protected:

    /**
     * \brief Default constructor
     */
    ClassVisitor();
};

} // namespace camp


#endif // CAMP_VALUEVISITOR_HPP
