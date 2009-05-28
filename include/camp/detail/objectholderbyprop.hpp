// <insert copyright>

#ifndef CAMP_DETAIL_OBJECTHOLDERBYPROP_HPP
#define CAMP_DETAIL_OBJECTHOLDERBYPROP_HPP


#include <camp/detail/objectholder.hpp>
#include <camp/userobject.hpp>


namespace camp
{
class UserProperty;

namespace detail
{
/**
 * \brief Specialization of AbstractObjectHolder for storage by parent + member
 */
class ObjectHolderByProp : public AbstractObjectHolder
{
public:

    /**
     * \brief Construct the holder from a parent object and a member property
     *
     * \param parent Parent user object
     * \param member Member of \a parent used to access the actual object
     */
    ObjectHolderByProp(const UserObject& parent, const UserProperty& member);

    /**
     * \brief Clone the instance
     *
     * \return New holder which is a copy of this
     */
    virtual AbstractObjectHolder* clone() const;

    /**
     * \brief Return a typeless pointer to the stored object
     *
     * \return Pointer to the object
     */
    virtual void* object();

    /**
     * \brief Update the stored object
     */
    virtual void updateObject();

    /**
     * \brief Retrieve the metaclass of the stored object
     *
     * \return Object metaclass
     */
    virtual const Class& getClass() const;

private:

    UserObject m_parent; ///< Parent user object
    const UserProperty* m_member; ///< Member property used to access the object
    UserObject m_object; ///< Local copy of the target user object (so that its lifetime is the same as this object)
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_OBJECTHOLDERBYPROP_HPP
