/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2015-2018 Nick Trout.
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

#ifndef PONDER_USERDATA_HPP
#define PONDER_USERDATA_HPP

#include <ponder/config.hpp>
#include <ponder/value.hpp>

namespace ponder {

class UserData
{
public:
    UserData(IdRef name, Value&& value) : m_name(name), m_value(value) {}
    
    IdReturn getName() const { return m_name; }
    
    const Value& getValue() const { return m_value; }
    Value& getValue() { return m_value; }

private:
    Id m_name;
    Value m_value;
};
    
class IUserDataStore
{
public:
    virtual ~IUserDataStore() {}
    virtual void setValue(const Type* t, IdRef name, const Value& v) = 0;
    virtual const Value* getValue(const Type* t, IdRef name) const = 0;
    virtual void removeValue(Type* t, IdRef name) = 0;
};

IUserDataStore* getUserDataStore();

} // namespace ponder


#endif // PONDER_USERDATA_HPP
