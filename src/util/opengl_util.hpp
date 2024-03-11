#ifndef __DEPTH_TEST_OPENGL_UTIL_HPP__
#define __DEPTH_TEST_OPENGL_UTIL_HPP__

#include <iostream>
#include <string>
#include <GL/glew.h>

namespace DepthTest {

GLuint compileAndLink(

    const std::string& vertex_str, 
    const std::string& fragment_str,
    std::ostream&      os

);

class OpenGLInfo {

public:

    OpenGLInfo();

    void show( std::ostream& os ) const;

    GLint                      m_major_version;
    GLint                      m_minor_version;
    std::vector< std::string > m_extensions;

    GLint                      m_GL_MAX_TEXTURE_SIZE;
    GLint                      m_GL_MAX_CUBE_MAP_TEXTURE_SIZE;

friend std::ostream& operator << ( std::ostream& os, const OpenGLInfo& info );

};

std::ostream& operator << ( std::ostream& os, const OpenGLInfo& info );

GLuint generateFontTexture( GLsizei width,GLsizei height, unsigned char* pixmap );

} // namespace DepthTest

#endif/*__DEPTH_TEST_OPENGL_UTIL_HPP__*/
