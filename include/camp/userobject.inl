// <insert copyright>


namespace camp
{
//-------------------------------------------------------------------------------------------------
template <typename T>
UserObject::UserObject(const T& object)
    : m_class(&classByObject(object))
    , m_holder()
    , m_parent()
    , m_child(0)
{
    typedef detail::ObjectTraits<const T&> Traits;
    typedef detail::ObjectHolderByRef<typename Traits::DataType> Holder;

    m_holder.reset(new Holder(Traits::getPointer(object)));
}

//-------------------------------------------------------------------------------------------------
template <typename T>
typename detail::ObjectTraits<T>::RefReturnType UserObject::get() const
{
    // Make sure we have a valid internal object
    void* ptr = pointer();
    if (!ptr)
        CAMP_ERROR(InvalidObject(*this));

    // Get the metaclass of T (we use classByTypeSafe because it may not exist)
    const Class* targetClass = classByTypeSafe<T>();
    if (!targetClass)
        CAMP_ERROR(InvalidObject(*this));

    // Apply the proper offset to the pointer (solves multiple inheritance issues)
    ptr = classCast(ptr, *m_class, *targetClass);

    // Check if the conversion was successful (i.e. if T is related to the actual metaclass of the object)
    if (!ptr)
        CAMP_ERROR(InvalidObject(*this));

    return detail::ObjectTraits<T>::get(ptr);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
UserObject UserObject::ref(const T& object)
{
    return UserObject(object);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
UserObject UserObject::copy(const T& object)
{
    typedef detail::ObjectTraits<const T&> Traits;
    typedef detail::ObjectHolderByCopy<typename Traits::DataType> Holder;

    UserObject userObject;
    userObject.m_class = &classByType<T>();
    userObject.m_holder.reset(new Holder(Traits::getPointer(object)));

    return userObject;
}

} // namespace camp
