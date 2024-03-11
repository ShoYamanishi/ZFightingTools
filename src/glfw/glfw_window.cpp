#include <GLFW/glfw3.h>

#include "glfw_window.hpp"

namespace DepthTest {

GLFWWindow::GLFWWindow(
    const int   ui_width,
    const int   ui_height,
    const char* title
)
    :m_window      { nullptr }
    ,m_callback_handler
    {
        GLFWCallbackHandlerSingleton::getInstance()
    }
    ,m_frame_width { 0 }
    ,m_frame_height{ 0 }
    ,m_ui_width    { 0 }
    ,m_ui_height   { 0 }
    ,m_xscale      { 0.0f }
    ,m_yscale      { 0.0f }
    ,m_title       { title }
    ,m_updated     { true }
{
    glfwWindowHint( GLFW_SAMPLES, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#ifdef __APPLE__
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwSwapInterval( 1 );

    m_window = glfwCreateWindow(
        ui_width,
        ui_height,
        title,
        nullptr,
        nullptr
    );

    if ( m_window == nullptr ) {

        throw std::runtime_error( "failed to create glfw window." );
    }

    glfwGetFramebufferSize( m_window, &m_frame_width, &m_frame_height );
    glfwGetWindowContentScale( m_window, &m_xscale, &m_yscale );
    glfwGetWindowSize( m_window, &m_ui_width, &m_ui_height);

    m_callback_handler.registerHandler( m_window, this );
}

GLFWWindow::~GLFWWindow()
{
    if ( m_window != nullptr ) {

        m_callback_handler.unregisterHandler( m_window, this );
        glfwDestroyWindow( m_window );
    }
}

glm::ivec2 GLFWWindow::frameBufferSize() const
{
    return glm::ivec2{ m_frame_width, m_frame_height };
}

glm::vec2 GLFWWindow::frameBufferSizeF() const
{
    return glm::vec2{
        static_cast<float>( m_frame_width ),
        static_cast<float>( m_frame_height )
    };
}

glm::ivec2 GLFWWindow::uiBufferSize() const
{
    return glm::ivec2{ m_ui_width, m_ui_height };
}

GLFWwindow* GLFWWindow::window()
{
    return m_window;
}

void GLFWWindow::makeContextCurrent()
{
    glfwMakeContextCurrent( m_window );
}

void GLFWWindow::updateAndWait( const bool need_buffer_swap )
{
    m_updated = false;

    if ( need_buffer_swap ) {

        glfwSwapBuffers( m_window );
    }

    // wait at the frequency of twice the screen update. (120FPS)
    glfwWaitEventsTimeout( 0.5f / 60.0f ); 
}

bool GLFWWindow::shouldClose() const
{
    return glfwWindowShouldClose( m_window ) != 0;
}

glm::vec2 GLFWWindow::convertToFrameBufferCoordinates( const glm::vec2& ui_coordinates ) const
{
    const auto xpos = ui_coordinates.x * m_xscale;
    const auto ypos = ui_coordinates.y * m_yscale;

    return glm::vec2{ xpos, m_frame_height - ypos };
}

glm::vec2 GLFWWindow::convertToUICoordinates( const glm::vec2& frame_buffer_coordinates ) const
{
    const auto xpos = frame_buffer_coordinates.x / m_xscale;
    const auto ypos = frame_buffer_coordinates.y / m_yscale;

    return glm::vec2{ xpos, m_ui_height - ypos };
}

void GLFWWindow::framebufferSizeCallback( GLFWwindow* window, int width, int height )
{
    m_frame_width  = width;
    m_frame_height = height;

    glfwGetWindowContentScale( m_window, &m_xscale, &m_yscale );
    glfwGetWindowSize( m_window, &m_ui_width, &m_ui_height);

    m_updated = true;
}

void GLFWWindow::windowContentScaleCallback( GLFWwindow* window, float xscale, float yscale )
{
    m_xscale = xscale;
    m_yscale = yscale;

    glfwGetFramebufferSize( m_window, &m_frame_width, &m_frame_height );
    glfwGetWindowSize( m_window, &m_ui_width, &m_ui_height);

    m_updated = true;
}

void GLFWWindow::windowSizeCallback( GLFWwindow* window, int width, int height )
{
    m_ui_width  = width;
    m_ui_height = height;

    glfwGetFramebufferSize( m_window, &m_frame_width, &m_frame_height );
    glfwGetWindowContentScale( m_window, &m_xscale, &m_yscale );

    m_updated = true;
}

bool GLFWWindow::isUpdated() const
{
    return m_updated;
}

} // namespace DepthTest
