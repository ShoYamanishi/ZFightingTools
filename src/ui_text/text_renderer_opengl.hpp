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
in vec4 inner_color_vin;\n\
in vec4 outer_color_vin;\n\
in vec2 texture_uv_vin;\n\
\n\
uniform mat4 P;\n\
uniform mat4 V;\n\
uniform mat4 M;\n\
\n\
out vec4 inner_color_vout;\n\
out vec4 outer_color_vout;\n\
out vec2 texture_uv_vout;\n\
\n\
void main() {\n\
    vec4 position_wcs = M * position_lcs;\n\
    vec4 position_vcs = V * position_wcs;\n\
    gl_Position = P * position_vcs;\n\
\n\
    inner_color_vout = inner_color_vin;\n\
    outer_color_vout = outer_color_vin;\n\
\n\
    texture_uv_vout = texture_uv_vin;\n\
\n\
}\n\
";

static constexpr const char* FRAG_STR_TEXT = "#version 330 core\n\
\n\
in  vec4 inner_color_vout;\n\
in  vec4 outer_color_vout;\n\
in  vec2 texture_uv_vout;\n\
\n\
uniform sampler2D sampler_font;\n\
uniform float gate1_low;\n\
uniform float gate1_high;\n\
uniform float gate2_low;\n\
uniform float gate2_high;\n\
\n\
out vec4 color_fout;\n\
\n\
void main()\n\
{\n\
    float step = texture( sampler_font, texture_uv_vout ).r;\n\
\n\
    if ( gate2_high <= step ) {\n\
\n\
        color_fout = inner_color_vout;\n\
    }\n\
    else if ( gate2_low <= step && step < gate2_high ) {\n\
\n\
        float alpha = smoothstep( gate2_low, gate2_high, step );\n\
        color_fout = inner_color_vout * alpha + outer_color_vout * ( 1.0 - alpha );\n\
    }\n\
    else {\n\
        float alpha = smoothstep( gate1_low, gate1_high, step );\n\
        color_fout = outer_color_vout;\n\
        color_fout.a = alpha;\n\
    }\n\
}\n\
";

static constexpr float OUTLINE_OFFSET = 0.1f;
static constexpr glm::vec4 OUTLINE_COLOR = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };

class TextRendererOpenGL {

  public:
    explicit TextRendererOpenGL(
        GLFWWindow&       window,
        const std::string font_path,
        const float       gate1_low,
        const float       gate1_high,
        const float       gate2_low,
        const float       gate2_high
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
    float          m_gate1_low;
    float          m_gate1_high;
    float          m_gate2_low;
    float          m_gate2_high;

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
    GLuint         m_vertex_location_inner_color;
    GLuint         m_vertex_location_outer_color;
    GLuint         m_vertex_location_texture_uv;

    GLuint         m_uniform_location_P;
    GLuint         m_uniform_location_V;
    GLuint         m_uniform_location_M;
    GLuint         m_uniform_location_gate1_low;
    GLuint         m_uniform_location_gate1_high;
    GLuint         m_uniform_location_gate2_low;
    GLuint         m_uniform_location_gate2_high;
    GLuint         m_uniform_location_sampler_font;
};

} // namespace DepthTest

#endif/*__DEPTH_TEST_TEXT_RENDERER_OPENGL_HPP__*/
