


namespace camp
{
//-------------------------------------------------------------------------------------------------
inline std::size_t enumCount()
{
    return detail::EnumManager::instance().count();
}

//-------------------------------------------------------------------------------------------------
template <typename T>
std::size_t enumCount()
{
    return detail::EnumManager::instance().count(detail::typeId<T>());
}

//-------------------------------------------------------------------------------------------------
template <typename T>
std::size_t enumCount(T value)
{
    return detail::EnumManager::instance().count(detail::typeId(value));
}

//-------------------------------------------------------------------------------------------------
inline const Enum& enumByIndex(std::size_t index)
{
    return detail::EnumManager::instance().getByIndex(index);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
const Enum& enumByType(std::size_t index)
{
    return detail::EnumManager::instance().getById(detail::typeId<T>(), index);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
const Enum& enumByObject(T value, std::size_t index)
{
    return detail::EnumManager::instance().getById(detail::typeId(value), index);
}

//-------------------------------------------------------------------------------------------------
inline const Enum& enumByName(const std::string& name)
{
    return detail::EnumManager::instance().getByName(name);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
const Enum* enumByTypeSafe(std::size_t index)
{
    try
    {
        return &detail::EnumManager::instance().getById(detail::safeTypeId<T>(), index);
    }
    catch (Error&)
    {
        return 0;
    }
}

} // namespace camp
