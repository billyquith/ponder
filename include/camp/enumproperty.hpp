// <insert copyright>

#ifndef CAMP_ENUMPROPERTY_HPP
#define CAMP_ENUMPROPERTY_HPP


#include <camp/property.hpp>


namespace camp
{
class Enum;

/**
 * \brief Specialized type of property for enums
 *
 */
class EnumProperty : public Property
{
public:

    /**
     * \brief Construct the property from its description
     *
     * \param name Name of the property
     * \param propEnum Eumeration the property is bound to
     */
    CAMP_API EnumProperty(const std::string& name, const Enum& propEnum);

    /**
     * \brief Destructor
     */
    CAMP_API virtual ~EnumProperty();

    /**
     * \brief Get the owner enum
     *
     * \return Enum the property is bound to
     */
    CAMP_API const Enum& getEnum() const;

    /**
     * \brief Accept the visitation of a ClassVisitor
     *
     * \param visitor Visitor to accept
     */
    CAMP_API virtual void accept(ClassVisitor& visitor) const;

private:

    const Enum* m_enum; ///< Owner enum of the property
};

} // namespace camp


#endif // CAMP_ENUMPROPERTY_HPP
