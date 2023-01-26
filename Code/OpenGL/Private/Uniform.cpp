#include "OpenGL/Public/Uniform.h"
#include "Window/Public/glad.h"
#include "Math/Public/Vec2.h"
#include "Math/Public/Vec3.h"
#include "Math/Public/Vec4.h"
#include "Math/Public/Quat.h"
#include "Math/Public/Mat4.h"

template Uniform<int>;
template Uniform<IVec4>;
template Uniform<IVec2>;
template Uniform<float>;
template Uniform<Vec2>;
template Uniform<Vec3>;
template Uniform<Vec4>;
template Uniform<Quat>;
template Uniform<Mat4>;

#define UNIFORM_IMPL(gl_func, tType, dType) \
template<> \
void Uniform<tType>::Set(unsigned int slot, tType* data, unsigned int length) { \
    gl_func(slot, (GLsizei)length, (dType*)&data[0]); \
}

UNIFORM_IMPL(glUniform1iv, int, int)
UNIFORM_IMPL(glUniform4iv, IVec4, int)
UNIFORM_IMPL(glUniform2iv, IVec2, int)
UNIFORM_IMPL(glUniform1fv, float, float)
UNIFORM_IMPL(glUniform2fv, Vec2, float)
UNIFORM_IMPL(glUniform3fv, Vec3, float)
UNIFORM_IMPL(glUniform4fv, Vec4, float)
UNIFORM_IMPL(glUniform4fv, Quat, float)

template <>
void Uniform<Mat4>::Set(unsigned int slot, Mat4* inputArray, unsigned int arrayLength)
{
    glUniformMatrix4fv(slot, static_cast<GLsizei>(arrayLength), false, (float*)&inputArray[0]);
}

template <typename T>
void Uniform<T>::Set(unsigned int slot, const T& value)
{
    Set(slot, (T*)(&value), 1);
}

template <typename T>
void Uniform<T>::Set(unsigned int slot, std::vector<T>& value)
{
    Set(slot, &value[0], static_cast<unsigned>(value.size()));
}
