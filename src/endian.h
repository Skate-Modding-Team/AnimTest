#include <cstdint>
#include <cstring>
#include <rwmath.h>

inline uint16_t Convert(const uint16_t x)
{
    return(
        (uint16_t)
        (((x & 0xff) << 8) |
            ((x >> 8) & 0xff)));
}

inline uint32_t Convert(const uint32_t x)
{
    return(
        ((x & 0xff) << 24) |
        ((x & 0xff00) << 8) |
        ((x >> 8) & 0xff00) |
        ((x >> 24) & 0xff));
}

inline uint64_t Convert(const uint64_t x)
{
    return(
        ((x & 0xff) << 56) |
        ((x & 0xff00) << 40) |
        ((x & 0xff0000) << 24) |
        ((x & 0xff000000) << 8) |
        ((x >> 8) & 0xff000000) |
        ((x >> 24) & 0xff0000) |
        ((x >> 40) & 0xff00) |
        ((x >> 56) & 0xff));
}

inline int16_t Convert(const int16_t x)
{
    return(
        (int16_t)
        (((x & 0xff) << 8) |
            ((x >> 8) & 0xff)));
}

inline int32_t Convert(const int32_t x)
{
    return(
        ((x & 0xff) << 24) |
        ((x & 0xff00) << 8) |
        ((x >> 8) & 0xff00) |
        ((x >> 24) & 0xff));
}

inline int64_t Convert(const int64_t x)
{
    return(
        ((x & 0xff) << 56) |
        ((x & 0xff00) << 40) |
        ((x & 0xff0000) << 24) |
        ((x & 0xff000000) << 8) |
        ((x >> 8) & 0xff000000) |
        ((x >> 24) & 0xff0000) |
        ((x >> 40) & 0xff00) |
        ((x >> 56) & 0xff));
}

inline float Convert(const float x)
{
    uint32_t temp;
    std::memcpy(&temp, &x, sizeof(temp));
    temp = Convert(temp);
    float result;
    std::memcpy(&result, &temp, sizeof(result));
    return result;
}

inline rw::math::vpu::Quaternion Convert(const rw::math::vpu::Quaternion quat)
{
    rw::math::vpu::Quaternion result;
    result.SetX(Convert(quat.GetX()));
    result.SetY(Convert(quat.GetY()));
    result.SetZ(Convert(quat.GetZ()));
    result.SetW(Convert(quat.GetW()));
    return result;
}

inline rw::math::vpu::Vector4 Convert(const rw::math::vpu::Vector4 quat)
{
    rw::math::vpu::Vector4 result;
    result.SetX(Convert(quat.GetX()));
    result.SetY(Convert(quat.GetY()));
    result.SetZ(Convert(quat.GetZ()));
    result.SetW(Convert(quat.GetW()));
    return result;
}

inline rw::math::vpu::Vector3 Convert(const rw::math::vpu::Vector3 quat)
{
    rw::math::vpu::Vector3 result;
    result.SetX(Convert(quat.GetX()));
    result.SetY(Convert(quat.GetY()));
    result.SetZ(Convert(quat.GetZ()));
    return result;
}