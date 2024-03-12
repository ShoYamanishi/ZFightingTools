#!/usr/bin/env python
# -*- coding: utf-8 -*-

import math
import numpy as np

import matplotlib.pyplot as plt

# Mapping z_vcs to z_scs
from vcs_to_scs_functions import Fz_perspective
from vcs_to_scs_functions import Fz_log_depth_fn
from vcs_to_scs_functions import Fz_log_depth_cf
from vcs_to_scs_functions import abs_dFzdz_perspective
from vcs_to_scs_functions import abs_dFzdz_log_depth_fn
from vcs_to_scs_functions import abs_dFzdz_log_depth_cf
from vcs_to_scs_functions import min_gap_perspective
from vcs_to_scs_functions import min_gap_log_depth_fn
from vcs_to_scs_functions import min_gap_log_depth_cf

from sample_generator import generate_Z_vcs_samples


# Sampled minimum differences

from output_parser import parse_output_from_experiments
from chart_plotter import plot_single_chart

OUTPUT_FILE_PATH = '../output/'

FILE_PATH_NORMAL_DEPTH    = OUTPUT_FILE_PATH + 'results_perspective_depth.txt'
FILE_PATH_LOG_DEPTH_FN    = OUTPUT_FILE_PATH + 'results_log_depth_fn.txt'
FILE_PATH_LOG_DEPTH_CF_06 = OUTPUT_FILE_PATH + 'results_log_depth_cf_06.txt'
FILE_PATH_LOG_DEPTH_CF_03 = OUTPUT_FILE_PATH + 'results_log_depth_cf_03.txt'
FILE_PATH_LOG_DEPTH_CF_00 = OUTPUT_FILE_PATH + 'results_log_depth_cf_00.txt'

sampled_min_gap_perspective       = parse_output_from_experiments( FILE_PATH_NORMAL_DEPTH )
sampled_min_gap_log_depth_fn      = parse_output_from_experiments( FILE_PATH_LOG_DEPTH_FN )
sampled_min_gap_log_depth_cf_06   = parse_output_from_experiments( FILE_PATH_LOG_DEPTH_CF_06 )
sampled_min_gap_log_depth_cf_03   = parse_output_from_experiments( FILE_PATH_LOG_DEPTH_CF_03 )
sampled_min_gap_log_depth_cf_00   = parse_output_from_experiments( FILE_PATH_LOG_DEPTH_CF_00 )

NUM_SAMPLES = 1000

NEAR = 1.0e-1

FARs      = [ 5.0e-1,       1.0e0,       2.0e0,       5.0e0,        1.0e1,      1.0e4,       1.0e10       ]
FARLabels = [ 'far=5.0E-1', 'far=1.0E0', 'far=2.0E0', 'far=5.0E0', 'far=1.0E1', 'far=1.0E4', 'far=1.0E10' ]

Fz_perspective_list       = []
dFzdz_perspective_list    = []
min_gap_perspective_list = []

Fz_log_depth_fn_list       = []
dFzdz_log_depth_fn_list    = []
min_gap_log_depth_fn_list = []

Fz_log_depth_cf_06_list       = []
dFzdz_log_depth_cf_06_list    = []
min_gap_log_depth_cf_06_list = []

Fz_log_depth_cf_03_list       = []
dFzdz_log_depth_cf_03_list    = []
min_gap_log_depth_cf_03_list = []

Fz_log_depth_cf_00_list       = []
dFzdz_log_depth_cf_00_list    = []
min_gap_log_depth_cf_00_list = []

for far in FARs:

    Z_VCS = generate_Z_vcs_samples( NEAR, far, NUM_SAMPLES )
    Z_VCS_NEG = Z_VCS * -1.0

    Fz    = [ Fz_perspective( z, NEAR, far ) for z in Z_VCS ]
    Fz_perspective_list.append( ( Z_VCS_NEG, Fz ) )

    dFzdz = [ abs_dFzdz_perspective( z, NEAR, far ) for z in Z_VCS ]
    dFzdz_perspective_list.append( ( Z_VCS_NEG, dFzdz ) )

    min_gap = [ min_gap_perspective( z, NEAR, far ) for z in Z_VCS ]
    min_gap_perspective_list.append( ( Z_VCS_NEG, min_gap ) )

    Fz    = [ Fz_log_depth_fn( z, NEAR, far ) for z in Z_VCS ]
    Fz_log_depth_fn_list.append( ( Z_VCS_NEG, Fz ) )

    dFzdz = [ abs_dFzdz_log_depth_fn( z, NEAR, far ) for z in Z_VCS ]
    dFzdz_log_depth_fn_list.append( ( Z_VCS_NEG, dFzdz ) )

    min_gap = [ min_gap_log_depth_fn( z, NEAR, far ) for z in Z_VCS ]
    min_gap_log_depth_fn_list.append( ( Z_VCS_NEG, min_gap ) )

    Fz    = [ Fz_log_depth_cf( z, 1.0E-06, far ) for z in Z_VCS ]
    Fz_log_depth_cf_06_list.append( ( Z_VCS_NEG, Fz ) )

    dFzdz = [ abs_dFzdz_log_depth_cf( z, 1.0E-06, far ) for z in Z_VCS ]
    dFzdz_log_depth_cf_06_list.append( ( Z_VCS_NEG, dFzdz ) )

    min_gap = [ min_gap_log_depth_cf( z, 1.0E-06, far ) for z in Z_VCS ]
    min_gap_log_depth_cf_06_list.append( ( Z_VCS_NEG, min_gap ) )

    Fz    = [ Fz_log_depth_cf( z, 1.0E-03, far ) for z in Z_VCS ]
    Fz_log_depth_cf_03_list.append( ( Z_VCS_NEG, Fz ) )

    dFzdz = [ abs_dFzdz_log_depth_cf( z, 1.0E-03, far ) for z in Z_VCS ]
    dFzdz_log_depth_cf_03_list.append( ( Z_VCS_NEG, dFzdz ) )

    min_gap = [ min_gap_log_depth_cf( z, 1.0E-03, far ) for z in Z_VCS ]
    min_gap_log_depth_cf_03_list.append( ( Z_VCS_NEG, min_gap ) )

    Fz    = [ Fz_log_depth_cf( z, 1.0, far ) for z in Z_VCS ]
    Fz_log_depth_cf_00_list.append( ( Z_VCS_NEG, Fz ) )

    dFzdz = [ abs_dFzdz_log_depth_cf( z, 1.0, far ) for z in Z_VCS ]
    dFzdz_log_depth_cf_00_list.append( ( Z_VCS_NEG, dFzdz ) )

    min_gap = [ min_gap_log_depth_cf( z, 1.0, far ) for z in Z_VCS ]
    min_gap_log_depth_cf_00_list.append( ( Z_VCS_NEG, min_gap ) )


# Plots for Perspective

plot_single_chart( 
    Fz_perspective_list,
    FARLabels,
    False, 
    None,
   'F(z)', 
   'F(z): Perspective Projection, near = 1.0E-1',
   OUTPUT_FILE_PATH + 'plot_Fz_perspective.png'
)

plot_single_chart( 
    dFzdz_perspective_list,
    FARLabels,
    True, 
    None,
   '|dF(z)/dz|', 
   'dF(z)/dz: Perspective Projection, near = 1.0E-1',   
    OUTPUT_FILE_PATH + 'plot_dFzdz_perspective.png'
)

min_gap_perspective_list_ex = min_gap_perspective_list.copy()
min_gap_perspective_list_ex.append( sampled_min_gap_perspective )

FARLabels_ex = FARLabels.copy()
FARLabels_ex.append( 'far=1.0E10 sampled with test rig' )

plot_single_chart( 
    min_gap_perspective_list_ex,
    FARLabels_ex,
    True, 
    (1.0e-10, None),
   'Δz in VCS', 
   'Minimum Gap in VCS: Perspective Projection, near = 1.0E-1',
   OUTPUT_FILE_PATH + 'plot_min_gap_perspective.png'
)

# Plots for Log Depth FN

plot_single_chart( 
    Fz_log_depth_fn_list,
    FARLabels,
    False, 
    None,
   'F(z)', 
   'F(z): Log Depth (ln(-z)-ln(n))/(ln(f)-ln(n)), near = 1.0E-1',
   OUTPUT_FILE_PATH + 'plot_Fz_log_depth_fn.png'
)

plot_single_chart( 
    dFzdz_log_depth_fn_list,
    FARLabels,
    True, 
    None,
   '|dF(z)/dz|', 
   'dF(z)/dz: Log Depth (ln(-z)-ln(n))/(ln(f)-ln(n)), near = 1.0E-1',
   OUTPUT_FILE_PATH + 'plot_dFzdz_log_depth_fn.png'
)

min_gap_log_depth_fn_list_ex = min_gap_log_depth_fn_list.copy()
min_gap_log_depth_fn_list_ex.append( sampled_min_gap_log_depth_fn )

FARLabels_ex = FARLabels.copy()
FARLabels_ex.append( 'far=1.0E10 sampled with test rig' )

plot_single_chart( 
    min_gap_log_depth_fn_list_ex,
    FARLabels_ex,
    True, 
    (1.0e-10, None),
   'Δz in VCS', 
   'Minimum Gap in VCS: Log Depth (ln(-z)-ln(n))/(ln(f)-ln(n)), near = 1.0E-1',
   OUTPUT_FILE_PATH + 'plot_min_gap_log_depth_fn.png'
)

# Plots for Log Depth CF; C=1.0e-6

plot_single_chart( 
    Fz_log_depth_cf_06_list,
    FARLabels,
    False, 
    None,
   'F(z)', 
   'F(z): Log Depth (ln(-cz +1)/ln(cf+1)), C = 1.0E-6',
   OUTPUT_FILE_PATH + 'plot_Fz_log_depth_cf_06.png'
)

plot_single_chart( 
    dFzdz_log_depth_cf_06_list,
    FARLabels,
    True, 
    None,
   '|dF(z)/dz|', 
   'dF(z)/dz: Log Depth (ln(-cz +1)/ln(cf+1)), C = 1.0E-6',
   OUTPUT_FILE_PATH + 'plot_dFzdz_log_depth_cf_06.png'
)

min_gap_log_depth_fn_list_ex = min_gap_log_depth_cf_06_list.copy()
min_gap_log_depth_fn_list_ex.append( sampled_min_gap_log_depth_cf_06 )

FARLabels_ex = FARLabels.copy()
FARLabels_ex.append( 'far=1.0E10 sampled with test rig' )

plot_single_chart( 
    min_gap_log_depth_fn_list_ex,
    FARLabels_ex,
    True, 
    (1.0e-10, None),
   'Δz in VCS', 
   'Minimum Gap in VCS: Log Depth (ln(-cz +1)/ln(cf+1)), C = 1.0E-6',
   OUTPUT_FILE_PATH + 'plot_min_gap_log_depth_cf_06.png'
)

# Plots for Log Depth CF; C=1.0e-3

plot_single_chart( 
    Fz_log_depth_cf_03_list,
    FARLabels,
    False, 
    None,
   'F(z)', 
   'F(z): Log Depth (ln(-cz +1)/ln(cf+1)), C = 1.0E-3',
   OUTPUT_FILE_PATH + 'plot_Fz_log_depth_cf_03.png'
)

plot_single_chart( 
    dFzdz_log_depth_cf_03_list,
    FARLabels,
    True, 
    None,
   '|dF(z)/dz|', 
   'dF(z)/dz: Log Depth (ln(-cz +1)/ln(cf+1)), C = 1.0E-3',
   OUTPUT_FILE_PATH + 'plot_dFzdz_log_depth_cf_03.png'
)

min_gap_log_depth_fn_list_ex = min_gap_log_depth_cf_03_list.copy()
min_gap_log_depth_fn_list_ex.append( sampled_min_gap_log_depth_cf_03 )

FARLabels_ex = FARLabels.copy()
FARLabels_ex.append( 'far=1.0E10 sampled with test rig' )

plot_single_chart( 
    min_gap_log_depth_fn_list_ex,
    FARLabels_ex,
    True, 
    (1.0e-10, None),
   'Δz in VCS', 
   'Minimum Gap in VCS: Log Depth (ln(-cz +1)/ln(cf+1)), C = 1.0E-3',
   OUTPUT_FILE_PATH + 'plot_min_gap_log_depth_cf_03.png'
)
# Plots for Log Depth CF; C=1.0e0

plot_single_chart( 
    Fz_log_depth_cf_00_list,
    FARLabels,
    False, 
    None,
   'F(z)', 
   'F(z): Log Depth (ln(-cz +1)/ln(cf+1)), C = 1.0',
   OUTPUT_FILE_PATH + 'plot_Fz_log_depth_cf_00.png'
)

plot_single_chart( 
    dFzdz_log_depth_cf_00_list,
    FARLabels,
    True, 
    None,
   '|dF(z)/dz|', 
   'dF(z)/dz: Log Depth (ln(-cz +1)/ln(cf+1)), C = 1.0',
   OUTPUT_FILE_PATH + 'plot_dFzdz_log_depth_cf_00.png'
)

min_gap_log_depth_fn_list_ex = min_gap_log_depth_cf_00_list.copy()
min_gap_log_depth_fn_list_ex.append( sampled_min_gap_log_depth_cf_00 )

FARLabels_ex = FARLabels.copy()
FARLabels_ex.append( 'far=1.0E10 sampled with test rig' )

plot_single_chart( 
    min_gap_log_depth_fn_list_ex,
    FARLabels_ex,
    True, 
    (1.0e-10, None),
   'Δz in VCS', 
   'Minimum Gap in VCS: Log Depth (ln(-cz +1)/ln(cf+1)), C = 1.0',
   OUTPUT_FILE_PATH + 'plot_min_gap_log_depth_cf_00.png'
)

# Comparison among different min_gaps: FAR = 1.0e10

theoretical_min_gap_far_e10_list= [
    min_gap_perspective_list[-1],
    min_gap_log_depth_fn_list[-1],
    min_gap_log_depth_cf_00_list[-1],
    min_gap_log_depth_cf_03_list[-1],
    min_gap_log_depth_cf_06_list[-1]
]

theoretical_min_gap_far_e10_labels= [
    'perspective: ≅ 1+ n/z',
    'log depth (ln(-z)-ln(n))/(ln(f)-ln(n))',
    'log depth CF: ln(-cz+1)/ln(cf+1), c=1.0',
    'log depth CF: ln(-cz+1)/ln(cf+1), c=1.0E-3',
    'log depth CF: ln(-cz+1)/ln(cf+1), c=1.0E-6'
]

plot_single_chart( 
    theoretical_min_gap_far_e10_list,
    theoretical_min_gap_far_e10_labels,
    True, 
    (1.0e-8, 1.0e6),
   'Δz in VCS',
   'Theoretical Minimum Gap in VCS : Different Depth Types, Near=1.0E-1, Far=1.0E10',
   OUTPUT_FILE_PATH + 'plot_theoretical_min_gap_all.png'
)


observed_min_gap_far_e10_list= [
    sampled_min_gap_perspective,
    sampled_min_gap_log_depth_fn,
    sampled_min_gap_log_depth_cf_00,
    sampled_min_gap_log_depth_cf_03,
    sampled_min_gap_log_depth_cf_06
]

observed_min_gap_far_e10_labels= [
    'perspective: ≅ 1+ n/z',
    'log depth (ln(-z)-ln(n))/(ln(f)-ln(n))',
    'log depth CF: ln(-cz+1)/ln(cf+1), c=1.0',
    'log depth CF: ln(-cz+1)/ln(cf+1), c=1.0E-3',
    'log depth CF: ln(-cz+1)/ln(cf+1), c=1.0E-6'
]

plot_single_chart( 
    observed_min_gap_far_e10_list,
    observed_min_gap_far_e10_labels,
    True, 
    (1.0e-8, 1.0e6),
   'Δz in VCS',
   'Observed Mininum Gap in VCS : Different Depth Types, Near=1.0E-1, Far=1.0E10',
   OUTPUT_FILE_PATH + 'plot_observed_min_gap_all.png'
)

# Comparison among different Cs for type CF: FAR = 1.0e10


Fz_log_depth_cf_list = [
    Fz_log_depth_cf_06_list[-1],
    Fz_log_depth_cf_03_list[-1],
    Fz_log_depth_cf_00_list[-1]
]

Fz_log_depth_cf_labels = [
    'c=1.0E-6',
    'c=1.0E-3',
    'c=1.0'
]

plot_single_chart( 
    Fz_log_depth_cf_list,
    Fz_log_depth_cf_labels,
    False,
    None,
   'F(z)',
   'F(z): Log Depth (ln(-cz +1)/ln(cf+1)), F = 1.0E10',
   OUTPUT_FILE_PATH + 'plot_Fz_log_depth_cf.png'
)

dFzdz_log_depth_cf_list = [
    dFzdz_log_depth_cf_06_list[-1],
    dFzdz_log_depth_cf_03_list[-1],
    dFzdz_log_depth_cf_00_list[-1]
]

dFzdz_log_depth_cf_labels = [
    'c=1.0E-6',
    'c=1.0E-3',
    'c=1.0'
]

plot_single_chart( 
    dFzdz_log_depth_cf_list,
    dFzdz_log_depth_cf_labels,
    True,
    None,
   '|dF(z)/dz|',
   'dF(z)/dz: Log Depth (ln(-cz +1)/ln(cf+1)), F = 1.0E10',
   OUTPUT_FILE_PATH + 'plot_dFzdz_log_depth_cf.png'
)

min_gap_log_depth_cf_list = [
    min_gap_log_depth_cf_06_list[-1],
    min_gap_log_depth_cf_03_list[-1],
    min_gap_log_depth_cf_00_list[-1],
    sampled_min_gap_log_depth_cf_06,
    sampled_min_gap_log_depth_cf_03,
    sampled_min_gap_log_depth_cf_00
]

min_gap_log_depth_cf_labels = [
    'c=1.0E-6',
    'c=1.0E-3',
    'c=1.0',
    'c=1.0E-6 sampled with test rig',
    'c=1.0E-3 sampled with test rig',
    'c=1.0 sampled with test rig'
]

plot_single_chart( 
    min_gap_log_depth_cf_list,
    min_gap_log_depth_cf_labels,
    True,
    (1.0e-8, 1.0e6),
   'Δz in VCS',
   'Minimum Gap in VCS: Log Depth (ln(-cz +1)/ln(cf+1)), F = 1.0E10',
   OUTPUT_FILE_PATH + 'plot_min_gap_log_depth_cf.png'
)

