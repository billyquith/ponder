


namespace camp
{
//-------------------------------------------------------------------------------------------------
template <typename T>
EnumObject::EnumObject(T value, typename boost::enable_if<boost::is_enum<T> >::type*)
    : m_value(value)
    , m_enum(&enumByType<T>())
{
}

} // namespace camp
