#ifndef __DEPTH_TEST_TEXT_RENDERER_LINE_HPP__
#define __DEPTH_TEST_TEXT_RENDERER_LINE_HPP__

#include <cstdint>
#include <glm/glm.hpp>

#include "font_runtime_helper.hpp"
#include "text_renderer_vertex.hpp"

namespace DepthTest {

class TextRendererLine {

public:
    static constexpr float DEFAULT_LETTER_SPACING = 1.0f;
    static constexpr float DEFAULT_FONT_SPREAD    = 0.6f;

    explicit TextRendererLine(
        Font::RuntimeHelper& helper,
        const std::string&   str,
        const float          font_size,
        const glm::vec4&     fg_color,
        const glm::vec4&     bg_color
    );

    ~TextRendererLine();

    void setForegroundColor( const glm::vec4& color );
    void setBackgroundColor( const glm::vec4& color );
    void setBaseXY( const glm::vec2& base );

    int32_t numVertices() const;
    int32_t numIndices() const;

    void fillVertexBuffer( TextRendererVertex* buf_vertex ) const;
    void fillIndexBuffer( uint32_t* buf_index, uint32_t vertex_start ) const;

private:

    void generateRenderVertices( const std::vector< Font::GlyphBound >& bounding_boxes );
    void findTotalBoundingBox( const std::vector< Font::GlyphBound >& bounding_boxes );
    vector< Font::GlyphBound > generateGlyphBoundingBoxes();

    Font::RuntimeHelper& m_helper;
    const std::string    m_str;
    const float          m_font_size;
    glm::vec4            m_fg_color;
    glm::vec4            m_bg_color;
    glm::vec2            m_base;
    glm::vec2            m_bounding_box_bottom_left;
    glm::vec2            m_bounding_box_top_right;
    int32_t              m_num_glyphs;
    int32_t              m_num_vertices;
    int32_t              m_num_indices;
    TextRendererVertex*  m_vertices;
};

} // namespace DepthTest

#endif/*__DEPTH_TEST_TEXT_RENDERER_LINE_HPP__*/
