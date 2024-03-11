#ifndef __DEPTH_TEST_SQUARE_RENDERER_HPP__
#define __DEPTH_TEST_SQUARE_RENDERER_HPP__

#include <cstdint>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "opengl_util.hpp"

namespace DepthTest {

static constexpr const char* VERT_STR_NORMAL_DEPTH = "#version 330 core\n\
\n\
in  vec4 position_lcs;\n\
\n\
uniform mat4 P;\n\
uniform mat4 V;\n\
uniform mat4 M;\n\
\n\
void main() {\n\
\n\
    vec4 position_wcs = M * position_lcs;\n\
    vec4 position_vcs = V * position_wcs;\n\
    gl_Position  = P * position_vcs;\n\
}\n\
";

static constexpr const char* FRAG_STR_NORMAL_DEPTH = "#version 330 core\n\
\n\
out vec4 color_fout;\n\
\n\
uniform vec4 fg_color;\n\
\n\
void main()\n\
{\n\
    color_fout = fg_color;\n\
}\n\
";

static constexpr const char* VERT_STR_LOG_DEPTH_FN = "#version 330 core\n\
\n\
in  vec4 position_lcs;\n\
\n\
out vec4  color_vout;\n\
out float position_vcs_z;\n\
\n\
uniform mat4 P;\n\
uniform mat4 V;\n\
uniform mat4 M;\n\
\n\
void main() {\n\
\n\
    vec4 position_wcs = M * position_lcs;\n\
    vec4 position_vcs = V * position_wcs;\n\
    gl_Position  = P * position_vcs;\n\
    position_vcs_z = position_vcs.z;\n\
}\n\
";

static constexpr const char* FRAG_STR_LOG_DEPTH_FN = "#version 330 core\n\
\n\
in float position_vcs_z;\n\
\n\
out vec4 color_fout;\n\
\n\
uniform float log_near;\n\
uniform float log_far;\n\
uniform vec4 fg_color;\n\
\n\
void main()\n\
{\n\
    float log_z  = log( max( 1.0e-20, -1.0 * position_vcs_z ) );\n\
    gl_FragDepth = ( log_z - log_near ) / ( log_far - log_near );\n\
    color_fout = fg_color;\n\
}\n\
";


static constexpr const char* VERT_STR_LOG_DEPTH_CF = "#version 330 core\n\
\n\
in  vec4 position_lcs;\n\
\n\
out vec4  color_vout;\n\
out float position_vcs_z;\n\
\n\
uniform mat4 P;\n\
uniform mat4 V;\n\
uniform mat4 M;\n\
\n\
void main() {\n\
\n\
    vec4 position_wcs = M * position_lcs;\n\
    vec4 position_vcs = V * position_wcs;\n\
    gl_Position  = P * position_vcs;\n\
    position_vcs_z = position_vcs.z;\n\
}\n\
";

static constexpr const char* FRAG_STR_LOG_DEPTH_CF = "#version 330 core\n\
\n\
in float position_vcs_z;\n\
\n\
out vec4 color_fout;\n\
\n\
uniform float param_c;\n\
uniform float log_cf_plus_1_inv;\n\
uniform vec4 fg_color;\n\
\n\
void main()\n\
{\n\
    gl_FragDepth = log( -1.0 * param_c * position_vcs_z + 1.0 )\n\
                 * log_cf_plus_1_inv;\n\
    color_fout = fg_color;\n\
}\n\
";

class SquareRenderer {

  public:

    typedef enum _DepthTestType {
        UNKNOWN,
        PERSPECTIVE,
        LOG_DEPTH_FN,
        LOG_DEPTH_CF
    } DepthTestType;

    explicit SquareRenderer( const DepthTestType depth_test_type );

    ~SquareRenderer();

    void renderInteractive(
        const glm::ivec2& screen_pos,
        const glm::ivec2& screen_wh,
        const float near,
        const float far,
        const float param_c,
        const float plane_1,
        const float plane_2,
        const float edge_length
    );

    void test(
        const float near,
        const float far,
        const float param_c,
        const float plane_1,
        const float plane_2,
        bool&       plane_1_detected,
        bool&       plane_2_detected
    );

private:

    const DepthTestType m_depth_test_type;

    glm::mat4  m_uniform_M_plane1;
    glm::mat4  m_uniform_M_plane2;
    glm::mat4  m_uniform_V;
    glm::mat4  m_uniform_P;

    glm::vec4  m_vertices[6];

    GLuint     m_gl_prog_id;
    GLuint     m_gl_vertex_array;
    GLuint     m_gl_vertex_buffer;

    GLuint     m_vertex_location_position_lcs;

    GLuint     m_uniform_location_P;
    GLuint     m_uniform_location_V;
    GLuint     m_uniform_location_M;
    GLuint     m_uniform_location_fg_color;
    GLuint     m_uniform_location_log_near;
    GLuint     m_uniform_location_log_far;
    GLuint     m_uniform_location_param_c;
    GLuint     m_uniform_location_log_cf_plus_1_inv;

    // test framebuffer of 1x1.
    GLuint     m_frame_buffer_tester;
    GLuint     m_render_buffer_color_tester;
    GLuint     m_render_buffer_depth_stencil_tester;
};

} // namespace DepthTest

#endif/*__DEPTH_TEST_SQUARE_RENDERER_HPP__*/
