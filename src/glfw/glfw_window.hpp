#ifndef __DEPTH_TEST_GLFW_WINDOW_HPP__
#define __DEPTH_TEST_GLFW_WINDOW_HPP__

#include <glm/glm.hpp>

#include "glfw_callback_handler_singleton.hpp"

namespace DepthTest {

class GLFWWindow : public GLFWCallbackReceiver {

  public:
    explicit GLFWWindow(
        const int   ui_width,
        const int   ui_height,
        const char* title
    );

    ~GLFWWindow();

    glm::ivec2 frameBufferSize() const;
    glm::vec2  frameBufferSizeF() const;
    glm::ivec2 uiBufferSize() const;

    GLFWwindow* window();

    void makeContextCurrent();
    void updateAndWait( const bool need_buffer_swap );
    bool shouldClose() const;
    glm::vec2 convertToFrameBufferCoordinates( const glm::vec2& ui_coordinates ) const;
    glm::vec2 convertToUICoordinates( const glm::vec2& frame_buffer_coordinates ) const;
    void framebufferSizeCallback( GLFWwindow* window, int width, int height );
    void windowContentScaleCallback( GLFWwindow* window, float xscale, float yscale );
    void windowSizeCallback( GLFWwindow* window, int width, int height );
    bool isUpdated() const;

    GLFWwindow* m_window;

    GLFWCallbackHandlerSingleton&
                m_callback_handler;

    int         m_frame_width;
    int         m_frame_height;
    int         m_ui_width;
    int         m_ui_height;
    float       m_xscale;
    float       m_yscale;
    const char* m_title;

    bool        m_updated;
};

} // namespace DepthTest

#endif/* __DEPTH_TEST_GLFW_WINDOW_HPP__*/
