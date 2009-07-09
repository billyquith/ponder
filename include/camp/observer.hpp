

#ifndef CAMP_OBSERVER_HPP
#define CAMP_OBSERVER_HPP


#include <camp/config.hpp>


namespace camp
{
class Class;
class Enum;

/**
 * \brief Receives notification about creation / destruction of metaclasses and metaenums
 *
 * This class is a base class which must be derived in order to create custom observers.
 * None of the virtual functions is pure, so you can only override the one you're interested in.
 *
 * \sa Class, Enum
 */
class CAMP_API Observer
{
public:

    /**
     * \brief Destructor
     */
    virtual ~Observer();

    /**
     * \brief Functon called when a new metaclass is created
     *
     * \param added Metaclass that have been added
     */
    virtual void classAdded(const Class& added);

    /**
     * \brief Functon called when an existing metaclass is destroyed
     *
     * \param removed Metaclass that have been destroyed
     */
    virtual void classRemoved(const Class& removed);

    /**
     * \brief Functon called when a new metaenum is created
     *
     * \param added Metaenum that have been added
     */
    virtual void enumAdded(const Enum& added);

    /**
     * \brief Functon called when an existing metaenum is destroyed
     *
     * \param removed Metaenum that have been destroyed
     */
    virtual void enumRemoved(const Enum& removed);

protected:

    /**
     * \brief Default constructor
     */
    Observer();
};

/**
 * \brief Register an observer
 *
 * \param observer Pointer to the observer instance to register
 */
CAMP_API void addObserver(Observer* observer);

/**
 * \brief Unregister an observer
 *
 * \param observer Pointer to the observer instance to unregister
 */
CAMP_API void removeObserver(Observer* observer);

} // namespace camp


#endif // CAMP_OBSERVER_HPP
