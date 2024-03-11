#ifndef __DEPTH_TEST_UI_TEXT_SHADER_COMPARATOR_HPP__
#define __DEPTH_TEST_UI_TEXT_SHADER_COMPARATOR_HPP__

#include <cstdint>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "font_runtime_helper.hpp"
#include "glfw_window.hpp"
#include "glfw_user_input_shader_comparator.hpp"

#include "text_renderer_opengl.hpp"
#include "text_renderer_vertex.hpp"
#include "text_renderer_line.hpp"

namespace DepthTest {

class UITextShaderComparator {

public:

static const std::string FONT_FILE_PATH_WO_EXT;
static const float       FONT_SMOOTH_LOW;
static const float       FONT_SMOOTH_HIGH;

static const std::string INSTRUCTION_LINE_01;
static const std::string INSTRUCTION_LINE_02;
static const std::string INSTRUCTION_LINE_03;
static const std::string INSTRUCTION_LINE_04;
static const std::string INSTRUCTION_LINE_05;
static const std::string INFO_LINE_OBJECT;
static const std::string INFO_LINE_OPERATION;
static const std::string PANE_01;    
static const std::string PANE_02;
static const std::string PANE_03;

static const float LINE_SPACING;
static const float VERTICAL_RATIO_BOTTOM_PANE;
static const float MARGIN_SCREEN_EDGE;

static const glm::vec4 COLOR_WHITE;
static const glm::vec4 COLOR_KHAKI;

    explicit UITextShaderComparator( GLFWWindow& window, GLFWUserInputShaderComparator& ui );
    ~UITextShaderComparator();

    void update();
    void render();

private:

    void updateWholeScreen();
    void updateUpdatedLines();

    void getLayoutHintsForBottomText();
    void layoutLines();

    void createLines();
    void deleteLines();

    void updateLineObject();
    void updateLineOperation();

    glm::vec2 max( const std::vector< glm::vec2 >& vecs );
    TextRendererLine* createLine( const std::string& str, const glm::vec4& color );
    glm::vec2 getWidthHeightOfText( const std::string& str, const float font_size );

    TextRendererOpenGL    m_renderer;
    Font::RuntimeHelper   m_font_helper;
    GLFWWindow&           m_window;
    GLFWUserInputShaderComparator&
                          m_ui;

    float                 m_font_size;
    float                 m_line_gap;
    glm::vec2             m_base_bottom_start;

    GLFWUserInputShaderComparator::ActiveObject
                          m_active_object;

    GLFWUserInputShaderComparator::ActiveOperation
                          m_active_operation;

    TextRendererLine*     m_line_fixed_01;
    TextRendererLine*     m_line_fixed_02;
    TextRendererLine*     m_line_fixed_03;
    TextRendererLine*     m_line_fixed_04;
    TextRendererLine*     m_line_fixed_05;

    TextRendererLine*     m_line_object;
    TextRendererLine*     m_line_operation;

    TextRendererLine*     m_line_title_left;
    TextRendererLine*     m_line_title_center;
    TextRendererLine*     m_line_title_right;
};

} // namespace DepthTest

#endif/*__DEPTH_TEST_UI_TEXT_SHADER_COMPARATOR_HPP__*/

