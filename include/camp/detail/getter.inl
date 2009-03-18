// <insert copyright>


namespace camp
{
namespace detail
{
//-------------------------------------------------------------------------------------------------
template <typename T>
GetterInterface<T>::~GetterInterface()
{
}

//-------------------------------------------------------------------------------------------------
template <typename T, typename C>
GetterImpl<T, C>::GetterImpl(boost::function<T (C&)> function)
    : m_function(function)
{
}

//-------------------------------------------------------------------------------------------------
template <typename T, typename C>
T GetterImpl<T, C>::get(const UserObject& object) const
{
    return m_function(object.get<C>());
}

//-------------------------------------------------------------------------------------------------
template <typename T>
Getter<T>::Getter(const T& defaultValue)
    : m_getter()
    , m_defaultValue(defaultValue)
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename C>
Getter<T>::Getter(boost::function<T (C&)> function)
    : m_getter(new GetterImpl<T, C>(function))
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
const T& Getter<T>::get() const
{
    return m_defaultValue;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
T Getter<T>::get(const UserObject& object) const
{
    return m_getter ? m_getter->get(object) : m_defaultValue;
}

} // namespace detail

} // namespace camp
