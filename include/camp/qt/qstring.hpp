// <insert copyright>

#ifndef CAMP_QT_QSTRING_HPP
#define CAMP_QT_QSTRING_HPP


#include <camp/valuemapper.hpp>
#include <QString>
#include <string>


namespace camp_ext
{
/**
 * \brief Value mapper for QString
 *
 * This class tells CAMP how to handle QString as a string type
 */
template <>
struct ValueMapper<QString>
{
    /**
     * \brief CAMP type associated to QString
     */
    static const int type = camp::stringType;

    /**
     * \brief Convert from QString to std::string
     *
     * \param source Source QString instance to convert
     *
     * \return \a source converted to a std::string
     */
    static std::string to(const QString& source)
    {
        return source.toStdString();
    }

    /**
     * \brief Convert from any type to QString
     *
     * We simply reuse ValueMapper<std::string>.
     *
     * \param source Source value to convert
     *
     * \return \a source converted to a QString
     */
    template <typename T>
    static QString from(const T& source)
    {
        return QString::fromStdString(ValueMapper<std::string>::from(source));
    }
};

} // namespace camp_ext


#endif // CAMP_QT_QSTRING_HPP
