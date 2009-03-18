// <insert copyright>


#include <camp/camptype.hpp>
#include <camp/detail/classmanager.hpp>
#include <camp/detail/enummanager.hpp>


namespace camp
{
namespace detail
{
//-------------------------------------------------------------------------------------------------
void ensureTypeRegistered(const char* id, void (*registerFunc)())
{
    if (registerFunc
        && !ClassManager::instance().classExists(id)
        && !EnumManager::instance().enumExists(id))
    {
        registerFunc();
    }
}

} // namespace detail

} // namespace camp
