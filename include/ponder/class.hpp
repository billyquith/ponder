/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
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


#ifndef PONDER_CLASS_HPP
#define PONDER_CLASS_HPP


#include <ponder/classget.hpp>
#include <ponder/classcast.hpp>
#include <ponder/property.hpp>
#include <ponder/function.hpp>
#include <ponder/tagholder.hpp>
#include <ponder/userobject.hpp>
#include <ponder/detail/typeid.hpp>
#include <ponder/detail/dictionary.hpp>
#include <string>
#include <map>

namespace ponder
{
template <typename T> class ClassBuilder;
class Constructor;
class Args;
class ClassVisitor;

/**
 * \brief ponder::Class represents a metaclass composed of properties and functions
 *
 * ponder::Class is the main class of the Ponder API. It defines a metaclass, which
 * is an abstract representation of a C++ class with its own properties,
 * functions, constructors, base classes, etc.
 *
 * Classes are declared, bound to a C++ type and filled with the \c declare
 * template function.
 *
 * \code
 * class MyClass
 * {
 * public:
 *
 *     MyClass();
 *     int getProp() const;
 *     void setProp(int);
 *     std::string func();
 * };
 *
 * ponder::Class::declare<MyClass>("MyClass")
 *     .tag("help", "this is my class")
 *     .constructor()
 *     .property("prop", &MyClass::getProp, &MyClass::setProp)
 *     .function("func", &MyClass::func);
 * \endcode
 *
 * It then provides a set of accessors to retrieve its member functions and properties.
 *
 * \code
 * const ponder::Class& metaclass = ponder::classByType<MyClass>();
 *
 * const ponder::Property& prop = metaclass.property("prop");
 * const ponder::Function& func = metaclass.function("func");
 * \endcode
 *
 * Another way to inspect a class, which is more type-safe, is to use a ClassVisitor.
 *
 * \code
 * MyVisitor visitor;
 * metaclass.visit(visitor);
 * \endcode
 *
 * It also allows to create and destroy instances of the bound C++ class.
 *
 * \code
 * MyClass* obj = metaclass.construct<MyClass>();
 * metaclass.destroy(obj);
 * \endcode
 *
 * \remark All function and property names are unique within the metaclass.
 *
 * \sa Enum, TagHolder, ClassBuilder, Function, Property
 */
class PONDER_API Class : public TagHolder, detail::noncopyable
{
public:

    /**
     * \brief Declare a new metaclass
     *
     * This is the function to call to create a new metaclass. The template
     * parameter T is the C++ class that will be bound to the metaclass.
     *
     * \param name Name of the metaclass in Ponder. This name identifies
     *             the metaclass and thus has to be unique. If not specified, the C++ type
     *             id is used.
     *
     * \return A ClassBuilder<T> object that will provide functions
     *         to fill the new metaclass with properties, functions, etc.
     */
    template <typename T>
    static ClassBuilder<T> declare(const std::string& name = std::string());

public:

    /**
     * \brief Return the name of the metaclass
     *
     * \return String containing the name of the metaclass
     */
    const std::string& name() const;

    /**
     * \brief Return the total number of base metaclasses of this metaclass
     *
     * \return Number of base metaclasses
     */
    std::size_t baseCount() const;

    /**
     * \brief Return a base metaclass from its index
     *
     * \param index Index of the base to get
     *
     * \return Reference to the index-th base metaclass of this metaclass
     *
     * \throw OutOfRange index is out of range
     */
    const Class& base(std::size_t index) const;

    /**
     * \brief Return the total number of functions of this metaclass
     *
     * \return Number of functions
     */
    std::size_t functionCount() const;

    /**
     * \brief Check if this metaclass contains the given function
     *
     * \param name Name of the function to check
     *
     * \return True if the function is in the metaclass, false otherwise
     */
    bool hasFunction(const std::string& name) const;

    /**
     * \brief Get a function from its index in this metaclass
     *
     * \param index Index of the function to get
     *
     * \return Reference to the function
     *
     * \throw OutOfRange index is out of range
     */
    const Function& function(std::size_t index) const;

    /**
     * \brief Get a function from its name
     *
     * \param name Name of the function to get (case sensitive)
     *
     * \return Reference to the function
     *
     * \throw FunctionNotFound \a name is not a function of the metaclass
     */
    const Function& function(const std::string& name) const;

    /**
     * \brief Return the total number of properties of this metaclass
     *
     * \return Number of properties
     */
    std::size_t propertyCount() const;

    /**
     * \brief Check if this metaclass contains the given property
     *
     * \param name Name of the property to check
     *
     * \return True if the property is in the metaclass, false otherwise
     */
    bool hasProperty(const std::string& name) const;

    /**
     * \brief Get a property from its index in this metaclass
     *
     * \param index Index of the property to get
     *
     * \return Reference to the property
     *
     * \throw OutOfRange index is out of range
     */
    const Property& property(std::size_t index) const;

    /**
     * \brief Get a property from its name
     *
     * \param name Name of the property to get (case sensitive)
     *
     * \return Reference to the property
     *
     * \throw PropertyNotFound \a name is not a property of the metaclass
     */
    const Property& property(const std::string& name) const;

    /**
    * \brief Return the total number of constructors of this metaclass
    *
    * \return Number of constructors
    */
    std::size_t constructorCount() const;

    /**
     * \brief Construct a new instance of the C++ class bound to the metaclass
     *
     * If no constructor can match the provided arguments, UserObject::nothing
     * is returned.
     * The new instance is wrapped into a UserObject. It must be destroyed
     * with the Class::destroy function.
     *
     * \param args Arguments to pass to the constructor (empty by default)
     *
     * \return New instance wrapped into a UserObject, or UserObject::nothing if it failed
     */
    UserObject construct(const Args& args = Args::empty) const;

    /**
     * \brief Destroy an instance of the C++ class bound to the metaclass
     *
     * This function must be called to destroy every instance created with
     * Class::construct.
     *
     * \param object Object to be destroyed
     */
    void destroy(const UserObject& object) const;

    /**
     * \brief Start visitation of a class
     *
     * \param visitor Visitor to use for visitation
     */
    void visit(ClassVisitor& visitor) const;

    /**
     * \brief Convert a pointer to an object to be compatible with a base or derived metaclass
     *
     * The target metaclass may be a base or a derived of this, both cases are properly handled.
     *
     * \param pointer Pointer to convert
     * \param target Target metaclass to convert to
     *
     * \return Converted pointer
     *
     * \throw ClassUnrelated \a target is not a base nor a derived class of this
     */
    void* applyOffset(void* pointer, const Class& target) const;

    /**
     * \brief Operator == to check equality between two metaclasses
     *
     * Two metaclasses are equal if their name is the same.
     *
     * \param other Metaclass to compare with this
     *
     * \return True if both metaclasses are the same, false otherwise
     */
    bool operator==(const Class& other) const;

    /**
     * \brief Operator != to check inequality between two metaclasses
     *
     * \param other Metaclass to compare with this
     *
     * \return True if metaclasses are different, false if they are equal
     */
     bool operator!=(const Class& other) const;

private:

    template <typename T> friend class ClassBuilder;
    friend class detail::ClassManager;

    /**
     * \brief Construct the metaclass from its name
     *
     * \param name Name of the metaclass
     */
    Class(const std::string& name);

    /**
     * \brief Get the offset of a base metaclass
     *
     * \param base Base metaclass to check
     *
     * \return offset between this and base, or -1 if both classes are unrelated
     */
    int baseOffset(const Class& base) const;

private:

    /**
     * \brief Structure holding informations about a base metaclass
     */
    struct BaseInfo
    {
        const Class* base;
        int offset;
    };

    typedef std::shared_ptr<Property> PropertyPtr;
    typedef std::shared_ptr<Function> FunctionPtr;
    typedef std::shared_ptr<Constructor> ConstructorPtr;

    struct PropertyCmp {
        bool operator () (const std::string& a, const std::string& b) const {
            return a < b;
        }
    };

    struct FunctionCmp {
        bool operator () (const std::string& a, const std::string& b) const {
            return a < b;
        }
    };

    typedef std::vector<ConstructorPtr> ConstructorList;
    typedef std::vector<BaseInfo> BaseList;
    typedef detail::Dictionary<std::string, PropertyPtr, PropertyCmp> PropertyTable;
    typedef detail::Dictionary<std::string, FunctionPtr, FunctionCmp> FunctionTable;
    typedef void (*Destructor)(const UserObject&);

    std::string m_id;           ///< Name of the metaclass
    FunctionTable m_functions;  ///< Table of metafunctions indexed by ID
    PropertyTable m_properties; ///< Table of metaproperties indexed by ID
    BaseList m_bases; ///< List of base metaclasses
    ConstructorList m_constructors; ///< List of metaconstructors
    Destructor m_destructor; ///< Destructor (function that is able to delete an abstract object)
};

} // namespace ponder

#include <ponder/class.inl>


#endif // PONDER_CLASS_HPP
