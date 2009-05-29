// <insert copyright>


namespace camp
{
namespace detail
{
//-------------------------------------------------------------------------------------------------
inline AbstractObjectHolder::~AbstractObjectHolder()
{
}

//-------------------------------------------------------------------------------------------------
inline AbstractObjectHolder::AbstractObjectHolder()
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ObjectHolderByRef<T>::ObjectHolderByRef(const T* object)
    : m_object(const_cast<T*>(object))
    , m_alignedPtr(classCast(m_object, classByType<T>(), classByObject(object)))
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
void* ObjectHolderByRef<T>::object()
{
    return m_alignedPtr;
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
