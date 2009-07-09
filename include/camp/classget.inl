


namespace camp
{
//-------------------------------------------------------------------------------------------------
inline std::size_t classCount()
{
    return detail::ClassManager::instance().count();
}

//-------------------------------------------------------------------------------------------------
template <typename T>
std::size_t classCount()
{
    return detail::ClassManager::instance().count(detail::typeId<T>());
}

//-------------------------------------------------------------------------------------------------
template <typename T>
std::size_t classCount(const T& object)
{
    return detail::ClassManager::instance().count(detail::typeId(object));
}

//-------------------------------------------------------------------------------------------------
inline const Class& classByIndex(std::size_t index)
{
    return detail::ClassManager::instance().getByIndex(index);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
const Class& classByObject(const T& object, std::size_t index)
{
    return detail::ClassManager::instance().getById(detail::typeId(object), index);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
const Class& classByType(std::size_t index)
{
    return detail::ClassManager::instance().getById(detail::typeId<T>(), index);
}

//-------------------------------------------------------------------------------------------------
inline const Class& classByName(const std::string& name)
{
    return detail::ClassManager::instance().getByName(name);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
const Class* classByTypeSafe(std::size_t index)
{
    try
    {
        return &detail::ClassManager::instance().getById(detail::safeTypeId<T>(), index);
    }
    catch (Error&)
    {
        return 0;
    }
}

} // namespace camp
