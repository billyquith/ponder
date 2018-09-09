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
    
template <typename T>
ClassBuilder<T>::ClassBuilder(Class& target)
    : m_target(&target)
    , m_currentType(&target)
{
}

template <typename T>   // class
template <typename U>   // base
ClassBuilder<T>& ClassBuilder<T>::base()
{
    // Retrieve the base metaclass and its name
    const Class& baseClass = classByType<U>();
    IdReturn baseName = baseClass.name();

    // First make sure that the base class is not already a base of the current class
    for (Class::BaseInfo const& bi : m_target->m_bases)
    {
        if (bi.base->name() == baseName)
            PONDER_ERROR(TypeAmbiguity(bi.base->name()));
    }

    // Compute the offset to apply for pointer conversions
    // - Note we do NOT support virtual inheritance here due to the associated problems
    //   with compiler specific class layouts.
    // - Use pointer dummy buffer here as some platforms seem to trap bad memory access even
    //   though not dereferencing the pointer.
    // - U : Base, T : Derived.
    char dummy[8];
    T* asDerived = reinterpret_cast<T*>(dummy);
    U* asBase = static_cast<U*>(asDerived);
    const int offset = static_cast<int>(reinterpret_cast<char*>(asBase) -
                                        reinterpret_cast<char*>(asDerived));
    
    // Add the base metaclass to the bases of the current class
    Class::BaseInfo baseInfos;
    baseInfos.base = &baseClass;
    baseInfos.offset = offset;
    m_target->m_bases.push_back(baseInfos);

    // Copy all properties of the base class into the current class
    for (auto&& it = baseClass.m_properties.begin(); it != baseClass.m_properties.end(); ++it)
    {
        m_target->m_properties.insert(it);
    }

    // Copy all functions of the base class into the current class
    for (auto&& it = baseClass.m_functions.begin(); it != baseClass.m_functions.end(); ++it)
    {
        m_target->m_functions.insert(it);
    }

    return *this;
}

template <typename T>
template <typename F>
ClassBuilder<T>& ClassBuilder<T>::property(IdRef name, F accessor)
{
    // Find factory able to construct a Property from an accessor of type F
    typedef detail::PropertyFactory1<T, F> Factory;

    // Construct and add the metaproperty
    return addProperty(Factory::create(name, accessor));
}

template <typename T>
template <typename F1, typename F2>
ClassBuilder<T>& ClassBuilder<T>::property(IdRef name, F1 accessor1, F2 accessor2)
{
    // Find factory able to construct a Property from accessors of type F1 and F2
    typedef detail::PropertyFactory2<T, F1, F2> Factory;

    // Construct and add the metaproperty
    return addProperty(Factory::create(name, accessor1, accessor2));
}

template <typename T>
template <typename F, typename... P>
ClassBuilder<T>& ClassBuilder<T>::function(IdRef name, F function, P... policies)
{
    // Construct and add the metafunction
    return addFunction(detail::newFunction(name, function, policies...));
}

template <typename T>
template <typename... A>
ClassBuilder<T>& ClassBuilder<T>::constructor()
{
    Constructor* constructor = new detail::ConstructorImpl<T, A...>();
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

    m_currentType = property;

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

    m_currentType = function;

    return *this;
}

} // namespace ponder
