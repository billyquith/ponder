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


#ifndef PONDER_CONSTRUCTOR_HPP
#define PONDER_CONSTRUCTOR_HPP


namespace ponder
{
class Args;
class UserObject;

/**
 * \brief ponder::Constructor represents a metaconstructor which is used to create objects instances from metaclasses
 *
 * This class is an interface which has to be derived to implement typed constructors.
 *
 * \sa Property, Function
 */
class Constructor
{
public:

    /**
     * \brief Destructor
     */
    virtual ~Constructor() {}

    /**
     * \brief Check if the constructor matches the given set of arguments
     *
     * \param args Set of arguments to check
     *
     * \return True if the constructor is compatible with the given arguments
     */
    virtual bool matches(const Args& args) const = 0;

    /**
     * \brief Use the constructor to create a new object
     *
     * \param args Set of arguments to pass to the constructor
     *
     * \return Pointer to the new object wrapped in a UserObject, or UserObject::nothing on failure
     */
    virtual UserObject create(const Args& args) const = 0;
};

} // namespace ponder


#endif // PONDER_CONSTRUCTOR_HPP
