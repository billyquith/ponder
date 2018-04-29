/****************************************************************************
**
** This file is part of the Ponder library.
**
** The MIT License (MIT)
**
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

#ifndef PONDER_USES_SERIALISE_HPP
#define PONDER_USES_SERIALISE_HPP

#include <ponder/class.hpp>

namespace ponder {
namespace uses {

namespace detail {



} // namespace detail

    
//class ArchiveWriter
//{
//public:
//    void write(const UserObject& obj)
//    {
//
//    }
//};

    
namespace archive {
    
    enum class ValueKind
    {
        UserObject
    };
    
    template <typename ARCHIVE_TRAITS>
    class Archive
    {
    public:
        
        using archive_traits_t = ARCHIVE_TRAITS;
        
        typename archive_traits_t::item_t beginItem(ValueKind vk)
        {
            typename archive_traits_t::item_t
        }
        
    };

    template <class ARCHIVE>
    class ObjectWriter
    {
    public:
        
        using archive_t = ARCHIVE;
        
        ObjectWriter(archive_t& archive)
        :   m_archive(archive)
        {}
        
        void write(const UserObject& obj)
        {
            auto& item = m_archive.beginItem(ValueKind::UserObject);
            item.setValue(obj);
            m_archive.endItem(item);
        }
        
    private:
        
        archive_t m_archive;

    };
    
} // namespace detail

    


/**
 * \brief Save object state to an archive.
 */
template <class WRITER>
PONDER_API void serialise(WRITER& writer, const UserObject& root);

/**
 * \brief Restore object state from an archive.
 */
template <class READER>
PONDER_API void deserialise(READER& archive, const UserObject& root);



template <class ARCHIVE>
inline void serialise(ARCHIVE& archive, const UserObject& obj)
{
    const Class& cls = obj.getClass();
    //for (auto const& prop : cls.)
}



} // namespace uses
} // namespace ponder

#endif // PONDER_USES_SERIALISE_HPP
