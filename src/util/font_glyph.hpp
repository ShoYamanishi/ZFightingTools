#ifndef __FONT_GLYPH_HPP__
#define __FONT_GLYPH_HPP__

namespace Font {


class Glyph {

  public:

    long  mCodePoint;
    float mWidth;
    float mHeight;
    float mHorizontalBearingX;
    float mHorizontalBearingY;
    float mHorizontalAdvance;
    float mVerticalBearingX;
    float mVerticalBearingY;
    float mVerticalAdvance;
    float mTextureCoordX;
    float mTextureCoordY;
    float mTextureWidth;
    float mTextureHeight;

    /*
     *  key:   code point
     *  value: kerning
     */
    std::map< long, float > mKernings;

};


} // namespace Font

#endif /*__FONT_GLYPH_HPP__*/
