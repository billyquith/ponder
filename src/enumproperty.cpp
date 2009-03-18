// <insert copyright>

#include <camp/enumproperty.hpp>
#include <camp/classvisitor.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
EnumProperty::EnumProperty(const std::string& name, const Enum& propEnum)
    : Property(name, enumType)
    , m_enum(&propEnum)
{
}

//-------------------------------------------------------------------------------------------------
EnumProperty::~EnumProperty()
{
}

//-------------------------------------------------------------------------------------------------
const Enum& EnumProperty::getEnum() const
{
    return *m_enum;
}

//-------------------------------------------------------------------------------------------------
void EnumProperty::accept(ClassVisitor& visitor) const
{
    visitor.visit(*this);
}

} // namespace camp
