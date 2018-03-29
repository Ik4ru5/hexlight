#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import math
from math import floor
from pprint import pprint
from svg import SVG, Polygon, Text

# Border and Spacing configuration
border = 0.0 # Border to the edge in mm
spacing = 1.0 # Space between parts in mm

# Hexagon dimenstions
sideLength = 50.0 # Length of a side of the hexagon in mm
height = 40.0 # Height of the lamp in mm
materialThickness = 4 # Thickness of the material in mm
numberOfInterlocks = 3
interlockDepth = materialThickness / 0.75
interlockWidth = (sideLength + (interlockDepth * 2)) / ((numberOfInterlocks * 2) + 1)
sideInterlockHeight = (height - (materialThickness * 2)) / ((numberOfInterlocks * 2) - 1)


# Groove dimenstions
grooveWidth = (materialThickness * 2) - 0.5 # Width of the groove
grooveDepth = (height / 3) * 2 # Depth of a groove where pieces are interlocked
doubleGroove = height - grooveDepth # Depth of a double groove for the parts in the middel

# Indentation configuration
indentationDepth = (height / 2)
indentationOffset = 5 # How fare should the slante groove be slanted
indentationWidth = (sideLength - 2 * indentationOffset)


# Interlocking groove configuration
numberOfGrooves = 6 # must be devisiable by two
interlockGrooveDepth = materialThickness / 0.75

showText = False

def drawEvenSidePart(part):
	part.drawToRelative()
	part.drawToRelative((sideLength+2*interlockGrooveDepth + 2*materialThickness, 0))
		
	for groove in range(numberOfGrooves / 2):
		part.drawToRelative((0, (height / numberOfGrooves) - 0.0125))
		part.drawToRelative((-interlockGrooveDepth, 0))
		part.drawToRelative((0, (height / numberOfGrooves) + 0.0125))
		if groove != (numberOfGrooves / 2) - 1:
			part.drawToRelative((interlockGrooveDepth, 0))
		
	part.drawToRelative((-sideLength - 2*materialThickness, 0))
	
	for groove in range(numberOfGrooves / 2):
		part.drawToRelative((0, -(height / numberOfGrooves) - 0.0125))
		part.drawToRelative((-interlockGrooveDepth, 0))
		part.drawToRelative((0, -(height / numberOfGrooves) + 0.0125))
		if groove != (numberOfGrooves / 2) - 1:
			part.drawToRelative((interlockGrooveDepth, 0))


part = SVG('test', "1220mm", "610mm")
	
partNo = 0
start = (border, border)

line = Polygon(part, start)

for i in range(3):
	line.drawToRelative((interlockWidth, 0))
	line.drawToRelative((0, materialThickness))
	line.drawToRelative((interlockWidth, 0))
	line.drawToRelative((0, -materialThickness))

line.drawToRelative((interlockWidth, 0))
#side
line.drawToRelative((0, materialThickness * 2))

for i in range(3):
	line.drawToRelative((-interlockDepth, 0))
	line.drawToRelative((0, sideInterlockHeight))
	line.drawToRelative((interlockDepth, 0))
	if i is not 2:
		line.drawToRelative((0, sideInterlockHeight))
	
line.drawToRelative((0, materialThickness * 2))

#bottom
for i in range(3):
	line.drawToRelative((-interlockWidth, 0))
	line.drawToRelative((0, -materialThickness))
	line.drawToRelative((-interlockWidth, 0))
	line.drawToRelative((0, materialThickness))

line.drawToRelative((-interlockWidth, 0))

line.drawToRelative((0, -materialThickness * 2))

#side
for i in range(3):
	if i is not 0:
		line.drawToRelative((0, -sideInterlockHeight))
	line.drawToRelative((interlockDepth, 0))
	line.drawToRelative((0, -sideInterlockHeight))
	line.drawToRelative((-interlockDepth, 0))
	
line.drawToRelative((0, -materialThickness * 2))

#line.drawToRelative((0, height))

part.writeSVG()