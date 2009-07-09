


namespace camp
{
namespace detail
{
//-------------------------------------------------------------------------------------------------
template <typename A>
SimplePropertyImpl<A>::SimplePropertyImpl(const std::string& name, const A& accessor)
    : SimpleProperty(name, mapType<typename A::DataType>())
    , m_accessor(accessor)
{
}

//-------------------------------------------------------------------------------------------------
template <typename A>
Value SimplePropertyImpl<A>::getValue(const UserObject& object) const
{
    return m_accessor.get(object.get<typename A::ClassType>());
}

//-------------------------------------------------------------------------------------------------
template <typename A>
void SimplePropertyImpl<A>::setValue(const UserObject& object, const Value& value) const
{
    if (!m_accessor.set(object.get<typename A::ClassType>(), value))
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Write));
}

//-------------------------------------------------------------------------------------------------
template <typename A>
bool SimplePropertyImpl<A>::isReadable() const
{
    return A::canRead;
}

//-------------------------------------------------------------------------------------------------
template <typename A>
bool SimplePropertyImpl<A>::isWritable() const
{
    return A::canWrite;
}

} // namespace detail

} // namespace camp
