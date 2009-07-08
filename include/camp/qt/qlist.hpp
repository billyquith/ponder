

#ifndef CAMP_QT_QLIST_HPP
#define CAMP_QT_QLIST_HPP


#include <camp/arraymapper.hpp>
#include <QList>


namespace camp_ext
{
/*
 * \brief Specialization of ArrayMapper for QList
 */
 template <typename T>
 struct ArrayMapper<QList<T> >
 {
    typedef T ElementType;

    static bool dynamic()
    {
        return true;
    }

    static std::size_t size(const QList<T>& arr)
    {
        return static_cast<std::size_t>(arr.size());
    }

    static const T& get(const QList<T>& arr, std::size_t index)
    {
        return arr[static_cast<int>(index)];
    }

    static void set(QList<T>& arr, std::size_t index, const T& value)
    {
        arr[static_cast<int>(index)] = value;
    }

    static void insert(QList<T>& arr, std::size_t before, const T& value)
    {
        arr.insert(static_cast<int>(before), value);
    }

    static void remove(QList<T>& arr, std::size_t index)
    {
        arr.removeAt(static_cast<int>(index));
    }
};

} // namespace camp_ext


#endif // CAMP_QT_QLIST_HPP
