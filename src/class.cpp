// <insert copyright>

#include <camp/class.hpp>
#include <camp/invalidindex.hpp>
#include <camp/invalidfunction.hpp>
#include <camp/invalidproperty.hpp>




//-------------------------------------------------------------------------------------------------
/*#error Système générique de proxy (voir ci-dessous)
--> pas inclus dans CAMP, complètement externe (tester quand même)

declareProxyClass<MyProxy>("MyDynamicClass")
    .property("p1", camp::string)
    .property("p2", camp::integer)
    .function("f1", camp::integer, camp::boolean, camp::string)
    .function("f2", camp::noType);

class MyProxy
{
    camp::Value get(const std::string& className, const std::string& propName, camp::Type type)
    {

    }

    void set(const std::string& className, const std::string& propName, camp::Type type, camp::Value value)
    {

    }

    camp::Value call(const std::string& className, const std::string& funcName, vector<camp::Type> types, vector<camp::Value> args)
    {

    }
};*/
//-------------------------------------------------------------------------------------------------




namespace camp
{
//-------------------------------------------------------------------------------------------------
const std::string& Class::name() const
{
    return m_name;
}

//-------------------------------------------------------------------------------------------------
std::size_t Class::baseCount() const
{
    return m_bases.size();
}

//-------------------------------------------------------------------------------------------------
const Class& Class::base(std::size_t index) const
{
    if (index >= m_bases.size())
        CAMP_ERROR(InvalidIndex(index, m_bases.size()));

    return *m_bases[index].base;
}

//-------------------------------------------------------------------------------------------------
std::size_t Class::functionCount() const
{
    return m_functions.size();
}

//-------------------------------------------------------------------------------------------------
bool Class::hasFunction(const std::string& name) const
{
    return m_functions.find(name) != m_functions.end();
}

//-------------------------------------------------------------------------------------------------
const Function& Class::function(std::size_t index) const
{
    if (index >= m_functions.size())
        CAMP_ERROR(InvalidIndex(index, m_functions.size()));

    FunctionTable::const_iterator it = m_functions.begin();
    std::advance(it, index);

    return *it->second;
}

//-------------------------------------------------------------------------------------------------
const Function& Class::function(const std::string& name) const
{
    FunctionTable::const_iterator it = m_functions.find(name);
    if (it == m_functions.end())
        CAMP_ERROR(InvalidFunction(name.c_str(), *this));

    return *it->second;
}

//-------------------------------------------------------------------------------------------------
std::size_t Class::propertyCount() const
{
    return m_properties.size();
}

//-------------------------------------------------------------------------------------------------
bool Class::hasProperty(const std::string& name) const
{
    return m_properties.find(name) != m_properties.end();
}

//-------------------------------------------------------------------------------------------------
const Property& Class::property(std::size_t index) const
{
    if (index >= m_properties.size())
        CAMP_ERROR(InvalidIndex(index, m_properties.size()));

    PropertyTable::const_iterator it = m_properties.begin();
    std::advance(it, index);

    return *it->second;
}

//-------------------------------------------------------------------------------------------------
const Property& Class::property(const std::string& name) const
{
    PropertyTable::const_iterator it = m_properties.find(name);
    if (it == m_properties.end())
        CAMP_ERROR(InvalidProperty(name.c_str(), *this));

    return *it->second;
}

//-------------------------------------------------------------------------------------------------
void Class::visit(ClassVisitor& visitor) const
{
    // First visit properties
    PropertyTable::const_iterator propEnd = m_properties.end();
    for (PropertyTable::const_iterator it = m_properties.begin(); it != propEnd; ++it)
    {
        it->second->accept(visitor);
    }

    // Then visit functions
    FunctionTable::const_iterator funcEnd = m_functions.end();
    for (FunctionTable::const_iterator it = m_functions.begin(); it != funcEnd; ++it)
    {
        it->second->accept(visitor);
    }
}

//-------------------------------------------------------------------------------------------------
bool Class::operator==(const Class& other) const
{
    return m_name == other.m_name;
}

//-------------------------------------------------------------------------------------------------
bool Class::operator!=(const Class& other) const
{
    return m_name != other.m_name;
}

//-------------------------------------------------------------------------------------------------
Class::Class(const std::string& name)
    : m_name(name)
{
}

//-------------------------------------------------------------------------------------------------
bool Class::applyOffset(void*& pointer, const Class& base) const
{
    // Check self
    if (&base == this)
        return true;

    // Check base classes
    std::vector<BaseInfo>::const_iterator end = m_bases.end();
    for (std::vector<BaseInfo>::const_iterator it = m_bases.begin(); it != end;  ++it)
    {
        if (it->base == &base)
        {
            // Special case for null pointers: don't apply offset to let them be null
            if (pointer)
                pointer = static_cast<void*>(static_cast<char*>(pointer) + it->offset);
            return true;
        }
        else if (it->base->applyOffset(pointer, base))
        {
            return true;
        }
    }

    // No match found for base class...
    return false;
}

} // namespace camp
