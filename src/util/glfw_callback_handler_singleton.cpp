#include <GLFW/glfw3.h>

#include "glfw_callback_handler_singleton.hpp"

namespace DepthTest {

static GLFWCallbackHandlerSingleton* singleton;

static void dispatchScrollCallback( GLFWwindow* window, double xoffset, double yoffset)
{
    if ( singleton != nullptr ) {

        singleton->scrollCallback( window, xoffset, yoffset );
    }
}

static void dispatchFramebufferSizeCallback( GLFWwindow* window, int width, int height )
{
    if ( singleton != nullptr ) {

        singleton->framebufferSizeCallback( window, width, height );
    }
}

static void dispatchWindowContentScaleCallback( GLFWwindow* window, float xscale, float yscale )
{
    if ( singleton != nullptr ) {

        singleton->windowContentScaleCallback( window, xscale, yscale );
    }
}

static void dispatchWindowSizeCallback( GLFWwindow* window, int width, int height )
{
    if ( singleton != nullptr ) {

        singleton->windowSizeCallback( window, width, height );
    }
}

static void dispatchCursorEnterCallback( GLFWwindow* window, int entered )
{
    if ( singleton != nullptr ) {

        singleton->cursorEnterCallback( window, entered );
    }
}

void GLFWCallbackHandlerSingleton::scrollCallback( GLFWwindow* window, double xoffset, double yoffset )
{
    const auto it = m_dispatch_map.find( window );

    if ( it != m_dispatch_map.end() ) {

        for ( auto* p : it->second ) {

            p->scrollCallback( window, xoffset, yoffset );
        }
    }
}

void GLFWCallbackHandlerSingleton::framebufferSizeCallback( GLFWwindow* window, int width, int height )
{
    const auto it = m_dispatch_map.find( window );

    if ( it != m_dispatch_map.end() ) {

        for ( auto* p : it->second ) {

            p->framebufferSizeCallback( window, width, height );
        }
    }
}

void GLFWCallbackHandlerSingleton::windowContentScaleCallback( GLFWwindow* window, float xscale, float yscale )
{
    const auto it = m_dispatch_map.find( window );

    if ( it != m_dispatch_map.end() ) {

        for ( auto* p : it->second ) {

            p->windowContentScaleCallback( window, xscale, yscale );
        }
    }
}

void GLFWCallbackHandlerSingleton::windowSizeCallback( GLFWwindow* window, int width, int height )
{
    const auto it = m_dispatch_map.find( window );

    if ( it != m_dispatch_map.end() ) {

        for ( auto* p : it->second ) {

            p->windowSizeCallback( window, width, height );
        }
    }
}

void GLFWCallbackHandlerSingleton::cursorEnterCallback( GLFWwindow* window, int entered )
{
    const auto it = m_dispatch_map.find( window );

    if ( it != m_dispatch_map.end() ) {

        for ( auto* p : it->second ) {

            p->cursorEnterCallback( window, entered );
        }
    }
}

GLFWCallbackHandlerSingleton::GLFWCallbackHandlerSingleton() noexcept
{
    singleton = this;
}

GLFWCallbackHandlerSingleton::~GLFWCallbackHandlerSingleton()
{

}

void GLFWCallbackHandlerSingleton::registerHandler( 
    GLFWwindow*           window,
    GLFWCallbackReceiver* handler
) {
    auto it = m_dispatch_map.find( window );

    if ( it == m_dispatch_map.end() ) {

        glfwSetScrollCallback            ( window, dispatchScrollCallback             );
        glfwSetFramebufferSizeCallback   ( window, dispatchFramebufferSizeCallback    );
        glfwSetWindowContentScaleCallback( window, dispatchWindowContentScaleCallback );
        glfwSetWindowSizeCallback        ( window, dispatchWindowSizeCallback         );
        glfwSetCursorEnterCallback       ( window, dispatchCursorEnterCallback        );

        auto res = m_dispatch_map.insert( std::pair( window, std::vector<GLFWCallbackReceiver*>() ) );

        if ( ! res.second ) {

            throw std::runtime_error( "insersion to m_dispatch_map failed." );
        }
        it = res.first;
    }
    it->second.push_back( handler );        
}

void GLFWCallbackHandlerSingleton::unregisterHandler(
    GLFWwindow*           window,
    GLFWCallbackReceiver* handler
) {
    auto it_map = m_dispatch_map.find( window );

    if ( it_map != m_dispatch_map.end() ) {

        auto& vec = it_map->second;
        auto it_vec = std::find( vec.begin(), vec.end(), handler );

        if ( it_vec != vec.end() ) {

            vec.erase( it_vec );

            if ( vec.empty() ) {

                m_dispatch_map.erase( it_map );

                glfwSetScrollCallback            ( window, nullptr );
                glfwSetFramebufferSizeCallback   ( window, nullptr );
                glfwSetWindowContentScaleCallback( window, nullptr );
                glfwSetWindowSizeCallback        ( window, nullptr );
                glfwSetCursorEnterCallback       ( window, nullptr );
            }
        }
    }
}

} //namespace DepthTest
