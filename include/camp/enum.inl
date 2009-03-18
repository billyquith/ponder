// <insert copyright>


namespace camp
{
//-------------------------------------------------------------------------------------------------
template <typename T>
EnumBuilder Enum::declare(const std::string& name)
{
    Enum& newEnum = detail::EnumManager::instance().registerNew(name, detail::StaticTypeId<T>::get(false));
    return EnumBuilder(newEnum);
}

} // namespace camp
