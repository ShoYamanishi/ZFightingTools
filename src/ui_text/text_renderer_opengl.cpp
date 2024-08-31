#include <glm/gtc/matrix_transform.hpp>

#include "text_renderer_opengl.hpp"


namespace DepthTest {

TextRendererOpenGL::TextRendererOpenGL(
    GLFWWindow&       window,
    const std::string font_path,
    const float       gate1_low,
    const float       gate1_high,
    const float       gate2_low,
    const float       gate2_high
)
    :m_window           { window }
    ,m_font_path        { font_path }
    ,m_gate1_low        { gate1_low }
    ,m_gate1_high       { gate1_high }
    ,m_gate2_low        { gate2_low }
    ,m_gate2_high       { gate2_high }
    ,m_uniform_M        { 1.0f }
    ,m_uniform_V        { 1.0f }
    ,m_uniform_P        { 1.0f }
    ,m_vertices         { nullptr }
    ,m_indices          { nullptr }
    ,m_num_vertices     { 0 }
    ,m_num_indices      { 0 }
    ,m_vertices_capacity_in_bytes{ 0 }
    ,m_indices_capacity_in_bytes { 0 }
    ,m_gl_prog_id       { 0 }
    ,m_gl_vertex_array  { 0 }
    ,m_gl_vertex_buffer { 0 }
    ,m_gl_index_buffer  { 0 }
{
    m_gl_prog_id = compileAndLink( VERT_STR_TEXT, FRAG_STR_TEXT, std::cerr );

    glGenVertexArrays ( 1, &m_gl_vertex_array  );

    PNG png{ font_path };
    std::cerr << "font image: " << png << "\n";

    m_font_texture = generateFontTexture( png.width(), png.height(), png.data() );

    glBindVertexArray( m_gl_vertex_array );

    glUseProgram( m_gl_prog_id );

    m_vertex_location_position_lcs = glGetAttribLocation( m_gl_prog_id, "position_lcs"   );
    m_vertex_location_fg_color     = glGetAttribLocation( m_gl_prog_id, "fg_color_vin"   );
    m_vertex_location_bg_color     = glGetAttribLocation( m_gl_prog_id, "bg_color_vin"   );
    m_vertex_location_texture_uv   = glGetAttribLocation( m_gl_prog_id, "texture_uv_vin" );


    m_uniform_location_P              = glGetUniformLocation( m_gl_prog_id, "P" );
    m_uniform_location_V              = glGetUniformLocation( m_gl_prog_id, "V" );
    m_uniform_location_M              = glGetUniformLocation( m_gl_prog_id, "M" );

    m_uniform_location_gate1_low      = glGetUniformLocation( m_gl_prog_id, "gate1_low" );
    m_uniform_location_gate1_high     = glGetUniformLocation( m_gl_prog_id, "gate1_high" );
    m_uniform_location_gate2_low      = glGetUniformLocation( m_gl_prog_id, "gate2_low" );
    m_uniform_location_gate2_high     = glGetUniformLocation( m_gl_prog_id, "gate2_high" );
    m_uniform_location_sampler_font   = glGetUniformLocation( m_gl_prog_id, "sampler_font" );
}

TextRendererOpenGL::~TextRendererOpenGL()
{
    glDeleteProgram      ( m_gl_prog_id );
    glDeleteVertexArrays ( 1, &m_gl_vertex_array  );

    if ( m_gl_vertex_buffer != 0 ) {

        glDeleteBuffers( 1, &m_gl_vertex_buffer );
    }        

    if ( m_vertices != nullptr ) {

        delete[] m_vertices;
    }

    if ( m_gl_index_buffer != 0 ) {

        glDeleteBuffers( 1, &m_gl_index_buffer );
    }        

    if ( m_indices != nullptr ) {

        delete[] m_indices;
    }

    glDeleteTextures( 1, &m_font_texture );
}

void TextRendererOpenGL::registerLine( TextRendererLine* line )
{ 
    m_lines.insert( line );    
}
void TextRendererOpenGL::unregisterLine( TextRendererLine* line )
{
    m_lines.erase( line );    
}

void TextRendererOpenGL::render( const bool initialize_screen )
{
    updateVertexAndIndexBuffers();

    const auto frame = m_window.frameBufferSize();

    m_uniform_P = glm::ortho(
        0.0f,
        static_cast<float>( frame.x ),
        0.0f,
        static_cast<float>( frame.y ),
        -1.0f,
         1.0f
    );

    if ( initialize_screen ) {

        glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        glClear( GL_STENCIL_BUFFER_BIT );
    }

    glClear( GL_DEPTH_BUFFER_BIT );
    glDisable( GL_DEPTH_TEST );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glBindVertexArray( m_gl_vertex_array );
    glUseProgram( m_gl_prog_id );

    glViewport( 0, 0, 
        static_cast<GLsizei>( frame.x ),
        static_cast<GLsizei>( frame.y )
    );

    glBindBuffer( GL_ARRAY_BUFFER, m_gl_vertex_buffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_gl_index_buffer );

    glUniformMatrix4fv( m_uniform_location_M, 1, GL_FALSE, &m_uniform_M[0][0] );
    glUniformMatrix4fv( m_uniform_location_V, 1, GL_FALSE, &m_uniform_V[0][0] );
    glUniformMatrix4fv( m_uniform_location_P, 1, GL_FALSE, &m_uniform_P[0][0] );

    glUniform1i( m_uniform_location_sampler_font, 0 );

    glEnableVertexAttribArray( m_vertex_location_position_lcs );
    glEnableVertexAttribArray( m_vertex_location_fg_color     );
    glEnableVertexAttribArray( m_vertex_location_bg_color     );
    glEnableVertexAttribArray( m_vertex_location_texture_uv   );

    glVertexAttribPointer(
        m_vertex_location_position_lcs,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(TextRendererVertex),
        (void*)0
    );

    glVertexAttribPointer(
        m_vertex_location_fg_color,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(TextRendererVertex),
        (void*)( 4 * sizeof(float) )
    );

    glVertexAttribPointer(
        m_vertex_location_bg_color,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(TextRendererVertex),
        (void*)( 8 * sizeof(float) )
    );


    glVertexAttribPointer(
        m_vertex_location_texture_uv,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(TextRendererVertex),
        (void*)( 12 * sizeof(float) )
    );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, m_font_texture );

    glUniform1f( m_uniform_location_gate1_low,  m_gate1_low );
    glUniform1f( m_uniform_location_gate1_high, m_gate1_high );
    glUniform1f( m_uniform_location_gate2_low,  m_gate2_low );
    glUniform1f( m_uniform_location_gate2_high, m_gate2_high );

    glDrawElements(
        GL_TRIANGLES,
        m_num_indices,
        GL_UNSIGNED_INT,
        nullptr
    );

    glDisableVertexAttribArray( m_vertex_location_position_lcs );
    glDisableVertexAttribArray( m_vertex_location_fg_color     );
    glDisableVertexAttribArray( m_vertex_location_bg_color     );
    glDisableVertexAttribArray( m_vertex_location_texture_uv   );

    glDisable( GL_BLEND );
}

void TextRendererOpenGL::updateVertexAndIndexBuffers()
{
    m_num_vertices = 0;
    m_num_indices  = 0;

    for ( auto* line : m_lines ) {

        m_num_vertices += line->numVertices();
        m_num_indices  += line->numIndices();
    }

    if ( m_num_vertices * sizeof(TextRendererVertex) > m_vertices_capacity_in_bytes ) {

        m_vertices_capacity_in_bytes = findBufferCapacityInBytes( m_num_vertices * sizeof(TextRendererVertex) );
        recreateVertexBuffer();
    }

    if ( m_num_indices * sizeof(uint32_t)  > m_indices_capacity_in_bytes ) {

        m_indices_capacity_in_bytes = findBufferCapacityInBytes( m_num_indices * sizeof(uint32_t) );
        recreateIndexBuffer();
    }

    m_num_vertices = 0;
    m_num_indices  = 0;
    
    for ( auto* line : m_lines ) {

        line->fillVertexBuffer( &m_vertices[ m_num_vertices ] );
        line->fillIndexBuffer ( &m_indices [ m_num_indices  ], m_num_vertices );

        m_num_vertices += line->numVertices();
        m_num_indices  += line->numIndices();
    }

    glBindBuffer( GL_ARRAY_BUFFER, m_gl_vertex_buffer );
    glBufferSubData(
        GL_ARRAY_BUFFER,
        0,
        m_num_vertices * sizeof(TextRendererVertex),
        m_vertices
    );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_gl_index_buffer );
    glBufferSubData(
        GL_ELEMENT_ARRAY_BUFFER,
        0,
        m_num_indices * sizeof(uint32_t),
   	    m_indices
    );
}

int32_t TextRendererOpenGL::findBufferCapacityInBytes( int32_t size_in_bytes ) const
{
    const int32_t INITIAL_CAPACITY = 1024;
    const int32_t MAXIMUM_CAPACITY = 4 * 1024 * 1024;

    for ( int32_t capacity = 1024; capacity <= MAXIMUM_CAPACITY; capacity *= 2 ) {
        if ( size_in_bytes <= capacity ) {
            return capacity;
        }
    }

    throw std::runtime_error( "buffer size too big." );
}

void TextRendererOpenGL::recreateVertexBuffer()
{
    if ( m_gl_vertex_buffer != 0 ) {

        glDeleteBuffers( 1, &m_gl_vertex_buffer );
    }        

    if ( m_vertices != nullptr ) {

        delete[] m_vertices;
    }

    glGenBuffers( 1, &m_gl_vertex_buffer );
    glBindBuffer( GL_ARRAY_BUFFER, m_gl_vertex_buffer );
    glBufferData(
        GL_ARRAY_BUFFER,
        m_vertices_capacity_in_bytes,
        nullptr, 
        GL_DYNAMIC_DRAW
    );

    m_vertices = new TextRendererVertex[ m_vertices_capacity_in_bytes / sizeof(TextRendererVertex) ];
}

void TextRendererOpenGL::recreateIndexBuffer()
{
    if ( m_gl_index_buffer != 0 ) {

        glDeleteBuffers( 1, &m_gl_index_buffer );
    }        

    if ( m_indices != nullptr ) {

        delete[] m_indices;
    }

    glGenBuffers( 1, &m_gl_index_buffer  );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_gl_index_buffer );
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, 
        m_indices_capacity_in_bytes,
        nullptr,
        GL_DYNAMIC_DRAW
    );

    m_indices = new uint32_t[ m_indices_capacity_in_bytes / sizeof(uint32_t) ];
}

} // namespace DepthTest
