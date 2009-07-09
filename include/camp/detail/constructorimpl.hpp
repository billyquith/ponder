

#ifndef CAMP_DETAIL_CONSTRUCTORIMPL_HPP
#define CAMP_DETAIL_CONSTRUCTORIMPL_HPP


#include <camp/constructor.hpp>
#include <camp/valuemapper.hpp>
#include <camp/value.hpp>
#include <camp/valuevisitor.hpp>


namespace camp
{
namespace detail
{
/**
 * \brief Check if a value is compatible with a C++ type
 *
 * This is a strong test, we check if the type of the value is directly mappable to T, not
 * just convertible.
 *
 * \param value Value to check
 *
 * \return True if the value is compatible with the type T
 */
template <typename T>
bool checkArg(const Value& value);

/**
 * \brief Implementation of metaconstructors with no parameter
 */
template <typename T>
class ConstructorImpl0 : public Constructor
{
public:

    /**
     * \see Constructor::matches
     */
    virtual bool matches(const Args& args) const
    {
        return (args.count() == 0);
    }

    /**
     * \see Constructor::create
     */
    virtual void* create(const Args&) const
    {
        return new T();
    }
};

/**
 * \brief Implementation of metaconstructors with 1 parameter
 */
template <typename T, typename A0>
class ConstructorImpl1 : public Constructor
{
public:

    /**
     * \see Constructor::matches
     */
    virtual bool matches(const Args& args) const
    {
        return (args.count() == 1)
               && checkArg<A0>(args[0]);
    }

    /**
     * \see Constructor::create
     */
    virtual void* create(const Args& args) const
    {
        return new T(args[0].to<A0>());
    }
};

/**
 * \brief Implementation of metaconstructors with 2 parameters
 */
template <typename T, typename A0, typename A1>
class ConstructorImpl2 : public Constructor
{
public:

    /**
     * \see Constructor::matches
     */
    virtual bool matches(const Args& args) const
    {
        return (args.count() == 2)
               && checkArg<A0>(args[0])
               && checkArg<A1>(args[1]);
    }

    /**
     * \see Constructor::create
     */
    virtual void* create(const Args& args) const
    {
        return new T(args[0].to<A0>(),
                     args[1].to<A1>());
    }
};

/**
 * \brief Implementation of metaconstructors with 3 parameters
 */
template <typename T, typename A0, typename A1, typename A2>
class ConstructorImpl3 : public Constructor
{
public:

    /**
     * \see Constructor::matches
     */
    virtual bool matches(const Args& args) const
    {
        return (args.count() == 3)
               && checkArg<A0>(args[0])
               && checkArg<A1>(args[1])
               && checkArg<A2>(args[2]);
    }

    /**
     * \see Constructor::create
     */
    virtual void* create(const Args& args) const
    {
        return new T(args[0].to<A0>(),
                     args[1].to<A1>(),
                     args[2].to<A2>());
    }
};

/**
 * \brief Implementation of metaconstructors with 4 parameters
 */
template <typename T, typename A0, typename A1, typename A2, typename A3>
class ConstructorImpl4 : public Constructor
{
public:

    /**
     * \see Constructor::matches
     */
    virtual bool matches(const Args& args) const
    {
        return (args.count() == 4)
               && checkArg<A0>(args[0])
               && checkArg<A1>(args[1])
               && checkArg<A2>(args[2])
               && checkArg<A3>(args[3]);
    }

    /**
     * \see Constructor::create
     */
    virtual void* create(const Args& args) const
    {
        return new T(args[0].to<A0>(),
                     args[1].to<A1>(),
                     args[2].to<A2>(),
                     args[3].to<A3>());
    }
};

/**
 * \brief Implementation of metaconstructors with 5 parameters
 */
template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4>
class ConstructorImpl5 : public Constructor
{
public:

    /**
     * \see Constructor::matches
     */
    virtual bool matches(const Args& args) const
    {
        return (args.count() == 5)
               && checkArg<A0>(args[0])
               && checkArg<A1>(args[1])
               && checkArg<A2>(args[2])
               && checkArg<A3>(args[3])
               && checkArg<A4>(args[4]);
    }

    /**
     * \see Constructor::create
     */
    virtual void* create(const Args& args) const
    {
        return new T(args[0].to<A0>(),
                     args[1].to<A1>(),
                     args[2].to<A2>(),
                     args[3].to<A3>(),
                     args[4].to<A4>());
    }
};

/**
 * \brief Value visitor which checks the type of the visited value against the C++ type T
 */
template <typename T>
struct CheckTypeVisitor : public ValueVisitor<bool>
{
    /**
     * \brief Common case: check mapping
     */
    template <typename U>
    bool operator()(const U&)
    {
        return mapType<T>() == mapType<U>();
    }

    /**
     * \brief Special case of enum objects: check metaenum and bound type
     */
    bool operator()(const EnumObject& obj)
    {
        const Enum* targetEnum = enumByTypeSafe<T>();
        return targetEnum && (*targetEnum == obj.getEnum());
    }

    /**
     * \brief Special case of user objects: check metaclass and bound type
     */
    bool operator()(const UserObject& obj)
    {
        const Class* targetClass = classByTypeSafe<T>();
        return targetClass && (*targetClass == obj.getClass());
    }
};

template <typename T>
bool checkArg(const Value& value)
{
    return value.visit(CheckTypeVisitor<T>());
}

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_CONSTRUCTORIMPL_HPP
