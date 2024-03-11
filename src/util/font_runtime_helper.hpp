#ifndef __FONT_RUNTIME_HELPER__
#define __FONT_RUNTIME_HELPER__

#include <vector>
#include <map>

#include "font_glyph.hpp"
#include "font_metrics_parser.hpp"

using namespace std;

namespace Font {


class Point2D {
  public:
    float mX;
    float mY;

    Point2D( const float x, const float y ) noexcept
        :mX{x} ,mY{y}{}
};

class Rect {
  public:
    float mX; // lower (left) X
    float mY; // lower (bottom) Y
    float mW;
    float mH;

    Rect( const float x, const float y, const float w, const float h ) noexcept
        :mX{x} ,mY{y}, mW{w}, mH{h}{}
};

/** @brief represents one glyph in terms of two bounding boxes: mFrame and mTexture.
 *
 *         - mFrame specifies the vertex positions of the glyph bonding box in the 
 *           render coordinate system.
 *
 *         - mTexture is to specify the bounding box in the uv texture coordinate system.
 */
class GlyphBound {
  public:
    Rect mFrame;
    Rect mTexture;

    GlyphBound( const Rect& frame, const Rect& texture ) noexcept : mFrame{frame}, mTexture{texture} {}
};

class RuntimeHelper {

  public:

    static const int NUM_POINTS_PER_GLYPH;
    static const int NUM_FLOATS_PER_POINT;
    static const int NUM_FLOATS_PER_GLYPH;
    static const int NUM_INDICES_PER_GLYPH;

    RuntimeHelper( string fileName );

    virtual ~RuntimeHelper();

    /** @brief
     *
     *  @param c (in): code point
     *
     *  @return pointer to Glyph that constains the metrics.
     *          nullptr if the given code point is not valid.
     */
    const Glyph* getGlyph( const long c ) const;

    /** @brief spread in lengths in the uv-texture coordinates.
     */
    float spreadInTexture() const     { return mSpreadInTexture;     }

    /** @brief spread in pixels in the font metrics. */
    float spreadInFontMetrics() const { return mSpreadInFontMetrics; }

    const map< long, Glyph>& glyphs() const { return mGlyphs; }

    /** @brief typesets a word.
     *
     *  @param s               (in):  the word to typeset
     *
     *  @param fontSize        (in):  font size in pixels.
     *
     *  @param letterSpacing   (in):  letter spacing similar to letter-spacing 
     *                                property in CSS. Use 1.0 as the base value.
     *                                If you want to stretch the spacing by 10%, then
     *                                set 1.1.
     *
     *  @param leftX           (in):  the X-coordinate to which the left
     *                                edge of the bounding box of the first glyph
     *                                is aligned in the render coordinate system.
     *
     *  @param baselineY       (in):  the Y-coordinate for the baseline 
     *                                in the render coordinate system.
     *
     *  @param glyphs          (out): the list of Glyphs which contain the metrics.
     *
     *  @param instanceOrigins (out): the list of the glyph origins 
     *                                in the render coordinate system.
     *
     *  @param width           (out): width of the rectangular area
     *                                in the render coordinate system
     *                                required to accommodate all the glyph bounding boxes 
     *                                for the word.
     *
     *  @param height          (out): height of the rectangular area
     *                                in the render coordinate system
     *                                required to accommodate all the glyph bounding boxes 
     *                                for the word.
     *
     *                                height = aboveBaselineY + belowBaselineY
     *
     *  @param aboveBaselineY  (out): maximum height above the baseline 
     *                                in the render coordinate system
     *                                required to accommodate all the glyph bounding boxes 
     *                                for the word.
     *
     *  @param belowBaselineY  (out): maximum height below the baseline 
     *                                in the render coordinate system
     *                                required to accommodate all the glyph bounding boxes 
     *                                for the word.
     */
    void getGlyphOriginsWidthAndHeight(
        
        const string&           s,
        const float             fontSize,
        const float             letterSpacing,
        const float&            leftX,
        const float&            baselineY,

        vector< const Glyph* >& glyphs,
        vector< Point2D >&      instanceOrigins,
        float&                  width,
        float&                  height,
        float&                  aboveBaselineY,
        float&                  belowBaselineY
    ) const;


    /** @brief generates bounding boxes for rendering.
     *
     *  @param fontSize        (in): font size in pixels.
     *
     *  @param spreadRatio     (in): specifies how much spread to allocate to expand the
     *                               glyph bounding boxes.
     *                               The bonding boxes are expanded by the following amount.
     *
     *                               - spreadRatio * spreadInTexture() 
     *                                   : for the uv-texture coordinates.
     *                               - spreadRatio * spreadInFontMetrics() 
     *                                   : for the render vertices
     *
     *   glyph                  (in): list of Glyphs from getGlyphOriginsWidthAndHeight().
     *   instanceOrigins        (in): list of glyph origins from getGlyphOriginsWidthAndHeight().
     *
     *   bounds                 (out): list of bounding boxes for rendering.
     */
    void getBoundingBoxes(

        const float                   fontSize,
        const float                   spreadRatio,
        const vector< const Glyph* >& glyphs,
        const vector< Point2D >&      instanceOrigins,
        vector< GlyphBound >&         bounds
    ) const;

  private:
    float             mSpreadInTexture;
    float             mSpreadInFontMetrics;
    map< long, Glyph> mGlyphs;
};


} // namespace Font

#endif/*__FONT_RUNTIME_HELPER__*/
