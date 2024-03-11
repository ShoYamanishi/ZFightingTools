#include "opengl_util.hpp"

namespace DepthTest {


static void compile( const GLuint id, const std::string&str, std::ostream& os );

static GLuint link( const GLuint vertex_id, const GLuint frag_id, std::ostream& os );

GLuint compileAndLink(

    const std::string& vertex_str, 
    const std::string& fragment_str,
    std::ostream&      os

) {
    const auto vertex_id = glCreateShader( GL_VERTEX_SHADER );

    if ( vertex_id == 0 ) {

        throw std::runtime_error("glCreateShader( GL_VERTEX_SHADER ) failed.");
    }

    const auto frag_id = glCreateShader( GL_FRAGMENT_SHADER );

    if ( frag_id == 0 ) {

        throw std::runtime_error("glCreateShader( GL_FRAGMENT_SHADER ) failed.");
    }

    compile( vertex_id, vertex_str,   os );
    compile( frag_id,   fragment_str, os );

    const auto prog_id = link( vertex_id, frag_id, os );

    return prog_id;
}

void compile( const GLuint id, const std::string& str, std::ostream& os )
{
    GLint result   = GL_FALSE;
    int   info_len = 0;

    const GLchar* c_str = str.c_str();

    glShaderSource( id, 1, &c_str , nullptr );
    glCompileShader( id );

    glGetShaderiv( id, GL_COMPILE_STATUS,  &result   );
    glGetShaderiv( id, GL_INFO_LOG_LENGTH, &info_len );

    if ( info_len > 0 ) {

        auto* p = new char[ info_len + 1 ];
        glGetShaderInfoLog( id, info_len, nullptr, p );
        os << p << "\n";
        delete[] p;

        throw std::runtime_error( "shader compilation failed." );
    }
}

GLuint link( const GLuint vertex_id, const GLuint frag_id, std::ostream& os )
{
    GLint result   = GL_FALSE;
    int   info_len = 0;

    const auto prog_id = glCreateProgram();

    if ( prog_id == 0 ) {

        throw std::runtime_error("glCreateProgram() failed.");
    }

    glAttachShader( prog_id, vertex_id );
    glAttachShader( prog_id, frag_id   );

    glLinkProgram( prog_id );

    glGetProgramiv( prog_id, GL_LINK_STATUS, &result);
    glGetProgramiv( prog_id, GL_INFO_LOG_LENGTH, &info_len );

    if ( info_len > 0 ) {

        auto* p = new char[ info_len + 1 ];
        glGetProgramInfoLog( prog_id, info_len, NULL, p );
        os << p << "\n";
        delete[] p;

        throw std::runtime_error( "shader link failed." );
    }

    glDetachShader( prog_id, vertex_id );
    glDetachShader( prog_id, frag_id );

    glDeleteShader( vertex_id );
    glDeleteShader( frag_id );

    return prog_id;
}

} // namespace DepthTest
