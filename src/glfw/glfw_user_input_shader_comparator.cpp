#include <iostream>
#include <GLFW/glfw3.h>

#include "glfw_user_input_shader_comparator.hpp"
#include "debug_print.hpp"

namespace DepthTest {

GLFWUserInputShaderComparator::GLFWUserInputShaderComparator( GLFWWindow& window )
    :m_window             { window }
    ,m_callback_handler
    {
        GLFWCallbackHandlerSingleton::getInstance()
    }
    ,m_updated            { false }
    ,m_mouse_butto_pressed{ false }
    ,m_cursor_pos         { 0.0f, 0.0f }
    ,m_scroll_delta_xy    { 0.0f, 0.0f }
    ,m_active_object      { OBJECT_NONE }
    ,m_active_operation   { OPERATION_NONE }
    ,m_rotation_object_1  { 0.0f, 0.0f }
    ,m_rotation_object_2  { 0.0f, 0.0f }
    ,m_rotation_camera    { 0.0f, 0.0f }
    ,m_position_object_1  { 0.0f, 0.0f, 0.0f, 1.0f }
    ,m_position_object_2  { 0.0f, 0.0f, 0.0f, 1.0f }
    ,m_position_camera    { 0.0f, 0.0f, 7.0f, 1.0f }
    ,m_scale_object_1     { 1.0f, 1.0f }
    ,m_scale_object_2     { 1.0f, 1.0f }
{
    updateModelMatrix1();

    updateModelMatrix2();

    updateViewMatrix();
}    

GLFWUserInputShaderComparator::~GLFWUserInputShaderComparator()
{
}

void GLFWUserInputShaderComparator::cursorEnterCallback( GLFWwindow* window, int entered )
{

}

void GLFWUserInputShaderComparator::scrollCallback(
    GLFWwindow* window, 
    double      xoffset, 
    double      yoffset
) {
    const glm::vec2 delta_xy {
        static_cast<float>( xoffset ),
        static_cast<float>( yoffset )
    };
    m_scroll_delta_xy = delta_xy;
}


bool GLFWUserInputShaderComparator::isUpdated() const {
    return m_updated;
}

bool GLFWUserInputShaderComparator::shouldClose() const
{
    return glfwGetKey( m_window.window(), GLFW_KEY_ESCAPE ) == GLFW_PRESS;
}

const glm::mat4& GLFWUserInputShaderComparator::modelMatrix1() const
{
    return m_model_matrix_1;
}

const glm::mat4& GLFWUserInputShaderComparator::modelMatrix2() const
{
    return m_model_matrix_2;
}

const glm::mat4& GLFWUserInputShaderComparator::viewMatrix() const
{
    return m_view_matrix;
}

const glm::vec3 GLFWUserInputShaderComparator::modelScaling1() const
{
    return glm::vec3{
        m_scale_object_1.x,
        m_scale_object_1.y,
        m_scale_object_1.y
    };
}

const glm::vec3 GLFWUserInputShaderComparator::modelScaling2() const
{
    return glm::vec3{
        m_scale_object_2.x,
        m_scale_object_2.y,
        m_scale_object_2.y 
    };
}


const glm::vec4& GLFWUserInputShaderComparator::cameraPositionWCS() const
{
    return m_position_camera;
}

std::string GLFWUserInputShaderComparator::activeObjectStr() const
{
    switch( m_active_object ) {

      case OBJECT_1:

        return std::string( "object 1(red)" );

      case OBJECT_2:

        return std::string( "object 2(blue)" );

      case OBJECT_CAMERA:

        return std::string( "camera" );

      default:

        return std::string( "none" );
    }
}

std::string GLFWUserInputShaderComparator::activeOperationStr() const
{
    switch ( m_active_operation ) {

      case OPERATION_ORIENT: 

        return std::string( "orient" );

      case OPERATION_TRANSLATE:

        return std::string( "translate" );

      case OPERATION_SCALE:

        if ( m_active_object != OBJECT_CAMERA ) {

            return std::string( "scale" );
        }
        else {
            return std::string( "scale (not supported for camera)" );
        }

      default:

        return std::string( "none" );
    }
}

GLFWUserInputShaderComparator::ActiveObject GLFWUserInputShaderComparator::activeObject() const
{
    return m_active_object;
}

GLFWUserInputShaderComparator::ActiveOperation GLFWUserInputShaderComparator::activeOperation() const
{
    return m_active_operation;
}

void GLFWUserInputShaderComparator::updateByScroll()
{
    if ( m_scroll_delta_xy.y > 0.0f ) {

        m_updated = true;

        updateByXY( m_scroll_delta_xy * SCROLL_COEFF * -1.0f );
    }
    else if ( m_scroll_delta_xy.y < 0.0f ) {

        m_updated = true;

        updateByXY( m_scroll_delta_xy * SCROLL_COEFF * -1.0f );
    }
    m_scroll_delta_xy = glm::vec2{ 0.0f, 0.0f };
}


void GLFWUserInputShaderComparator::updateByCursorPosition()
{
    int state = glfwGetMouseButton( m_window.window(), GLFW_MOUSE_BUTTON_LEFT);
    if ( state != GLFW_PRESS )
    {
        m_mouse_butto_pressed = false;
        return;
    }

    double xpos, ypos;
    glfwGetCursorPos( m_window.window(), &xpos, &ypos);

    const glm::vec2 ui_pos{ static_cast<float>( xpos ), static_cast<float>( ypos ) };
    const auto cursor_pos = m_window.convertToFrameBufferCoordinates( ui_pos );

    if ( m_mouse_butto_pressed ) {

        m_updated = true;

        const auto diff = cursor_pos - m_cursor_pos;

        updateByXY( diff * MOUSE_DRAG_COEFF );
    }
    m_cursor_pos = cursor_pos;
    m_mouse_butto_pressed = true;
}

void GLFWUserInputShaderComparator::updateByKeys()
{
    if ( glfwGetKey( m_window.window(), GLFW_KEY_LEFT ) == GLFW_PRESS ) {

        m_updated = true;
        updateByX( KEY_PRESS_DIST_COEFF * -1.0f );
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_RIGHT ) == GLFW_PRESS ) {

        m_updated = true;
        updateByX( KEY_PRESS_DIST_COEFF );
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_UP ) == GLFW_PRESS ) {

        m_updated = true;
        updateByY( KEY_PRESS_DIST_COEFF );
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_DOWN ) == GLFW_PRESS ) {

        m_updated = true;
        updateByY( KEY_PRESS_DIST_COEFF * -1.0f );
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_1 ) == GLFW_PRESS ) {

        m_updated = true;
        m_active_object = OBJECT_1;
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_2 ) == GLFW_PRESS ) {

        m_updated = true;
        m_active_object = OBJECT_2;
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_C ) == GLFW_PRESS ) {

        m_updated = true;
        m_active_object = OBJECT_CAMERA;
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_O ) == GLFW_PRESS ) {

        m_updated = true;
        m_active_operation = OPERATION_ORIENT;
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_T ) == GLFW_PRESS ) {

        m_updated = true;
        m_active_operation = OPERATION_TRANSLATE;
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_S ) == GLFW_PRESS ) {

        m_updated = true;
        m_active_operation = OPERATION_SCALE;
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_Z ) == GLFW_PRESS ) {

        m_updated = true;
        updateByZ( KEY_PRESS_DIST_COEFF * -1.0f );
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_X ) == GLFW_PRESS ) {

        m_updated = true;
        updateByZ( KEY_PRESS_DIST_COEFF );
    }
    else if ( glfwGetKey( m_window.window(), GLFW_KEY_R ) == GLFW_PRESS ) {

        m_updated = true;
        resetAll();
    }
}

void GLFWUserInputShaderComparator::update()
{
    m_updated = false;

    updateByScroll();

    updateByCursorPosition();

    updateByKeys();
}

void GLFWUserInputShaderComparator::updateByX( const float x )
{
    updateByXYZ( glm::vec3{ x, 0.0f, 0.0f } );
}

void GLFWUserInputShaderComparator::updateByY( const float y )
{
    updateByXYZ( glm::vec3{ 0.0f, y, 0.0f } );
}

void GLFWUserInputShaderComparator::updateByZ( const float z )
{
    updateByXYZ( glm::vec3{ 0.0f, 0.0f, z } );
}

void GLFWUserInputShaderComparator::updateByXY( const glm::vec2& xy )
{
    updateByXYZ( glm::vec3{ xy.x, xy.y, 0.0f } );
}

void GLFWUserInputShaderComparator::updateByXYZ( const glm::vec3& xyz )
{
    const auto xy = glm::vec2{ xyz.x, xyz.y } * ORIENTATION_COEFF;
    const auto xyzw = glm::vec4{ xyz.x, xyz.y, xyz.z, 0.0f } * TRANSLATION_COEFF;

    const auto camera_xyz  = ( m_rotation_camera.getRotMatrix() * xyz ) * TRANSLATION_COEFF;
    const auto camera_xyzw = glm::vec4{ camera_xyz.x,  camera_xyz.y,  camera_xyz.z, 0.0f };
    switch ( m_active_operation ) {

      case OPERATION_ORIENT: 

        switch( m_active_object ) {

            case OBJECT_1:
              m_rotation_object_1.updateRotation( xy );
              updateModelMatrix1();
              break;

            case OBJECT_2:
              m_rotation_object_2.updateRotation( xy );
              updateModelMatrix2();
              break;

            case OBJECT_CAMERA:
              m_rotation_camera.updateRotation( xy );
              updateViewMatrix();
              break;

            default:
              ;
        }
        break;

      case OPERATION_TRANSLATE:

        switch( m_active_object ) {

            case OBJECT_1:
              m_position_object_1 += xyzw;
              updateModelMatrix1();
              break;

            case OBJECT_2:
              m_position_object_2 += xyzw;
              updateModelMatrix2();
              break;

            case OBJECT_CAMERA:

              m_position_camera += camera_xyzw;
              updateViewMatrix();
              break;

            default:
              ;
        }
        break;

      case OPERATION_SCALE:

        switch( m_active_object ) {

            case OBJECT_1:

              m_scale_object_1.x *= ( 1.0f + xyz.x * SCALING_COEFF );
              m_scale_object_1.y *= ( 1.0f + xyz.y * SCALING_COEFF );
              updateModelMatrix1();
              break;

            case OBJECT_2:

              m_scale_object_2.x *= ( 1.0f + xyz.x * SCALING_COEFF );
              m_scale_object_2.y *= ( 1.0f + xyz.y * SCALING_COEFF );
              updateModelMatrix2();
              break;

            default:
              ;
        }
        break;

      default:
        ;
    }
}

void GLFWUserInputShaderComparator::updateModelMatrix1()
{
    m_model_matrix_1 = m_rotation_object_1.getRotMatrix();
    m_model_matrix_1[3] = m_position_object_1;
}

void GLFWUserInputShaderComparator::updateModelMatrix2()
{
    m_model_matrix_2 = m_rotation_object_2.getRotMatrix();
    m_model_matrix_2[3] = m_position_object_2;
}

void GLFWUserInputShaderComparator::updateViewMatrix()
{
    glm::mat3 Rinv = m_rotation_camera.getInvRotMatrix();
    glm::vec3 Rinv_t = ( Rinv * m_position_camera ) * -1.0f;
    m_view_matrix = Rinv;
    m_view_matrix[3] = glm::vec4{ Rinv_t.x, Rinv_t.y, Rinv_t.z, 1.0f };
}

void GLFWUserInputShaderComparator::resetAll()
{

    m_rotation_object_1.setRotation( 0.0f, 0.0f );
    m_rotation_object_2.setRotation( 0.0f, 0.0f );
    m_rotation_camera.  setRotation( 0.0f, 0.0f );
    m_position_object_1 = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
    m_position_object_2 = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
    m_position_camera   = glm::vec4{ 0.0f, 0.0f, 7.0f, 1.0f };
    m_scale_object_1    = glm::vec2{ 1.0f, 1.0f };
    m_scale_object_2    = glm::vec2{ 1.0f, 1.0f };

    updateModelMatrix1();

    updateModelMatrix2();

    updateViewMatrix();
}


} // namespace DepthTest
