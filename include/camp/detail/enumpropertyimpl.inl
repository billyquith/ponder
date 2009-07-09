


namespace camp
{
namespace detail
{
//-------------------------------------------------------------------------------------------------
template <typename A>
EnumPropertyImpl<A>::EnumPropertyImpl(const std::string& name, const A& accessor)
    : EnumProperty(name, enumByType<typename A::DataType>())
    , m_accessor(accessor)
{
}

//-------------------------------------------------------------------------------------------------
template <typename A>
Value EnumPropertyImpl<A>::getValue(const UserObject& object) const
{
    return m_accessor.get(object.get<typename A::ClassType>());
}

//-------------------------------------------------------------------------------------------------
template <typename A>
void EnumPropertyImpl<A>::setValue(const UserObject& object, const Value& value) const
{
    if (!m_accessor.set(object.get<typename A::ClassType>(), value))
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Write));
}

//-------------------------------------------------------------------------------------------------
template <typename A>
bool EnumPropertyImpl<A>::isReadable() const
{
    return A::canRead;
}

//-------------------------------------------------------------------------------------------------
template <typename A>
bool EnumPropertyImpl<A>::isWritable() const
{
    return A::canWrite;
}

} // namespace detail

} // namespace camp
