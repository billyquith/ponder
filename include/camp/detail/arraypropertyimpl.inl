


namespace camp
{
namespace detail
{
//-------------------------------------------------------------------------------------------------
template <typename A>
ArrayPropertyImpl<A>::ArrayPropertyImpl(const std::string& name, const A& accessor)
    : ArrayProperty(name, mapType<ElementType>(), Mapper::dynamic())
    , m_accessor(accessor)
{
}

//-------------------------------------------------------------------------------------------------
template <typename A>
std::size_t ArrayPropertyImpl<A>::getSize(const UserObject& object) const
{
    return Mapper::size(array(object));
}

//-------------------------------------------------------------------------------------------------
template <typename A>
Value ArrayPropertyImpl<A>::getElement(const UserObject& object, std::size_t index) const
{
    return Mapper::get(array(object), index);
}

//-------------------------------------------------------------------------------------------------
template <typename A>
void ArrayPropertyImpl<A>::setElement(const UserObject& object, std::size_t index, const Value& value) const
{
    Mapper::set(array(object), index, value.to<ElementType>());
}

//-------------------------------------------------------------------------------------------------
template <typename A>
void ArrayPropertyImpl<A>::insertElement(const UserObject& object, std::size_t before, const Value& value) const
{
    Mapper::insert(array(object), before, value.to<ElementType>());
}

//-------------------------------------------------------------------------------------------------
template <typename A>
void ArrayPropertyImpl<A>::removeElement(const UserObject& object, std::size_t index) const
{
    Mapper::remove(array(object), index);
}

//-------------------------------------------------------------------------------------------------
template <typename A>
typename ArrayPropertyImpl<A>::ArrayType& ArrayPropertyImpl<A>::array(const UserObject& object) const
{
    return m_accessor.get(object.get<typename A::ClassType>());
}

} // namespace detail

} // namespace camp
