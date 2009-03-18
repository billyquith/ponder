// <insert copyright>

#include <camp/userobject.hpp>
#include <camp/class.hpp>


namespace camp
{
const UserObject UserObject::nothing;

//-------------------------------------------------------------------------------------------------
UserObject::UserObject()
    : m_object(0)
    , m_class(0)
{
}

//-------------------------------------------------------------------------------------------------
UserObject::UserObject(const UserObject& copy)
    : m_object(copy.m_object)
    , m_class(copy.m_class)
{
}

//-------------------------------------------------------------------------------------------------
void* UserObject::pointer() const
{
    return m_object;
}

//-------------------------------------------------------------------------------------------------
const Class& UserObject::getClass() const
{
    if (!m_class)
        CAMP_ERROR(InvalidObject(*this));

    return *m_class;
}

//-------------------------------------------------------------------------------------------------
bool UserObject::operator==(const UserObject& other) const
{
    return (m_class == other.m_class) && (m_object == other.m_object);
}

//-------------------------------------------------------------------------------------------------
bool UserObject::operator<(const UserObject& other) const
{
    if (m_class != other.m_class)
    {
        return m_class < other.m_class;
    }
    else
    {
        return m_object < other.m_object;
    }
}

//-------------------------------------------------------------------------------------------------
void* UserObject::convertPtr(void* pointer, const Class& targetClass) const
{
    // Note: this function exists only to hide the usage of camp::Class to a separate compile unit;
    //       including <camp/class.hpp> in our header would cause a cycle in inclusions.

    bool ok = m_class->applyOffset(pointer, targetClass);
    return ok ? pointer : 0;
}

} // namespace camp
