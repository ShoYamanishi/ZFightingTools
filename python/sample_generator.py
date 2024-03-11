#!/usr/bin/env python
# -*- coding: utf-8 -*-

import math
import numpy as np

# Sample Generation

def generate_Z_vcs_samples( near, far, num_samples ):

    '''
    generates the equidistant samples between near and far 
    in the log scale.
    '''

    near_log = math.log( near )
    far_log  = math.log( far  )


    range_log = far_log - near_log
    step_log  = range_log / float( num_samples )

    samples= []

    for x in range( 0, num_samples ):

        sample_log = near_log + float(x) * step_log

        sample = math.exp( sample_log )

        samples.append( sample )

    # multiply by -1 to make the values negative.
    return np.array( samples ) * -1.0 
