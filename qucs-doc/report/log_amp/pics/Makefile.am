## Process this file with automake to produce Makefile.in
# 
# report/log_amp/pics/Makefile.am
#
# Automake input file.
#
# Copyright (C) 2008 Stefan Jahn <stefan@lkcc.org>
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this package; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
# Boston, MA 02110-1301, USA.  
#

EXTRA_DIST = $(PDFS)

PDFS = log_amp_A.pdf log_amp_Fig2.pdf log_amp_Fig7.pdf log_amp_Fig10.pdf \
  log_amp_Fig3.pdf log_amp_Fig9.pdf log_amp_Fig1.pdf log_amp_Fig4.pdf \
  log_amp_Fig_temp.pdf

report: $(XPDF)

SUFFIXES = .fig .pdf

.fig.pdf:
	$(FIG2DEV) -L pdf $< > $@

CLEANFILES = $(XPDF) *~ *.bak *.orig *.rej
MAINTAINERCLEANFILES = Makefile.in
