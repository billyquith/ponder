// <insert copyright>

#include <camp/detail/observernotifier.hpp>
#include <camp/observer.hpp>
#include <cassert>


namespace camp
{
namespace detail
{
//-------------------------------------------------------------------------------------------------
void ObserverNotifier::addObserver(Observer* observer)
{
    assert(observer != 0);

    m_observers.insert(observer);
}

//-------------------------------------------------------------------------------------------------
void ObserverNotifier::removeObserver(Observer* observer)
{
    m_observers.erase(observer);
}

//-------------------------------------------------------------------------------------------------
ObserverNotifier::ObserverNotifier()
{
}

//-------------------------------------------------------------------------------------------------
ObserverNotifier::ObserverIterator ObserverNotifier::observersBegin() const
{
    return m_observers.begin();
}

//-------------------------------------------------------------------------------------------------
ObserverNotifier::ObserverIterator ObserverNotifier::observersEnd() const
{
    return m_observers.end();
}

} // namespace detail

} // namespace camp
