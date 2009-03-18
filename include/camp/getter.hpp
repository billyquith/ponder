// <insert copyright>

#ifndef CAMP_ABSTRACTGETTER_HPP
#define CAMP_ABSTRACTGETTER_HPP


#include <camp/userobject.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>


namespace camp
{
namespace detail
{
template <typename T>
class GetterInterface
{
public:

    virtual ~GetterInterface();

    virtual T get(const UserObject& object) const = 0;
};


template <typename T, typename C>
class GetterImpl : public GetterInterface<T>
{
public:

    GetterImpl(boost::function<T (C&)> function);

    virtual T get(const UserObject& object) const;

private:

    boost::function<T (C&)> m_function;
};


template <typename T>
class Getter
{
public:

    Getter(const T& defaultValue = T());

    template <typename C> Getter(boost::function<T (C&)> function);

    const T& get() const;

    T get(const UserObject& object) const;

private:

    boost::shared_ptr<GetterInterface<T> > m_getter;
    T m_defaultValue;
};

} // namespace detail

} // namespace camp

#include <camp/getter.inl>


#endif // CAMP_ABSTRACTGETTER_HPP
