#!/usr/bin/env python
# -*- coding: utf-8 -*-

import math

# F(z)

def Fz_perspective( z_vcs, near, far ):

    m11 = -1.0 * ( far + near ) / ( far - near )
    m12 = -2.0 * far * near / ( far - near )
    m21 = -1.0
    m22 = 0.0

    z_ndcs = m11 * z_vcs + m12 * 1.0
    w_ndcs = m21 * z_vcs + m22 * 1.0
     
    return  ( z_ndcs / w_ndcs + 1.0 )/2.0

def Fz_log_depth_fn( z_vcs, near, far ):

    log_n = math.log( near )
    log_f = math.log( far )
    log_z = math.log( -1.0 * z_vcs )

    return ( log_z - log_n ) / ( log_f - log_n )


def Fz_log_depth_cf( z_vcs, C, far ):

    log_cf_plus_one       = math.log( C * far + 1.0 )
    log_minus_cz_plus_one = math.log( -1.0 * C * z_vcs + 1.0 )

    return log_minus_cz_plus_one / log_cf_plus_one

# dF(z)/dz

def abs_dFzdz_perspective( z_vcs, near, far ):

    m12 = -2.0 * far * near / ( far - near )

    return abs(0.5 * m12 / (z_vcs * z_vcs))

def abs_dFzdz_log_depth_fn( z_vcs, near, far ):

    n_log = math.log( near )
    f_log = math.log( far )
    C1 = (f_log - n_log)

    return abs(1.0 / ( C1 * z_vcs ))

def abs_dFzdz_log_depth_cf( z_vcs, C, far ):

    log_cf_plus_one   = math.log( C * far + 1.0 )
    minus_cz_plus_one = -1.0 * C * z_vcs + 1.0

    return abs( -1.0 * C / ( log_cf_plus_one * minus_cz_plus_one ) )

# Minimum Gap

# assuming the depth format is GL_DEPTH24_STENCIL8.
FLOAT_PRECISION_LOG = -24.0 * math.log(2.0)
FLOAT_PRECISION = math.exp( FLOAT_PRECISION_LOG )

def min_gap_perspective( z_vcs, near, far ):

    return FLOAT_PRECISION / abs_dFzdz_perspective( z_vcs, near, far )

def min_gap_log_depth_fn( z_vcs, near, far ):

    return FLOAT_PRECISION / abs_dFzdz_log_depth_fn( z_vcs, near, far )

def min_gap_log_depth_cf( z_vcs, C, far ):

    return FLOAT_PRECISION / abs_dFzdz_log_depth_cf( z_vcs, C, far )

