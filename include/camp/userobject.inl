// <insert copyright>


namespace camp
{
//-------------------------------------------------------------------------------------------------
template <typename T>
UserObject::UserObject(const T& object)
{
    typedef detail::ObjectTraits<const T&> Traits;
    typedef detail::ObjectHolderByRef<typename Traits::DataType> Holder;

    // Create the holder from the object
    m_holder.reset(new Holder(Traits::getPointer(object)));

    // Get the actual metaclass of the object (i.e. its dynamic one)
    m_class = &classByObject(object);

    // Get a direct pointer to the stored object, and apply conversion if necessary
    // in order to get a pointer to the actual derived type (in case of multiple inheritance with offset)
    m_pointer = convertPtr(m_holder->object(), classByType<T>(), *m_class);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
typename detail::ObjectTraits<T>::RefReturnType UserObject::get() const
{
    // Make sure we have a valid class and pointer
    if (!m_class || !m_holder)
        CAMP_ERROR(InvalidObject(*this));

    // Get the metaclass of T (we use classByTypeSafe because it may not exist)
    const Class* targetClass = classByTypeSafe<T>();
    if (!targetClass)
        CAMP_ERROR(InvalidObject(*this));

    // Apply the proper offset to the pointer (solves multiple inheritance issues)
    void* pointer = convertPtr(m_pointer, *m_class, *targetClass);

    // Error, T is unrelated to the actual metaclass of the object
    if (!pointer)
        CAMP_ERROR(InvalidObject(*this));

    return detail::ObjectTraits<T>::get(pointer);
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

    // Create the holder from the object
    userObject.m_holder.reset(new Holder(Traits::getPointer(object)));

    // Get the metaclass of the object
    // IMPORTANT: here we retrieve the *static* metaclass of the object, because it is stored by copy as a T.
    // Having a polymorphic object here would be an error anyway.
    userObject.m_class = &classByType<T>();

    // No conversion needed for the direct pointer, the dynamic metaclass is the same as the static one
    userObject.m_pointer = userObject.m_holder->object();

    return userObject;
}

} // namespace camp
