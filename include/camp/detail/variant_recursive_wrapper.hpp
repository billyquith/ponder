//
//    Copyright (c) MapBox
//    All rights reserved.
//
//    Redistribution and use in source and binary forms, with or without modification,
//    are permitted provided that the following conditions are met:
//
//    - Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//    - Redistributions in binary form must reproduce the above copyright notice, this
//    list of conditions and the following disclaimer in the documentation and/or
//    other materials provided with the distribution.
//    - Neither the name "MapBox" nor the names of its contributors may be
//    used to endorse or promote products derived from this software without
//    specific prior written permission.
//
//    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
//    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
//    ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#ifndef MAPBOX_UTIL_VARIANT_RECURSIVE_WRAPPER_HPP
#define MAPBOX_UTIL_VARIANT_RECURSIVE_WRAPPER_HPP

#include <utility>

namespace mapbox { namespace util {

template <typename T>
class recursive_wrapper
{
public:
    using type = T;
private:

    T* p_;

public:

    ~recursive_wrapper();
    recursive_wrapper();

    recursive_wrapper(recursive_wrapper const& operand);
    recursive_wrapper(T const& operand);
    recursive_wrapper(recursive_wrapper&& operand);
    recursive_wrapper(T&& operand);

private:

    void assign(const T& rhs);

public:

    inline recursive_wrapper& operator=(recursive_wrapper const& rhs)
    {
        assign( rhs.get() );
        return *this;
    }

    inline recursive_wrapper& operator=(T const& rhs)
    {
        assign( rhs );
        return *this;
    }

    inline void swap(recursive_wrapper& operand) noexcept
    {
        T* temp = operand.p_;
        operand.p_ = p_;
        p_ = temp;
    }


    recursive_wrapper& operator=(recursive_wrapper&& rhs) noexcept
    {
        swap(rhs);
        return *this;
    }

    recursive_wrapper& operator=(T&& rhs)
    {
        get() = std::move(rhs);
        return *this;
    }


public:

    T& get() { return *get_pointer(); }
    const T& get() const { return *get_pointer(); }
    T* get_pointer() { return p_; }
    const T* get_pointer() const { return p_; }
    operator T const&() const { return this->get(); }
    operator T&() { return this->get(); }
};

template <typename T>
recursive_wrapper<T>::~recursive_wrapper()
{
    delete p_;
}

template <typename T>
recursive_wrapper<T>::recursive_wrapper()
    : p_(new T)
{
}

template <typename T>
recursive_wrapper<T>::recursive_wrapper(recursive_wrapper const& operand)
    : p_(new T( operand.get() ))
{
}

template <typename T>
recursive_wrapper<T>::recursive_wrapper(T const& operand)
    : p_(new T(operand))
{
}

template <typename T>
recursive_wrapper<T>::recursive_wrapper(recursive_wrapper&& operand)
    : p_(operand.p_)
{
    operand.p_ = nullptr;
}

template <typename T>
recursive_wrapper<T>::recursive_wrapper(T&& operand)
    : p_(new T( std::move(operand) ))
{
}

template <typename T>
void recursive_wrapper<T>::assign(const T& rhs)
{
    this->get() = rhs;
}

template <typename T>
inline void swap(recursive_wrapper<T>& lhs, recursive_wrapper<T>& rhs) noexcept
{
    lhs.swap(rhs);
}

}}

#endif // MAPBOX_UTIL_VARIANT_RECURSIVE_WRAPPER_HPP
