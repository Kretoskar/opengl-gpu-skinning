#include "OpenGL/Public/Attribute.h"
#include "Window/Public/glad.h"
#include "Math/Public/Vec2.h"
#include "Math/Public/Vec3.h"
#include "Math/Public/Vec4.h"
#include "Math/Public/Quat.h"

template Attribute<int>;
template Attribute<float>;
template Attribute<Vec2>;
template Attribute<Vec3>;
template Attribute<Vec4>;
template Attribute<IVec4>;
template Attribute<Quat>;

template <typename T>
Attribute<T>::Attribute()
{
    glGenBuffers(1, &mHandle);
    mCount = 0;
}

template <typename T>
Attribute<T>::~Attribute()
{
    glDeleteBuffers(1, &mHandle);
}

template <typename T>
unsigned int Attribute<T>::Count()
{
    return mCount;
}

template <typename T>
unsigned int Attribute<T>::GetHandle()
{
    return mHandle;
}

template <typename T>
void Attribute<T>::Set(T* inputArray, unsigned int arrayLength)
{
    mCount = arrayLength;
    unsigned int size = sizeof(T);

    glBindBuffer(GL_ARRAY_BUFFER, mHandle);
    glBufferData(GL_ARRAY_BUFFER, size * mCount, inputArray, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <typename T>
void Attribute<T>::Set(std::vector<T>& input)
{
    Set(&input[0], static_cast<unsigned>(input.size()));
}

template <>
void Attribute<int>::SetAttribPointer(unsigned int slot)
{
    glVertexAttribIPointer(slot, 1, GL_INT, 0, static_cast<void*>(0));
}

template <>
void Attribute<IVec4>::SetAttribPointer(unsigned int slot)
{
    glVertexAttribIPointer(slot, 4, GL_INT, 0, static_cast<void*>(0));
}

template <>
void Attribute<float>::SetAttribPointer(unsigned int slot)
{
    glVertexAttribPointer(slot, 1, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));
}

template <>
void Attribute<Vec2>::SetAttribPointer(unsigned int slot)
{
    glVertexAttribPointer(slot, 2, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));
}

template <>
void Attribute<Vec3>::SetAttribPointer(unsigned int slot)
{
    glVertexAttribPointer(slot, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));
}

template <>
void Attribute<Vec4>::SetAttribPointer(unsigned int slot)
{
    glVertexAttribPointer(slot, 4, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));
}

template <>
void Attribute<Quat>::SetAttribPointer(unsigned int slot)
{
    glVertexAttribPointer(slot, 4, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));
}

template <typename T>
void Attribute<T>::BindTo(unsigned int slot)
{
    glBindBuffer(GL_ARRAY_BUFFER, mHandle);
    glEnableVertexAttribArray(slot);
    SetAttribPointer(slot);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <typename T>
void Attribute<T>::UnBindFrom(unsigned int slot)
{
    glBindBuffer(GL_ARRAY_BUFFER, mHandle);
    glDisableVertexAttribArray(slot);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
