#include <string>
#include <iostream>

#include "font_runtime_helper.hpp"

namespace Font {

const int RuntimeHelper::NUM_POINTS_PER_GLYPH  = 4;
const int RuntimeHelper::NUM_FLOATS_PER_POINT  = 8;
const int RuntimeHelper::NUM_FLOATS_PER_GLYPH  = 4 * 8;
const int RuntimeHelper::NUM_INDICES_PER_GLYPH = 6;

RuntimeHelper::RuntimeHelper( string fileName ): mSpreadInTexture(0.0), mSpreadInFontMetrics(0.0)
{
    MetricsParser parser( mGlyphs, mSpreadInTexture, mSpreadInFontMetrics );
    parser.parseSpec( fileName );
}

RuntimeHelper::~RuntimeHelper() {;}

const Glyph* RuntimeHelper::getGlyph( const long c ) const
{
    auto git = mGlyphs.find( c );

    if ( git != mGlyphs.end() ) {

         return &( git->second );
    }
    else {

        return nullptr;
    }
}

void RuntimeHelper::getGlyphOriginsWidthAndHeight(
        
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
) const {

    glyphs.clear();
    instanceOrigins.clear();
    width          = 0.0f;
    height         = 0.0f;
    aboveBaselineY = 0.0f;
    belowBaselineY = 0.0f;    

    for ( auto i = 0 ; i < s.length() ; i++ ) {

        auto ch  = s.at(i);
        auto it = mGlyphs.find(ch);

        if ( it != mGlyphs.end() ) {

            glyphs.push_back( &(it->second) );
        }
    }

    if ( glyphs.size() == 0 ) {
        return;
    }

    instanceOrigins.emplace_back(

        leftX - glyphs[0]->mHorizontalBearingX * fontSize,
        baselineY
    );

    aboveBaselineY = std::max( 0.0f, glyphs[0]->mHorizontalBearingY * fontSize );
    belowBaselineY = std::max( 0.0f,(   glyphs[0]->mHeight
                                      - glyphs[0]->mHorizontalBearingY ) * fontSize );

    const auto numGlyphs = glyphs.size();

    for ( int i = 1; i < numGlyphs; i++ ) {

        instanceOrigins.emplace_back(

                instanceOrigins[i-1].mX
              + glyphs[i-1]->mHorizontalAdvance * fontSize * letterSpacing

            , baselineY
        );

        aboveBaselineY = std::max( aboveBaselineY, glyphs[i]->mHorizontalBearingY * fontSize ); 
        belowBaselineY = std::max( belowBaselineY,(  glyphs[i]->mHeight
                                                   - glyphs[i]->mHorizontalBearingY ) * fontSize );
    }

    width =   instanceOrigins[numGlyphs-1].mX
            + glyphs[numGlyphs-1]->mWidth * fontSize
            - instanceOrigins[0].mX;

    height = aboveBaselineY + belowBaselineY;
}

void RuntimeHelper::getBoundingBoxes(

    const float                   fontSize,
    const float                   spreadRatio,
    const vector< const Glyph* >& glyphs,
    const vector< Point2D >&      instanceOrigins,
    vector< GlyphBound >&         bounds
) const {

    bounds.clear();

    const auto numGlyphs = glyphs.size();

    const float spreadVertex  = spreadInFontMetrics() * spreadRatio;
    const float spreadTexture = spreadInTexture()     * spreadRatio;

    for ( int i = 0; i < numGlyphs; i++ ) {

        const auto* glyph = glyphs[i];

        const Point2D bottomLeft(

              instanceOrigins[i].mX
            + glyph->mHorizontalBearingX * fontSize

          ,   instanceOrigins[i].mY 
            +   ( glyph->mHorizontalBearingY - glyph->mHeight )
              * fontSize
        );

        const Rect frameBound(
            bottomLeft.mX - spreadVertex * fontSize,
            bottomLeft.mY - spreadVertex * fontSize,
            ( glyph->mWidth  + 2.0f * spreadVertex ) * fontSize,
            ( glyph->mHeight + 2.0f * spreadVertex ) * fontSize
        );

        const Rect textureBound(
            glyph->mTextureCoordX - spreadTexture,
            glyph->mTextureCoordY - spreadTexture,
            glyph->mTextureWidth  + 2.0f * spreadTexture,
            glyph->mTextureHeight + 2.0f * spreadTexture
        );

        bounds.emplace_back( frameBound, textureBound );
    }
}

} // namespace Font
