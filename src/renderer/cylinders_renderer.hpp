#ifndef __DEPTH_TEST_CYLINDERS_RENDERER_HPP__
#define __DEPTH_TEST_CYLINDERS_RENDERER_HPP__

#include <cstdint>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "opengl_util.hpp"

namespace DepthTest {

static constexpr const char* VERT_STR_NORMAL_DEPTH = "#version 330 core\n\
\n\
in vec4 position_lcs;\n\
in vec4 normal_lcs;\n\
\n\
uniform mat4 P;\n\
uniform mat4 V;\n\
uniform mat4 M;\n\
uniform vec3 scaling;\n\
\n\
out vec4 position_wcs;\n\
out vec4 normal_wcs;\n\
\n\
void main() {\n\
\n\
    vec4 position_scaled = vec4(\n\
        position_lcs.x * scaling.x,\n\
        position_lcs.y * scaling.y,\n\
        position_lcs.z * scaling.z,\n\
        1.0\n\
    );\n\
    position_wcs = M * position_scaled;\n\
    vec4 position_vcs = V * position_wcs;\n\
    gl_Position  = P * position_vcs;\n\
    normal_wcs   = M * vec4( normal_lcs.xyz, 0.0 );\n\
}\n\
";

static constexpr const char* FRAG_STR_NORMAL_DEPTH = "#version 330 core\n\
\n\
in vec4 position_wcs;\n\
in vec4 normal_wcs;\n\
\n\
out vec4 color_fout;\n\
\n\
uniform vec4 color_fin;\n\
uniform vec4 camera_pos_wcs;\n\
\n\
void main()\n\
{\n\
    const vec4 light_wcs = vec4( 10.0, 10.0, 10.0, 1.0 );\n\
    const vec4 light_color = vec4( 1.0, 1.0, 1.0, 1.0 );\n\
\n\
    // Ambient part\n\
    vec4 color_a = color_fin;\n\
    color_a = color_a * light_color;\n\
\n\
    // Diffuse part\n\
    vec4 color_d = color_fin;\n\
\n\
    float diffuse_intensity = max( 0.0, dot(\n\
        normalize( normal_wcs.xyz ),\n\
        normalize( light_wcs.xyz - position_wcs.xyz )\n\
    ) );\n\
\n\
    color_d = color_d * diffuse_intensity;\n\
\n\
    color_fout = color_a * 0.5 + color_d * 0.5;\n\
}\n\
";

static constexpr const char* VERT_STR_LOG_DEPTH_TO_GL_POSITION = "#version 330 core\n\
\n\
in vec4 position_lcs;\n\
in vec4 normal_lcs;\n\
\n\
uniform mat4 P;\n\
uniform mat4 V;\n\
uniform mat4 M;\n\
uniform vec3 scaling;\n\
uniform float log_near;\n\
uniform float log_far;\n\
\n\
out vec4 position_wcs;\n\
out vec4 normal_wcs;\n\
\n\
void main() {\n\
\n\
    vec4 position_scaled = vec4(\n\
        position_lcs.x * scaling.x,\n\
        position_lcs.y * scaling.y,\n\
        position_lcs.z * scaling.z,\n\
        1.0\n\
    );\n\
    position_wcs = M * position_scaled;\n\
    vec4 position_vcs = V * position_wcs;\n\
    gl_Position   = P * position_vcs;\n\
\n\
    float z_log = log( max( 1.0e-20, -1.0 * position_vcs.z ) );\n\
    gl_Position.z = ( 2.0 * ( z_log - log_near ) / ( log_far - log_near ) - 1.0 ) * gl_Position.w;\n\
\n\
    normal_wcs = M * vec4( normal_lcs.xyz, 0.0 );\n\
}\n\
";

static constexpr const char* FRAG_STR_LOG_DEPTH_TO_GL_POSITION = "#version 330 core\n\
\n\
in vec4 position_wcs;\n\
in vec4 normal_wcs;\n\
\n\
out vec4 color_fout;\n\
\n\
uniform vec4 color_fin;\n\
uniform vec4 camera_pos_wcs;\n\
\n\
void main()\n\
{\n\
    const vec4 light_wcs = vec4( 10.0, 10.0, 10.0, 1.0 );\n\
    const vec4 light_color = vec4( 1.0, 1.0, 1.0, 1.0 );\n\
\n\
    // Ambient part\n\
    vec4 color_a = color_fin;\n\
    color_a = color_a * light_color;\n\
\n\
    // Diffuse part\n\
    vec4 color_d = color_fin;\n\
\n\
    float diffuse_intensity = max( 0.0, dot(\n\
        normal_wcs.xyz,\n\
        normalize( light_wcs.xyz - position_wcs.xyz )\n\
    ) );\n\
\n\
    color_d = color_d * diffuse_intensity;\n\
\n\
    color_fout = color_a * 0.5 + color_d * 0.5;\n\
}\n\
";

static constexpr const char* VERT_STR_LOG_DEPTH_TO_GL_FRAGDEPTH = "#version 330 core\n\
\n\
in vec4 position_lcs;\n\
in vec4 normal_lcs;\n\
\n\
uniform mat4 P;\n\
uniform mat4 V;\n\
uniform mat4 M;\n\
uniform vec3 scaling;\n\
\n\
out vec4 position_wcs;\n\
out vec4 normal_wcs;\n\
out float position_vcs_z;\n\
\n\
void main() {\n\
\n\
    vec4 position_scaled = vec4(\n\
        position_lcs.x * scaling.x,\n\
        position_lcs.y * scaling.y,\n\
        position_lcs.z * scaling.z,\n\
        1.0\n\
    );\n\
    position_wcs = M * position_scaled;\n\
    vec4 position_vcs = V * position_wcs;\n\
    position_vcs_z = position_vcs.z;\n\
    gl_Position  = P * position_vcs;\n\
    normal_wcs   = M * vec4( normal_lcs.xyz, 0.0 );\n\
}\n\
";

static constexpr const char* FRAG_STR_LOG_DEPTH_TO_GL_FRAGDEPTH = "#version 330 core\n\
\n\
in vec4 position_wcs;\n\
in vec4 normal_wcs;\n\
in float position_vcs_z;\n\
\n\
out vec4 color_fout;\n\
\n\
uniform vec4 color_fin;\n\
uniform vec4 camera_pos_wcs;\n\
uniform float log_near;\n\
uniform float log_far;\n\
\n\
void main()\n\
{\n\
    float log_z  = log( max( 1.0e-35, -1.0 * position_vcs_z ) );\n\
    gl_FragDepth = ( log_z - log_near ) / ( log_far - log_near );\n\
\n\
    const vec4 light_wcs = vec4( 10.0, 10.0, 10.0, 1.0 );\n\
    const vec4 light_color = vec4( 1.0, 1.0, 1.0, 1.0 );\n\
\n\
    // Ambient part\n\
    vec4 color_a = color_fin;\n\
    color_a = color_a * light_color;\n\
\n\
    // Diffuse part\n\
    vec4 color_d = color_fin;\n\
\n\
    float diffuse_intensity = max( 0.0, dot(\n\
        normal_wcs.xyz,\n\
        normalize( light_wcs.xyz - position_wcs.xyz )\n\
    ) );\n\
\n\
    color_d = color_d * diffuse_intensity;\n\
\n\
    color_fout = color_a * 0.5 + color_d * 0.5;\n\
}\n\
";

class CylindersRenderer {

  public:

    struct Vertex {

        explicit Vertex() noexcept
            :m_position_lcs{ 0.0f, 0.0f, 0.0f, 0.0f }
            ,m_normal_lcs  { 0.0f, 0.0f, 0.0f, 0.0f }
        {
        }

        explicit Vertex(
            const glm::vec4& position_lcs,
            const glm::vec4& normal_lcs
        ) noexcept
            :m_position_lcs{ position_lcs }
            ,m_normal_lcs  { normal_lcs   }
        {
        }

        glm::vec4 m_position_lcs;
        glm::vec4 m_normal_lcs;
    };


    typedef enum _RenderType {

        RENDER_NORMAL,
        RENDER_LOG_DEPTH_TO_GL_POSITION,
        RENDER_LOG_DEPTH_TO_GL_FRAGDEPTH

    } RenderType;

    explicit CylindersRenderer(

        const RenderType render_type,
        const int        num_edges_cylinder_1,
        const int        num_edges_cylinder_2
    );

    ~CylindersRenderer();

    void render(

        const glm::ivec2& screen_pos,
        const glm::ivec2& screen_wh,
        const glm::mat4& M_1,
        const glm::mat4& M_2,
        const glm::vec3& scaling_1,
        const glm::vec3& scaling_2,
        const glm::vec4& color_1,
        const glm::vec4& color_2,
        const glm::mat4& V,
        const glm::mat4& P,
        const glm::vec4& camera_pos_wcs,
        const float      log_near,
        const float      log_far
    );

private:

    static std::vector< Vertex > generateVertices( const int num_edges );

    const RenderType m_render_type;

    const int m_num_edges_cylinder_1;
    const int m_num_edges_cylinder_2;
    int       m_start_index_cylinder_1;
    int       m_start_index_cylinder_2;
    int       m_num_vertices_cylinder_1;
    int       m_num_vertices_cylinder_2;

    GLuint    m_gl_prog_id;
    GLuint    m_gl_vertex_array;
    GLuint    m_gl_vertex_buffer;

    GLuint    m_vertex_location_position_lcs;
    GLuint    m_vertex_location_normal_lcs;

    GLuint    m_uniform_location_P;
    GLuint    m_uniform_location_V;
    GLuint    m_uniform_location_M;
    GLuint    m_uniform_location_scaling;
    GLuint    m_uniform_location_color_fin;
    GLuint    m_uniform_location_camera_pos_wcs;
    GLuint    m_uniform_location_log_near;
    GLuint    m_uniform_location_log_far;
};

} // namespace DepthTest

#endif/*__DEPTH_TEST_CYLINDERS_RENDERER_HPP__*/
