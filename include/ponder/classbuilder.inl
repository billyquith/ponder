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


namespace ponder
{
template <typename T>
ClassBuilder<T>::ClassBuilder(Class& target)
    : m_target(&target)
    , m_currentTagHolder(m_target)
    , m_currentProperty(nullptr)
    , m_currentFunction(nullptr)
{
}

template <typename T>
template <typename U>
ClassBuilder<T>& ClassBuilder<T>::base()
{
    // Retrieve the base metaclass and its name
    const Class& baseClass = classByType<U>();
    std::string baseName = baseClass.name();

    // First make sure that the base class is not already a base of the current class
    for (Class::BaseInfo& bi : m_target->m_bases)
    {
        assert(bi.base->name() != baseName);
    }

    // Compute the offset to apply for pointer conversions
    T* asDerived = reinterpret_cast<T*>(1);
    U* asBase = static_cast<U*>(asDerived);
    int offset = static_cast<int>(reinterpret_cast<char*>(asBase) - reinterpret_cast<char*>(asDerived));

    // Add the base metaclass to the bases of the current class
    Class::BaseInfo baseInfos;
    baseInfos.base = &baseClass;
    baseInfos.offset = offset;
    m_target->m_bases.push_back(baseInfos);

    // Copy all properties of the base class into the current class
    for (Class::PropertyTable::const_iterator it = baseClass.m_properties.begin();
        it != baseClass.m_properties.end();
        ++it)
    {
        m_target->m_properties.insert(it);
    }

    // Copy all functions of the base class into the current class
    for (Class::FunctionTable::const_iterator it = baseClass.m_functions.begin();
        it != baseClass.m_functions.end();
        ++it)
    {
        m_target->m_functions.insert(it);
    }

    return *this;
}

template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::property(const std::string& name, F accessor)
{
    // Find the factory which will be able to construct a ponder::Property from an accessor of type F
    typedef detail::PropertyFactory1<T, F> Factory;

    // Construct and add the metaproperty
    return addProperty(Factory::get(name, accessor));
}

template <typename T>
template <typename F1, typename F2>
ClassBuilder<T>& ClassBuilder<T>::property(const std::string& name, F1 accessor1, F2 accessor2)
{
    // Find the factory which will be able to construct a ponder::Property from accessors of type F1 and F2
    typedef detail::PropertyFactory2<T, F1, F2> Factory;

    // Construct and add the metaproperty
    return addProperty(Factory::get(name, accessor1, accessor2));
}

template <typename T>
template <typename F1, typename F2, typename F3>
ClassBuilder<T>& ClassBuilder<T>::property(const std::string& name, F1 accessor1, F2 accessor2, F3 accessor3)
{
    // Find the factory which will be able to construct a ponder::Property from accessors of type F1, F2 and F3
    typedef detail::PropertyFactory3<T, F1, F2, F3> Factory;

    // Construct and add the metaproperty
    return addProperty(Factory::get(name, accessor1, accessor2, accessor3));
}

template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::function(const std::string& name, F function)
{
    // Get a uniform function type from F, whatever it really is
    typedef typename detail::FunctionTraits<F>::type Signature;

    // Construct and add the metafunction
    return addFunction(new detail::FunctionImpl<Signature>(name, function));
}

template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::function(const std::string& name, std::function<F> function)
{
    return addFunction(new detail::FunctionImpl<F>(name, function));
}

template <typename T>
template <typename F1, typename F2>
ClassBuilder<T>& ClassBuilder<T>::function(const std::string& name, F1 function1, F2 function2)
{
    // Get uniform function types from F1 and F2, whatever they really are
    typedef typename detail::FunctionTraits<F1>::type Signature1;
    typedef typename detail::FunctionTraits<F2>::type Signature2;

    // Construct and add the metafunction
    return addFunction(new detail::FunctionImpl<Signature1, Signature2>(name, function1, function2));
}

template <typename T>
ClassBuilder<T>& ClassBuilder<T>::tag(const Value& id)
{
    return tag(id, Value::nothing);
}

template <typename T>
template <typename U>
ClassBuilder<T>& ClassBuilder<T>::tag(const Value& id, const U& value)
{
    // Make sure we have a valid tag holder, and the tag doesn't already exists
    assert(m_currentTagHolder && !m_currentTagHolder->hasTag(id));

    // For the special case of Getter<Value>, the ambiguity between both constructors
    // cannot be automatically solved, so let's do it manually
    typedef typename detail::if_c<detail::FunctionTraits<U>::isFunction, std::function<Value (T&)>, Value>::type Type;

    // Add the new tag (override if already exists)
    m_currentTagHolder->m_tags[id] = detail::Getter<Value>(Type(value));

    return *this;
}

template <typename T>
ClassBuilder<T>& ClassBuilder<T>::readable(bool value)
{
    // Make sure we have a valid property
    assert(m_currentProperty != nullptr);

    m_currentProperty->m_readable = detail::Getter<bool>(value);

    return *this;
}

template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::readable(F function)
{
    // Make sure we have a valid property
    assert(m_currentProperty != nullptr);

    m_currentProperty->m_readable = detail::Getter<bool>(std::function<bool (T&)>(function));

    return *this;
}

template <typename T>
ClassBuilder<T>& ClassBuilder<T>::writable(bool value)
{
    // Make sure we have a valid property
    assert(m_currentProperty != nullptr);

    m_currentProperty->m_writable = detail::Getter<bool>(value);

    return *this;
}

template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::writable(F function)
{
    // Make sure we have a valid property
    assert(m_currentProperty != nullptr);

    m_currentProperty->m_writable = detail::Getter<bool>(std::function<bool (T&)>(function));

    return *this;
}

template <typename T>
ClassBuilder<T>& ClassBuilder<T>::callable(bool value)
{
    // Make sure we have a valid function
    assert(m_currentFunction != nullptr);

    m_currentFunction->m_callable = detail::Getter<bool>(value);

    return *this;
}

template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::callable(F function)
{
    // Make sure we have a valid function
    assert(m_currentFunction != nullptr);

    m_currentFunction->m_callable = detail::Getter<bool>(std::function<bool (T&)>(function));

    return *this;
}

template <typename T>
template <typename... A>
ClassBuilder<T>& ClassBuilder<T>::constructor()
{
    Constructor* constructor = new detail::ConstructorImpl<T, A...>;
    m_target->m_constructors.push_back(Class::ConstructorPtr(constructor));

    return *this;
}

template <typename T>
template <template <typename> class U>
ClassBuilder<T>& ClassBuilder<T>::external()
{
    // Create an instance of the mapper
    U<T> mapper;

    // Retrieve the properties
    std::size_t propertyCount = mapper.propertyCount();
    for (std::size_t i = 0; i < propertyCount; ++i)
        addProperty(mapper.property(i));

    // Retrieve the functions
    std::size_t functionCount = mapper.functionCount();
    for (std::size_t i = 0; i < functionCount; ++i)
        addFunction(mapper.function(i));

    return *this;
}

template <typename T>
ClassBuilder<T>& ClassBuilder<T>::addProperty(Property* property)
{
    // Retrieve the class' properties indexed by name
    Class::PropertyTable& properties = m_target->m_properties;

    // First remove any property that already exists with the same name
    properties.erase(property->name());

    // Insert the new property
    properties.insert(property->name(), Class::PropertyPtr(property));

    m_currentTagHolder = m_currentProperty = property;
    m_currentFunction = nullptr;

    return *this;
}

template <typename T>
ClassBuilder<T>& ClassBuilder<T>::addFunction(Function* function)
{
    // Retrieve the class' functions indexed by name
    Class::FunctionTable& functions = m_target->m_functions;

    // First remove any function that already exists with the same name
    functions.erase(function->name());

    // Insert the new function
    functions.insert(function->name(), Class::FunctionPtr(function));

    m_currentTagHolder = m_currentFunction = function;
    m_currentProperty = nullptr;

    return *this;
}

} // namespace ponder
