#ifndef __DEPTH_TEST_GFLW_CALLBACK_HANDLER_SINGLETON_HPP__
#define __DEPTH_TEST_GFLW_CALLBACK_HANDLER_SINGLETON_HPP__

#include <map>

class GLFWwindow;

namespace DepthTest {

class GLFWCallbackReceiver {

public:
    GLFWCallbackReceiver(){;}
    virtual ~GLFWCallbackReceiver(){;}

    virtual void framebufferSizeCallback   ( GLFWwindow* window, int    width,   int    height  ){;}
    virtual void windowContentScaleCallback( GLFWwindow* window, float  xscale,  float  yscale  ){;}
    virtual void windowSizeCallback        ( GLFWwindow* window, int    width,   int    height  ){;}

    virtual void scrollCallback            ( GLFWwindow* window, double xoffset, double yoffset ){;}
    virtual void cursorEnterCallback       ( GLFWwindow* window, int    entered ){;}
};

class GLFWCallbackHandlerSingleton {

public:
    static GLFWCallbackHandlerSingleton& getInstance()
    {
        static GLFWCallbackHandlerSingleton instance;
        return instance;
    }

    void registerHandler  ( GLFWwindow* window, GLFWCallbackReceiver* handler );
    void unregisterHandler( GLFWwindow* window, GLFWCallbackReceiver* handler );


    GLFWCallbackHandlerSingleton( GLFWCallbackHandlerSingleton const& ) = delete;
    void operator = ( GLFWCallbackHandlerSingleton const& ) = delete;

    /**
     * called by the static functions in glfw_callback_handler_singleton.cpp.
     */
    void scrollCallback            ( GLFWwindow* window, double xoffset, double yoffset );
    void framebufferSizeCallback   ( GLFWwindow* window, int    width,   int    height  );
    void windowContentScaleCallback( GLFWwindow* window, float  xscale,  float  yscale  );
    void windowSizeCallback        ( GLFWwindow* window, int    width,   int    height  );
    void cursorEnterCallback       ( GLFWwindow* window, int    entered );

private:

    GLFWCallbackHandlerSingleton() noexcept;
    ~GLFWCallbackHandlerSingleton();

    std::map<
        GLFWwindow*, 
        std::vector< GLFWCallbackReceiver* >
    > m_dispatch_map;
};

} // namespace DepthTest

#endif /*__DEPTH_TEST_GFLW_CALLBACK_HANDLER_SINGLETON_HPP__*/
