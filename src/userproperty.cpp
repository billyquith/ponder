

#include <camp/userproperty.hpp>
#include <camp/classvisitor.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
UserProperty::UserProperty(const std::string& name, const Class& propClass)
    : Property(name, userType)
    , m_class(&propClass)
{
}

//-------------------------------------------------------------------------------------------------
UserProperty::~UserProperty()
{
}

//-------------------------------------------------------------------------------------------------
const Class& UserProperty::getClass() const
{
    return *m_class;
}

//-------------------------------------------------------------------------------------------------
void UserProperty::accept(ClassVisitor& visitor) const
{
    visitor.visit(*this);
}

} // namespace camp
