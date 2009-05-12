// <insert copyright>


namespace camp
{
namespace detail
{
/**
 * \brief Helper structure to construct a UserObject according
 *        to whether a type is a reference type or not
 *
 * The generic version assumes a non-reference type and stores
 * the object by copy.
 */
template <bool IsRef>
struct ToUserObject
{
    template <typename T>
    static UserObject get(const T& value)
    {
        return UserObject::copy(value);
    }
};

/**
 * \brief Specialization of ToUserObject for reference types
 *
 * This version stores the object by reference (no copy).
 */
template <>
struct ToUserObject<true>
{
    template <typename T>
    static UserObject get(const T& value)
    {
        return UserObject::ref(value);
    }
};

//-------------------------------------------------------------------------------------------------
template <typename A>
UserPropertyImpl<A>::UserPropertyImpl(const std::string& name, const A& accessor)
    : UserProperty(name, classByType<typename A::DataType>())
    , m_accessor(accessor)
{
}

//-------------------------------------------------------------------------------------------------
template <typename A>
Value UserPropertyImpl<A>::getValue(const UserObject& object) const
{
    return ToUserObject<A::Traits::isRef>::get(m_accessor.get(object.get<typename A::ClassType>()));
}

//-------------------------------------------------------------------------------------------------
template <typename A>
void UserPropertyImpl<A>::setValue(const UserObject& object, const Value& value) const
{
    if (!m_accessor.set(object.get<typename A::ClassType>(), value))
        CAMP_ERROR(InvalidAccess(name().c_str(), InvalidAccess::Write));
}

//-------------------------------------------------------------------------------------------------
template <typename A>
bool UserPropertyImpl<A>::isReadable() const
{
    return A::canRead;
}

//-------------------------------------------------------------------------------------------------
template <typename A>
bool UserPropertyImpl<A>::isWritable() const
{
    return A::canWrite;
}

} // namespace detail

} // namespace camp
