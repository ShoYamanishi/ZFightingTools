#ifndef __DEPTH_TEST_UI_PNG_UTIL_HPP__
#define __DEPTH_TEST_UI_PNG_UTIL_HPP__
#include <iostream>
#include <png.h>

namespace DepthTest {

class PNG {

public:
    PNG( const std::string& file_path );

    virtual ~PNG();

    void showParams( std::ostream& os ) const;

    int width()  const { return static_cast<int>(m_width);  }
    int height() const { return static_cast<int>(m_height); }
    unsigned char* data() const;
    bool isRGB()  const { return m_color_type == PNG_COLOR_TYPE_RGB; }
    bool isRGBA() const { return m_color_type == PNG_COLOR_TYPE_RGB_ALPHA; }

private:
    
    void loadImage();

    const std::string strColor() const;
    const std::string strInterlace() const;

    const std::string  m_file_path;
    png_uint_32        m_width;
    png_uint_32        m_height;
    int                m_color_type;
    int                m_interlace;
    int                m_depth;
    unsigned char*     m_pixels;
};

std::ostream& operator << ( std::ostream& os, const PNG& png );

} // namespace DepthTest

#endif /*__DEPTH_TEST_UI_PNG_UTIL_HPP__*/
