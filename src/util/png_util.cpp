#include <stdexcept>
#include "png_util.hpp"

namespace DepthTest {


PNG::PNG( const std::string& file_path )
    :m_file_path{ file_path }
    ,m_pixels   { nullptr }
{
    loadImage();
}


PNG::~PNG()
{
    if ( m_pixels != nullptr ) {
        free( m_pixels );
    }
}


const std::string PNG::strColor() const
{
    if ( m_color_type == PNG_COLOR_TYPE_GRAY ) {

        return "PNG_COLOR_TYPE_GRAY";
    }
    else if ( m_color_type == PNG_COLOR_TYPE_PALETTE ) {

        return "PNG_COLOR_TYPE_PALETTE";
    } 
    else if ( m_color_type == PNG_COLOR_TYPE_RGB ) {

        return "PNG_COLOR_TYPE_RGB";
    } 
    else if ( m_color_type == PNG_COLOR_TYPE_RGB_ALPHA ) {

        return "PNG_COLOR_TYPE_RGB_ALPHA";
    }
    else if ( m_color_type == PNG_COLOR_TYPE_GRAY_ALPHA ) {

        return "PNG_COLOR_TYPE_GRAY_ALPHA";
    }

    return "UNKNOWN";
}


const std::string PNG::strInterlace() const
{
    if ( m_interlace == PNG_INTERLACE_NONE ) {

        return "PNG_INTERLACE_NONE";
    }
    else if ( m_interlace == PNG_INTERLACE_ADAM7 ) {
        return "PNG_INTERLACE_ADAM7";
    }
    else if ( m_interlace == PNG_INTERLACE_LAST ) {
        return "PNG_INTERLACE_LAST";
    }

    return "UNKNOWN";
}

void PNG::showParams( std::ostream& os ) const
{
    os << "file: "      << m_file_path << ", ";
    os << "width: "     << m_width << ", ";
    os << "height: "    << m_height << ", ";
    os << "color: "     << strColor() << ", ";
    os << "interlace: " << strInterlace() << ", ";
    os << "depth: "     << m_depth << "";
}

unsigned char* PNG::data() const
{
    return m_pixels;
}

void PNG::loadImage()
{
    FILE* fp = fopen( m_file_path.c_str(), "rb" );

    if (fp == nullptr ) {

        throw std::runtime_error( "Can not open file." );
    }

    png_structp png_struct = png_create_read_struct (

        PNG_LIBPNG_VER_STRING,
        NULL,
        NULL,
        NULL
    );

    if ( png_struct == nullptr ) {

        fclose(fp);

        throw std::runtime_error( "error in png_create_read_struct." );
    }

    png_infop png_info = png_create_info_struct( png_struct );

    if ( png_info == NULL ) {


        fclose(fp);
        png_destroy_read_struct( &png_struct, NULL, NULL );


        throw std::runtime_error( "error in png_create_info_struct." );
    }

    if ( setjmp( png_jmpbuf( png_struct ) ) != 0 ) {


        png_destroy_read_struct( &png_struct, &png_info, NULL );
        fclose(fp);

        throw std::runtime_error( "error in setjmp" );
    }

    png_init_io( png_struct, fp);

    unsigned int sig_read = 0;

    png_set_sig_bytes( png_struct, sig_read );

    png_read_png (

        png_struct,
        png_info,
        PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND,
        NULL
    );

    png_get_IHDR (

        png_struct,
        png_info,
        &m_width,
        &m_height,
        &m_depth,
        &m_color_type,
        &m_interlace,
        NULL,
        NULL
    );

    auto row_bytes = png_get_rowbytes( png_struct, png_info );

    m_pixels = (unsigned char*) malloc( row_bytes * m_height );

    auto row_pointers = png_get_rows( png_struct, png_info );

    for ( int i = 0; i < m_height; i++ ) {

        // flipping the Y-direction.
        memcpy(
            m_pixels + ( row_bytes * ( m_height - 1 - i ) ),
            row_pointers[ i ],
            row_bytes
        );
    }

    png_destroy_read_struct( &png_struct, &png_info, NULL );

    fclose( fp );
}

std::ostream& operator << ( std::ostream& os, const PNG& png )
{
    png.showParams( os );
    return os;
}

} // namespace DepthTest
