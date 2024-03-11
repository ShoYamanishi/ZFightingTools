#ifndef __DEPTH_TEST_OPTION_PARSE_HPP__
#define __DEPTH_TEST_OPTION_PARSE_HPP__

#include <string>

#include "square_renderer.hpp"

namespace DepthTest {

class OptionParser 
{

public:

    explicit OptionParser( int argc, char* argv[] ) noexcept
        :m_depth_test_type       { SquareRenderer::UNKNOWN }
        ,m_near                  { 0.0f }
        ,m_far                   { 0.0f }
        ,m_param_c               { 0.0f }
        ,m_num_points            { 0 }
        ,m_num_perturbed_samples { 0 }
    {
        for ( auto i = 1; i < argc ; i++ ) {

            std::string arg( argv[i] );

            if (    arg.compare ( HELP1 ) == 0
                 || arg.compare ( HELP2 ) == 0
                 || arg.compare ( HELP3 ) == 0 ) {
            
                std::cerr << USAGE;
                exit(1);
            }
            else if ( arg.compare ( NEAR ) == 0 ) {

                std::string arg2( argv[++i] );
                m_near = std::stof( arg2 );
            }
            else if ( arg.compare ( FAR ) == 0 ) {

                std::string arg2( argv[++i] );
                m_far = std::stof( arg2 );
            }
            else if ( arg.compare ( PARAM_C ) == 0 ) {

                std::string arg2( argv[++i] );
                m_param_c = std::stof( arg2 );
            }
            else if ( arg.compare ( NUM_POINTS ) == 0 ) {

                std::string arg2( argv[++i] );
                m_num_points = std::stoi( arg2 );
            }
            else if ( arg.compare ( NUM_PERTURBED_SAMPLES ) == 0 ) {

                std::string arg2( argv[++i] );
                m_num_perturbed_samples = std::stoi( arg2 );
            }
            else if ( arg.compare ( DEPTH_TYPE ) == 0 ) {

                std::string arg2( argv[++i] );
                if ( arg2.compare( DEPTH_TYPE_PERSPECTIVE ) == 0 ) {

                    m_depth_test_type = SquareRenderer::PERSPECTIVE;
                }
                else if ( arg2.compare( DEPTH_TYPE_LOGFN ) == 0 ) {

                    m_depth_test_type = SquareRenderer::LOG_DEPTH_FN;
                }
                else if ( arg2.compare( DEPTH_TYPE_LOGCF ) == 0 ) {

                    m_depth_test_type = SquareRenderer::LOG_DEPTH_CF;
                }
            }
            else {
                std::cerr << USAGE;
                exit(1);
            }
        }

        if (    m_depth_test_type == SquareRenderer::UNKNOWN
             || m_near == 0.0f 
             || m_far == 0.0f
             || m_param_c == 0.0f 
             || m_num_points == 0
             || m_num_perturbed_samples == 0
        ) {
            std::cerr << USAGE;
            exit(1);
        }
    }

    SquareRenderer::DepthTestType depthTestType() const 
    {
        return m_depth_test_type;
    }

    float near() const
    {
        return m_near;
    }

    float far() const
    {
        return m_far;
    }

    float paramC() const
    {
        return m_param_c;
    }

    int numPoints() const
    {
        return m_num_points;
    }

    int numPerturbedSamples() const
    {
        return m_num_perturbed_samples;
    }

private:

    static const std::string DEPTH_TYPE;
    static const std::string DEPTH_TYPE_PERSPECTIVE;
    static const std::string DEPTH_TYPE_LOGFN;
    static const std::string DEPTH_TYPE_LOGCF;
    static const std::string NEAR;
    static const std::string FAR;
    static const std::string PARAM_C;
    static const std::string NUM_POINTS;
    static const std::string NUM_PERTURBED_SAMPLES;
    static const std::string HELP1;
    static const std::string HELP2;
    static const std::string HELP3;
    static const std::string USAGE;

    SquareRenderer::DepthTestType m_depth_test_type;

    float m_near;
    float m_far;
    float m_param_c;
    int   m_num_points;
    int   m_num_perturbed_samples;
};

} // namespace DepthTest {

#endif/*__DEPTH_TEST_OPTION_PARSE_HPP__*/

///////////////////////

namespace DepthTest {

const std::string OptionParser::DEPTH_TYPE            = "-depth_type";
const std::string OptionParser::DEPTH_TYPE_PERSPECTIVE= "perspective";
const std::string OptionParser::DEPTH_TYPE_LOGFN      = "logfn";
const std::string OptionParser::DEPTH_TYPE_LOGCF      = "logcf";
const std::string OptionParser::NEAR                  = "-near";
const std::string OptionParser::FAR                   = "-far";
const std::string OptionParser::PARAM_C               = "-c";
const std::string OptionParser::NUM_POINTS            = "-num_points";
const std::string OptionParser::NUM_PERTURBED_SAMPLES = "-num_perturbed_samples";
const std::string OptionParser::HELP1                 = "-h";
const std::string OptionParser::HELP2                 = "-help";
const std::string OptionParser::HELP3                 = "-H";
const std::string OptionParser::USAGE                 = "depth_test_batch -h <for help> -depth_type <\"perspective\"(perspective depth test)/\"logfn\"(log depth test of FN-type)/\"logcf\"(log depth test of CF-type)> -near <near(positive)> -far <far(positive)> -c <parameter C for CF-type> -num_points <num points> -num_perturbed_samples <num samples>\n";

} // namespace DepthTest {
