// <insert copyright>

#include <camp/arrayproperty.hpp>
#include <camp/classvisitor.hpp>
#include <camp/invalidaccess.hpp>
#include <camp/invalidobject.hpp>
#include <camp/invalidvalue.hpp>
#include <camp/invalidindex.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
ArrayProperty::ArrayProperty(const std::string& name, Type elementType, bool dynamic)
    : Property(name, arrayType)
    , m_elementType(elementType)
    , m_dynamic(dynamic)
{
}

//-------------------------------------------------------------------------------------------------
ArrayProperty::~ArrayProperty()
{
}

//-------------------------------------------------------------------------------------------------
Type ArrayProperty::elementType() const
{
    return m_elementType;
}

//-------------------------------------------------------------------------------------------------
bool ArrayProperty::dynamic() const
{
    return m_dynamic;
}

//-------------------------------------------------------------------------------------------------
std::size_t ArrayProperty::size(const UserObject& object) const
{
    // Check if the property is readable
    if (!readable(object))
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Read));

    return getSize(object);
}

//-------------------------------------------------------------------------------------------------
Value ArrayProperty::get(const UserObject& object, std::size_t index) const
{
    // Check if the property is readable
    if (!readable(object))
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Read));

    // Check if the index is in range
    std::size_t range = size(object);
    if (index >= range)
        CAMP_ERROR(InvalidIndex(index, range));

    return getElement(object, index);
}

//-------------------------------------------------------------------------------------------------
void ArrayProperty::set(const UserObject& object, std::size_t index, const Value& value) const
{
    // Check if the property is writable
    if (!writable(object))
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Write));

    // Check if the index is in range
    std::size_t range = size(object);
    if (index >= range)
        CAMP_ERROR(InvalidIndex(index, range));

    return setElement(object, index, value);
}

//-------------------------------------------------------------------------------------------------
void ArrayProperty::insert(const UserObject& object, std::size_t before, const Value& value) const
{
    // Check if the array is dynamic
    if (!dynamic())
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Write));

    // Check if the property is writable
    if (!writable(object))
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Write));

    // Check if the index is in range
    std::size_t range = size(object) + 1;
    if (before >= range)
        CAMP_ERROR(InvalidIndex(before, range));

    return insertElement(object, before, value);
}

//-------------------------------------------------------------------------------------------------
void ArrayProperty::remove(const UserObject& object, std::size_t index) const
{
    // Check if the array is dynamic
    if (!dynamic())
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Write));

    // Check if the property is writable
    if (!writable(object))
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Write));

    // Check if the index is in range
    std::size_t range = size(object);
    if (index >= range)
        CAMP_ERROR(InvalidIndex(index, range));

    return removeElement(object, index);
}

//-------------------------------------------------------------------------------------------------
void ArrayProperty::accept(ClassVisitor& visitor) const
{
    visitor.visit(*this);
}

//-------------------------------------------------------------------------------------------------
Value ArrayProperty::getValue(const UserObject& object) const
{
    // Return first element
    return get(object, 0);
}

//-------------------------------------------------------------------------------------------------
void ArrayProperty::setValue(const UserObject& object, const Value& value) const
{
    // Set first element
    set(object, 0, value);
}

} // namespace camp
