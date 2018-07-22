/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
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


namespace ponder {
namespace detail {

template <typename T>
void destroy(const UserObject& object, bool destruct)
{
    if (destruct)
        object.get<T*>()->~T();
    else
        delete object.get<T*>();
}

template <typename T>
static inline UserObject userObjectCreator(void* ptr)
{
    return UserObject::makeRef(*static_cast<T*>(ptr));
}
    
} // namespace detail

template <typename T>
inline ClassBuilder<T> Class::declare(IdRef id)
{
    Class& newClass =
        detail::ClassManager::instance().addClass(
            id.empty() ? detail::StaticTypeId<T>::get(false) : id);
    newClass.m_sizeof = sizeof(T);
    newClass.m_destructor = &detail::destroy<T>;
    newClass.m_userObjectCreator = &detail::userObjectCreator<T>;
    return ClassBuilder<T>(newClass);
}

template <typename T>
inline void Class::undeclare(IdRef id)
{
    detail::ClassManager::instance().removeClass(
        id.empty() ? detail::StaticTypeId<T>::get(false) : id);
}

inline Class::FunctionTable::Iterator Class::functionIterator() const
{
    return m_functions.getIterator();
}

inline bool Class::tryFunction(const IdRef name, const Function *& funcRet) const
{
    FunctionTable::const_iterator it;
    if (m_functions.tryFind(name, it))
    {
        funcRet = it->value().get();
        return true;
    }
    return false;
}

inline Class::PropertyTable::Iterator Class::propertyIterator() const
{
    return m_properties.getIterator();
}

inline bool Class::tryProperty(const IdRef name, const Property *& propRet) const
{
    PropertyTable::const_iterator it;
    if (m_properties.tryFind(name, it))
    {
        propRet = it->value().get();
        return true;
    }
    return false;
}

inline UserObject Class::getUserObjectFromPointer(void* ptr) const
{
    return m_userObjectCreator(ptr);
}

} // namespace ponder
