#include <iostream>
#include <GLFW/glfw3.h>

#include "glfw_user_input_interactive.hpp"

namespace DepthTest {

GLFWUserInputInteractive::GLFWUserInputInteractive( GLFWWindow& window )
    :m_window      { window }
    ,m_callback_handler
    {
        GLFWCallbackHandlerSingleton::getInstance()
    }
    ,m_updated         { false }
    ,m_cursor_pos      { 0.0f, 0.0f }
    ,m_scroll_delta_xy { 0.0f, 0.0f }
    ,m_active_param    { NONE }
    ,m_z_near          { 1.0e-1f }
    ,m_z_far           { 1.0e4f }
    ,m_param_c         { 1.0e-3 }
    ,m_z_plane_1       { 0.9f }
    ,m_z_plane_2       { 0.8f }
    ,m_edge_length     { 0.1f }
{
    m_callback_handler.registerHandler( m_window.window(), this );
}

GLFWUserInputInteractive::~GLFWUserInputInteractive()
{
    m_callback_handler.unregisterHandler( m_window.window(), this );
}

void GLFWUserInputInteractive::update()
{
    m_updated = false;

    updateByKeys();
    updateByCursorPosition();
    updateByScroll();
}

bool GLFWUserInputInteractive::isUpdated() const {
    return m_updated;
}

bool GLFWUserInputInteractive::shouldClose() const
{
    return glfwGetKey( m_window.window(), GLFW_KEY_ESCAPE ) == GLFW_PRESS;
}

float GLFWUserInputInteractive::near() const {
    return m_z_near;
}

float GLFWUserInputInteractive::far() const {
    return m_z_far;
}

float GLFWUserInputInteractive::plane1() const {
    return m_z_plane_1;
}

float GLFWUserInputInteractive::plane2() const {
    return m_z_plane_2;
}

float GLFWUserInputInteractive::paramC() const {
    return m_param_c;
}

float GLFWUserInputInteractive::edgeLength() const {
    return m_edge_length;
}

GLFWUserInputInteractive::ActiveParam GLFWUserInputInteractive::activeParam() const {
    return m_active_param;
}

void GLFWUserInputInteractive::cursorEnterCallback( GLFWwindow* window, int entered )
{

}

void GLFWUserInputInteractive::scrollCallback( GLFWwindow* window, double xoffset, double yoffset )
{
    const glm::vec2 delta_xy {
        static_cast<float>( xoffset ),
        static_cast<float>( yoffset )
    };
    m_scroll_delta_xy = delta_xy;
}

void GLFWUserInputInteractive::updateByCursorPosition()
{
    double xpos, ypos;
    glfwGetCursorPos( m_window.window(), &xpos, &ypos);

    const glm::vec2 ui_pos{ static_cast<float>( xpos ), static_cast<float>( ypos ) };
    m_cursor_pos = m_window.convertToFrameBufferCoordinates( ui_pos );
}

void GLFWUserInputInteractive::updateByScroll()
{
    if ( m_scroll_delta_xy.y > 0.0f ) {

        m_updated = true;
        updateDepthOfActivePlane( std::abs( m_scroll_delta_xy.y ) * DECREASE_BY_SCROLL );
    }
    else if ( m_scroll_delta_xy.y < 0.0f ) {

        m_updated = true;
        updateDepthOfActivePlane( std::abs( m_scroll_delta_xy.y ) * INCREASE_BY_SCROLL );
    }

    m_scroll_delta_xy = glm::vec2{ 0.0f, 0.0f };
}

void GLFWUserInputInteractive::updateByKeys()
{
    float multiplier = 1.0f;

    if (   glfwGetKey( m_window.window(), GLFW_KEY_LEFT_SHIFT  ) == GLFW_PRESS 
        || glfwGetKey( m_window.window(), GLFW_KEY_RIGHT_SHIFT ) == GLFW_PRESS
    ) {
        multiplier = FINE_TUNE_FACTOR;
    }

    if ( glfwGetKey( m_window.window(), GLFW_KEY_LEFT ) == GLFW_PRESS ) {

        m_updated = true;
        updateDepthOfActivePlane( SMALL_DEREASE_BY_KEY * multiplier );
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_RIGHT ) == GLFW_PRESS ) {

        m_updated = true;
        updateDepthOfActivePlane( SMALL_INCREASE_BY_KEY * multiplier );
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_UP ) == GLFW_PRESS ) {

        m_updated = true;
        updateDepthOfActivePlane( LARGE_INCREASE_BY_KEY * multiplier );
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_DOWN ) == GLFW_PRESS ) {

        m_updated = true;
        updateDepthOfActivePlane( LARGE_DECREASE_BY_KEY * multiplier );
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_1 ) == GLFW_PRESS ) {

        m_updated = true;
        m_active_param = PLANE_1;
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_2 ) == GLFW_PRESS ) {

        m_updated = true;
        m_active_param = PLANE_2;
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_N ) == GLFW_PRESS ) {

        m_updated = true;
        m_active_param = NEAR;
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_F ) == GLFW_PRESS ) {

        m_updated = true;
        m_active_param = FAR;	  
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_C ) == GLFW_PRESS ) {

        m_updated = true;
        m_active_param = PARAM_C;	  
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_Z ) == GLFW_PRESS ) {

        m_updated = true;
        updateEdgeLengthOfPlanes( LARGE_DECREASE_BY_KEY * multiplier );
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_X ) == GLFW_PRESS ) {

        m_updated = true;
        updateEdgeLengthOfPlanes( LARGE_INCREASE_BY_KEY * multiplier );
    }
}

void GLFWUserInputInteractive::updateDepthOfActivePlane( const float delta ) {
 
    const float coeff = std::max( 0.0f, 1.0f + delta );
    switch ( m_active_param ) {

      case NEAR:
        m_z_near = clamp( m_z_near * coeff );
        m_z_near = std::min( m_z_near, m_z_far );
        break;

      case FAR:
        m_z_far = clamp( m_z_far * coeff );
        m_z_far = std::max( m_z_near, m_z_far );
        break;

      case PARAM_C:
        m_param_c = clamp( m_param_c * coeff );
        break;

      case PLANE_1:
        m_z_plane_1 = clamp( m_z_plane_1 * coeff );
        m_z_plane_1 = std::max( m_z_plane_1, m_z_near );
        m_z_plane_1 = std::min( m_z_plane_1, m_z_far );
        break;

      case PLANE_2:
        m_z_plane_2 = clamp( m_z_plane_2 * coeff );
        m_z_plane_2 = std::max( m_z_plane_2, m_z_near );
        m_z_plane_2 = std::min( m_z_plane_2, m_z_far );
        break;

      default:
        break;
    }
}

void GLFWUserInputInteractive::updateEdgeLengthOfPlanes( const float delta )
{
    const float coeff = std::max( 0.0f, 1.0f + delta );
    m_edge_length *= coeff;
    m_edge_length = std::max( MIN_EDGE_LENGTH, m_edge_length );
    m_edge_length = std::min( MAX_EDGE_LENGTH, m_edge_length );
}

float GLFWUserInputInteractive::clamp( const float v ) {
    return std::min( MAX_Z, std::max( MIN_Z, v ) );
}

} // namespace DepthTest
