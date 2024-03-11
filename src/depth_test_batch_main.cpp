#include <iostream>
#include <string>
#include <chrono>
#include <random>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "square_renderer.hpp"
#include "option_parser.hpp"
#include "batch_tester.hpp"

using namespace std::chrono;

int main( int argc, char* argv[] )
{
    DepthTest::OptionParser opt{ argc, argv };

    if( !glfwInit() ) {
        exit(1);
    }

    glfwWindowHint( GLFW_SAMPLES, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#ifdef __APPLE__
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(100, 100, "none", nullptr, nullptr );

    if( window == nullptr ) {
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent( window );

    glewExperimental = GL_TRUE;
    if ( glewInit() != GLEW_OK ) {
        exit(1);
    }

    DepthTest::OpenGLInfo gl_info;
    std::cout << "Open GL Info: " << gl_info << "\n";

    DepthTest::BatchTester tester{
        opt.depthTestType(),
        opt.near(),
        opt.far(),
        opt.paramC(),
        opt.numPoints(),
        opt.numPerturbedSamples()
    };

    auto start = high_resolution_clock::now();

    tester.run();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);

    // you can retrieve the results from:
    // tester.m_sample_points and tester.m_results.

    std::cerr << "Test finished in " << duration.count() << " seconds\n";

    glfwDestroyWindow( window );
    glfwTerminate();
    return 0;
}
