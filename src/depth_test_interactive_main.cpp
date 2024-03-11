#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "opengl_util.hpp"
#include "glfw_window.hpp"
#include "glfw_user_input_interactive.hpp"
#include "ui_text_interactive.hpp"

#include "square_renderer.hpp"

static constexpr int     WINDOW_WIDTH  = 1024;
static constexpr int     WINDOW_HEIGHT = 768;
static const std::string WINDOW_TITLE  = "Depth Test";

int main( int argc, char* argv[] )
{
    if( !glfwInit() ) {
        exit(1);
    }

    DepthTest::GLFWWindow main_window{ WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str() };

    DepthTest::GLFWUserInputInteractive ui{ main_window };

    main_window.makeContextCurrent();

    glewExperimental = GL_TRUE;
    if ( glewInit() != GLEW_OK ) {
        exit(1);
    }

    DepthTest::OpenGLInfo gl_info;
    std::cout << "Open GL Info: " << gl_info << "\n";

    DepthTest::SquareRenderer square_renderer_normal_depth{ DepthTest::SquareRenderer::PERSPECTIVE };
    DepthTest::SquareRenderer square_renderer_log_depth_fn{ DepthTest::SquareRenderer::LOG_DEPTH_FN };
    DepthTest::SquareRenderer square_renderer_log_depth_cf{ DepthTest::SquareRenderer::LOG_DEPTH_CF };
    DepthTest::UITextInteractive ui_text{ main_window, ui };

    while( true ) {

        ui.update();

        if ( ui.shouldClose() ) {
            break;
        }

        const bool window_updated = main_window.isUpdated();
        const bool ui_updated     = ui.isUpdated();

        const bool need_buffer_swap = window_updated || ui_updated;

        if ( window_updated || ui_updated ) {

            glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

            auto window_dim = main_window.frameBufferSize();
            window_dim.x /= 3;

            // left pane
            square_renderer_normal_depth.renderInteractive(
                glm::ivec2{ 0, 0 }, window_dim, // viewport
                ui.near(),
                ui.far(),
                ui.paramC(),
                ui.plane1(),
                ui.plane2(),
                ui.edgeLength()
            );

            // center pane
            square_renderer_log_depth_fn.renderInteractive(
                glm::ivec2{ window_dim.x, 0 }, window_dim, // viewport
                ui.near(),
                ui.far(),
                ui.paramC(),
                ui.plane1(),
                ui.plane2(),
                ui.edgeLength()
            );

            // right pane
            square_renderer_log_depth_cf.renderInteractive(
                glm::ivec2{ window_dim.x * 2, 0 }, window_dim, // viewport
                ui.near(),
                ui.far(),
                ui.paramC(),
                ui.plane1(),
                ui.plane2(),
                ui.edgeLength()
            );

            ui_text.update();
            ui_text.render();
        }

        main_window.updateAndWait( need_buffer_swap );

        if ( main_window.shouldClose() ) {
            break;
        }
    }

    glfwTerminate();

    return 0;
}
