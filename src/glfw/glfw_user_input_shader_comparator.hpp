#ifndef __DEPTH_TEST_GLFW_USER_INPUT_SHADER_COMPARATOR_HPP__
#define __DEPTH_TEST_GLFW_USER_INPUT_SHADER_COMPARATOR_HPP__

#include "yaw_pitch_rotation.hpp"
#include "glfw_callback_handler_singleton.hpp"
#include "glfw_window.hpp"

namespace DepthTest {

class GLFWUserInputShaderComparator : public GLFWCallbackReceiver {

  public:

    static constexpr float SCROLL_COEFF         = 1.0f;
    static constexpr float MOUSE_DRAG_COEFF     = 1.0f;
    static constexpr float KEY_PRESS_DIST_COEFF = 10.0f;
    static constexpr float ORIENTATION_COEFF    = 0.001f;
    static constexpr float TRANSLATION_COEFF    = 0.001f;
    static constexpr float SCALING_COEFF        = 0.001f;

    typedef enum _ActiveObject {
        OBJECT_NONE,
        OBJECT_CAMERA,
        OBJECT_1,
        OBJECT_2
    } ActiveObject;

    typedef enum _ActiveOperation {
        OPERATION_NONE,
        OPERATION_ORIENT,
        OPERATION_TRANSLATE,
        OPERATION_SCALE
    } ActiveOperation;

    explicit GLFWUserInputShaderComparator( GLFWWindow& window );

    ~GLFWUserInputShaderComparator();

    void update();
    bool isUpdated() const;
    bool shouldClose() const;

    const glm::mat4& modelMatrix1() const;
    const glm::mat4& modelMatrix2() const;
    const glm::mat4& viewMatrix() const;
    const glm::vec3 modelScaling1() const;
    const glm::vec3 modelScaling2() const;

    const glm::vec4& cameraPositionWCS() const;
    std::string activeObjectStr() const;
    std::string activeOperationStr() const;

    ActiveObject activeObject() const;
    ActiveOperation activeOperation() const;

    void  cursorEnterCallback( GLFWwindow* window, int entered );
    void  scrollCallback( GLFWwindow* window, double xoffset, double yoffset );

private:

    void updateByCursorPosition();

    void updateByScroll();

    void updateByKeys();

    void resetAll();

    void updateByXYZ( const glm::vec3& xyz );
    void updateByXY( const glm::vec2& xy );
    void updateByX( const float x );
    void updateByY( const float y );
    void updateByZ( const float z );

    void updateModelMatrix1();
    void updateModelMatrix2();
    void updateViewMatrix();

    GLFWCallbackHandlerSingleton& 
                     m_callback_handler;

    GLFWWindow&      m_window;

    bool             m_updated;

    bool             m_mouse_butto_pressed;
    glm::vec2        m_cursor_pos;
    glm::vec2        m_scroll_delta_xy;

    ActiveObject     m_active_object;
    ActiveOperation  m_active_operation;

    YawPitchRotation m_rotation_object_1;
    YawPitchRotation m_rotation_object_2;
    YawPitchRotation m_rotation_camera;

    glm::vec4        m_position_object_1;
    glm::vec4        m_position_object_2;
    glm::vec4        m_position_camera;

    glm::vec2        m_scale_object_1;
    glm::vec2        m_scale_object_2;

    glm::mat4        m_model_matrix_1;
    glm::mat4        m_model_matrix_2;
    glm::mat4        m_view_matrix;
};

} // namespace DepthTest

#endif/*__DEPTH_TEST_GLFW_USER_INPUT_SHADER_COMPARATOR_HPP__*/

