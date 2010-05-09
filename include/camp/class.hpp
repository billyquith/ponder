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


#ifndef CAMP_CLASS_HPP
#define CAMP_CLASS_HPP


#include <camp/config.hpp>
#include <camp/args.hpp>
#include <camp/error.hpp>
#include <camp/constructornotfound.hpp>
#include <camp/tagholder.hpp>
#include <camp/detail/classmanager.hpp>
#include <camp/detail/typeid.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>


namespace camp
{
template <typename T> class ClassBuilder;
class UserObject;
class Property;
class Function;
class Constructor;
class Value;
class ClassVisitor;

/**
 * \brief camp::Class represents a metaclass composed of properties and functions
 *
 * camp::Class is the main class of the CAMP API. It defines a metaclass, which
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
 * camp::Class::declare<MyClass>("MyClass")
 *     .tag("help", "this is my class")
 *     .constructor()
 *     .property("prop", &MyClass::getProp, &MyClass::setProp)
 *     .function("func", &MyClass::func);
 * \endcode
 *
 * It then provides a set of accessors to retrieve its member functions and properties.
 *
 * \code
 * const camp::Class& metaclass = camp::classByType<MyClass>();
 *
 * const camp::Property& prop = metaclass.property("prop");
 * const camp::Function& func = metaclass.function("func");
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
class CAMP_API Class : public TagHolder, boost::noncopyable
{
public:

    /**
     * \brief Declare a new metaclass
     *
     * This is the function to call to create a new metaclass. The template
     * parameter T is the C++ class that will be bound to the metaclass.
     *
     * \param name Name of the metaclass in CAMP. This name identifies
     *             the metaclass and thus has to be unique
     *
     * \return A ClassBuilder<T> object that will provide functions
     *         to fill the new metaclass with properties, functions, etc.
     */
    template <typename T>
    static ClassBuilder<T> declare(const std::string& name);

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
     * \brief Construct a new instance of the C++ class bound to the metaclass
     *
     * The template parameter T is the target C++ type. It can be the class bound
     * to the metaclass, one of its base classes, or void. The returned instance
     * must be destroyed with the Class::destroy function.
     *
     * \param args Arguments to pass to the constructor (empty by default)
     *
     * \return Pointer to the new instance, or 0 if it failed
     *
     * \throw ConstructorNotFound no matching constructor was found
     * \throw ClassUnrelated T is not a base or derived class of this
     * \throw BadArgument one of the arguments can't be converted to the requested type
     */
    template <typename T>
    T* construct(const Args& args = Args::empty) const;

    /**
     * \brief Destroy an instance of the C++ class bound to the metaclass
     *
     * This function must be called to destroy every instance created with
     * Class::construct. The object to destroy must be properly typed
     * (i.e. not be void*) so that its destructor gets called
     *
     * \param object Pointer to the object to destroyed
     *
     * \return Pointer to the new instance, or 0 if it failed
     */
    template <typename T>
    void destroy(const T* object) const;

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

    typedef boost::shared_ptr<Property> PropertyPtr;
    typedef std::map<std::string, PropertyPtr> PropertyTable;
    typedef std::vector<PropertyPtr> PropertyArray;

    typedef boost::shared_ptr<Function> FunctionPtr;
    typedef std::map<std::string, FunctionPtr> FunctionTable;
    typedef std::vector<FunctionPtr> FunctionArray;

    typedef boost::shared_ptr<Constructor> ConstructorPtr;
    typedef std::vector<ConstructorPtr> ConstructorList;

    std::string m_name; ///< Name of the metaclass
    std::vector<BaseInfo> m_bases; ///< List of base metaclasses
    FunctionTable m_functions; ///< List of metafunctions sorted by name
    FunctionArray m_functionsByIndex; ///< List of metafunctions sorted by index
    PropertyTable m_properties; ///< List of metaproperties sorted by name
    PropertyArray m_propertiesByIndex; ///< List of metaproperties sorted by index
    ConstructorList m_constructors; ///< List of metaconstructors
};

} // namespace camp

// Must be included here because of mutual dependance between Class and ClassBuilder
#include <camp/classbuilder.hpp>

#include <camp/class.inl>


#endif // CAMP_CLASS_HPP
