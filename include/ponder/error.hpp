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


#ifndef PONDER_ERROR_HPP
#define PONDER_ERROR_HPP


#include <ponder/config.hpp>
#include <ponder/detail/util.hpp>
#include <exception>
#include <string>


namespace ponder
{
/**
 * \brief Base class for every exception thrown in Ponder.
 */
class PONDER_API Error : public std::exception
{
public:

    /**
     * \brief Destructor
     */
    virtual ~Error() throw();

    /**
     * \brief Return a description of the error
     *
     * \return Pointer to a string containing the error message
     */
    virtual const char* what() const throw();

    /**
     * \brief Return the error location (file + line + function)
     *
     * \return String containing the error location
     */
    virtual const char* where() const throw();

    /**
     * \brief Prepare an error to be thrown
     *
     * This function is meant for internal use only. It adds
     * the current context of execution (file, line and function)
     * to the given error and returns it.
     *
     * \param error Error to prepare
     * \param file Source filename
     * \param line Line number in the source file
     * \param function Name of the function where the error was thrown
     *
     * \return Modified error, ready to be thrown
     */
    template <typename T>
    static T prepare(T error, const std::string& file, int line, const std::string& function);

protected:

    /**
     * \brief Default constructor
     *
     * \param message Error message to return in what()
     */
    Error(const std::string& message);

    /**
     * \brief Helper function to convert anything to a string
     *
     * This is a convenience function provided to help derived
     * classes to easily build their full message
     *
     * \param x Value to convert
     *
     * \return \a x converted to a string
     */
    template <typename T>
    static std::string str(T x);

private:

    std::string m_message; ///< Error message
    std::string m_location; ///< Location of the error (file, line and function)
};

} // namespace ponder

#include <ponder/error.inl>

/**
 * \brief Trigger a Ponder error
 */
#define PONDER_ERROR(error) throw ponder::Error::prepare(error, __FILE__, __LINE__, __func__)


#endif // PONDER_ERROR_HPP
