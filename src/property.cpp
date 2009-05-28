// <insert copyright>

#include <camp/property.hpp>
#include <camp/invalidaccess.hpp>
#include <camp/classvisitor.hpp>
#include <camp/userobject.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
Property::~Property()
{
}

//-------------------------------------------------------------------------------------------------
const std::string& Property::name() const
{
    return m_name;
}

//-------------------------------------------------------------------------------------------------
Type Property::type() const
{
    return m_type;
}

//-------------------------------------------------------------------------------------------------
bool Property::readable(const UserObject& object) const
{
    return isReadable() && m_readable.get(object);
}

//-------------------------------------------------------------------------------------------------
bool Property::writable(const UserObject& object) const
{
    return isWritable() && m_writable.get(object);
}

//-------------------------------------------------------------------------------------------------
Value Property::get(const UserObject& object) const
{
    // Check if the property is readable
    if (!readable(object))
        CAMP_ERROR(InvalidAccess(m_name.c_str(), InvalidAccess::Read));

    return getValue(object);
}

//-------------------------------------------------------------------------------------------------
void Property::set(const UserObject& object, const Value& value) const
{
    // Check if the property is writable
    if (!writable(object))
        CAMP_ERROR(InvalidAccess(m_name.c_str(), InvalidAccess::Write));

    // Here we don't call setValue directly, we rather let the user object do it
    // and add any processing needed for proper propagation of the modification
    object.set(*this, value);
}

//-------------------------------------------------------------------------------------------------
void Property::accept(ClassVisitor& visitor) const
{
    visitor.visit(*this);
}

//-------------------------------------------------------------------------------------------------
bool Property::isReadable() const
{
    return true;
}

//-------------------------------------------------------------------------------------------------
bool Property::isWritable() const
{
    return true;
}

//-------------------------------------------------------------------------------------------------
Property::Property(const std::string& name, Type type)
    : m_name(name)
    , m_type(type)
    , m_readable(true)
    , m_writable(true)
{
}

} // namespace camp
