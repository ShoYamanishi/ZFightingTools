#include "ui_text_interactive.hpp"

namespace DepthTest {

const std::string UITextInteractive::FONT_FILE_PATH_WO_EXT = "../data/font";
const float       UITextInteractive::FONT_SMOOTH_LOW       = 0.45;
const float       UITextInteractive::FONT_SMOOTH_HIGH      = 0.55;
const std::string UITextInteractive::INSTRUCTION_LINE_01 = "Press 'f', 'n', 'c', '1(red)', or '2(blue)' to select the parameter.";
const std::string UITextInteractive::INSTRUCTION_LINE_02 = "Press vertical arrow keys to change the Z-coordinates by a large amount.";
const std::string UITextInteractive::INSTRUCTION_LINE_03 = "Press horizontal arrow keys to change the Z-coordinates by a small amount .";
const std::string UITextInteractive::INSTRUCTION_LINE_04 = "Keep pressing shift to make the amount smaller by 1/100 for fine tuning.";
const std::string UITextInteractive::INSTRUCTION_LINE_05 = "You can also use the scroll wheel to change the Z-coordinates.";
const std::string UITextInteractive::INSTRUCTION_LINE_06 = "Press 'z' or 'x' to decrease or increase the size of the planes.";

const std::string UITextInteractive::INFO_LINE_NEAR      = "Near (not used by log-CF type): ";
const std::string UITextInteractive::INFO_LINE_FAR       = "Far: ";
const std::string UITextInteractive::INFO_LINE_PARAM_C   = "Parameter C (used only by log-CF type): ";
const std::string UITextInteractive::INFO_LINE_PLANE_1   = "Plane 1(red): ";
const std::string UITextInteractive::INFO_LINE_PLANE_2   = "Plane 2(blue): ";
const std::string UITextInteractive::INFO_LINE_DIFF      = "Diff: ";
const std::string UITextInteractive::INFO_LINE_EDGE_LENGTH = "Plane Edge Length: ";
const std::string UITextInteractive::PANE_01             = "Normal Perspective Depth Test";
const std::string UITextInteractive::PANE_02             = "Logarithmic Depth Test NF-type";
const std::string UITextInteractive::PANE_03             = "Logarithmic Depth Test CF-type";
const float UITextInteractive::LINE_SPACING               = 1.2f;
const float UITextInteractive::VERTICAL_RATIO_BOTTOM_PANE = 0.3f;
const float UITextInteractive::MARGIN_SCREEN_EDGE         = 0.1f;
const glm::vec4 UITextInteractive::COLOR_WHITE = glm::vec4{ 1.0f,  1.0f,  1.0f,  1.0f };
const glm::vec4 UITextInteractive::COLOR_KHAKI = glm::vec4{ 0.93f, 0.90f, 0.55f, 1.0f };

UITextInteractive::UITextInteractive( GLFWWindow& window, GLFWUserInputInteractive& ui )

    :m_renderer{

        window,
        FONT_FILE_PATH_WO_EXT + ".png",
        FONT_SMOOTH_LOW,
        FONT_SMOOTH_HIGH
    }
    ,m_font_helper      { FONT_FILE_PATH_WO_EXT + ".txt" }
    ,m_window           { window }
    ,m_ui               { ui }
    ,m_font_size        { 0.0f }
    ,m_line_gap         { 0.0f }
    ,m_base_bottom_start{ 0.0f }
    ,m_value_near       { ui.near() }
    ,m_value_far        { ui.far() }
    ,m_value_param_c    { ui.paramC() }
    ,m_value_plane_1    { ui.plane1() }
    ,m_value_plane_2    { ui.plane2() }
    ,m_value_diff       { std::abs( ui.plane2() - ui.plane1() ) }
    ,m_value_edge_length{ ui.edgeLength() }
    ,m_active_param     { GLFWUserInputInteractive::NONE }
    ,m_color_near       { COLOR_WHITE }
    ,m_color_far        { COLOR_WHITE }
    ,m_color_param_c    { COLOR_WHITE }
    ,m_color_plane_1    { COLOR_WHITE }
    ,m_color_plane_2    { COLOR_WHITE }
    ,m_line_fixed_01    { nullptr }
    ,m_line_fixed_02    { nullptr }
    ,m_line_fixed_03    { nullptr }
    ,m_line_fixed_04    { nullptr }
    ,m_line_fixed_05    { nullptr }
    ,m_line_fixed_06    { nullptr }
    ,m_line_near        { nullptr }
    ,m_line_far         { nullptr }
    ,m_line_param_c     { nullptr }
    ,m_line_plane_1     { nullptr }
    ,m_line_plane_2     { nullptr }
    ,m_line_diff        { nullptr }
    ,m_line_edge_length { nullptr }
    ,m_line_title_left  { nullptr }
    ,m_line_title_center{ nullptr }
    ,m_line_title_right { nullptr }
{
    updateWholeScreen();
}

UITextInteractive::~UITextInteractive()
{
    deleteLines();
}

void UITextInteractive::getLayoutHintsForBottomText()
{
    std::vector< glm::vec2 > wh;

    wh.push_back( getWidthHeightOfText( INSTRUCTION_LINE_01,  1.0f ) );
    wh.push_back( getWidthHeightOfText( INSTRUCTION_LINE_02,  1.0f ) );
    wh.push_back( getWidthHeightOfText( INSTRUCTION_LINE_03,  1.0f ) );
    wh.push_back( getWidthHeightOfText( INSTRUCTION_LINE_04,  1.0f ) );
    wh.push_back( getWidthHeightOfText( INSTRUCTION_LINE_05,  1.0f ) );
    wh.push_back( getWidthHeightOfText( INSTRUCTION_LINE_06,  1.0f ) );
    wh.push_back( getWidthHeightOfText( INFO_LINE_NEAR,       1.0f ) );
    wh.push_back( getWidthHeightOfText( INFO_LINE_FAR,        1.0f ) );
    wh.push_back( getWidthHeightOfText( INFO_LINE_PARAM_C,    1.0f ) );
    wh.push_back( getWidthHeightOfText( INFO_LINE_PLANE_1,    1.0f ) );
    wh.push_back( getWidthHeightOfText( INFO_LINE_PLANE_2,    1.0f ) );
    wh.push_back( getWidthHeightOfText( INFO_LINE_DIFF,       1.0f ) );
    wh.push_back( getWidthHeightOfText( INFO_LINE_EDGE_LENGTH, 1.0f ) );

    const auto max_wh = max( wh );

    const auto window_size = m_window.frameBufferSizeF();

    const auto factor_x = window_size.x * ( 1.0f - MARGIN_SCREEN_EDGE ) / max_wh.x;
    const auto factor_y = window_size.y * VERTICAL_RATIO_BOTTOM_PANE    
                        / ( max_wh.y * static_cast<float>( wh.size() ) * LINE_SPACING );

    m_font_size = std::min( factor_x, factor_y );

    m_base_bottom_start = glm::vec2{
        window_size.x * MARGIN_SCREEN_EDGE * 0.5f,
        window_size.y * VERTICAL_RATIO_BOTTOM_PANE - max_wh.y * m_font_size
    };

    m_line_gap = max_wh.y * m_font_size * LINE_SPACING;
}

void UITextInteractive::updateDistanceValues()
{
    m_value_near        = m_ui.near();
    m_value_far         = m_ui.far();
    m_value_param_c     = m_ui.paramC();
    m_value_plane_1     = m_ui.plane1();
    m_value_plane_2     = m_ui.plane2();
    m_value_diff        = std::abs( m_value_plane_2 - m_value_plane_1 );
    m_value_edge_length = m_ui.edgeLength();
}

void UITextInteractive::createLines()
{
    m_line_fixed_01    = createLine( INSTRUCTION_LINE_01, COLOR_WHITE );
    m_line_fixed_02    = createLine( INSTRUCTION_LINE_02, COLOR_WHITE );
    m_line_fixed_03    = createLine( INSTRUCTION_LINE_03, COLOR_WHITE );
    m_line_fixed_04    = createLine( INSTRUCTION_LINE_04, COLOR_WHITE );
    m_line_fixed_05    = createLine( INSTRUCTION_LINE_05, COLOR_WHITE );
    m_line_fixed_06    = createLine( INSTRUCTION_LINE_06, COLOR_WHITE );
    m_line_near        = createLine( INFO_LINE_NEAR    + std::to_string( m_value_near    ), m_color_near );
    m_line_far         = createLine( INFO_LINE_FAR     + std::to_string( m_value_far     ), m_color_far );
    m_line_param_c     = createLine( INFO_LINE_PARAM_C + std::to_string( m_value_param_c ), m_color_far );
    m_line_plane_1     = createLine( INFO_LINE_PLANE_1 + std::to_string( m_value_plane_1 ), m_color_plane_1 );
    m_line_plane_2     = createLine( INFO_LINE_PLANE_2 + std::to_string( m_value_plane_2 ), m_color_plane_2 );

    if ( m_value_plane_1 < m_value_plane_2 ) {
        m_line_diff = createLine( INFO_LINE_DIFF + " (plane1/red  is closer): "    + std::to_string( m_value_diff    ), COLOR_WHITE );
    }
    else {
        m_line_diff = createLine( INFO_LINE_DIFF + " (plane2/blue is closer): "    + std::to_string( m_value_diff    ), COLOR_WHITE );
    }
    m_line_edge_length = createLine( INFO_LINE_EDGE_LENGTH + std::to_string( m_value_edge_length ), COLOR_WHITE );
    m_line_title_left   = createLine( PANE_01, COLOR_WHITE );
    m_line_title_center = createLine( PANE_02, COLOR_WHITE );
    m_line_title_right  = createLine( PANE_03, COLOR_WHITE );

    m_renderer.registerLine( m_line_fixed_01 );
    m_renderer.registerLine( m_line_fixed_02 );
    m_renderer.registerLine( m_line_fixed_03 );
    m_renderer.registerLine( m_line_fixed_04 );
    m_renderer.registerLine( m_line_fixed_05 );
    m_renderer.registerLine( m_line_fixed_06 );
    m_renderer.registerLine( m_line_near );
    m_renderer.registerLine( m_line_far );
    m_renderer.registerLine( m_line_param_c );
    m_renderer.registerLine( m_line_plane_1 );
    m_renderer.registerLine( m_line_plane_2 );
    m_renderer.registerLine( m_line_diff );
    m_renderer.registerLine( m_line_edge_length );
    m_renderer.registerLine( m_line_title_left );
    m_renderer.registerLine( m_line_title_center );
    m_renderer.registerLine( m_line_title_right );
}

void UITextInteractive::updateWholeScreen()
{
    deleteLines();

    updateDistanceValues();

    getLayoutHintsForBottomText();

    createLines();

    layoutLines();
}

void UITextInteractive::layoutLines()
{
    auto base = m_base_bottom_start;

    m_line_fixed_01->setBaseXY( base );

    base.y -= m_line_gap;

    m_line_fixed_02->setBaseXY( base );

    base.y -= m_line_gap;

    m_line_fixed_03->setBaseXY( base );

    base.y -= m_line_gap;

    m_line_fixed_04->setBaseXY( base );

    base.y -= m_line_gap;

    m_line_fixed_05->setBaseXY( base );

    base.y -= m_line_gap;

    m_line_fixed_06->setBaseXY( base );

    base.y -= m_line_gap;

    m_line_near->setBaseXY( base );

    base.y -= m_line_gap;

    m_line_far->setBaseXY( base );

    base.y -= m_line_gap;

    m_line_param_c->setBaseXY( base );

    base.y -= m_line_gap;

    m_line_plane_1->setBaseXY( base );

    base.y -= m_line_gap;

    m_line_plane_2->setBaseXY( base );

    base.y -= m_line_gap;

    m_line_diff->setBaseXY( base );

    base.y -= m_line_gap;

    m_line_edge_length->setBaseXY( base );

    const auto window_size = m_window.frameBufferSizeF();
    const auto margin      = window_size.x * MARGIN_SCREEN_EDGE * 0.5f;

    m_line_title_left  ->setBaseXY( glm::vec2{ margin,                         window_size.y - m_line_gap } );
    m_line_title_center->setBaseXY( glm::vec2{ margin + window_size.x * 0.333, window_size.y - m_line_gap } );
    m_line_title_right ->setBaseXY( glm::vec2{ margin + window_size.x * 0.666, window_size.y - m_line_gap } );
}

void UITextInteractive::updateUpdatedLines()
{
    const auto value_near = m_ui.near();
    if ( m_value_near != value_near ) {

        m_value_near = value_near;
        updateLineNear();
    }

    const auto value_far = m_ui.far();
    if ( m_value_far != value_far ) {

        m_value_far = value_far;
        updateLineFar();
    }

    const auto value_param_c = m_ui.paramC();
    if ( m_value_param_c != value_param_c ) {

        m_value_param_c = value_param_c;
        updateLineParamC();
    }

    const auto value_plane_1 = m_ui.plane1();
    if ( m_value_plane_1 != value_plane_1 ) {

        m_value_plane_1 = value_plane_1;
        updateLinePlane1();
    }

    const auto value_plane_2 = m_ui.plane2();
    if ( m_value_plane_2 != value_plane_2 ) {

        m_value_plane_2 = value_plane_2;
        updateLinePlane2();
    }

    const auto value_diff = std::abs( value_plane_2 - value_plane_1 );
    if ( m_value_diff != value_diff ) {

        m_value_diff = value_diff;
        updateLineDiff();
    }

    const auto value_edge_length = m_ui.edgeLength();
    if ( m_value_edge_length != value_edge_length ) {

        m_value_edge_length = value_edge_length;
        updateLineEdgeLength();
    }
}

void UITextInteractive::updateLineNear()
{
    if ( m_line_near != nullptr ) {

        m_renderer.unregisterLine( m_line_near );
        delete m_line_near;
        m_line_near = nullptr;
    }

    m_line_near = createLine( INFO_LINE_NEAR + std::to_string( m_value_near ), m_color_near );
    m_renderer.registerLine( m_line_near );

    auto base = m_base_bottom_start;
    base.y -= ( m_line_gap * 6 );

    m_line_near->setBaseXY( base );
}

void UITextInteractive::updateLineFar()
{
    if ( m_line_far != nullptr ) {

        m_renderer.unregisterLine( m_line_far );
        delete m_line_far;
        m_line_far = nullptr;
    }

    m_line_far = createLine( INFO_LINE_FAR + std::to_string( m_value_far ), m_color_far );
    m_renderer.registerLine( m_line_far );

    auto base = m_base_bottom_start;
    base.y -= ( m_line_gap * 7 );

    m_line_far->setBaseXY( base );
}

void UITextInteractive::updateLineParamC()
{
    if ( m_line_param_c != nullptr ) {

        m_renderer.unregisterLine( m_line_param_c );
        delete m_line_param_c;
        m_line_param_c = nullptr;
    }

    m_line_param_c = createLine( INFO_LINE_PARAM_C + std::to_string( m_value_param_c ), m_color_param_c );
    m_renderer.registerLine( m_line_param_c );

    auto base = m_base_bottom_start;
    base.y -= ( m_line_gap * 8 );

    m_line_param_c->setBaseXY( base );
}

void UITextInteractive::updateLinePlane1()
{
    if ( m_line_plane_1!= nullptr ) {

        m_renderer.unregisterLine( m_line_plane_1 );
        delete m_line_plane_1;
        m_line_plane_1 = nullptr;
    }

    m_line_plane_1 = createLine( INFO_LINE_PLANE_1 + std::to_string( m_value_plane_1 ), m_color_plane_1 );
    m_renderer.registerLine( m_line_plane_1 );

    auto base = m_base_bottom_start;
    base.y -= ( m_line_gap * 9 );

    m_line_plane_1->setBaseXY( base );
}

void UITextInteractive::updateLinePlane2()
{
    if ( m_line_plane_2 != nullptr ) {

        m_renderer.unregisterLine( m_line_plane_2 );
        delete m_line_plane_2;
        m_line_plane_2 = nullptr;
    }

    m_line_plane_2 = createLine( INFO_LINE_PLANE_2 + std::to_string( m_value_plane_2 ), m_color_plane_2 );
    m_renderer.registerLine( m_line_plane_2 );

    auto base = m_base_bottom_start;
    base.y -= ( m_line_gap * 10 );

    m_line_plane_2->setBaseXY( base );
}

void UITextInteractive::updateLineDiff()
{
    if ( m_line_diff != nullptr ) {

        m_renderer.unregisterLine( m_line_diff );
        delete m_line_diff;
        m_line_diff = nullptr;
    }

    if ( m_value_plane_1 < m_value_plane_2 ) {
        m_line_diff = createLine( INFO_LINE_DIFF + " (plane1/red  is closer): "    + std::to_string( m_value_diff    ), COLOR_WHITE );
    }
    else {
        m_line_diff = createLine( INFO_LINE_DIFF + " (plane2/blue is closer): "    + std::to_string( m_value_diff    ), COLOR_WHITE );
    }

    m_renderer.registerLine( m_line_diff );

    auto base = m_base_bottom_start;
    base.y -= ( m_line_gap * 11 );

    m_line_diff->setBaseXY( base );
}

void UITextInteractive::updateLineEdgeLength()
{
    if ( m_line_edge_length != nullptr ) {

        m_renderer.unregisterLine( m_line_edge_length );
        delete m_line_edge_length;
        m_line_edge_length = nullptr;
    }

    m_line_edge_length = createLine( INFO_LINE_EDGE_LENGTH + std::to_string( m_value_edge_length ), COLOR_WHITE );
    m_renderer.registerLine( m_line_edge_length );

    auto base = m_base_bottom_start;
    base.y -= ( m_line_gap * 12 );

    m_line_edge_length->setBaseXY( base );
}

void UITextInteractive::update()
{
    if ( m_window.isUpdated() ) {

        updateWholeScreen();
    }
    else if ( m_ui.isUpdated() ) { 

        updateColors();
        updateUpdatedLines(); 
    }
}

void UITextInteractive::updateColors()
{
    const auto active_param = m_ui.activeParam();

    if ( m_active_param == active_param ) {

        return;
    }

    switch ( m_active_param ) {

      case GLFWUserInputInteractive::NEAR:

        m_color_near = COLOR_WHITE;
        updateLineNear();
        break;

      case GLFWUserInputInteractive::FAR:

        m_color_far = COLOR_WHITE;
        updateLineFar();
        break;

      case GLFWUserInputInteractive::PARAM_C:

        m_color_param_c = COLOR_WHITE;
        updateLineParamC();
        break;

      case GLFWUserInputInteractive::PLANE_1:

        m_color_plane_1 = COLOR_WHITE;
        updateLinePlane1();
        break;

      case GLFWUserInputInteractive::PLANE_2:

        m_color_plane_2 = COLOR_WHITE;
        updateLinePlane2();
        break;

      default:
        break;
    }

    m_active_param = active_param;

    switch ( m_active_param ) {

      case GLFWUserInputInteractive::NEAR:

        m_color_near = COLOR_KHAKI;
        updateLineNear();
        break;

      case GLFWUserInputInteractive::FAR:

        m_color_far = COLOR_KHAKI;
        updateLineFar();
        break;

      case GLFWUserInputInteractive::PARAM_C:

        m_color_param_c = COLOR_KHAKI;
        updateLineParamC();
        break;

      case GLFWUserInputInteractive::PLANE_1:

        m_color_plane_1 = COLOR_KHAKI;
        updateLinePlane1();
        break;

      case GLFWUserInputInteractive::PLANE_2:

        m_color_plane_2 = COLOR_KHAKI;
        updateLinePlane2();
        break;

      default:
        break;
    }
}

void UITextInteractive::render()
{
    const bool initialize_screen = false;

    m_renderer.render( initialize_screen );
}

void UITextInteractive::deleteLines()
{
    if ( m_line_fixed_01 != nullptr ) {

        m_renderer.unregisterLine( m_line_fixed_01 );

        delete m_line_fixed_01;
        m_line_fixed_01 = nullptr;
    }

    if ( m_line_fixed_02 != nullptr ) {

        m_renderer.unregisterLine( m_line_fixed_02 );

        delete m_line_fixed_02;
        m_line_fixed_02 = nullptr;
    }

    if ( m_line_fixed_03 != nullptr ) {

        m_renderer.unregisterLine( m_line_fixed_03 );

        delete m_line_fixed_03;
        m_line_fixed_03 = nullptr;
    }

    if ( m_line_fixed_04 != nullptr ) {

        m_renderer.unregisterLine( m_line_fixed_04 );

        delete m_line_fixed_04;
        m_line_fixed_04 = nullptr;
    }

    if ( m_line_fixed_05 != nullptr ) {

        m_renderer.unregisterLine( m_line_fixed_05 );

        delete m_line_fixed_05;
        m_line_fixed_05 = nullptr;
    }

    if ( m_line_fixed_06 != nullptr ) {

        m_renderer.unregisterLine( m_line_fixed_06 );

        delete m_line_fixed_06;
        m_line_fixed_06 = nullptr;
    }

    if ( m_line_title_left != nullptr ) {

        m_renderer.unregisterLine( m_line_title_left );

        delete m_line_title_left;
        m_line_title_left = nullptr;
    }

    if ( m_line_title_center != nullptr ) {

        m_renderer.unregisterLine( m_line_title_center );

        delete m_line_title_center;
        m_line_title_center = nullptr;
    }

    if ( m_line_title_right != nullptr ) {

        m_renderer.unregisterLine( m_line_title_right );

        delete m_line_title_right;
        m_line_title_right = nullptr;
    }

    if ( m_line_near != nullptr ) {

        m_renderer.unregisterLine( m_line_near );

        delete m_line_near;
        m_line_near = nullptr;
    }

    if ( m_line_far != nullptr ) {

        m_renderer.unregisterLine( m_line_far );

        delete m_line_far;
        m_line_far = nullptr;
    }

    if ( m_line_param_c != nullptr ) {

        m_renderer.unregisterLine( m_line_param_c );

        delete m_line_param_c;
        m_line_param_c = nullptr;
    }

    if ( m_line_plane_1 != nullptr ) {

        m_renderer.unregisterLine( m_line_plane_1 );

        delete m_line_plane_1;
        m_line_plane_1 = nullptr;
    }

    if ( m_line_plane_2 != nullptr ) {

        m_renderer.unregisterLine( m_line_plane_2 );

        delete m_line_plane_2;
        m_line_plane_2 = nullptr;
    }

    if ( m_line_diff != nullptr ) {

        m_renderer.unregisterLine( m_line_diff );

        delete m_line_diff;
        m_line_diff = nullptr;
    }

    if ( m_line_edge_length != nullptr ) {

        m_renderer.unregisterLine( m_line_edge_length );

        delete m_line_edge_length;
        m_line_edge_length = nullptr;
    }
}

glm::vec2 UITextInteractive::max( const std::vector< glm::vec2 >& vecs )
{
    glm::vec2 max_v{ 0.0f, 0.0f };

    for ( const auto& vec : vecs ) {

        max_v.x = std::max( max_v.x, vec.x );
        max_v.y = std::max( max_v.y, vec.y );
    }
    return max_v;
}

TextRendererLine* UITextInteractive::createLine( const std::string& str, const glm::vec4& color )
{
    return new TextRendererLine{ m_font_helper, str, m_font_size, color };
}

glm::vec2 UITextInteractive::getWidthHeightOfText( const std::string& str, const float font_size )
{
    vector< const Font::Glyph* > glyphs;
    vector< Font::Point2D >      instance_origins;

    float width, height, above_baseline_y, below_baseline_y;

    m_font_helper.getGlyphOriginsWidthAndHeight(
        str,
        font_size,
        1.0f, // letter spacing
        0.0f, // base x
        0.0f, // base y
        glyphs,
        instance_origins,
        width,
        height,
        above_baseline_y,
        below_baseline_y
    );

    return glm::vec2{ width, height };
}

} // namespace DepthTest
