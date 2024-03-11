#include "ui_text_shader_comparator.hpp"

namespace DepthTest {

const std::string UITextShaderComparator::FONT_FILE_PATH_WO_EXT = "../data/font";
const float       UITextShaderComparator::FONT_SMOOTH_LOW       = 0.45;
const float       UITextShaderComparator::FONT_SMOOTH_HIGH      = 0.55;
const std::string UITextShaderComparator::INSTRUCTION_LINE_01 = "Press '1'(red), '2'(blue), or 'c'(camera) to select the cylinder to move.";
const std::string UITextShaderComparator::INSTRUCTION_LINE_02 = "Press 'o(orient)', 't(translate)', or 's(scale)' to select the operations.";
const std::string UITextShaderComparator::INSTRUCTION_LINE_03 = "Use drag-cursor or arrow keys to alter the x- and y-coordinates and angles.";
const std::string UITextShaderComparator::INSTRUCTION_LINE_04 = "Use the scroll, or 'z' and 'x' to alter the z-coordinate and the angle.";
const std::string UITextShaderComparator::INSTRUCTION_LINE_05 = "Press 'r' to reset all to the initial configuration.";

const std::string UITextShaderComparator::INFO_LINE_OBJECT    = "Selected object: ";
const std::string UITextShaderComparator::INFO_LINE_OPERATION = "Selected operation: ";
const std::string UITextShaderComparator::PANE_01             = "Normal perspective projection";
const std::string UITextShaderComparator::PANE_02             = "Log-depth set to gl_Position.z in the VS.";
const std::string UITextShaderComparator::PANE_03             = "Log-depth set to gl_FragDepth in the FS.";

const float UITextShaderComparator::LINE_SPACING               = 1.2f;
const float UITextShaderComparator::VERTICAL_RATIO_BOTTOM_PANE = 0.2f;
const float UITextShaderComparator::MARGIN_SCREEN_EDGE         = 0.1f;
const glm::vec4 UITextShaderComparator::COLOR_WHITE = glm::vec4{ 1.0f,  1.0f,  1.0f,  1.0f };
const glm::vec4 UITextShaderComparator::COLOR_KHAKI = glm::vec4{ 0.93f, 0.90f, 0.55f, 1.0f };

UITextShaderComparator::UITextShaderComparator( GLFWWindow& window, GLFWUserInputShaderComparator& ui )

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
    ,m_active_object    { ui.activeObject() }
    ,m_active_operation { ui.activeOperation() }
    ,m_line_fixed_01    { nullptr }
    ,m_line_fixed_02    { nullptr }
    ,m_line_fixed_03    { nullptr }
    ,m_line_fixed_04    { nullptr }
    ,m_line_fixed_05    { nullptr }
    ,m_line_object      { nullptr }
    ,m_line_operation   { nullptr }
    ,m_line_title_left  { nullptr }
    ,m_line_title_center{ nullptr }
    ,m_line_title_right { nullptr }
{
    updateWholeScreen();
}

UITextShaderComparator::~UITextShaderComparator()
{
    deleteLines();
}

void UITextShaderComparator::getLayoutHintsForBottomText()
{
    std::vector< glm::vec2 > wh;

    wh.push_back( getWidthHeightOfText( INSTRUCTION_LINE_01,  1.0f ) );
    wh.push_back( getWidthHeightOfText( INSTRUCTION_LINE_02,  1.0f ) );
    wh.push_back( getWidthHeightOfText( INSTRUCTION_LINE_03,  1.0f ) );
    wh.push_back( getWidthHeightOfText( INSTRUCTION_LINE_04,  1.0f ) );
    wh.push_back( getWidthHeightOfText( INSTRUCTION_LINE_05,  1.0f ) );
    wh.push_back( getWidthHeightOfText( INFO_LINE_OBJECT,     1.0f ) );
    wh.push_back( getWidthHeightOfText( INFO_LINE_OPERATION,  1.0f ) );

    const auto max_wh = max( wh );

    const auto window_size = m_window.frameBufferSizeF();

    const auto factor_x = window_size.x * ( 1.0f - MARGIN_SCREEN_EDGE ) / max_wh.x / 1.8f;
    const auto factor_y = window_size.y * VERTICAL_RATIO_BOTTOM_PANE    
                        / ( max_wh.y * static_cast<float>( wh.size() ) * LINE_SPACING );

    m_font_size = std::min( factor_x, factor_y );

    m_base_bottom_start = glm::vec2{
        window_size.x * MARGIN_SCREEN_EDGE * 0.5f,
        window_size.y * VERTICAL_RATIO_BOTTOM_PANE - max_wh.y * m_font_size
    };

    m_line_gap = max_wh.y * m_font_size * LINE_SPACING;
}

void UITextShaderComparator::createLines()
{
    m_line_fixed_01    = createLine( INSTRUCTION_LINE_01, COLOR_WHITE );
    m_line_fixed_02    = createLine( INSTRUCTION_LINE_02, COLOR_WHITE );
    m_line_fixed_03    = createLine( INSTRUCTION_LINE_03, COLOR_WHITE );
    m_line_fixed_04    = createLine( INSTRUCTION_LINE_04, COLOR_WHITE );
    m_line_fixed_05    = createLine( INSTRUCTION_LINE_05, COLOR_WHITE );
    m_line_object      = createLine( INFO_LINE_OBJECT    + m_ui.activeObjectStr(),    COLOR_KHAKI );
    m_line_operation   = createLine( INFO_LINE_OPERATION + m_ui.activeOperationStr(), COLOR_KHAKI );

    m_line_title_left   = createLine( PANE_01, COLOR_WHITE );
    m_line_title_center = createLine( PANE_02, COLOR_WHITE );
    m_line_title_right  = createLine( PANE_03, COLOR_WHITE );

    m_renderer.registerLine( m_line_fixed_01 );
    m_renderer.registerLine( m_line_fixed_02 );
    m_renderer.registerLine( m_line_fixed_03 );
    m_renderer.registerLine( m_line_fixed_04 );
    m_renderer.registerLine( m_line_fixed_05 );
    m_renderer.registerLine( m_line_object );
    m_renderer.registerLine( m_line_operation );
    m_renderer.registerLine( m_line_title_left );
    m_renderer.registerLine( m_line_title_center );
    m_renderer.registerLine( m_line_title_right );
}

void UITextShaderComparator::updateWholeScreen()
{
    deleteLines();

    getLayoutHintsForBottomText();

    createLines();
    layoutLines();
}

void UITextShaderComparator::layoutLines()
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

    m_line_object->setBaseXY( base );

    base.y -= m_line_gap;

    m_line_operation->setBaseXY( base );

    base.y -= m_line_gap;

    const auto window_size = m_window.frameBufferSizeF();
    const auto margin      = window_size.x * MARGIN_SCREEN_EDGE * 0.5f;

    m_line_title_left  ->setBaseXY( glm::vec2{ margin,                          window_size.y - m_line_gap } );
    m_line_title_center->setBaseXY( glm::vec2{ margin + window_size.x * 0.3333, window_size.y - m_line_gap } );
    m_line_title_right ->setBaseXY( glm::vec2{ margin + window_size.x * 0.6666, window_size.y - m_line_gap } );
}

void UITextShaderComparator::updateUpdatedLines()
{
    const auto active_object = m_ui.activeObject();
    const auto active_operation = m_ui.activeOperation();

    if (   ( m_active_object    != active_object    ) 
        || ( m_active_operation != active_operation )  )
    {
        m_active_object    = active_object;
        m_active_operation = active_operation;

        updateLineObject();
        updateLineOperation();
    }
}

void UITextShaderComparator::updateLineObject()
{
    if ( m_line_object != nullptr ) {

        m_renderer.unregisterLine( m_line_object );
        delete m_line_object;
        m_line_object = nullptr;
    }

    m_line_object = createLine( INFO_LINE_OBJECT + m_ui.activeObjectStr(), COLOR_KHAKI );
    m_renderer.registerLine( m_line_object );

    auto base = m_base_bottom_start;
    base.y -= ( m_line_gap * 5 );

    m_line_object->setBaseXY( base );
}

void UITextShaderComparator::updateLineOperation()
{
    if ( m_line_operation != nullptr ) {

        m_renderer.unregisterLine( m_line_operation );
        delete m_line_operation;
        m_line_operation = nullptr;
    }

    m_line_operation = createLine( INFO_LINE_OPERATION + m_ui.activeOperationStr(), COLOR_KHAKI );
    m_renderer.registerLine( m_line_operation );

    auto base = m_base_bottom_start;
    base.y -= ( m_line_gap * 6 );

    m_line_operation->setBaseXY( base );
}

void UITextShaderComparator::update()
{
    if ( m_window.isUpdated() ) {

        updateWholeScreen();
    }
    else if ( m_ui.isUpdated() ) { 

        updateUpdatedLines(); 
    }
}

void UITextShaderComparator::render()
{
    const bool initialize_screen = false;

    m_renderer.render( initialize_screen );
}

void UITextShaderComparator::deleteLines()
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

    if ( m_line_object != nullptr ) {

        m_renderer.unregisterLine( m_line_object );

        delete m_line_object;
        m_line_object = nullptr;
    }

    if ( m_line_operation != nullptr ) {

        m_renderer.unregisterLine( m_line_operation );

        delete m_line_operation;
        m_line_operation = nullptr;
    }
}

glm::vec2 UITextShaderComparator::max( const std::vector< glm::vec2 >& vecs )
{
    glm::vec2 max_v{ 0.0f, 0.0f };

    for ( const auto& vec : vecs ) {

        max_v.x = std::max( max_v.x, vec.x );
        max_v.y = std::max( max_v.y, vec.y );
    }
    return max_v;
}

TextRendererLine* UITextShaderComparator::createLine( const std::string& str, const glm::vec4& color )
{
    return new TextRendererLine{ m_font_helper, str, m_font_size, color };
}

glm::vec2 UITextShaderComparator::getWidthHeightOfText( const std::string& str, const float font_size )
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
