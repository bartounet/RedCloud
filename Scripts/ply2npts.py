#! /usr/bin/python
# A script wich convert ply file extracted from #REPLACEME#
# to .npts file in order to use the poisson reconstruction


import sys


# ==========================================================
# ----------------------------------------------------------
# ==========================================================

if (len(sys.argv) != 2):
	print 'Usage: ' + sys.argv[0] + ' <plyFile>'

plyFilename = sys.argv[1]
plyFile = open(plyFilename, 'r')

nptsFilename = plyFilename[:len(plyFilename) - 4] + '.npts'
nptsFile = open(nptsFilename, 'w')

readable = False
for line in plyFile:
	if (not readable):
		if (line == 'end_header\n' or line == 'end_header\r\n'):
			readable = True
	else:
		vertex = line.split(' ')
		newLine = vertex[0] + ' ' + vertex[1] + ' ' + vertex[2] + ' ' +\
			vertex[3] + ' ' + vertex[4] + ' ' + vertex[5] + '\r\n'
		nptsFile.write(newLine)

plyFile.close()
nptsFile.close()
# ==========================================================
# ----------------------------------------------------------
# ==========================================================
