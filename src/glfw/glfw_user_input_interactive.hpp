#ifndef __DEPTH_TEST_GLFW_USER_INPUT_INTERACTIVE_HPP__
#define __DEPTH_TEST_GLFW_USER_INPUT_INTERACTIVE_HPP__

#include "glfw_callback_handler_singleton.hpp"
#include "glfw_window.hpp"

namespace DepthTest {

class GLFWUserInputInteractive : public GLFWCallbackReceiver {

  public:
    static constexpr float INCREASE_BY_SCROLL    =  0.001f;
    static constexpr float DECREASE_BY_SCROLL    = -0.001f;
    static constexpr float LARGE_INCREASE_BY_KEY =  0.05f;
    static constexpr float LARGE_DECREASE_BY_KEY = -0.05f;
    static constexpr float SMALL_INCREASE_BY_KEY =  0.00005f;
    static constexpr float SMALL_DEREASE_BY_KEY  = -0.00005f;
    static constexpr float FINE_TUNE_FACTOR      = 0.01f;

    static constexpr float MIN_Z = 1.0e-25;
    static constexpr float MAX_Z = 1.0e+25;

    static constexpr float MIN_EDGE_LENGTH = 1.0e-25;
    static constexpr float MAX_EDGE_LENGTH = 1.0e+25;

    typedef enum _ActiveParam {
        NONE,
        NEAR,
        FAR,
        PARAM_C,
        PLANE_1,
        PLANE_2
    } ActiveParam;

    explicit GLFWUserInputInteractive( GLFWWindow& window );

    ~GLFWUserInputInteractive();

    void  update();
    bool  isUpdated() const;
    bool  shouldClose() const;
    float near() const;
    float far() const;
    float paramC() const;
    float plane1() const;
    float plane2() const;
    float edgeLength() const;

    ActiveParam activeParam() const;
    void  cursorEnterCallback( GLFWwindow* window, int entered );
    void  scrollCallback( GLFWwindow* window, double xoffset, double yoffset );

private:

    void updateByCursorPosition();

    void updateByScroll();

    void updateByKeys();

    void updateDepthOfActivePlane( const float delta );

    void updateEdgeLengthOfPlanes( const float delta );

    float clamp( const float v );

    GLFWCallbackHandlerSingleton& m_callback_handler;

    GLFWWindow& m_window;

    bool m_updated;

    glm::vec2 m_cursor_pos;
    glm::vec2 m_scroll_delta_xy;

    ActiveParam m_active_param;
    
    float m_z_near;
    float m_z_far;
    float m_param_c;
    float m_z_plane_1;
    float m_z_plane_2;
    float m_edge_length;
};

} // namespace DepthTest

#endif/*__DEPTH_TEST_GLFW_USER_INPUT_INTERACTIVE_HPP__*/
