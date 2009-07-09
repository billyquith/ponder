

#ifndef CAMP_ENUMBUILDER_HPP
#define CAMP_ENUMBUILDER_HPP


#include <camp/config.hpp>
#include <boost/noncopyable.hpp>
#include <string>


namespace camp
{
class Enum;

/**
 * \brief Proxy class which fills a metaenum with its members
 *
 * This class is returned by Enum::declare<T> in order construct a
 * new metaenum. It contains functions to declare <name, value> pairs to
 * fill the metaenum.
 *
 * This class should never be explicitely instanciated, unless you
 * need to split the metaenum creation in multiple parts.
 */
class CAMP_API EnumBuilder
{
public:

    /**
     * \brief Construct the builder with a target metaenum
     *
     * \param target Target metaenum to construct
     */
    EnumBuilder(Enum& target);

    /**
     * \brief Add a new pair to the metaenum
     *
     * \param name Name of the pair
     * \param value Value of the pair
     */
    EnumBuilder& value(const std::string& name, long value);

private:

    Enum* m_target; ///< Target metaenum to construct
};

} // namespace camp


#endif // CAMP_ENUMBUILDER_HPP
