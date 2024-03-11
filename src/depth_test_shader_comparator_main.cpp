#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "opengl_util.hpp"
#include "debug_print.hpp"
#include "glfw_window.hpp"
#include "glfw_user_input_shader_comparator.hpp"
#include "ui_text_shader_comparator.hpp"
#include "cylinders_renderer.hpp"

static constexpr int     WINDOW_WIDTH  = 1024;
static constexpr int     WINDOW_HEIGHT = 768;
static const std::string WINDOW_TITLE  = "Depth Test";

static constexpr glm::vec4 COLOR_RED  = glm::vec4{ 1.0f, 0.5f, 0.5f, 1.0f };
static constexpr glm::vec4 COLOR_BLUE = glm::vec4{ 0.5f, 0.7f, 1.0f, 1.0f };

static constexpr float NEAR = 1.0e-1;
static constexpr float FAR  = 1.0e+6;
static constexpr int   NUM_EDGES_CYLINDER_1 = 6;
static constexpr int   NUM_EDGES_CYLINDER_2 = 10;

int main( int argc, char* argv[] )
{
    if( !glfwInit() ) {
        exit(1);
    }

    DepthTest::GLFWWindow main_window{ WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str() };

    DepthTest::GLFWUserInputShaderComparator ui{ main_window };

    main_window.makeContextCurrent();

    glewExperimental = GL_TRUE;
    if ( glewInit() != GLEW_OK ) {
        exit(1);
    }

    DepthTest::OpenGLInfo gl_info;
    std::cout << "Open GL Info: " << gl_info << "\n";

    DepthTest::UITextShaderComparator ui_text{ main_window, ui };

    DepthTest::CylindersRenderer renderer_normal{

        DepthTest::CylindersRenderer::RENDER_NORMAL,
        NUM_EDGES_CYLINDER_1,
        NUM_EDGES_CYLINDER_2
    };

    DepthTest::CylindersRenderer renderer_log_depth_in_vs{

        DepthTest::CylindersRenderer::RENDER_LOG_DEPTH_TO_GL_POSITION,
        NUM_EDGES_CYLINDER_1,
        NUM_EDGES_CYLINDER_2
    };

    DepthTest::CylindersRenderer renderer_log_depth_in_fs{

        DepthTest::CylindersRenderer::RENDER_LOG_DEPTH_TO_GL_FRAGDEPTH,
        NUM_EDGES_CYLINDER_1,
        NUM_EDGES_CYLINDER_2
    };

    while( true ) {

        ui.update();

        if ( ui.shouldClose() ) {
            break;
        }

        const bool window_updated = main_window.isUpdated();
        const bool ui_updated     = ui.isUpdated();

        const bool need_buffer_swap = window_updated || ui_updated;

        if ( need_buffer_swap ) {

            glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

            auto window_dim = main_window.frameBufferSize();
            window_dim.x /= 3;

            const glm::mat4 Mproj = glm::perspective(
                0.22f * static_cast<float>(M_PI),
                static_cast<float>(window_dim.x) / static_cast<float>(window_dim.y),
                NEAR,
                FAR
            );

            const float log_near =  log( NEAR );
            const float log_far  =  log( FAR  );

            renderer_normal.render(
                glm::ivec2{ 0, 0 },
                window_dim,
                ui.modelMatrix1(),
                ui.modelMatrix2(),
                ui.modelScaling1(),
                ui.modelScaling2(),
                COLOR_RED,
                COLOR_BLUE,
                ui.viewMatrix(),
                Mproj,
                ui.cameraPositionWCS(),
                log_near,
                log_far
            );


            // center pane
            renderer_log_depth_in_vs.render(
                glm::ivec2{ window_dim.x, 0 },
                window_dim,
                ui.modelMatrix1(),
                ui.modelMatrix2(),
                ui.modelScaling1(),
                ui.modelScaling2(),
                COLOR_RED,
                COLOR_BLUE,
                ui.viewMatrix(),
                Mproj,
                ui.cameraPositionWCS(),
                log_near,
                log_far
            );

            // right pane
            renderer_log_depth_in_fs.render(
                glm::ivec2{ window_dim.x * 2.0f, 0 },
                window_dim,
                ui.modelMatrix1(),
                ui.modelMatrix2(),
                ui.modelScaling1(),
                ui.modelScaling2(),
                COLOR_RED,
                COLOR_BLUE,
                ui.viewMatrix(),
                Mproj,
                ui.cameraPositionWCS(),
                log_near,
                log_far
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
