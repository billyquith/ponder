


namespace camp
{
namespace detail
{
/**
 * \brief Helper structure allowing a shortcut when converting a camp::Value to camp::Value
 */
template <typename T>
struct ValueTo
{
    static T convert(const Value& value) {return value.visit(ConvertVisitor<T>());}
};
template <>
struct ValueTo<Value>
{
    static Value convert(const Value& value) {return value;}
};

} // namespace detail

//-------------------------------------------------------------------------------------------------
template <typename T>
Value::Value(const T& val)
    : m_value(camp_ext::ValueMapper<T>::to(val))
    , m_type(mapType<T>())
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
T Value::to() const
{
    try
    {
        return detail::ValueTo<T>::convert(*this);
    }
    catch (std::exception&)
    {
        CAMP_ERROR(InvalidValue(m_type, mapType<T>()));
    }
}

//-------------------------------------------------------------------------------------------------
template <typename T>
Value::operator T() const
{
    return to<T>();
}

//-------------------------------------------------------------------------------------------------
template <typename T>
bool Value::isCompatible() const
{
    try
    {
        to<T>();
        return true;
    }
    catch (std::exception&)
    {
        return false;
    }
};

//-------------------------------------------------------------------------------------------------
template <typename T>
typename T::result_type Value::visit(T visitor) const
{
    return boost::apply_visitor(visitor, m_value);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
typename T::result_type Value::visit(T visitor, const Value& other) const
{
    return boost::apply_visitor(visitor, m_value, other.m_value);
}

} // namespace camp
