

#include <camp/function.hpp>
#include <camp/classvisitor.hpp>
#include <camp/invalidindex.hpp>
#include <camp/invalidaccess.hpp>
#include <camp/invalidargument.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
Function::~Function()
{
}

//-------------------------------------------------------------------------------------------------
const std::string& Function::name() const
{
    return m_name;
}

//-------------------------------------------------------------------------------------------------
std::size_t Function::argCount() const
{
    return m_argTypes.size();
}

//-------------------------------------------------------------------------------------------------
Type Function::returnType() const
{
    return m_returnType;
}

//-------------------------------------------------------------------------------------------------
Type Function::argType(std::size_t index) const
{
    if (index >= m_argTypes.size())
        CAMP_ERROR(InvalidIndex(index, m_argTypes.size()));

    return m_argTypes[index];
}

//-------------------------------------------------------------------------------------------------
bool Function::callable(const UserObject& object) const
{
    return m_callable.get(object);
}

//-------------------------------------------------------------------------------------------------
Value Function::call(const UserObject& object, const Args& args) const
{
    // Check if the function is callable
    if (!callable(object))
        CAMP_ERROR(InvalidAccess(m_name.c_str(), InvalidAccess::Call));

    // Check the arguments count
    if (args.count() < m_argTypes.size())
        CAMP_ERROR(InvalidArgument(noType, m_argTypes[args.count()], m_name.c_str(), args.count()));

    // Execute the function
    return execute(object, args);
}

//-------------------------------------------------------------------------------------------------
void Function::accept(ClassVisitor& visitor) const
{
    visitor.visit(*this);
}

//-------------------------------------------------------------------------------------------------
Function::Function(const std::string& name, Type returnType, const std::vector<Type>& argTypes)
    : m_name(name)
    , m_returnType(returnType)
    , m_argTypes(argTypes)
    , m_callable(true)
{
}

} // namespace camp
