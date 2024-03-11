cmake_minimum_required( VERSION 3.10 )

project( depth_test )

find_package( GLEW   REQUIRED )
find_package( OpenGL REQUIRED )
find_package( glfw3  REQUIRED )
find_package( PNG    REQUIRED )
find_package( glm    REQUIRED )

# interactive Depth Visualizer

add_executable( depth_test_interactive 
    src/util/png_util.cpp
    src/util/opengl_util_shader.cpp
    src/util/opengl_util_misc.cpp
    src/util/glfw_callback_handler_singleton.cpp
    src/util/font_metrics_parser.cpp
    src/util/font_runtime_helper.cpp
    src/ui_text/text_renderer_line.cpp
    src/ui_text/text_renderer_opengl.cpp
    src/ui_text/ui_text_interactive.cpp
    src/glfw/glfw_window.cpp
    src/glfw/glfw_user_input_interactive.cpp
    src/renderer/square_renderer.cpp
    src/depth_test_interactive_main.cpp
)

target_include_directories( depth_test_interactive PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/util
    ${PROJECT_SOURCE_DIR}/src/ui_text
    ${PROJECT_SOURCE_DIR}/src/glfw
    ${PROJECT_SOURCE_DIR}/src/renderer
)

target_compile_features( depth_test_interactive PRIVATE cxx_std_17 )

target_link_directories( depth_test_interactive PRIVATE "/usr/local/lib" )

target_include_directories( depth_test_interactive PRIVATE ${PNG_INCLUDE_DIR} )

target_link_libraries( depth_test_interactive ${PNG_LIBRARIES} )

target_link_libraries( depth_test_interactive GLEW::glew )
target_link_libraries( depth_test_interactive glm::glm )

if( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    target_link_libraries( depth_test_interactive glfw3 )
    target_link_libraries( depth_test_interactive "-framework Cocoa" )
    target_link_libraries( depth_test_interactive "-framework IOKit" )
    target_link_libraries( depth_test_interactive "-framework OpenGL" )

else()
    target_link_libraries( depth_test_interactive glfw )
    target_link_libraries( depth_test_interactive OpenGL )

endif()

# batch tester

add_executable( depth_test_batch
    src/util/opengl_util_shader.cpp
    src/util/opengl_util_misc.cpp
    src/renderer/square_renderer.cpp
    src/depth_test_batch_main.cpp
)
target_include_directories( depth_test_batch PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/util
    ${PROJECT_SOURCE_DIR}/src/renderer
)

target_compile_features( depth_test_batch PRIVATE cxx_std_17 )

target_link_directories( depth_test_batch PRIVATE "/usr/local/lib" )

target_link_libraries( depth_test_batch GLEW::glew )
target_link_libraries( depth_test_batch glm::glm )

if( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    target_link_libraries( depth_test_batch glfw3 )
    target_link_libraries( depth_test_batch "-framework Cocoa" )
    target_link_libraries( depth_test_batch "-framework IOKit" )
    target_link_libraries( depth_test_batch "-framework OpenGL" )
else()
    target_link_libraries( depth_test_batch glfw3 )
    target_link_libraries( depth_test_batch OpenGL )
endif()

# interactive shader comparator

add_executable( depth_test_shader_comparator
    src/util/png_util.cpp
    src/util/opengl_util_shader.cpp
    src/util/opengl_util_misc.cpp
    src/util/glfw_callback_handler_singleton.cpp
    src/util/font_metrics_parser.cpp
    src/util/font_runtime_helper.cpp
    src/ui_text/text_renderer_line.cpp
    src/ui_text/text_renderer_opengl.cpp
    src/ui_text/ui_text_shader_comparator.cpp
    src/glfw/glfw_window.cpp
    src/glfw/glfw_user_input_shader_comparator.cpp
    src/renderer/cylinders_renderer.cpp
    src/depth_test_shader_comparator_main.cpp
)

target_include_directories( depth_test_shader_comparator PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/util
    ${PROJECT_SOURCE_DIR}/src/ui_text
    ${PROJECT_SOURCE_DIR}/src/glfw
    ${PROJECT_SOURCE_DIR}/src/renderer
)

target_compile_features( depth_test_shader_comparator PRIVATE cxx_std_17 )

target_link_directories( depth_test_shader_comparator PRIVATE "/usr/local/lib" )

target_include_directories( depth_test_shader_comparator PRIVATE ${PNG_INCLUDE_DIR} )

target_link_libraries( depth_test_shader_comparator ${PNG_LIBRARIES} )

target_link_libraries( depth_test_shader_comparator GLEW::glew )
target_link_libraries( depth_test_shader_comparator glm::glm )

if( ${CMAKE_SYSTEM_NAME} MATCHES Darwin )
    target_link_libraries( depth_test_shader_comparator glfw3 )
    target_link_libraries( depth_test_shader_comparator "-framework Cocoa" )
    target_link_libraries( depth_test_shader_comparator "-framework IOKit" )
    target_link_libraries( depth_test_shader_comparator "-framework OpenGL" )

else()
    target_link_libraries( depth_test_shader_comparator glfw )
    target_link_libraries( depth_test_shader_comparator OpenGL )

endif()
