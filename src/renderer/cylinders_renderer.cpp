#include <cmath>

#include "cylinders_renderer.hpp"
#include "debug_print.hpp"

namespace DepthTest {

CylindersRenderer::CylindersRenderer(
    const RenderType render_type,
    const int        num_edges_cylinder_1,
    const int        num_edges_cylinder_2
)
    :m_render_type                    { render_type }
    ,m_num_edges_cylinder_1           { num_edges_cylinder_1 }
    ,m_num_edges_cylinder_2           { num_edges_cylinder_2 }
    ,m_start_index_cylinder_1         { 0 }
    ,m_start_index_cylinder_2         { 0 }
    ,m_num_vertices_cylinder_1        { 0 }
    ,m_num_vertices_cylinder_2        { 0 }
    ,m_gl_prog_id                     { 0 }
    ,m_gl_vertex_array                { 0 }
    ,m_vertex_location_position_lcs   { 0 }
    ,m_vertex_location_normal_lcs     { 0 }
    ,m_uniform_location_P             { 0 }
    ,m_uniform_location_V             { 0 }
    ,m_uniform_location_M             { 0 }
    ,m_uniform_location_scaling       { 0 }
    ,m_uniform_location_color_fin     { 0 }
    ,m_uniform_location_camera_pos_wcs{ 0 }
    ,m_uniform_location_log_near      { 0 }
    ,m_uniform_location_log_far       { 0 }
{
    switch ( m_render_type ) {

      case RENDER_NORMAL:

        m_gl_prog_id = compileAndLink(

            VERT_STR_NORMAL_DEPTH,
            FRAG_STR_NORMAL_DEPTH,
            std::cerr
        );
        break;

      case RENDER_LOG_DEPTH_TO_GL_POSITION:

        m_gl_prog_id = compileAndLink(

            VERT_STR_LOG_DEPTH_TO_GL_POSITION,
            FRAG_STR_LOG_DEPTH_TO_GL_POSITION,
            std::cerr
        );
        break;

      case RENDER_LOG_DEPTH_TO_GL_FRAGDEPTH:

        m_gl_prog_id = compileAndLink(

            VERT_STR_LOG_DEPTH_TO_GL_FRAGDEPTH,
            FRAG_STR_LOG_DEPTH_TO_GL_FRAGDEPTH,
            std::cerr
        );
        break;
    }

    glGenVertexArrays ( 1, &m_gl_vertex_array  );

    glBindVertexArray( m_gl_vertex_array );

    glUseProgram( m_gl_prog_id );

    m_vertex_location_position_lcs = glGetAttribLocation( m_gl_prog_id, "position_lcs" );
    m_vertex_location_normal_lcs   = glGetAttribLocation( m_gl_prog_id, "normal_lcs"   );

    m_uniform_location_P              = glGetUniformLocation( m_gl_prog_id, "P" );
    m_uniform_location_V              = glGetUniformLocation( m_gl_prog_id, "V" );
    m_uniform_location_M              = glGetUniformLocation( m_gl_prog_id, "M" );
    m_uniform_location_scaling        = glGetUniformLocation( m_gl_prog_id, "scaling" );
    m_uniform_location_color_fin      = glGetUniformLocation( m_gl_prog_id, "color_fin" );
    m_uniform_location_camera_pos_wcs = glGetUniformLocation( m_gl_prog_id, "camera_pos_wcs" );

    if ( m_render_type != RENDER_NORMAL ) {

        m_uniform_location_log_near = glGetUniformLocation( m_gl_prog_id, "log_near" );
        m_uniform_location_log_far  = glGetUniformLocation( m_gl_prog_id, "log_far"  );
    }

    const auto vertices_cylinder_1 = generateVertices( num_edges_cylinder_1 );
    const auto vertices_cylinder_2 = generateVertices( num_edges_cylinder_2 );

    m_start_index_cylinder_1 = 0;
    m_start_index_cylinder_2 = vertices_cylinder_1.size();

    m_num_vertices_cylinder_1 = vertices_cylinder_1.size();
    m_num_vertices_cylinder_2 = vertices_cylinder_2.size();

    glGenBuffers( 1, &m_gl_vertex_buffer );
    glBindBuffer( GL_ARRAY_BUFFER, m_gl_vertex_buffer );

    glBufferData(

        GL_ARRAY_BUFFER, 
        ( m_num_vertices_cylinder_1 + m_num_vertices_cylinder_2 ) * sizeof(Vertex),
        nullptr,
        GL_STATIC_DRAW
    );

    glBufferSubData(

        GL_ARRAY_BUFFER,
        m_start_index_cylinder_1 * sizeof(Vertex),
        m_num_vertices_cylinder_1 * sizeof(Vertex),
        vertices_cylinder_1.data()
    );

    glBufferSubData(

        GL_ARRAY_BUFFER,
        m_start_index_cylinder_2 * sizeof(Vertex),
        m_num_vertices_cylinder_2 * sizeof(Vertex),
        vertices_cylinder_2.data()
    );
}

CylindersRenderer::~CylindersRenderer()
{
    glDeleteProgram      ( m_gl_prog_id );
    glDeleteVertexArrays ( 1, &m_gl_vertex_array  );
    glDeleteBuffers      ( 1, &m_gl_vertex_buffer );
}

void CylindersRenderer::render(

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
) {
    glClear( GL_DEPTH_BUFFER_BIT );
    glEnable( GL_DEPTH_TEST );
    glDisable( GL_BLEND );

    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );

    glStencilMask( 0x00 );
    glDisable(GL_STENCIL_TEST);
    glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

    glViewport(
        static_cast<GLint>( screen_pos.x ),
        static_cast<GLint>( screen_pos.y ),
        static_cast<GLint>( screen_wh.x ),
        static_cast<GLint>( screen_wh.y )
    );

    glBindVertexArray( m_gl_vertex_array );
    glUseProgram( m_gl_prog_id );

    glUniformMatrix4fv( m_uniform_location_V, 1, GL_FALSE, &(V[0][0]) );
    glUniformMatrix4fv( m_uniform_location_P, 1, GL_FALSE, &(P[0][0]) );

    glUniform4fv( m_uniform_location_camera_pos_wcs, 1, &(camera_pos_wcs[0]) );

    if ( m_render_type != RENDER_NORMAL ) {

         glUniform1f ( m_uniform_location_log_near, log_near );
         glUniform1f ( m_uniform_location_log_far,  log_far );
    }

    glBindBuffer( GL_ARRAY_BUFFER, m_gl_vertex_buffer );

    glEnableVertexAttribArray( m_vertex_location_normal_lcs );
    glEnableVertexAttribArray( m_vertex_location_position_lcs );

    glVertexAttribPointer(
        m_vertex_location_position_lcs,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)0
    );

    glVertexAttribPointer(
        m_vertex_location_normal_lcs,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)( 4 * sizeof(float) )
    );

    glUniformMatrix4fv( m_uniform_location_M,   1, GL_FALSE, &(M_1[0][0]) );
    glUniform3fv( m_uniform_location_scaling,   1, &(scaling_1[0]) );
    glUniform4fv( m_uniform_location_color_fin, 1, &(color_1[0]) );

    glDrawArrays( GL_TRIANGLES, m_start_index_cylinder_1, m_num_vertices_cylinder_1 );

    glUniformMatrix4fv( m_uniform_location_M, 1, GL_FALSE, &(M_2[0][0]) );
    glUniform3fv( m_uniform_location_scaling,   1, &(scaling_2[0]) );
    glUniform4fv( m_uniform_location_color_fin, 1, &(color_2[0]) );

    glDrawArrays( GL_TRIANGLES, m_start_index_cylinder_2, m_num_vertices_cylinder_2 );

    glDisableVertexAttribArray( m_vertex_location_normal_lcs );
    glDisableVertexAttribArray( m_vertex_location_position_lcs );
}

std::vector< CylindersRenderer::Vertex > 
CylindersRenderer::generateVertices( const int num_edges )
{
    std::vector< Vertex > vertices;

    const auto num_edges_f = static_cast< float >( num_edges );

    for ( int strip = 0; strip < num_edges; strip++ ) {

        const float rad_1 = static_cast< float >( strip ) 
                          / num_edges_f * 2.0f * M_PI;

        const float rad_2 = static_cast< float >( (strip + 1) % num_edges ) 
                          / num_edges_f * 2.0f * M_PI;

        const float rad_mid = ( rad_1 + rad_2 ) * 0.5f;

        const auto cos_rad_1 = std::cos( rad_1 );
        const auto sin_rad_1 = std::sin( rad_1 );

        const auto cos_rad_2 = std::cos( rad_2 );
        const auto sin_rad_2 = std::sin( rad_2 );

        const auto cos_rad_mid = std::cos( rad_mid );
        const auto sin_rad_mid = std::sin( rad_mid );

        const glm::vec4 n_side  { 0.0f, cos_rad_mid, sin_rad_mid, 0.0f };
        const glm::vec4 n_disc_1{ 1.0f, 0.0f, 0.0f, 0.0f };
        const glm::vec4 n_disc_2{-1.0f, 0.0f, 0.0f, 0.0f };

        const auto y1 = cos_rad_1;
        const auto z1 = sin_rad_1;
        const auto y2 = cos_rad_2;
        const auto z2 = sin_rad_2;

        const glm::vec4 p_side_1{ 0.5f, y1, z1, 1.0f };
        const glm::vec4 p_side_2{-0.5f, y1, z1, 1.0f };
        const glm::vec4 p_side_3{ 0.5f, y2, z2, 1.0f };
        const glm::vec4 p_side_4{-0.5f, y2, z2, 1.0f };

        const glm::vec4 p_disc_center_1{ 0.5f, 0.0f, 0.0f, 1.0f };
        const glm::vec4 p_disc_center_2{-0.5f, 0.0f, 0.0f, 1.0f };

        vertices.emplace_back( p_side_1, n_side );
        vertices.emplace_back( p_side_2, n_side );
        vertices.emplace_back( p_side_4, n_side );
        vertices.emplace_back( p_side_1, n_side );
        vertices.emplace_back( p_side_4, n_side );
        vertices.emplace_back( p_side_3, n_side );

        vertices.emplace_back( p_disc_center_1, n_disc_1 );
        vertices.emplace_back( p_side_1, n_disc_1 );
        vertices.emplace_back( p_side_3, n_disc_1 );

        vertices.emplace_back( p_disc_center_2, n_disc_2 );
        vertices.emplace_back( p_side_4, n_disc_2 );
        vertices.emplace_back( p_side_2, n_disc_2 );
    }

    return vertices;
}

} // namespace DepthTest
