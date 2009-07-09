


namespace camp
{
//-------------------------------------------------------------------------------------------------
template <typename T>
ClassBuilder<T> Class::declare(const std::string& name)
{
    Class& newClass = detail::ClassManager::instance().registerNew(name, detail::StaticTypeId<T>::get(false));
    return ClassBuilder<T>(newClass);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
T* Class::construct(const Args& args) const
{
    void* object = 0;

    // Search an args match among the list of available constructors
    ConstructorList::const_iterator end = m_constructors.end();
    for (ConstructorList::const_iterator it = m_constructors.begin();
         (it != end) && !object;
         ++it)
    {
        Constructor& constructor = **it;

        if (constructor.matches(args))
        {
            // Match found: use the constructor to create the new instance
            object = constructor.create(args);
        }
    }

    if (!object)
    {
        // Error: no constructor found for the given list of arguments
        // @todo CAMP_ERROR(InvalidConstructor(args));
        return 0;
    }

    // Get the metaclass of T, if any
    const Class* targetClass = classByTypeSafe<T>();

    // Apply the proper offset in case T is a base of this class
    if (!targetClass || applyOffset(object, *targetClass))
    {
        return static_cast<T*>(object);
    }
    else
    {
        // Error: T is unrelated to this class
        // @todo CAMP_ERROR(InvalidConstructor(args));
        return 0;
    }
}

//-------------------------------------------------------------------------------------------------
template <typename T>
void Class::destroy(const T* object) const
{
    delete object;
}

} // namespace camp
