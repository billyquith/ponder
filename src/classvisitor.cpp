// <insert copyright>

#include <camp/classvisitor.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
ClassVisitor::~ClassVisitor()
{
    // Nothing to do
}

//-------------------------------------------------------------------------------------------------
void ClassVisitor::visit(const Property&)
{
    // The default implementation does nothing
}

//-------------------------------------------------------------------------------------------------
void ClassVisitor::visit(const SimpleProperty&)
{
    // The default implementation does nothing
}

//-------------------------------------------------------------------------------------------------
void ClassVisitor::visit(const ArrayProperty&)
{
    // The default implementation does nothing
}

//-------------------------------------------------------------------------------------------------
void ClassVisitor::visit(const EnumProperty&)
{
    // The default implementation does nothing
}

//-------------------------------------------------------------------------------------------------
void ClassVisitor::visit(const UserProperty&)
{
    // The default implementation does nothing
}

//-------------------------------------------------------------------------------------------------
void ClassVisitor::visit(const Function&)
{
    // The default implementation does nothing
}

//-------------------------------------------------------------------------------------------------
ClassVisitor::ClassVisitor()
{
    // Nothing to do
}

} // namespace camp
