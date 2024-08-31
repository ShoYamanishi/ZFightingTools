#include "text_renderer_line.hpp"

namespace DepthTest {

TextRendererLine::TextRendererLine(
    Font::RuntimeHelper& helper,
    const std::string&   str,
    const float          font_size,
    const glm::vec4&     fg_color,
    const glm::vec4&     bg_color
)
    :m_helper                  { helper }
    ,m_str                     { str }
    ,m_font_size               { font_size }
    ,m_fg_color                { fg_color }
    ,m_bg_color                { bg_color }
    ,m_base                    { 0.0f, 0.0f }
    ,m_bounding_box_bottom_left{ 0.0f, 0.0f }
    ,m_bounding_box_top_right  { 0.0f, 0.0f }
    ,m_num_glyphs              { 0 }
    ,m_num_vertices            { 0 }
    ,m_num_indices             { 0 }
    ,m_vertices                { nullptr }
{
    const auto glyph_bounding_boxes = generateGlyphBoundingBoxes();

    m_num_glyphs = glyph_bounding_boxes.size();

    m_num_vertices = m_num_glyphs * 4;
    m_num_indices  = m_num_glyphs * 6;

    findTotalBoundingBox( glyph_bounding_boxes );

    generateRenderVertices( glyph_bounding_boxes );
}

TextRendererLine::~TextRendererLine()
{
    if ( m_vertices != nullptr ) {

        delete[] m_vertices;
    }
}

vector< Font::GlyphBound > TextRendererLine::generateGlyphBoundingBoxes()
{
    vector< const Font::Glyph* > glyphs;
    vector< Font::Point2D >      instance_origins;
    float                        width;
    float                        height;
    float                        above_baseline_y;
    float                        below_baseline_y;

    m_helper.getGlyphOriginsWidthAndHeight(
        m_str,
        m_font_size,
        DEFAULT_LETTER_SPACING,
        m_base.x,
        m_base.y,
        glyphs,
        instance_origins,
        width,
        height,
        above_baseline_y,
        below_baseline_y
    );

    vector< Font::GlyphBound > bounding_boxes;
    m_helper.getBoundingBoxes(
        m_font_size,
        DEFAULT_FONT_SPREAD,
        glyphs,
        instance_origins,
        bounding_boxes
    );

    return bounding_boxes;
}

void TextRendererLine::findTotalBoundingBox( const std::vector< Font::GlyphBound >& bounding_boxes )
{
    if ( bounding_boxes.empty() ) {
        return;
    }

    float min_x = bounding_boxes[0].mFrame.mX;
    float max_x = bounding_boxes[0].mFrame.mX + bounding_boxes[0].mFrame.mW;
    float min_y = bounding_boxes[0].mFrame.mY;
    float max_y = bounding_boxes[0].mFrame.mX + bounding_boxes[0].mFrame.mH;

    for ( int i = 0; i < m_num_glyphs; i++ ) {

        min_x = std::min( min_x, bounding_boxes[i].mFrame.mX                       );
        max_x = std::max( max_x, bounding_boxes[i].mFrame.mX + bounding_boxes[i].mFrame.mW );
        min_y = std::min( min_y, bounding_boxes[i].mFrame.mY                       );
        max_y = std::max( max_y, bounding_boxes[i].mFrame.mY + bounding_boxes[i].mFrame.mH );
    }

    m_bounding_box_bottom_left = glm::vec2{ min_x, min_y };
    m_bounding_box_top_right   = glm::vec2{ max_x, max_y };
}

void TextRendererLine::generateRenderVertices( const std::vector< Font::GlyphBound >& bounding_boxes )
{
    if ( bounding_boxes.empty() ) {
        return;
    }

    m_vertices = new TextRendererVertex[ m_num_vertices ];

    for ( int i = 0; i < m_num_glyphs; i++ ) {

        const auto& f = bounding_boxes[i].mFrame;
        const auto& t = bounding_boxes[i].mTexture;

        const auto x0 = f.mX;
        const auto x1 = f.mX + f.mW;
        const auto y0 = f.mY;
        const auto y1 = f.mY + f.mH; 

        const auto u0 = t.mX;
        const auto u1 = t.mX + t.mW;
        const auto v0 = t.mY;
        const auto v1 = t.mY + t.mH;

        const glm::vec4 p0{ x0, y0, 0.0f, 1.0f };
        const glm::vec4 p1{ x1, y0, 0.0f, 1.0f };
        const glm::vec4 p2{ x1, y1, 0.0f, 1.0f };
        const glm::vec4 p3{ x0, y1, 0.0f, 1.0f };

        const glm::vec2 uv0{ u0, v0 };
        const glm::vec2 uv1{ u1, v0 };
        const glm::vec2 uv2{ u1, v1 };
        const glm::vec2 uv3{ u0, v1 };

        const TextRendererVertex vert0{  p0, m_fg_color, m_bg_color, uv0 };
        const TextRendererVertex vert1{  p1, m_fg_color, m_bg_color, uv1 };
        const TextRendererVertex vert2{  p2, m_fg_color, m_bg_color, uv2 };
        const TextRendererVertex vert3{  p3, m_fg_color, m_bg_color, uv3 };

        m_vertices[ i * 4     ] = vert0;
        m_vertices[ i * 4 + 1 ] = vert1;
        m_vertices[ i * 4 + 2 ] = vert2;
        m_vertices[ i * 4 + 3 ] = vert3;
    }

}

void TextRendererLine::setForegroundColor( const glm::vec4& color )
{
    for ( int i = 0; i < m_num_glyphs; i++ ) {

        m_vertices[i].setForegroundColor( color );
    }
}

void TextRendererLine::setBackgroundColor( const glm::vec4& color )
{
    for ( int i = 0; i < m_num_glyphs; i++ ) {

        m_vertices[i].setBackgroundColor( color );
    }
}

void TextRendererLine::setBaseXY( const glm::vec2& base )
{
    const auto delta = base - m_base;
    m_base = base;

    for ( int i = 0; i < m_num_vertices; i++ ) {

        auto& v = m_vertices[i];
        auto  p = v.pos();
        p.x += delta.x;
        p.y += delta.y;
        v.setPos( p );
    }
}

int32_t TextRendererLine::numVertices() const
{
    return m_num_vertices;
}

int32_t TextRendererLine::numIndices() const
{
    return m_num_indices;
}

void TextRendererLine::fillVertexBuffer( TextRendererVertex* buf_vertex ) const
{
    memcpy( buf_vertex, m_vertices, sizeof( TextRendererVertex ) * m_num_vertices );
}

void TextRendererLine::fillIndexBuffer( uint32_t* buf_index, uint32_t vertex_start ) const
{
    for ( int i = 0; i < m_num_indices; i+= 6 ) {

        buf_index[i  ] = vertex_start;
        buf_index[i+1] = vertex_start + 1;
        buf_index[i+2] = vertex_start + 2;

        buf_index[i+3] = vertex_start;
        buf_index[i+4] = vertex_start + 2;
        buf_index[i+5] = vertex_start + 3;

        vertex_start += 4;
    }
}

} // namespace DepthTest
