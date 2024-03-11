#!/usr/bin/env python
# -*- coding: utf-8 -*-

VALID_LINE_PREFIX = 'sample point'

def parse_output_from_experiments( file_path ):

    fh = open( file_path )

    lines = fh.readlines()
    
    z_vcs  = []
    dz_vcs = []

    for line in lines:

        if line.startswith( VALID_LINE_PREFIX ):

            fields = line.split( '\t' )
            z_vcs.append( float(fields[1]) )
            dz_vcs.append( float(fields[2]) )

    return z_vcs, dz_vcs
