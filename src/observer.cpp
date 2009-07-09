

#include <camp/observer.hpp>
#include <camp/detail/classmanager.hpp>
#include <camp/detail/enummanager.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
Observer::~Observer()
{
    // Nothing to do
}

//-------------------------------------------------------------------------------------------------
void Observer::classAdded(const Class&)
{
    // Default implementation does nothing
}

//-------------------------------------------------------------------------------------------------
void Observer::classRemoved(const Class&)
{
    // Default implementation does nothing
}

//-------------------------------------------------------------------------------------------------
void Observer::enumAdded(const Enum&)
{
    // Default implementation does nothing
}

//-------------------------------------------------------------------------------------------------
void Observer::enumRemoved(const Enum&)
{
    // Default implementation does nothing
}

//-------------------------------------------------------------------------------------------------
Observer::Observer()
{
    // Nothing to do
}

//-------------------------------------------------------------------------------------------------
CAMP_API void addObserver(Observer* observer)
{
    detail::ClassManager::instance().addObserver(observer);
    detail::EnumManager::instance().addObserver(observer);
}

//-------------------------------------------------------------------------------------------------
CAMP_API void removeObserver(Observer* observer)
{
    detail::ClassManager::instance().removeObserver(observer);
    detail::EnumManager::instance().removeObserver(observer);
}

} // namespace camp
