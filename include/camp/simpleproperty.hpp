

#ifndef CAMP_SIMPLEPROPERTY_HPP
#define CAMP_SIMPLEPROPERTY_HPP


#include <camp/property.hpp>


namespace camp
{
/**
 * \brief Base class for all simple types of properties
 *
 * This class actually does nothing more than its base, it's just a way to separate
 * simple properties from other types.
 *
 * \sa ArrayProperty, EnumProperty, ObjectProperty
 */
class CAMP_API SimpleProperty : public Property
{
public:

    /**
     * \brief Construct the property from its description
     *
     * \param name Name of the property
     * \param type Type of the property
     */
    SimpleProperty(const std::string& name, Type type);

    /**
     * \brief Destructor
     */
    virtual ~SimpleProperty();

    /**
     * \brief Accept the visitation of a ClassVisitor
     *
     * \param visitor Visitor to accept
     */
    virtual void accept(ClassVisitor& visitor) const;
};

} // namespace camp


#endif // CAMP_SIMPLEPROPERTY_HPP
