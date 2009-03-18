// <insert copyright>

#ifndef CAMP_DETAIL_USERPROPERTYIMPL_HPP
#define CAMP_DETAIL_USERPROPERTYIMPL_HPP


#include <camp/userproperty.hpp>
#include <camp/invalidaccess.hpp>


namespace camp
{
namespace detail
{
/**
 * \brief Typed implementation of UserProperty
 *
 * UserPropertyImpl is a template implementation of UserProperty, which is strongly typed
 * in order to keep track of the true underlying C++ types involved in the property.
 *
 * The template parameter A is an abstract helper to access the actual C++ property.
 *
 * \sa UserProperty
 */
template <typename A>
class UserPropertyImpl : public UserProperty
{
public:

    /**
     * \brief Construct the property from its accessors
     *
     * \param name Name of the property
     * \param accessor Object used to access the actual C++ property
     */
    UserPropertyImpl(const std::string& name, const A& accessor);

protected:

    /**
     * \see Property::isReadable
     */
    virtual bool isReadable() const;

    /**
     * \see Property::isWritable
     */
    virtual bool isWritable() const;

    /**
     * \see Property::getValue
     */
    virtual Value getValue(const UserObject& object) const;

    /**
     * \see Property::setValue
     */
    virtual void setValue(const UserObject& object, const Value& value) const;

private:

    A m_accessor; ///< Object used to access the actual C++ property
};

} // namespace detail

} // namespace camp

#include <camp/detail/userpropertyimpl.inl>


#endif // CAMP_DETAIL_USERPROPERTYIMPL_HPP
