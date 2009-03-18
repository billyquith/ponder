// <insert copyright>

#ifndef CAMP_QT_QVECTOR_HPP
#define CAMP_QT_QVECTOR_HPP


#include <camp/arraymapper.hpp>
#include <QVector>


namespace camp_ext
{
/*
 * \brief Specialization of ArrayMapper for QVector
 */
 template <typename T>
 struct ArrayMapper<QVector<T> >
 {
    typedef T ElementType;

    static bool dynamic()
    {
        return true;
    }

    static std::size_t size(const QVector<T>& arr)
    {
        return static_cast<std::size_t>(arr.size());
    }

    static const T& get(const QVector<T>& arr, std::size_t index)
    {
        return arr[static_cast<int>(index)];
    }

    static void set(QVector<T>& arr, std::size_t index, const T& value)
    {
        arr[static_cast<int>(index)] = value;
    }

    static void insert(QVector<T>& arr, std::size_t before, const T& value)
    {
        arr.insert(static_cast<int>(before), value);
    }

    static void remove(QVector<T>& arr, std::size_t index)
    {
        arr.remove(static_cast<int>(index));
    }
};

} // namespace camp_ext


#endif // CAMP_QT_QVECTOR_HPP
