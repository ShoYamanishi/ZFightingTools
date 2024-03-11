#ifndef __DEPTH_TEST_YAW_PITCH_ROTATION_HPP__
#define __DEPTH_TEST_YAW_PITCH_ROTATION_HPP__

#include <iostream>
#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/norm.hpp>

namespace DepthTest {

// product of two quaternions. (not the simple dot product of vec4)

inline glm::quat operator*( const glm::quat& lhs, const glm::quat& rhs)
{
    const glm::vec3 lhs_xyz{ lhs.x, lhs.y, lhs.z };
    const glm::vec3 rhs_xyz{ rhs.x, rhs.y, rhs.z };

    auto  out_w = lhs.w * rhs.w - glm::dot( lhs_xyz, rhs_xyz );

    const auto out_xyz = lhs_xyz * rhs.w
                       + rhs_xyz * lhs.w
                       + glm::cross( lhs_xyz, rhs_xyz );

    // The PI (180) duality is solved in favor of (1,0) to avoid ambiguity.
    if ( out_w == -1.0f && out_xyz == glm::vec3{ 0.0f, 0.0f, 0.0f } ) {

        out_w = 1.0f;
    }

    return glm::quat( out_w, out_xyz );
}

// Assumes the pitch axis is in the xy-plane and the rotation axis is the z-axis.
class YawPitchRotation {

public:
    static constexpr float EPSILON = 1.0e-6;

    YawPitchRotation() noexcept
        :m_euler_angle_z  { 0.0f }
        ,m_euler_angle_xy { 0.0f }
    {
        updateMatrices();
    }

    YawPitchRotation( const float euler_angle_z, const float euler_angle_xy ) noexcept
        :m_euler_angle_z  { euler_angle_z }
        ,m_euler_angle_xy { euler_angle_xy }
    {
        updateMatrices();
    }

    /**
     * the UI specifies the orientation in two euler angles.
     *    1. rotation around Z-axis, i.e., (0,0,1) by angle_z.
     *    2. rotation around post-transformed X-axis, i.e. (cos(0.5*angle_z), sin(0.5*angle_z), 0)
     *       by angle_xy.
     */
    void updateRotation( const glm::vec2& delta )
    {
        m_euler_angle_z  = getAdjustedAngle( m_euler_angle_z  + delta.x );
        m_euler_angle_xy = getAdjustedAngle( m_euler_angle_xy + delta.y );

        updateMatrices();
    }

    void setRotation( const float euler_angle_z, const float euler_angle_xy )
    {
        m_euler_angle_z  = getAdjustedAngle( euler_angle_z  );
        m_euler_angle_xy = getAdjustedAngle( euler_angle_xy );

        updateMatrices();
    }

    void updateMatrices()
    {
        m_q = getQuaternionFromEulerAnglesZandXY( glm::vec2{ m_euler_angle_z, m_euler_angle_xy } );

        m_m = glm::mat3_cast( m_q );

        m_inv = glm::transpose( m_m );// inverse
    }

    const glm::mat3& getRotMatrix() const {
        return m_m;
    }

    const glm::mat3& getInvRotMatrix() const {
        return m_inv;
    }

private:

    glm::quat getQuaternionFromEulerAnglesZandXY( const glm::vec2& rad_z_xy )
    {
        const float rad_z  = rad_z_xy.x;
        const float rad_xy = rad_z_xy.y;

        // rotate around z-axis by m_rad_z.
        const glm::quat q_z{ std::cos( rad_z * 0.5f ), 0.0f, 0.0f, std::sin( rad_z * 0.5f) };

        // rotate around x-axis by m_rad_xy.
        const glm::quat q_xy{ std::cos( rad_xy * 0.5f ), std::sin( rad_xy * 0.5f ), 0.0f, 0.0f };

        return glm::normalize( q_xy * q_z ); // composite rotation by q_z and then q_xy;
    }

    float getAdjustedAngle( const float rad )
    {
        auto val = rad;

        // adjust the range in [ -PI, PI )
        while( val >= M_PI ) {

            val -= (2.0f * M_PI);
        }

        while( val < 0.0f ) {

            val += (2.0f * M_PI);
        }
        return val;
    }

    glm::quat m_q;
    glm::mat3 m_m;
    glm::mat3 m_inv;

    float m_euler_angle_z;
    float m_euler_angle_xy;
};

} //namespace DepthTest

#endif/*__DEPTH_TEST_YAW_PITCH_ROTATION_HPP__*/
