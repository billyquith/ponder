// <insert copyright>


namespace camp
{
namespace detail
{
//-------------------------------------------------------------------------------------------------
template <typename T>
ObjectHolderByRef<T>::ObjectHolderByRef(const T* object)
    : m_object(const_cast<T*>(object))
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
void* ObjectHolderByRef<T>::object()
{
    return m_object;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ObjectHolderByCopy<T>::ObjectHolderByCopy(const T* object)
    : m_object(*object)
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
void* ObjectHolderByCopy<T>::object()
{
    return &m_object;
}

} // namespace detail

} // namespace camp
