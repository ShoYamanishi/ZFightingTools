#ifndef __DEPTH_TEST_TEXT_RENDERER_VERTEX_HPP__
#define __DEPTH_TEST_TEXT_RENDERER_VERTEX_HPP__

#include <cstdint>
#include <glm/glm.hpp>

namespace DepthTest {

class TextRendererVertex {
public:

    glm::vec4 m_pos;
    glm::vec4 m_color;
    glm::vec2 m_uv;
    int32_t   __padding01__;
    int32_t   __padding02__;

    explicit TextRendererVertex(
        const glm::vec4& pos,
        const glm::vec4& color,
        const glm::vec2& uv
    ) noexcept
        :m_pos  { pos }
        ,m_color{ color }
        ,m_uv   { uv }
    {
    }

    explicit TextRendererVertex() noexcept
        :m_pos  { 0.0f, 0.0f, 0.0f, 0.0f }
        ,m_color{ 0.0f, 0.0f, 0.0f, 0.0f }
        ,m_uv   { 0.0f, 0.0f }
    {
    }

    void setColor( const glm::vec4& color )
    {
        m_color = color;
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
