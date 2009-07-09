/****************************************************************************
**
** Copyright (C) 2009 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** Commercial Usage
** Licensees holding valid CAMP Commercial licenses may use this file in
** accordance with the CAMP Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and TECHNOGERMA Systems France.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3.txt included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at contact@technogerma.fr.
**
****************************************************************************/



namespace camp
{
//-------------------------------------------------------------------------------------------------
template <typename T>
ClassBuilder<T>::ClassBuilder(Class& target)
    : m_target(&target)
    , m_currentTagHolder(0)
    , m_currentProperty(0)
    , m_currentFunction(0)
{
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename U>
ClassBuilder<T>& ClassBuilder<T>::base(const std::string& className)
{
    // Retrieve the base metaclass and its name
    const Class& baseClass = className.empty() ? classByType<U>() : classByName(className);
    std::string baseName = baseClass.name();

    // First make sure that the base class is not already a base of the current class
    std::vector<Class::BaseInfo>::const_iterator endBase = m_target->m_bases.end();
    for (std::vector<Class::BaseInfo>::const_iterator it = m_target->m_bases.begin(); it != endBase; ++it)
    {
        assert(it->base->name() != baseName);
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
    Class::PropertyTable::const_iterator endProp = baseClass.m_properties.end();
    for (Class::PropertyTable::const_iterator it = baseClass.m_properties.begin(); it != endProp; ++it)
    {
        if (m_target->m_properties.insert(*it).second)
            m_target->m_propertiesByIndex.push_back(it->second);
    }

    // Copy all functions of the base class into the current class
    Class::FunctionTable::const_iterator endFunc = baseClass.m_functions.end();
    for (Class::FunctionTable::const_iterator it = baseClass.m_functions.begin(); it != endFunc; ++it)
    {
        if (m_target->m_functions.insert(*it).second)
            m_target->m_functionsByIndex.push_back(it->second);
    }

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::property(const std::string& name, F accessor)
{
    // Find the factory which will be able to construct a camp::Property from an accessor of type F
    typedef detail::PropertyFactory1<T, F> Factory;

    // Construct and add the metaproperty
    return addProperty(Factory::get(name, accessor));
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F1, typename F2>
ClassBuilder<T>& ClassBuilder<T>::property(const std::string& name, F1 accessor1, F2 accessor2)
{
    // Find the factory which will be able to construct a camp::Property from accessors of type F1 and F2
    typedef detail::PropertyFactory2<T, F1, F2> Factory;

    // Construct and add the metaproperty
    return addProperty(Factory::get(name, accessor1, accessor2));
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F1, typename F2, typename F3>
ClassBuilder<T>& ClassBuilder<T>::property(const std::string& name, F1 accessor1, F2 accessor2, F3 accessor3)
{
    // Find the factory which will be able to construct a camp::Property from accessors of type F1, F2 and F3
    typedef detail::PropertyFactory3<T, F1, F2, F3> Factory;

    // Construct and add the metaproperty
    return addProperty(Factory::get(name, accessor1, accessor2, accessor3));
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::function(const std::string& name, F function)
{
    // Get a uniform function type from F, whatever it really is
    typedef typename boost::function_types::function_type<F>::type Signature;

    // Construct and add the metafunction
    return addFunction(new detail::FunctionImpl<Signature>(name, function));
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::function(const std::string& name, boost::function<F> function)
{
    return addFunction(new detail::FunctionImpl<F>(name, function));
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F1, typename F2>
ClassBuilder<T>& ClassBuilder<T>::function(const std::string& name, F1 function1, F2 function2)
{
    // Get uniform function types from F1 and F2, whatever they really are
    typedef typename boost::function_types::function_type<F1>::type Signature1;
    typedef typename boost::function_types::function_type<F2>::type Signature2;

    // Construct and add the metafunction
    return addFunction(new detail::FunctionImpl<Signature1, Signature2>(name, function1, function2));
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ClassBuilder<T>& ClassBuilder<T>::tag(const Value& id)
{
    return tag(id, Value::nothing);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename U>
ClassBuilder<T>& ClassBuilder<T>::tag(const Value& id, const U& value)
{
    // Make sure we have a valid tag holder, and the tag doesn't already exists
    assert(m_currentTagHolder && !m_currentTagHolder->hasTag(id));

    // For the special case of Getter<Value>, the ambiguity between both constructors
    // cannot be automatically solved, so let's do it manually
    typedef typename boost::mpl::if_c<detail::FunctionTraits<U>::isFunction, boost::function<Value (T&)>, Value>::type Type;

    // Add the new tag (override if already exists)
    m_currentTagHolder->m_tags[id] = detail::Getter<Value>(Type(value));

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ClassBuilder<T>& ClassBuilder<T>::readable(bool value)
{
    // Make sure we have a valid property
    assert(m_currentProperty != 0);

    m_currentProperty->m_readable = detail::Getter<bool>(value);

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::readable(F function)
{
    // Make sure we have a valid property
    assert(m_currentProperty != 0);

    m_currentProperty->m_readable = detail::Getter<bool>(boost::function<bool (T&)>(function));

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ClassBuilder<T>& ClassBuilder<T>::writable(bool value)
{
    // Make sure we have a valid property
    assert(m_currentProperty != 0);

    m_currentProperty->m_writable = detail::Getter<bool>(value);

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::writable(F function)
{
    // Make sure we have a valid property
    assert(m_currentProperty != 0);

    m_currentProperty->m_writable = detail::Getter<bool>(boost::function<bool (T&)>(function));

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ClassBuilder<T>& ClassBuilder<T>::callable(bool value)
{
    // Make sure we have a valid function
    assert(m_currentFunction != 0);

    m_currentFunction->m_callable = detail::Getter<bool>(value);

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::callable(F function)
{
    // Make sure we have a valid function
    assert(m_currentFunction != 0);

    m_currentFunction->m_callable = detail::Getter<bool>(boost::function<bool (T&)>(function));

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ClassBuilder<T>& ClassBuilder<T>::constructor0()
{
    Constructor* constructor = new detail::ConstructorImpl0<T>;
    m_target->m_constructors.push_back(Class::ConstructorPtr(constructor));

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename A0>
ClassBuilder<T>& ClassBuilder<T>::constructor1()
{
    Constructor* constructor = new detail::ConstructorImpl1<T, A0>;
    m_target->m_constructors.push_back(Class::ConstructorPtr(constructor));

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename A0, typename A1>
ClassBuilder<T>& ClassBuilder<T>::constructor2()
{
    Constructor* constructor = new detail::ConstructorImpl2<T, A0, A1>;
    m_target->m_constructors.push_back(Class::ConstructorPtr(constructor));

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename A0, typename A1, typename A2>
ClassBuilder<T>& ClassBuilder<T>::constructor3()
{
    Constructor* constructor = new detail::ConstructorImpl3<T, A0, A1, A2>;
    m_target->m_constructors.push_back(Class::ConstructorPtr(constructor));

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename A0, typename A1, typename A2, typename A3>
ClassBuilder<T>& ClassBuilder<T>::constructor4()
{
    Constructor* constructor = new detail::ConstructorImpl4<T, A0, A1, A2, A3>;
    m_target->m_constructors.push_back(Class::ConstructorPtr(constructor));

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename A0, typename A1, typename A2, typename A3, typename A4>
ClassBuilder<T>& ClassBuilder<T>::constructor5()
{
    Constructor* constructor = new detail::ConstructorImpl5<T, A0, A1, A2, A3, A4>;
    m_target->m_constructors.push_back(Class::ConstructorPtr(constructor));

    return *this;
}

//-------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------
template <typename T>
ClassBuilder<T>& ClassBuilder<T>::addProperty(Property* property)
{
    Class::PropertyPtr newProperty(property);
    Class::PropertyPtr& oldProperty = m_target->m_properties[property->name()];
    if (oldProperty)
    {
        Class::PropertyArray::iterator it = std::find(m_target->m_propertiesByIndex.begin(), m_target->m_propertiesByIndex.end(), oldProperty);
        *it = newProperty;
    }
    else
    {
        m_target->m_propertiesByIndex.push_back(newProperty);
    }

    oldProperty = newProperty;
    m_currentTagHolder = m_currentProperty = property;
    m_currentFunction = 0;

    return *this;
}

//-------------------------------------------------------------------------------------------------
template <typename T>
ClassBuilder<T>& ClassBuilder<T>::addFunction(Function* function)
{
    Class::FunctionPtr newFunction(function);
    Class::FunctionPtr& oldFunction = m_target->m_functions[function->name()];
    if (oldFunction)
    {
        Class::FunctionArray::iterator it = std::find(m_target->m_functionsByIndex.begin(), m_target->m_functionsByIndex.end(), oldFunction);
        if (it != m_target->m_functionsByIndex.end())
            *it = newFunction;
    }
    else
    {
        m_target->m_functionsByIndex.push_back(newFunction);
    }

    oldFunction = newFunction;
    m_currentTagHolder = m_currentFunction = function;
    m_currentProperty = 0;

    return *this;
}

} // namespace camp
