/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
**
****************************************************************************/


#ifndef CAMP_UTIL_HPP
#define CAMP_UTIL_HPP

#include "format.h"

namespace camp
{

namespace util
{
    
    class noncopyable
    {
    protected:
        noncopyable() {}
        ~noncopyable() {}
        
    private:
        noncopyable( const noncopyable& ) = delete;
        noncopyable& operator=( const noncopyable& ) = delete;
    };
    
    
    template <bool B, class T = void>
    struct enable_if_c {
        typedef T type;
    };
    
    template <class T>
    struct enable_if_c<false, T> {};
    
    template <class Cond, class T = void>
    struct enable_if : public enable_if_c<Cond::value, T> {};
    
    
    template<bool C, typename T, typename F>
    struct if_c
    {
        typedef T type;
    };
    
    template<typename T, typename F>
    struct if_c<false,T,F>
    {
        typedef F type;
    };
    
    
    class bad_conversion : std::exception {};
    
} // util

namespace detail
{
    template <typename T, typename F, typename O = void>
    struct convert
    {
        T operator () (const F& from)
        {
            const T result = static_cast<T>(from);
            return result;
        }
    };
    
    template <typename F>
    std::string to_str(F from)
    {
        return fmt::format("{}", from);
    }
    
    template <typename S>
    struct convert <std::string, S>
    {
        std::string operator () (const S& from)
        {
            return detail::to_str(from);
        }
    };

    template <>
    struct convert <std::string, bool>
    {
        std::string operator () (const bool& from)
        {
            static const std::string t("1"), f("0");
            return from ? t : f;
        }
    };

    bool conv(const std::string& from, bool& to);
    bool conv(const std::string& from, char& to);
    bool conv(const std::string& from, unsigned char& to);
    bool conv(const std::string& from, short& to);
    bool conv(const std::string& from, unsigned short& to);
    bool conv(const std::string& from, int& to);
    bool conv(const std::string& from, unsigned int& to);
    bool conv(const std::string& from, long& to);
    bool conv(const std::string& from, unsigned long& to);
    bool conv(const std::string& from, long long& to);
    bool conv(const std::string& from, unsigned long long& to);
    bool conv(const std::string& from, float& to);
    bool conv(const std::string& from, double& to);
    
    template <typename T>
    struct convert <T, std::string,
        typename util::enable_if_c<(std::is_integral<T>::value || std::is_floating_point<T>::value)
                                   && !std::is_const<T>::value
                                   && !std::is_reference<T>::value >::type >
    {
        T operator () (const std::string& from)
        {
            T result;
            if (!conv(from, result))
                throw util::bad_conversion();
            return result;
        }
    };

} // detail

namespace util
{
    
template <typename T, typename U>
T convert(const U& from)
{
    return detail::convert<T,U>()(from);
}
    
} // util
} // camp


#endif // CAMP_UTIL_HPP
