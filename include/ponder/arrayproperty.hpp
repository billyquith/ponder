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


#ifndef PONDER_ARRAYPROPERTY_HPP
#define PONDER_ARRAYPROPERTY_HPP


#include <ponder/property.hpp>


namespace ponder
{
/**
 * \brief Specialized type of property for arrays
 *
 */
class PONDER_API ArrayProperty : public Property
{
public:

    /**
     * \brief Construct the property from its description
     *
     * \param name Name of the property
     * \param elementType Type of the property
     * \param dynamic Tells if the array is dynamic or not
     */
    ArrayProperty(const std::string& name, Type elementType, bool dynamic);

    /**
     * \brief Destructor
     */
    virtual ~ArrayProperty();

    /**
     * \brief Get the type of the array elements
     *
     * \return Type of elements
     */
    Type elementType() const;

    /**
     * \brief Check if the array is dynamic
     *
     * If the array is not dynamic, the insert and remove functions
     * are disabled.
     *
     * \return True if the array is dynamic, false otherwise
     */
    bool dynamic() const;

    /**
     * \brief Get the current size of the array
     *
     * \param object Object
     *
     * \return Number of elements in the array
     *
     * \throw NullObject object is invalid
     * \throw ForbiddenRead property is not readable
     */
    std::size_t size(const UserObject& object) const;

    /**
     * \brief Resize the array
     *
     * If \a size is lesser than the current size of the array,
     * the last elements will be removed; if \a size is greater
     * than the current size of the array, default-constructed
     * elements will be added at the end.
     *
     * This function will throw an error if the array is not dynamic
     *
     * \param object Object
     * \param newSize New size of the array
     *
     * \throw InvalidObject object has an invalid value
     * \throw InvalidAccess array is not writable or not dynamic
     * \throw InvalidIndex index is out of range
     */
    void resize(const UserObject& object, std::size_t newSize) const;

    /**
     * \brief Get an element of the array for a given object
     *
     * \param object Object
     * \param index Index of the element to get
     *
     * \return Value of the index-th element
     *
     * \throw NullObject object is invalid
     * \throw ForbiddenRead property is not readable
     * \throw OutOfRange index is out of range
     */
    Value get(const UserObject& object, std::size_t index) const;

    /**
     * \brief Set an element of the array for a given object
     *
     * \param object Object
     * \param index Index of the element to get
     * \param value New value to assign to the element
     *
     * \throw NullObject object is invalid
     * \throw ForbiddenWrite property is not writable
     * \throw BadType \a value can't be converted to the property's type
     * \throw OutOfRange index is out of range
     */
    void set(const UserObject& object, std::size_t index, const Value& value) const;

    /**
     * \brief Insert a new element into the array
     *
     * This function will throw an error if the array is not dynamic
     *
     * \param object Object
     * \param before Index before which the new element will be inserted
     * \param value Value of the element to insert
     *
     * \throw NullObject object is invalid
     * \throw ForbiddenWrite array is not writable or not dynamic
     * \throw BadType \a value can't be converted to the property's type
     * \throw OutOfRange index is out of range
     */
    void insert(const UserObject& object, std::size_t before, const Value& value) const;

    /**
     * \brief Remove an element from the array
     *
     * This function will throw an error if the array is not dynamic
     *
     * \param object Object
     * \param index Index of the element to remove
     *
     * \throw NullObject object is invalid
     * \throw ForbiddenWrite array is not writable or not dynamic
     * \throw OutOfRange index is out of range
     */
    void remove(const UserObject& object, std::size_t index) const;

    /**
     * \brief Accept the visitation of a ClassVisitor
     *
     * \param visitor Visitor to accept
     */
    virtual void accept(ClassVisitor& visitor) const override;

protected:

    /**
     * \see Property::getValue
     */
    virtual Value getValue(const UserObject& object) const override;

    /**
     * \see Property::setValue
     */
    virtual void setValue(const UserObject& object, const Value& value) const override;

    /**
     * \brief Do the actual retrieval of the size
     *
     * \param object Object
     *
     * \return Size of the array
     */
    virtual std::size_t getSize(const UserObject& object) const = 0;

    /**
     * \brief Do the actual resize of the array
     *
     * \param object Object
     * \param size New size of the array
     */
    virtual void setSize(const UserObject& object, std::size_t size) const = 0;

    /**
     * \brief Do the actual reading of an element
     *
     * This function is a pure virtual which has to be implemented in derived classes
     *
     * \param object Object
     * \param index Index of the element to get
     *
     * \return Value of the index-th element
     */
    virtual Value getElement(const UserObject& object, std::size_t index) const = 0;

    /**
     * \brief Do the actual writing of an element
     *
     * This function is a pure virtual which has to be implemented in derived classes
     *
     * \param object Object
     * \param index Index of the element to get
     * \param value New value to assign to the element
     */
    virtual void setElement(const UserObject& object, std::size_t index, const Value& value) const = 0;

    /**
     * \brief Do the actual insertion of an element
     *
     * This function is a pure virtual which has to be implemented in derived classes
     *
     * \param object Object
     * \param before Index before which the new element will be inserted
     * \param value Value of the element to insert
     */
    virtual void insertElement(const UserObject& object, std::size_t before, const Value& value) const = 0;

    /**
     * \brief Do the actual removal of an element
     *
     * This function is a pure virtual which has to be implemented in derived classes
     *
     * \param object Object
     * \param index Index of the element to remove
     */
    virtual void removeElement(const UserObject& object, std::size_t index) const = 0;

private:

    Type m_elementType; ///< Type of the individual elements of the array
    bool m_dynamic; ///< Is the array dynamic?
};

} // namespace ponder


#endif // PONDER_ARRAYPROPERTY_HPP
