// <insert copyright>

#ifndef CAMP_DETAIL_CALLHELPER_HPP
#define CAMP_DETAIL_CALLHELPER_HPP


#include <camp/value.hpp>


namespace camp
{
namespace detail
{
/**
 * \class CallHelper
 *
 * \brief Helper structure used to make a function call and return the result as a camp::Value.
 *
 * This structure is necessary because we need a specific code for functions returning void.
 */

/*
 * Generic version of CallHelper
 */
template <typename R, typename C>
struct CallHelper
{
    template <typename F>
    static Value call(F func, C obj) {return func(obj);}

    template <typename F, typename A0>
    static Value call(F func, C obj, A0 a0) {return func(obj, a0);}

    template <typename F, typename A0, typename A1>
    static Value call(F func, C obj, A0 a0, A1 a1) {return func(obj, a0, a1);}

    template <typename F, typename A0, typename A1, typename A2>
    static Value call(F func, C obj, A0 a0, A1 a1, A2 a2) {return func(obj, a0, a1, a2);}

    template <typename F, typename A0, typename A1, typename A2, typename A3>
    static Value call(F func, C obj, A0 a0, A1 a1, A2 a2, A3 a3) {return func(obj, a0, a1, a2, a3);}

    template <typename F, typename A0, typename A1, typename A2, typename A3, typename A4>
    static Value call(F func, C obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {return func(obj, a0, a1, a2, a3, a4);}
};

/*
 * Specialization of CallHelper for functions returning void
 */
template <typename C>
struct CallHelper<void, C>
{
    template <typename F>
    static Value call(F func, C obj) {func(obj); return Value::nothing;}

    template <typename F, typename A0>
    static Value call(F func, C obj, A0 a0) {func(obj, a0); return Value::nothing;}

    template <typename F, typename A0, typename A1>
    static Value call(F func, C obj, A0 a0, A1 a1) {func(obj, a0, a1); return Value::nothing;}

    template <typename F, typename A0, typename A1, typename A2>
    static Value call(F func, C obj, A0 a0, A1 a1, A2 a2) {func(obj, a0, a1, a2); return Value::nothing;}

    template <typename F, typename A0, typename A1, typename A2, typename A3>
    static Value call(F func, C obj, A0 a0, A1 a1, A2 a2, A3 a3) {func(obj, a0, a1, a2, a3); return Value::nothing;}

    template <typename F, typename A0, typename A1, typename A2, typename A3, typename A4>
    static Value call(F func, C obj, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {func(obj, a0, a1, a2, a3, a4); return Value::nothing;}
};

} // namespace detail

} // namespace camp


#endif // CAMP_DETAIL_CALLHELPER_HPP
