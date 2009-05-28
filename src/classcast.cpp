// <insert copyright>

#include <camp/classcast.hpp>
#include <camp/class.hpp>


namespace camp
{
//-------------------------------------------------------------------------------------------------
void* classCast(void* pointer, const Class& sourceClass, const Class& targetClass)
{
    if (sourceClass.applyOffset(pointer, targetClass))
        return pointer;
    else
        return 0;
}

} // namespace camp
