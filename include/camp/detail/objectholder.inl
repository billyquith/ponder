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
inline void AbstractObjectHolder::updateObject()
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
    , m_class(&classByObject(object))
    , m_alignedPtr(classCast(m_object, classByType<T>(), *m_class))
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
AbstractObjectHolder* ObjectHolderByRef<T>::clone() const
{
    return new ObjectHolderByRef<T>(*this);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
void* ObjectHolderByRef<T>::object()
{
    return m_alignedPtr;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
const Class& ObjectHolderByRef<T>::getClass() const
{
    return *m_class;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ObjectHolderByCopy<T>::ObjectHolderByCopy(const T* object)
    : m_object(*object)
    , m_class(&classByType<T>())
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
AbstractObjectHolder* ObjectHolderByCopy<T>::clone() const
{
    return new ObjectHolderByCopy<T>(*this);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
void* ObjectHolderByCopy<T>::object()
{
    return &m_object;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
const Class& ObjectHolderByCopy<T>::getClass() const
{
    return *m_class;
}

} // namespace detail

} // namespace camp
