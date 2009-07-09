

#include <camp/simpleproperty.hpp>
#include <camp/classvisitor.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
SimpleProperty::SimpleProperty(const std::string& name, Type type)
    : Property(name, type)
{
}

//-------------------------------------------------------------------------------------------------
SimpleProperty::~SimpleProperty()
{
}

//-------------------------------------------------------------------------------------------------
void SimpleProperty::accept(ClassVisitor& visitor) const
{
    visitor.visit(*this);
}

} // namespace camp
