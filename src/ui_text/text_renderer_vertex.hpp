#ifndef __DEPTH_TEST_TEXT_RENDERER_VERTEX_HPP__
#define __DEPTH_TEST_TEXT_RENDERER_VERTEX_HPP__

#include <cstdint>
#include <glm/glm.hpp>

namespace DepthTest {

class TextRendererVertex {
public:

    glm::vec4 m_pos;
    glm::vec4 m_inner_color;
    glm::vec4 m_outer_color;
    glm::vec2 m_uv;
    int32_t   __padding01__;
    int32_t   __padding02__;

    explicit TextRendererVertex(
        const glm::vec4& pos,
        const glm::vec4& inner_color,
        const glm::vec4& outer_color,
        const glm::vec2& uv
    ) noexcept
        :m_pos        { pos }
        ,m_inner_color{ inner_color }
        ,m_outer_color{ outer_color }
        ,m_uv         { uv }
    {
    }

    explicit TextRendererVertex() noexcept
        :m_pos        { 0.0f, 0.0f, 0.0f, 0.0f }
        ,m_inner_color{ 0.0f, 0.0f, 0.0f, 0.0f }
        ,m_outer_color{ 0.0f, 0.0f, 0.0f, 0.0f }
        ,m_uv         { 0.0f, 0.0f }
    {
    }

    void setInnerColor( const glm::vec4& color )
    {
        m_inner_color = color;
    }

    void setOuterColor( const glm::vec4& color )
    {
        m_outer_color = color;
    }

    glm::vec4 pos() const
    {
        return m_pos;
    }
   
    void setPos( const glm::vec4& pos )
    {
        m_pos = pos;
    }
};

} // namespace DepthTest

#endif/*__DEPTH_TEST_TEXT_RENDERER_VERTEX_HPP__*/
