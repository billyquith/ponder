// <insert copyright>


namespace camp
{
//-------------------------------------------------------------------------------------------------
template <typename T>
UserObject::UserObject(const T& object)
{
    const void* pointer = detail::ObjectTraits<const T&>::getPointer(object);
    m_object = const_cast<void*>(pointer);
    m_class = &classByObject(object);

    // --> if it is required to be able to store non-CAMP classes,
    // then we can just use classByTypeSafe and handle it properly in get()
}

//-------------------------------------------------------------------------------------------------
template <typename T>
typename detail::ObjectTraits<T>::RefReturnType UserObject::get() const
{
    // Make sure we have a valid class and pointer
    if (!m_class || !m_object)
        CAMP_ERROR(InvalidObject(*this));

    // Get the metaclass of T (we use classByTypeSafe because it may not exist)
    const Class* targetClass = classByTypeSafe<T>();
    if (!targetClass)
        CAMP_ERROR(InvalidObject(*this));

    // Apply the proper offset to the pointer (solves multiple inheritance issues)
    void* pointer = convertPtr(m_object, *targetClass);

    // Error, T is unrelated to the actual metaclass of the object
    if (!pointer)
        CAMP_ERROR(InvalidObject(*this));

    return detail::ObjectTraits<T>::get(pointer);
}

} // namespace camp
