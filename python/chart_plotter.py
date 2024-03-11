#!/usr/bin/env python
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt

from sample_generator import generate_Z_vcs_samples

def plot_single_chart( data_xys, labels, scale_log_y, y_lim, y_label, title, filename ):

    fig, ax = plt.subplots( 1, constrained_layout = True )

    for data_xy, label in zip( data_xys, labels ):

        ax.plot( data_xy[0], data_xy[1], label = label )
        ax.legend()

    ax.set( xlabel = '-z_vcs', ylabel = y_label, title = title )

    ax.set_xscale('log')

    if scale_log_y:
        ax.set_yscale('log')

    if y_lim:
        ax.set_ylim( y_lim[0], y_lim[1] )

    ax.grid()

    fig.set_size_inches( 8, 4 )
    fig.savefig( filename, dpi = 160 )
