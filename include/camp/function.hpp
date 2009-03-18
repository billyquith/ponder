// <insert copyright>

#ifndef CAMP_FUNCTION_HPP
#define CAMP_FUNCTION_HPP


#include <camp/config.hpp>
#include <camp/detail/getter.hpp>
#include <camp/args.hpp>
#include <camp/tagholder.hpp>
#include <camp/type.hpp>
#include <camp/value.hpp>
#include <string>
#include <vector>


namespace camp
{
template <typename T> class ClassBuilder;
class UserObject;
class ClassVisitor;

/**
 * \brief Abstract representation of a function
 *
 * Functions are members of metaclasses. Their main purpose is to be called; they also provide
 * detailed informations about their prototype.
 */
class Function : public TagHolder
{
public:

    /**
     * \brief Destructor
     */
    CAMP_API virtual ~Function();

    /**
     * \brief Get the name of the function
     *
     * \return Name of the function
     */
    CAMP_API const std::string& name() const;

    /**
     * \brief Get the number of arguments of the function
     *
     * \return Total number of arguments taken by the function
     */
    CAMP_API std::size_t argCount() const;

    /**
     * \brief Get the type of variable returned by the function
     *
     * \return Type of the result of the function
     */
    CAMP_API Type returnType() const;

    /**
     * \brief Get the type of an argument given by its index
     *
     * \param index Index of the argument
     *
     * \return Type of the index-th argument
     *
     * \throw InvalidIndex index is out of range
     */
    CAMP_API Type argType(std::size_t index) const;

    /**
     * \brief Check if the function is currently callable for a given object
     *
     * \param object Object
     *
     * \return True if the function can be called, false otherwise
     *
     * \throw InvalidObject object has an invalid value
     */
    CAMP_API bool callable(const UserObject& object) const;

    /**
     * \brief Call the function
     *
     * \param object Object
     * \param args Arguments to pass to the function (empty list by default)
     *
     * \return Value returned by the function call
     *
     * \throw InvalidAccess function is not callable
     * \throw InvalidObject object has an invalid value
     * \throw InvalidArgument one of the arguments can't be converted to the requested type
     */
    CAMP_API Value call(const UserObject& object, const Args& args = Args::empty) const;

    /**
     * \brief Accept the visitation of a ClassVisitor
     *
     * \param visitor Visitor to accept
     */
    CAMP_API virtual void accept(ClassVisitor& visitor) const;

protected:

    template <typename T> friend class ClassBuilder;

    /**
     * \brief Construct the function from its description
     *
     * \param name Name of the function
     * \param returnType Type of the function result
     * \param argTypes Types of the function arguments (empty array by default)
     *
     * \return Value returned by the function call
     */
    CAMP_API Function(const std::string& name, Type returnType, const std::vector<Type>& argTypes = std::vector<Type>());

    /**
     * \brief Do the actual call
     *
     * This function is a pure virtual which has to be implemented in derived classes.
     *
     * \param object Object
     * \param args Arguments to pass to the function
     *
     * \return Value returned by the function call
     *
     * \throw InvalidObject object has an invalid value
     * \throw InvalidArgument one of the arguments can't be converted to the requested type
     */
    CAMP_API virtual Value execute(const UserObject& object, const Args& args) const = 0;

private:

    std::string m_name; ///< Name of the function
    Type m_returnType; ///< Return type
    std::vector<Type> m_argTypes; ///< Type of all the function arguments
    detail::Getter<bool> m_callable; ///< Accessor to get the callable state of the function
};

} // namespace camp


#endif // CAMP_FUNCTION_HPP
