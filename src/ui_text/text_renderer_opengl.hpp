#ifndef __DEPTH_TEST_TEXT_RENDERER_OPENGL_HPP__
#define __DEPTH_TEST_TEXT_RENDERER_OPENGL_HPP__

#include <cstdint>
#include <set>
#include <glm/glm.hpp>

#include "font_runtime_helper.hpp"
#include "opengl_util.hpp"
#include "png_util.hpp"

#include "glfw_window.hpp"

#include "text_renderer_vertex.hpp"
#include "text_renderer_line.hpp"

namespace DepthTest {

static constexpr const char* VERT_STR_TEXT = "#version 330 core\n\
\n\
in vec4 position_lcs;\n\
in vec4 color_vin;\n\
in vec2 texture_uv_vin;\n\
\n\
uniform mat4 P;\n\
uniform mat4 V;\n\
uniform mat4 M;\n\
\n\
out vec4 color_vout;\n\
out vec2 texture_uv_vout;\n\
\n\
void main() {\n\
    vec4 position_wcs = M * position_lcs;\n\
    vec4 position_vcs = V * position_wcs;\n\
    gl_Position = P * position_vcs;\n\
\n\
    color_vout = color_vin;\n\
\n\
    texture_uv_vout = texture_uv_vin;\n\
\n\
}\n\
";

static constexpr const char* FRAG_STR_TEXT = "#version 330 core\n\
\n\
in  vec4 color_vout;\n\
in  vec2 texture_uv_vout;\n\
\n\
uniform sampler2D sampler_font;\n\
uniform float smooth_low;\n\
uniform float smooth_high;\n\
uniform vec4 color_override;\n\
\n\
out vec4 color_fout;\n\
\n\
void main()\n\
{\n\
    float step = texture( sampler_font, texture_uv_vout ).r;\n\
\n\
    if ( color_override[3] == 0.0 ) {\n\
        color_fout = color_vout;\n\
    }\n\
    else {\n\
        color_fout = color_override;\n\
    }\n\
\n\
    color_fout.a = smoothstep( smooth_low, smooth_high, step );\n\
}\n\
\n\
";

static constexpr float OUTLINE_OFFSET = 0.1f;
static constexpr glm::vec4 OUTLINE_COLOR = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };

class TextRendererOpenGL {

  public:
    explicit TextRendererOpenGL(
        GLFWWindow&       window,
        const std::string font_path,
        const float       font_smooth_low,
        const float       font_smooth_high
    );

    ~TextRendererOpenGL();

    void registerLine( TextRendererLine* line );

    void unregisterLine( TextRendererLine* line );

    void render( const bool initialize_screen );

private:
    void updateVertexAndIndexBuffers();

    int32_t findBufferCapacityInBytes( int32_t size_in_bytes ) const;

    void recreateVertexBuffer();

    void recreateIndexBuffer();

    GLFWWindow&    m_window;

    std::string    m_font_path;
    float          m_font_smooth_low;
    float          m_font_smooth_high;
    std::set<
        TextRendererLine* 
    >              m_lines;
    glm::mat4      m_uniform_M;
    glm::mat4      m_uniform_V;
    glm::mat4      m_uniform_P;

    TextRendererVertex*
                   m_vertices;
    uint32_t*      m_indices;
    int32_t        m_num_vertices;
    int32_t        m_num_indices;
    int32_t        m_vertices_capacity_in_bytes;
    int32_t        m_indices_capacity_in_bytes;

    GLuint         m_gl_prog_id;
    GLuint         m_gl_vertex_array;
    GLuint         m_gl_vertex_buffer;
    GLuint         m_gl_index_buffer;
    GLuint         m_font_texture;

    GLuint         m_vertex_location_position_lcs;
    GLuint         m_vertex_location_color;
    GLuint         m_vertex_location_texture_uv;

    GLuint         m_uniform_location_P;
    GLuint         m_uniform_location_V;
    GLuint         m_uniform_location_M;
    GLuint         m_uniform_location_smooth_low;
    GLuint         m_uniform_location_smooth_high;
    GLuint         m_uniform_location_color_override;
    GLuint         m_uniform_location_sampler_font;
};

} // namespace DepthTest

#endif/*__DEPTH_TEST_TEXT_RENDERER_OPENGL_HPP__*/
