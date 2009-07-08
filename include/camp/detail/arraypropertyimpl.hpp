

#ifndef CAMP_DETAIL_ARRAYPROPERTYIMPL_HPP
#define CAMP_DETAIL_ARRAYPROPERTYIMPL_HPP


#include <camp/arrayproperty.hpp>
#include <camp/arraymapper.hpp>
#include <boost/type_traits/remove_reference.hpp>


namespace camp
{
namespace detail
{
/**
 * \brief Typed implementation of ArrayProperty
 *
 * ArrayPropertyImpl is a template implementation of ArrayProperty, which is strongly typed
 * in order to keep track of the true underlying C++ types involved in the property.
 *
 * The template parameter A is an abstract helper to access the actual C++ property.
 *
 * This class uses the camp_ext::ArrayMapper template to implement its operations according
 * to the type of array.
 *
 * \sa ArrayProperty, camp_ext::ArrayMapper
 */
template <typename A>
class ArrayPropertyImpl : public ArrayProperty
{
public:

    /**
     * \brief Construct the property
     *
     * \param name Name of the property
     * \param accessor Object used to access the actual C++ property
     */
    ArrayPropertyImpl(const std::string& name, const A& accessor);

protected:

    /**
     * \see ArrayProperty::getSize
     */
    virtual std::size_t getSize(const UserObject& object) const;

    /**
     * \see ArrayProperty::getElement
     */
    virtual Value getElement(const UserObject& object, std::size_t index) const;

    /**
     * \see ArrayProperty::setElement
     */
    virtual void setElement(const UserObject& object, std::size_t index, const Value& value) const;

    /**
     * \see ArrayProperty::insertElement
     */
    virtual void insertElement(const UserObject& object, std::size_t before, const Value& value) const;

    /**
     * \see ArrayProperty::removeElement
     */
    virtual void removeElement(const UserObject& object, std::size_t index) const;

private:

    typedef typename boost::remove_reference<typename A::DataType>::type ArrayType;
    typedef camp_ext::ArrayMapper<ArrayType> Mapper;
    typedef typename Mapper::ElementType ElementType;

    /**
     * \brief Retrieve a reference to the array
     *
     * \param object Owner object
     *
     * \return Reference to the underlying array
     */
    ArrayType& array(const UserObject& object) const;

    A m_accessor; ///< Object used to access the actual C++ property
};

} // namespace detail

} // namespace camp

#include <camp/detail/arraypropertyimpl.inl>


#endif // CAMP_DETAIL_ARRAYPROPERTYIMPL_HPP
