/****************************************************************************
**
** Copyright (C) 2009-2010 TECHNOGERMA Systems France and/or its subsidiary(-ies).
** Contact: Technogerma Systems France Information (contact@technogerma.fr)
**
** This file is part of the CAMP library.
**
** CAMP is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** CAMP is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public License
** along with CAMP.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/


#ifndef CAMP_ERROR_HPP
#define CAMP_ERROR_HPP


#include <camp/config.hpp>
#include <boost/current_function.hpp>
#include <boost/lexical_cast.hpp>
#include <exception>
#include <string>


namespace camp
{
/**
 * \brief Base class for every exception thrown in CAMP
 */
class CAMP_API Error : public std::exception
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

} // namespace camp

#include <camp/error.inl>

/**
 * \brief Trigger a CAMP error
 */
#define CAMP_ERROR(error) throw camp::Error::prepare(error, __FILE__, __LINE__, BOOST_CURRENT_FUNCTION)


#endif // CAMP_ERROR_HPP
