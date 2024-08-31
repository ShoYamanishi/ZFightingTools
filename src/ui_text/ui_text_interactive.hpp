#ifndef __DEPTH_TEST_UI_TEXT_INTERACTIVE_HPP__
#define __DEPTH_TEST_UI_TEXT_INTERACTIVE_HPP__

#include <cstdint>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "font_runtime_helper.hpp"
#include "glfw_window.hpp"
#include "glfw_user_input_interactive.hpp"

#include "text_renderer_opengl.hpp"
#include "text_renderer_vertex.hpp"
#include "text_renderer_line.hpp"

namespace DepthTest {

class UITextInteractive {

public:

static const std::string FONT_FILE_PATH_WO_EXT;
static const float       FONT_GATE1_LOW;
static const float       FONT_GATE1_HIGH;
static const float       FONT_GATE2_LOW;
static const float       FONT_GATE2_HIGH;

static const std::string INSTRUCTION_LINE_01;
static const std::string INSTRUCTION_LINE_02;
static const std::string INSTRUCTION_LINE_03;
static const std::string INSTRUCTION_LINE_04;
static const std::string INSTRUCTION_LINE_05;
static const std::string INSTRUCTION_LINE_06;
static const std::string INFO_LINE_NEAR;
static const std::string INFO_LINE_FAR;
static const std::string INFO_LINE_PARAM_C;
static const std::string INFO_LINE_PLANE_1;
static const std::string INFO_LINE_PLANE_2;
static const std::string INFO_LINE_DIFF; 
static const std::string INFO_LINE_EDGE_LENGTH;
static const std::string PANE_01;    
static const std::string PANE_02;
static const std::string PANE_03;

static const float LINE_SPACING;
static const float VERTICAL_RATIO_BOTTOM_PANE;
static const float MARGIN_SCREEN_EDGE;

static const glm::vec4 COLOR_WHITE;
static const glm::vec4 COLOR_BLACK;
static const glm::vec4 COLOR_KHAKI;


    explicit UITextInteractive( GLFWWindow& window, GLFWUserInputInteractive& ui );
    ~UITextInteractive();

    void update();
    void render();

private:

    void updateWholeScreen();
    void updateUpdatedLines();

    void getLayoutHintsForBottomText();
    void layoutLines();

    void updateDistanceValues();
    void createLines();
    void deleteLines();

    void updateLineNear();
    void updateLineFar();
    void updateLineParamC();
    void updateLinePlane1();
    void updateLinePlane2();
    void updateLineDiff();
    void updateLineEdgeLength();

    void updateColors();

    glm::vec2 max( const std::vector< glm::vec2 >& vecs );
    TextRendererLine* createLine( const std::string& str, const glm::vec4& fg_color, const glm::vec4& bg_color );
    glm::vec2 getWidthHeightOfText( const std::string& str, const float font_size );

    TextRendererOpenGL    m_renderer;
    Font::RuntimeHelper   m_font_helper;
    GLFWWindow&           m_window;
    GLFWUserInputInteractive&
                          m_ui;

    float                 m_tmp_01;
    float                 m_tmp_02;

    float                 m_font_size;
    float                 m_line_gap;
    glm::vec2             m_base_bottom_start;

    float                 m_value_near;
    float                 m_value_far;
    float                 m_value_param_c;
    float                 m_value_plane_1;
    float                 m_value_plane_2;
    float                 m_value_diff;
    float                 m_value_edge_length;

    GLFWUserInputInteractive::ActiveParam
                          m_active_param;

    glm::vec4             m_color_near;
    glm::vec4             m_color_far;
    glm::vec4             m_color_param_c;
    glm::vec4             m_color_plane_1;
    glm::vec4             m_color_plane_2;

    TextRendererLine*     m_line_fixed_01;
    TextRendererLine*     m_line_fixed_02;
    TextRendererLine*     m_line_fixed_03;
    TextRendererLine*     m_line_fixed_04;
    TextRendererLine*     m_line_fixed_05;
    TextRendererLine*     m_line_fixed_06;

    TextRendererLine*     m_line_near;
    TextRendererLine*     m_line_far;
    TextRendererLine*     m_line_param_c;
    TextRendererLine*     m_line_plane_1;
    TextRendererLine*     m_line_plane_2;
    TextRendererLine*     m_line_diff;
    TextRendererLine*     m_line_edge_length;

    TextRendererLine*     m_line_title_left;
    TextRendererLine*     m_line_title_center;
    TextRendererLine*     m_line_title_right;
};

} // namespace DepthTest

#endif/*__DEPTH_TEST_UI_TEXT_INTERACTIVE_HPP__*/
