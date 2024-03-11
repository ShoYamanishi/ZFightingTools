#include "opengl_util.hpp"

namespace DepthTest {

OpenGLInfo::OpenGLInfo()
{
    glGetIntegerv( GL_MAJOR_VERSION, &m_major_version );
    glGetIntegerv( GL_MINOR_VERSION, &m_minor_version );
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &m_GL_MAX_TEXTURE_SIZE );
    glGetIntegerv( GL_MAX_CUBE_MAP_TEXTURE_SIZE, &m_GL_MAX_CUBE_MAP_TEXTURE_SIZE );

    GLint num_extensions;
    glGetIntegerv( GL_NUM_EXTENSIONS, &num_extensions );

    for ( int i = 0; i < num_extensions; i++ ) {

        const auto* p = glGetStringi( GL_EXTENSIONS, i );

        if ( p == nullptr ) {

            throw std::runtime_error(
                "glGetStringi( GL_EXTENSIONS, i ) failed."
                "Probably the context not yet set."
             );
        }

        m_extensions.emplace_back( reinterpret_cast<const char*>( p ) );
    }
}    

void OpenGLInfo::show( std::ostream& os ) const
{
    os << "major version: " << m_major_version << ", ";
    os << "minor version: " << m_minor_version << ", ";
    os << "GL_MAX_TEXTURE_SIZE: " <<  m_GL_MAX_TEXTURE_SIZE << ", ";
    os << "GL_MAX_CUBE_MAP_TEXTURE_SIZE: " << m_GL_MAX_CUBE_MAP_TEXTURE_SIZE << ", ";
    os << "extensions:";

    for ( const auto& e : m_extensions ) {

        os << " " << e;
    }
}

std::ostream& operator << ( std::ostream& os, const OpenGLInfo& info )
{
    info.show( os );
    return os;
}

GLuint generateFontTexture( GLsizei width,GLsizei height, unsigned char* pixmap )
{
    GLuint texture_name;
    glGenTextures( 1, &texture_name );

    glBindTexture( GL_TEXTURE_2D, texture_name );

    glTexImage2D( GL_TEXTURE_2D,
                  0,
                  GL_R8,
                  width,
                  height,
                  0,
                  GL_RED,
                  GL_UNSIGNED_BYTE,
                  pixmap
    );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT  );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT  );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR  );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR  );
    glGenerateMipmap( GL_TEXTURE_2D );

    return texture_name;
}

} // namespace DepthTest
