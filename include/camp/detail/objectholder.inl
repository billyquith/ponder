


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
ObjectHolderByConstRef<T>::ObjectHolderByConstRef(const T* object)
    : m_object(object)
    , m_alignedPtr(classCast(const_cast<T*>(object), classByType<T>(), classByObject(object)))
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
void* ObjectHolderByConstRef<T>::object()
{
    return m_alignedPtr;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
AbstractObjectHolder* ObjectHolderByConstRef<T>::getWritable()
{
    // We hold a read-only object: return a holder which stores a copy of it
    return new ObjectHolderByCopy<T>(m_object);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ObjectHolderByRef<T>::ObjectHolderByRef(T* object)
    : m_object(object)
    , m_alignedPtr(classCast(object, classByType<T>(), classByObject(object)))
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
AbstractObjectHolder* ObjectHolderByRef<T>::getWritable()
{
    // We already store a modifiable object
    return this;
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

//-------------------------------------------------------------------------------------------------
template <typename T>
AbstractObjectHolder* ObjectHolderByCopy<T>::getWritable()
{
    // We already store a modifiable object
    return this;
}

} // namespace detail

} // namespace camp
