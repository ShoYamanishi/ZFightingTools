#include <cmath>

#include "square_renderer.hpp"

namespace DepthTest {

SquareRenderer::SquareRenderer( const DepthTestType depth_test_type )
    :m_depth_test_type             { depth_test_type }
    ,m_uniform_M_plane1            { 1.0f }
    ,m_uniform_M_plane2            { 1.0f }
    ,m_uniform_V                   { 1.0f }
    ,m_uniform_P                   { 1.0f }
    ,m_gl_prog_id                  { 0 }
    ,m_gl_vertex_array             { 0 }
    ,m_gl_vertex_buffer            { 0 }
    ,m_vertex_location_position_lcs{ 0 }
    ,m_uniform_location_P          { 0 }
    ,m_uniform_location_V          { 0 }
    ,m_uniform_location_M          { 0 }
    ,m_uniform_location_param_c    { 0 }
    ,m_uniform_location_log_cf_plus_1_inv
                                   { 0 }
    ,m_uniform_location_fg_color   { 0 }
{
    switch( m_depth_test_type ) {

      case PERSPECTIVE:
        m_gl_prog_id = compileAndLink( VERT_STR_NORMAL_DEPTH, FRAG_STR_NORMAL_DEPTH, std::cerr );
        break;

      case LOG_DEPTH_FN:
        m_gl_prog_id = compileAndLink( VERT_STR_LOG_DEPTH_FN, FRAG_STR_LOG_DEPTH_FN, std::cerr );
        break;

      case LOG_DEPTH_CF:
        m_gl_prog_id = compileAndLink( VERT_STR_LOG_DEPTH_CF, FRAG_STR_LOG_DEPTH_CF, std::cerr );
        break;

      default:
        throw std::runtime_error("unknown depth type");
    }

    glGenVertexArrays ( 1, &m_gl_vertex_array  );

    glBindVertexArray( m_gl_vertex_array );

    glUseProgram( m_gl_prog_id );

    m_vertex_location_position_lcs = glGetAttribLocation( m_gl_prog_id, "position_lcs"   );

    m_uniform_location_P           = glGetUniformLocation( m_gl_prog_id, "P" );
    m_uniform_location_V           = glGetUniformLocation( m_gl_prog_id, "V" );
    m_uniform_location_M           = glGetUniformLocation( m_gl_prog_id, "M" );
    m_uniform_location_fg_color    = glGetUniformLocation( m_gl_prog_id, "fg_color" );

    switch( m_depth_test_type ) {

      case PERSPECTIVE:
        break;

      case LOG_DEPTH_FN:
        m_uniform_location_log_near = glGetUniformLocation( m_gl_prog_id, "log_near" );
        m_uniform_location_log_far  = glGetUniformLocation( m_gl_prog_id, "log_far" );
        break;

      case LOG_DEPTH_CF:
        m_uniform_location_param_c = glGetUniformLocation( m_gl_prog_id, "param_c" );
        m_uniform_location_log_cf_plus_1_inv
                                   = glGetUniformLocation( m_gl_prog_id, "log_cf_plus_1_inv" );
        break;
      default:
        ;
    }

    glGenBuffers( 1, &m_gl_vertex_buffer );
    glBindBuffer( GL_ARRAY_BUFFER, m_gl_vertex_buffer );
    glBufferData( GL_ARRAY_BUFFER, 6 * sizeof(glm::vec4),  nullptr, GL_DYNAMIC_DRAW );

    glGenFramebuffers( 1, &m_frame_buffer_tester );
    glBindFramebuffer( GL_FRAMEBUFFER, m_frame_buffer_tester );

    glGenRenderbuffers( 1, &m_render_buffer_color_tester );
    glBindRenderbuffer( GL_RENDERBUFFER, m_render_buffer_color_tester );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_RGBA8, 1, 1 );
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_render_buffer_color_tester );

    glGenRenderbuffers( 1, &m_render_buffer_depth_stencil_tester );
    glBindRenderbuffer( GL_RENDERBUFFER, m_render_buffer_depth_stencil_tester );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1, 1 );
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_render_buffer_depth_stencil_tester );
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

SquareRenderer::~SquareRenderer()
{
    glDeleteRenderbuffers( 1, &m_render_buffer_depth_stencil_tester );
    glDeleteRenderbuffers( 1, &m_render_buffer_color_tester );
    glDeleteFramebuffers( 1, &m_frame_buffer_tester );

    glDeleteBuffers      ( 1, &m_gl_vertex_buffer );
    glDeleteProgram      (     m_gl_prog_id       );
    glDeleteVertexArrays ( 1, &m_gl_vertex_array  );
}

void SquareRenderer::renderInteractive(
    const glm::ivec2& screen_pos,
    const glm::ivec2& screen_wh,
    const float near,
    const float far,
    const float param_c,
    const float plane_1,
    const float plane_2,
    const float edge_length
){
    glm::vec4 color_1{ 1.0f, 0.5f, 0.5f, 1.0f};
    glm::vec4 color_2{ 0.0f, 0.8f, 1.0f, 1.0f};

    const float fovy_half = 0.22f * M_PI;
    const float top  = atan( fovy_half * 0.5f ) * near;

    const float aspect_ratio =  static_cast<float>( screen_wh.x ) 
                              / static_cast<float>( screen_wh.y );
    glm::mat4 Mview{1.0f};
    glm::mat4 Mproj = glm::perspective( fovy_half, aspect_ratio, near, far );

    glm::mat4 Mmodel_1{1.0f};
    Mmodel_1[3][0] = 0.2 * edge_length;
    Mmodel_1[3][1] = 0.5 * edge_length;
    Mmodel_1[3][2] = -1.0 * plane_1;

    glm::mat4 Mmodel_2{1.0f};
    Mmodel_2[3][0] = -0.2 * edge_length;
    Mmodel_2[3][1] = -0.5 * edge_length;
    Mmodel_2[3][2] = -1.0 * plane_2;  

    m_vertices[0] = glm::vec4{ -1.0f * edge_length, -1.0f * edge_length, 0.0f, 1.0f };
    m_vertices[1] = glm::vec4{         edge_length, -1.0f * edge_length, 0.0f, 1.0f };
    m_vertices[2] = glm::vec4{ -1.0  * edge_length,         edge_length, 0.0f, 1.0f };
    m_vertices[3] = m_vertices[1];
    m_vertices[4] = glm::vec4{         edge_length,         edge_length, 0.0f, 1.0f };
    m_vertices[5] = m_vertices[2];

    glEnable( GL_DEPTH_TEST );
    glDisable( GL_BLEND );

    glBindVertexArray( m_gl_vertex_array );
    glUseProgram( m_gl_prog_id );

    glViewport(
        static_cast<GLint>( screen_pos.x ),
        static_cast<GLint>( screen_pos.y ),
        static_cast<GLsizei>( screen_wh.x ),
        static_cast<GLsizei>( screen_wh.y )
    );

    glBindBuffer( GL_ARRAY_BUFFER, m_gl_vertex_buffer );       
    glBufferSubData( GL_ARRAY_BUFFER, 0, 6 * sizeof(glm::vec4), m_vertices );

    glUniformMatrix4fv( m_uniform_location_V, 1, GL_FALSE, &Mview[0][0] );
    glUniformMatrix4fv( m_uniform_location_P, 1, GL_FALSE, &Mproj[0][0] );

    glUniformMatrix4fv( m_uniform_location_M, 1, GL_FALSE, &Mmodel_1[0][0] );
    glUniform4fv( m_uniform_location_fg_color, 1, &(color_1[0] ) );

    switch( m_depth_test_type ) {

      case PERSPECTIVE:
        break;

      case LOG_DEPTH_FN:

        glUniform1f( m_uniform_location_log_near, log( near ) );
        glUniform1f( m_uniform_location_log_far,  log( far  ) );
        break;

      case LOG_DEPTH_CF:
        glUniform1f( m_uniform_location_param_c, param_c );
        glUniform1f( m_uniform_location_log_cf_plus_1_inv, 1.0f / log( param_c * far + 1.0f ) ); 
        break;

      default:
        ;
    }

    glEnableVertexAttribArray( m_vertex_location_position_lcs );

    glVertexAttribPointer(
        m_vertex_location_position_lcs,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(glm::vec4),
        (void*)0
    );

    glDrawArrays( GL_TRIANGLES, 0, 6 );

    glUniformMatrix4fv( m_uniform_location_M, 1, GL_FALSE, &Mmodel_2[0][0] );
    glUniform4fv( m_uniform_location_fg_color, 1, &(color_2[0] ) );

    glDrawArrays( GL_TRIANGLES, 0, 6 );

    glDisableVertexAttribArray( m_vertex_location_position_lcs );
}


void SquareRenderer::test(
    const float near,
    const float far,
    const float param_c,
    const float plane_1,
    const float plane_2,
    bool&       plane_1_detected,
    bool&       plane_2_detected
) {

    glBindFramebuffer( GL_FRAMEBUFFER, m_frame_buffer_tester );

    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    glClear( GL_DEPTH_BUFFER_BIT );
    glStencilMask( 0xff );
    glClear( GL_STENCIL_BUFFER_BIT );
    glDisable( GL_BLEND );
    glEnable( GL_DEPTH_TEST );
    glDisable( GL_CULL_FACE );

    glDisable(GL_STENCIL_TEST);
    glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

    glm::vec4 color_1{ 1.0f, 0.0f, 0.0f, 1.0f};
    glm::vec4 color_2{ 0.0f, 1.0f, 0.0f, 1.0f};

    const float fovy_half = 0.22f * M_PI;
    const float top  = atan( fovy_half * 0.5f ) * near;
    const float edge_one_pixel = top / 512.0f; // assuming 1024 pixels.

    glm::mat4 Mview{1.0f};
    glm::mat4 Mproj = glm::frustum(
        -0.5f * edge_one_pixel,
         0.5f * edge_one_pixel,
        -0.5f * edge_one_pixel,
         0.5f * edge_one_pixel,
         near,
         far 
    );

    glm::mat4 Mmodel_1{1.0f};
    Mmodel_1[3][2] = -1.0 * plane_1;

    glm::mat4 Mmodel_2{1.0f};
    Mmodel_2[3][2] = -1.0 * plane_2;

    const auto edge_length = ( top / near ) * std::max( plane_1, plane_2 );

    m_vertices[0] = glm::vec4{ -1.0f * edge_length, -1.0f * edge_length, 0.0f, 1.0f };
    m_vertices[1] = glm::vec4{         edge_length, -1.0f * edge_length, 0.0f, 1.0f };
    m_vertices[2] = glm::vec4{ -1.0  * edge_length,         edge_length, 0.0f, 1.0f };
    m_vertices[3] = m_vertices[1];
    m_vertices[4] = glm::vec4{         edge_length,         edge_length, 0.0f, 1.0f };
    m_vertices[5] = m_vertices[2];

    glEnable( GL_DEPTH_TEST );
    glDisable( GL_BLEND );

    glViewport( 0, 0, 1, 1 );

    glBindVertexArray( m_gl_vertex_array );
    glUseProgram( m_gl_prog_id );

    glBindBuffer( GL_ARRAY_BUFFER, m_gl_vertex_buffer );       
    glBufferSubData( GL_ARRAY_BUFFER, 0, 6 * sizeof(glm::vec4), m_vertices );

    glUniformMatrix4fv( m_uniform_location_V, 1, GL_FALSE, &Mview[0][0] );
    glUniformMatrix4fv( m_uniform_location_P, 1, GL_FALSE, &Mproj[0][0] );

    glUniformMatrix4fv( m_uniform_location_M, 1, GL_FALSE, &Mmodel_1[0][0] );
    glUniform4fv( m_uniform_location_fg_color, 1, &(color_1[0] ) );

    switch( m_depth_test_type ) {

      case PERSPECTIVE:
        break;

      case LOG_DEPTH_FN:

        glUniform1f( m_uniform_location_log_near, log( near ) );
        glUniform1f( m_uniform_location_log_far,  log( far  ) );
        break;

      case LOG_DEPTH_CF:
        glUniform1f( m_uniform_location_param_c, param_c );
        glUniform1f( m_uniform_location_log_cf_plus_1_inv, 1.0f / log( param_c * far + 1.0f ) ); 
        break;

      default:
        ;
    }

    glEnableVertexAttribArray( m_vertex_location_position_lcs );

    glVertexAttribPointer(
        m_vertex_location_position_lcs,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(glm::vec4),
        (void*)0
    );

    glDrawArrays( GL_TRIANGLES, 0, 6 );

    glUniformMatrix4fv( m_uniform_location_M, 1, GL_FALSE, &Mmodel_2[0][0] );
    glUniform4fv( m_uniform_location_fg_color, 1, &(color_2[0] ) );

    glDrawArrays( GL_TRIANGLES, 0, 6 );

    glDisableVertexAttribArray( m_vertex_location_position_lcs );

    glFlush();
    glFinish();
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    unsigned char pixel_read[4];
    glReadPixels( 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel_read );
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );

    plane_1_detected = ( pixel_read[0] == 255 && pixel_read[1] == 0 );

    plane_2_detected = ( pixel_read[1] == 255 && pixel_read[0] == 0 );
}

} // namespace DepthTest
