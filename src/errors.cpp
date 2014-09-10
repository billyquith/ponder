/****************************************************************************
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
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


#include <camp/errors.hpp>
#include <camp/class.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
BadType::BadType(Type provided, Type expected)
    : Error("value of type " + typeName(provided) + " couldn't be converted to type " + typeName(expected))
{
}

//-------------------------------------------------------------------------------------------------
BadType::BadType(const std::string& message)
    : Error(message)
{
}

//-------------------------------------------------------------------------------------------------
std::string BadType::typeName(Type type)
{
    switch (type)
    {
        case noType:     return "none";
        case boolType:   return "boolean";
        case intType:    return "integer";
        case realType:   return "real";
        case stringType: return "string";
        case enumType:   return "enum";
        case arrayType:  return "array";
        case userType:   return "user";
        default:         return "unknown";
    }
}

//-------------------------------------------------------------------------------------------------
BadArgument::BadArgument(Type provided, Type expected, std::size_t index, const std::string& functionName)
    : BadType("the argument #" + str(index) + " of function " + functionName +
              " couldn't be converted from type " + typeName(provided) + " to type " + typeName(expected))
{
}

//-------------------------------------------------------------------------------------------------
ClassAlreadyCreated::ClassAlreadyCreated(const std::string& name, const std::string& type)
    : Error("a metaclass named " + name + ", or bound to the type " + type + " already exists")
{
}

//-------------------------------------------------------------------------------------------------
ClassNotFound::ClassNotFound(const std::string& name)
    : Error("the metaclass " + name + " couldn't be found")
{
}

//-------------------------------------------------------------------------------------------------
ClassUnrelated::ClassUnrelated(const std::string& sourceClass, const std::string& requestedClass)
    : Error("failed to convert from " + sourceClass + " to " + requestedClass + ": it is not a base nor a derived")
{
}

//-------------------------------------------------------------------------------------------------
EnumAlreadyCreated::EnumAlreadyCreated(const std::string& name, const std::string& type)
    : Error("a metaenum named " + name + ", or bound to the type " + type + " already exists")
{
}

//-------------------------------------------------------------------------------------------------
EnumNameNotFound::EnumNameNotFound(const std::string& name, const std::string& enumName)
    : Error("the value " + name + " couldn't be found in metaenum " + enumName)
{
}

//-------------------------------------------------------------------------------------------------
EnumNotFound::EnumNotFound(const std::string& name)
    : Error("the metaenum " + name + " couldn't be found")
{
}

//-------------------------------------------------------------------------------------------------
EnumValueNotFound::EnumValueNotFound(long value, const std::string& enumName)
    : Error("the value " + str(value) + " couldn't be found in metaenum " + enumName)
{
}

//-------------------------------------------------------------------------------------------------
ForbiddenCall::ForbiddenCall(const std::string& functionName)
    : Error("the function " + functionName + " is not callable")
{
}

//-------------------------------------------------------------------------------------------------
ForbiddenRead::ForbiddenRead(const std::string& propertyName)
    : Error("the property " + propertyName + " is not readable")
{
}

//-------------------------------------------------------------------------------------------------
ForbiddenWrite::ForbiddenWrite(const std::string& propertyName)
    : Error("the property " + propertyName + " is not writable")
{
}

//-------------------------------------------------------------------------------------------------
FunctionNotFound::FunctionNotFound(const std::string& name, const std::string& className)
    : Error("the function " + name + " couldn't be found in metaclass " + className)
{
}

//-------------------------------------------------------------------------------------------------
NotEnoughArguments::NotEnoughArguments(const std::string& functionName, std::size_t provided, std::size_t expected)
    : Error("not enough arguments for calling " + functionName + " - provided " + str(provided) + ", expected " + str(expected))
{
}

//-------------------------------------------------------------------------------------------------
NullObject::NullObject(const Class* objectClass)
    : Error("trying to use a null metaobject of class " + (objectClass ? objectClass->name() : "unknown"))
{
}

//-------------------------------------------------------------------------------------------------
OutOfRange::OutOfRange(std::size_t index, std::size_t size)
    : Error("the index (" + str(index) + ") is out of the allowed range [0, " + str(size - 1) + "]")
{
}

//-------------------------------------------------------------------------------------------------
PropertyNotFound::PropertyNotFound(const std::string& name, const std::string& className)
    : Error("the property " + name + " couldn't be found in metaclass " + className)
{
}

} // namespace camp
