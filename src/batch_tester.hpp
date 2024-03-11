#ifndef __DEPTH_TEST_BATCH_TESTER_HPP__
#define __DEPTH_TEST_BATCH_TESTER_HPP__

#include <vector>
#include <cmath>

namespace DepthTest {

class BatchTester {

public:

    static constexpr float MINIMUM_GAP = 1.0e-20; // real limit around 1.0E-37
    static constexpr float MAXIMUM_GAP = 1.0e30;  // real limit around 1.0E+37

    explicit BatchTester(

        const SquareRenderer::DepthTestType depth_test_type,
        const float near,
        const float far,
        const float param_c,
        const int   num_sample_points,
        const int   num_perturbed_samples
    ) noexcept
        :m_tester               { depth_test_type }
        ,m_rand_gen             { }
        ,m_depth_test_type      { depth_test_type }
        ,m_near                 { near }
        ,m_far                  { far }
        ,m_param_c              { param_c }
        ,m_num_samples          { num_sample_points }
        ,m_num_perturbed_samples{ num_perturbed_samples }
    {
    }

    void run()
    {
        switch ( m_depth_test_type ) {
          case SquareRenderer::PERSPECTIVE:
            std::cerr << "Testing Perspective (normal) Depth.\n";
            break;
          case SquareRenderer::LOG_DEPTH_FN:
            std::cerr << "Testing Log Depth ((log(-z)-log(n)) / (log(f)-log(n)) type).\n";
            break;
          case SquareRenderer::LOG_DEPTH_CF:
            std::cerr << "Testing Log Depth (log(-cz+1)/ log(cf+1) type).\n";
            break;

          default:
            throw std::runtime_error( "unknown depth type" );
        }

        std::cerr << "Parameters: " << m_near << "\n";
        std::cerr << "    near: " << m_near << "\n";
        std::cerr << "    far: " << m_far   << "\n";
        std::cerr << "    param C: " << m_param_c  << "\n";
        std::cerr << "    test points: " << m_num_samples << "\n";
        std::cerr << "    num_perturbed_samples: " << m_num_perturbed_samples << "\n";

        generateSamplePoints();
        int index{0};
        for ( const auto& sample_point: m_sample_points ) {

            std::cerr << "sample point [" << index << "]:\t" << sample_point << "\t";

            const auto result = testOneSamplePoint( sample_point );

            std::cerr << result << "\n";

            m_results.push_back( result );

            index++;
        }
    }    

private:

    void generateSamplePoints()
    {
        const float log_near = log( m_near );
        const float log_far  = log( m_far  );
        const float log_diff = log_far - log_near;

        const float num_samples_f = static_cast<float>( m_num_samples );

        for ( int i = 1; i < m_num_samples; i++ ) {

            const auto alpha = static_cast<float>(i) / num_samples_f;
            const auto log_point = log_near + alpha * log_diff;

            const auto point = exp( log_point );

            m_sample_points.push_back( point );
        }
    }

    float testOneSamplePoint( const float sample_point )
    {
        if ( sample_point < m_near || m_far < sample_point ) {
            return m_far;
        }
        const auto min_gap = std::min( sample_point - m_near, m_far - sample_point );

        auto base_gap = min_gap * 0.1f;

        auto range = base_gap;

        while( isBaseGapAndRangeOK( base_gap, range, sample_point ) ) {

            for ( int i = 0; i < 4; i++ ) {

                const auto current_gap = base_gap - static_cast<float>(i) / 4.0f * range;

                const auto result = testOneGap( sample_point, current_gap );

                if ( result == false ) {

                    if ( i == 0 )  {

                        base_gap += range;
                        range = 2.0f * range; // widen the search
                    }
                    else if ( i == 1 ) {

                        range = 0.5f * range; // narrow the search
                    }
                    else if ( i == 2 ) {

                        base_gap -= ( 0.25f * range );
                        range = 0.5f * range; // narrow the search
                    }            
                    else{ // i == 3
                        base_gap -= ( 0.5f * range );
                        range = 0.5f * range; // narrow the search
                    }
                    break;
                }
                else if ( i == 3 ) { // true until i = 3
                    base_gap -= ( 0.5f * range );
                    range = 0.5f * range; // narrow the search
                }
            }
        }

        return base_gap;
    }

    bool isBaseGapAndRangeOK(

        const float base_gap, 
        const float range, 
        const float sample_point

    ) const {

        if( range <= MINIMUM_GAP ) {

            return false; // the gap too small in general.
        }
        if( base_gap >= MAXIMUM_GAP ) {

            return false; // the gap too big in general.
        }

        if ( base_gap >= sample_point * 0.5f ) {

            return false; // the gap too big for the sample point.
        }

        if ( sample_point + base_gap * 0.5f >= m_far ) {

            return false; // the grid exceeds the far limit.
        }

        if ( sample_point - base_gap * 0.5f <= m_near ) {

            return false; // the grid exceeds the near limit.
        }

        return true;
    }

    bool testOneGap( const float sample_point, const float gap )
    {
        std::uniform_real_distribution< float > distribution{ gap / -500.0f, gap / 500.0f };

        for ( int i = 0; i < m_num_perturbed_samples; i++ ) {

            const auto perturbation = distribution( m_rand_gen );

            bool plane_1_detected;
            bool plane_2_detected;

            const auto plane_1 = sample_point - 0.5f * ( gap + perturbation );
            const auto plane_2 = sample_point + 0.5f * ( gap + perturbation );

            m_tester.test(
                m_near,
                m_far,
                m_param_c,
                plane_1,
                plane_2,
                plane_1_detected,
                plane_2_detected
            );

            if ( (!plane_1_detected) || plane_2_detected ) {

                return false;
            }

            m_tester.test(
                m_near,
                m_far,
                m_param_c,
                plane_2,
                plane_1,
                plane_2_detected,
                plane_1_detected
            );

            if ( (!plane_1_detected) || plane_2_detected ) {

                return false;
            }
        }

        return true;
    }

    SquareRenderer  m_tester;
    std::default_random_engine m_rand_gen;

    const SquareRenderer::DepthTestType m_depth_test_type;
    const float m_near;
    const float m_far;
    const float m_param_c;
    const int   m_num_samples;
    const int   m_num_perturbed_samples;

    std::vector< float > m_sample_points;
    std::vector< float > m_results;
};

} //namespace DepthTest

#endif/*__DEPTH_TEST_BATCH_TESTER_HPP__*/
