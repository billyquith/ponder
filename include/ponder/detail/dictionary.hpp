/****************************************************************************
 **
 ** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
 ** Contact: Tegesoft Information (contact@tegesoft.com)
 **
 ** This file is part of the Ponder library, formerly CAMP.
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


#ifndef PONDER_DICTIONARY_HPP
#define PONDER_DICTIONARY_HPP

#include <utility>
#include <vector>

namespace ponder
{
namespace detail
{

template <typename KEY, typename VALUE, class CMP>
class Dictionary
{
    typedef std::pair<KEY,VALUE> pair;

    typedef std::vector<pair> Container;
    Container m_contents;
    
    struct KeyCmp {
        bool operator () (const pair& a, const KEY &b) const {
            return CMP() (a.first, b);
        }
    };

public:
    
    typedef typename Container::const_iterator const_iterator;
    
    const_iterator begin() const    { return m_contents.begin(); }
    const_iterator end() const      { return m_contents.end(); }

    const_iterator findKey(const KEY &key) const
    {
        for (auto it = m_contents.begin(); it != m_contents.end(); ++it)
        {
            if (it->first == key)
                return it;
        }
        return m_contents.end();
    }

    const_iterator findValue(const VALUE &value) const
    {
        for (auto it = m_contents.begin(); it != m_contents.end(); ++it)
        {
            if (it->second == value)
                return it;
        }
        return m_contents.end();
    }

    bool tryFind(const KEY &key, const_iterator &returnValue) const
    {
        auto it = findKey(key);
        if (it != m_contents.end())
        {
            returnValue = it;
            return true;
        }
        return false; // not found
    }
    
    bool containsKey(const KEY &key) const
    {
        return findKey(key) != m_contents.end();
    }
    
    bool containsValue(const VALUE &value) const
    {
        return findValue(value) != m_contents.end();
    }
    
    std::size_t size() const { return m_contents.size(); }

    void insert(const KEY &key, const VALUE &value)
    {
        erase(key);
        m_contents.push_back(pair(key,value));
    }
    
    void insert(const_iterator it)
    {
        insert(it->first, it->second);
    }
    
    void erase(const KEY &key)
    {
        auto it = findKey(key);
        if (it != m_contents.end())
            m_contents.erase(it);
    }
    
    const_iterator at(std::size_t index) const
    {
        const_iterator it(begin());
        std::advance(it, index);
        return it;
    }
};

} // detail
} // ponder

#endif // PONDER_DICTIONARY_HPP
