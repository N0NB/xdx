#
#  xdx - GTK+ DX-cluster client for amateur radio
#  Copyright (C) 2002-2006 Joop Stakenborg <pg4i@amsat.org>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Library General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#

# gnuplot script for displaying wwv data, call with 'gnuplot wwv.gnuplot'
# version 1.0, March 8, 2006 - initial release, distributed with xdx-2.0

set term x11 title "DK0WCY"
unset mouse
set format x "%.0f"
set xtics rotate

set multiplot

set title "Solar Flux Index"
set size 0.4,0.45
set origin 0.0,0.55
plot '~/.xdx/DK0WCY-3.tsv' using 1:2  with lines notitle

set title "A Index"
set size 0.4,0.45
set origin 0.5,0.55
plot '~/.xdx/DK0WCY-3.tsv' using 1:3  with lines notitle

set title "K Index"
set size 0.4,0.45
set origin 0.0,0.05
plot '~/.xdx/DK0WCY-3.tsv' using 1:4  with lines notitle

set title "Sunspot number"
set size 0.4,0.45
set origin 0.5,0.05
plot '~/.xdx/DK0WCY-3.tsv' using 1:5  with lines notitle

unset multiplot

pause -1 "Hit return to continue"
