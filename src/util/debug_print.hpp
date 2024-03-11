#ifndef __DEPTH_TEST_DEBUG_PRINT_HPP__
#define __DEPTH_TEST_DEBUG_PRINT_HPP__

#include <iostream>
#include <glm/glm.hpp>

inline std::ostream& operator << ( std::ostream& os, const glm::vec2& v)
{
    os << "( " << v.x << ", " << v.y << " )";
    return os;
}

inline std::ostream& operator << ( std::ostream& os, const glm::ivec2& v)
{
    os << "( " << v.x << ", " << v.y << " )";
    return os;
}

inline std::ostream& operator << ( std::ostream& os, const glm::vec3& v)
{
    os << "( " << v.x << ", " << v.y << ", " << v.z << " )";
    return os;
}

inline std::ostream& operator << ( std::ostream& os, const glm::ivec3& v)
{
    os << "( " << v.x << ", " << v.y << ", " << v.z << " )";
    return os;
}

inline std::ostream& operator << ( std::ostream& os, const glm::vec4& v)
{
    if ( std::abs(v.w) < 1.0e-8 ) {
        os << "( " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " )";
    }
    else {
        os << "[normalized]( " << v.x/v.w << ", " << v.y/v.w << ", " << v.z/v.w << " )";
    }
    return os;
}

inline std::ostream& operator << ( std::ostream& os, const glm::ivec4& v)
{
    os << "( " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " )";
    return os;
}

inline std::ostream& operator << ( std::ostream& os, const glm::mat3& v)
{
    os << "\n";
    os << "[ " << v[0][0] << "\t" << v[1][0] << "\t" << v[2][0] << "]\n";
    os << "[ " << v[0][1] << "\t" << v[1][1] << "\t" << v[2][1] << "]\n";
    os << "[ " << v[0][2] << "\t" << v[1][2] << "\t" << v[2][2] << "]\n";
    return os;
}

inline std::ostream& operator << ( std::ostream& os, const glm::mat4& v)
{
    os << "\n";
    os << "[ " << v[0][0] << "\t" << v[1][0] << "\t" << v[2][0] << "\t" << v[3][0] << "]\n";
    os << "[ " << v[0][1] << "\t" << v[1][1] << "\t" << v[2][1] << "\t" << v[3][1] << "]\n";
    os << "[ " << v[0][2] << "\t" << v[1][2] << "\t" << v[2][2] << "\t" << v[3][2] << "]\n";
    os << "[ " << v[0][3] << "\t" << v[1][3] << "\t" << v[2][3] << "\t" << v[3][3] << "]\n";

    return os;
}


namespace DepthTest {

inline std::ostream& operator << ( std::ostream& os, const glm::vec2& v)
{
    os << "( " << v.x << ", " << v.y << " )";
    return os;
}

inline std::ostream& operator << ( std::ostream& os, const glm::ivec2& v)
{
    os << "( " << v.x << ", " << v.y << " )";
    return os;
}

inline std::ostream& operator << ( std::ostream& os, const glm::vec3& v)
{
    os << "( " << v.x << ", " << v.y << ", " << v.z << " )";
    return os;
}

inline std::ostream& operator << ( std::ostream& os, const glm::ivec3& v)
{
    os << "( " << v.x << ", " << v.y << ", " << v.z << " )";
    return os;
}

inline std::ostream& operator << ( std::ostream& os, const glm::vec4& v)
{
    if ( std::abs(v.w) < 1.0e-8 ) {
        os << "( " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " )";
    }
    else {
        os << "[normalized]( " << v.x/v.w << ", " << v.y/v.w << ", " << v.z/v.w << " )";
    }
    return os;
}

inline std::ostream& operator << ( std::ostream& os, const glm::ivec4& v)
{
    os << "( " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " )";
    return os;
}

inline std::ostream& operator << ( std::ostream& os, const glm::mat3& v)
{
    os << "\n";
    os << "[ " << v[0][0] << "\t" << v[1][0] << "\t" << v[2][0] << "]\n";
    os << "[ " << v[0][1] << "\t" << v[1][1] << "\t" << v[2][1] << "]\n";
    os << "[ " << v[0][2] << "\t" << v[1][2] << "\t" << v[2][2] << "]\n";
    return os;
}

inline std::ostream& operator << ( std::ostream& os, const glm::mat4& v)
{
    os << "\n";
    os << "[ " << v[0][0] << "\t" << v[1][0] << "\t" << v[2][0] << "\t" << v[3][0] << "]\n";
    os << "[ " << v[0][1] << "\t" << v[1][1] << "\t" << v[2][1] << "\t" << v[3][1] << "]\n";
    os << "[ " << v[0][2] << "\t" << v[1][2] << "\t" << v[2][2] << "\t" << v[3][2] << "]\n";
    os << "[ " << v[0][3] << "\t" << v[1][3] << "\t" << v[2][3] << "\t" << v[3][3] << "]\n";

    return os;
}

} // namespace DepthTest


#endif/*__DEPTH_TEST_DEBUG_PRINT_HPP__*/
