#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import math
from math import floor
from pprint import pprint
from svg import SVG, Polygon, Text

# Border and Spacing configuration
border = 0 # Border to the edge in mm
spacing = 5 # Space between parts in mm

# Hexagon dimenstions
sideLength = 50 # Length of a side of the hexagon in mm
height = 50 # Height of the lamp in mm
materialThickness = 2 # Thickness of the material in mm

# Groove dimenstions
grooveWidth = materialThickness * 2 # Width of the groove
grooveDepth = (height / 3) * 2 # Depth of a groove where pieces are interlocked
doubleGroove = height - grooveDepth # Depth of a double groove for the parts in the middel

# Indentation configuration
indentationDepth = (height / 2)
indentationOffset = 5 # How fare should the slante groove be slanted
indentationWidth = (sideLength - 2 * indentationOffset)

# Interlocking groove configuration
numberOfGrooves = 10 # must be devisiable by two
interlockGrooveDepth = 2

def drawEndInterlock(line):
	for groove in range(numberOfGrooves / 2):
		line.drawToRelative( (0, (height / numberOfGrooves)) )
		line.drawToRelative( (interlockGrooveDepth, 0) )
		line.drawToRelative( (0, (height / numberOfGrooves)) )
		line.drawToRelative( (-interlockGrooveDepth, 0) )

def drawStartInterlock(line):
	for groove in range(numberOfGrooves / 2):
		line.drawToRelative( (0, -(height / numberOfGrooves)) )
		line.drawToRelative( (interlockGrooveDepth, 0) )
		line.drawToRelative( (0, -(height / numberOfGrooves)) )
		if groove != (numberOfGrooves / 2) - 1:
			line.drawToRelative( (-interlockGrooveDepth, 0) )

def drawGroovePattern(line, count, interlocked = False, double = False):
	if double:
		depth = doubleGroove
	else:
		depth = grooveDepth
	for i in range(count):
		line.drawToRelative( (0, -depth) ) #doubleGroove
		line.drawToRelative( (-grooveWidth, 0) )
		line.drawToRelative( (0, depth) ) #doubleGroove
		if i != count and interlocked:
			line.drawToRelative( (-sideLength, 0) )
		else:
			line.drawToRelative( (-sideLength - interlockGrooveDepth, 0) )

def test():	
	part = SVG('test', 10000, 10000)
	
	partNo = 0
	start = (border, border)
	###################################################################################
	# Part 1
	text = Text(part, ((start[0] + 13), (start[1] + height/2 + height*partNo ) ), "Part: %i" % (partNo + 1), 20)
	line = Polygon(part, (start[0] + interlockGrooveDepth, start[1]) )
	
	# Top
	line.drawToRelative( ( ((sideLength + grooveWidth) * 2) - interlockGrooveDepth, 0 ) )
	
	#Indentation
	line.drawToRelative( (indentationOffset, indentationDepth) )
	line.drawToRelative( (indentationWidth, 0) )
	line.drawToRelative( (indentationOffset, -indentationDepth) )
	
	line.drawToRelative( ((sideLength + grooveWidth) - interlockGrooveDepth, 0) )
	
	# end
	drawEndInterlock(line)
	
	line.drawToRelative( (-sideLength + interlockGrooveDepth, 0) )
	
	# bottom
	drawGroovePattern(line, 3, True)
	
	# start
	drawStartInterlock(line)
	
	partNo += 1
	start = (line.end[0] - interlockGrooveDepth, line.end[1] + spacing + border + height)
	###################################################################################
	# Part 2
	text = Text(part, ((start[0] + 13), (start[1] + (height / 2) ) ), "Part: %s" % str(partNo + 1), 20)
	line = Polygon(part, start)
	
	line.drawToRelative( ((sideLength + grooveWidth) , 0 ) )
	
	#Indentation
	line.drawToRelative( (indentationOffset, indentationDepth) )
	line.drawToRelative( (indentationWidth, 0) )
	line.drawToRelative( (indentationOffset, -indentationDepth) )
	
	line.drawToRelative( (sideLength * 2 + grooveWidth * 3, 0) )
	
	#indentation
	line.drawToRelative( (indentationOffset, indentationDepth) )
	line.drawToRelative( (indentationWidth + indentationOffset, 0) )
	
	line.drawToRelative( (0, height - indentationDepth) )
	
	line.drawToRelative( (-sideLength, 0) )
	
	drawGroovePattern(line, 4, True)

	line.drawTo(start)
	
	start = (line.end[0], line.end[1] + spacing + border + height)
	partNo += 1
	
	###################################################################################
	# Part 3
	text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 1), 20)
	line = Polygon(part, start)
	
	# Top
	line.drawToRelative( ((sideLength * 2 + 2 * grooveWidth) , 0 ) )
	
	#Indentation
	line.drawToRelative( (indentationOffset, indentationDepth) )
	line.drawToRelative( (indentationWidth, 0) )
	line.drawToRelative( (indentationOffset, -indentationDepth) )
	
	line.drawToRelative( ((sideLength + grooveWidth), 0) )
	
	line.drawToRelative( (0, height) )
	line.drawToRelative( (-sideLength, 0) )
	
	# bottom
	drawGroovePattern(line, 3, True)
	
	# start
	line.drawTo(start)
	
	start = (line.end[0], line.end[1] + spacing + border + height)
	partNo += 1
	
	###################################################################################
	# Part 4
	text = Text(part, ((start[0] + 13), (start[1] + (height / 2) ) ), "Part: %s" % str(partNo + 1), 20)
	line = Polygon(part, start)
	
	line.drawToRelative( ((sideLength + grooveWidth) , 0 ) )
	
	#Indentation
	line.drawToRelative( (indentationOffset, indentationDepth) )
	line.drawToRelative( (indentationWidth, 0) )
	line.drawToRelative( (indentationOffset, -indentationDepth) )
	
	line.drawToRelative( (sideLength * 2 + grooveWidth * 3, 0) )
	
	#indentation
	line.drawToRelative( (indentationOffset, indentationDepth) )
	line.drawToRelative( (indentationWidth + indentationOffset, 0) )
	
	line.drawToRelative( (0, height - indentationDepth) )
	
	line.drawToRelative( (-sideLength, 0) )
	
	drawGroovePattern(line, 4, True)
	
	line.drawTo(start)
	
	start = (line.end[0], line.end[1] + spacing + border + height)
	partNo += 1
	
	###################################################################################
	# Part 5 # interlocking
	text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 1), 20)
	line = Polygon(part, (start[0] + interlockGrooveDepth,start[1]))
	
	# Top
	line.drawToRelative( ( ((sideLength + grooveWidth) * 2) - interlockGrooveDepth, 0 ) )
	
	#Indentation
	line.drawToRelative( (indentationOffset, indentationDepth) )
	line.drawToRelative( (indentationWidth, 0) )
	line.drawToRelative( (indentationOffset, -indentationDepth) )
	
	line.drawToRelative( ((sideLength + grooveWidth) - interlockGrooveDepth, 0) )
	
	# end
	drawEndInterlock(line)
	line.drawToRelative( (-sideLength + interlockGrooveDepth, 0) )
	
	# bottom
	drawGroovePattern(line, 3, True)
		
	# start
	drawStartInterlock(line)
	
	start = (line.end[0] - interlockGrooveDepth, line.end[1] + spacing + border + height)
	partNo += 1
	
	###################################################################################
	# Part 6 # Endpiece
	###################################################################################
	# Part 7 # Endpiece
	
	###################################################################################
	# Part 8 # interlocking
	text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 3), 20)
	line = Polygon(part, (start[0] + interlockGrooveDepth, start[1]))
	
	line.drawToRelative( (sideLength - interlockGrooveDepth, 0) )
	line.drawToRelative( (0, doubleGroove) )
	line.drawToRelative( (grooveWidth, 0) )
	
	# Intendtation
	line.drawToRelative( (0, (indentationDepth - doubleGroove) ) )
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, -(indentationDepth - doubleGroove) ) )
	
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -doubleGroove) )
			
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, doubleGroove) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -doubleGroove) )
		
	line.drawToRelative( (sideLength - interlockGrooveDepth, 0 ) )
	
	# end
	drawEndInterlock(line)

	line.drawToRelative( (-sideLength + interlockGrooveDepth, 0) )
	
	drawGroovePattern(line, 3, True, True)
	
	# start
	drawStartInterlock(line)
	
	#line.drawTo(start)
	
	start = (line.end[0] - interlockGrooveDepth, line.end[1] + spacing + border + height)
	partNo += 1
	
	###################################################################################
	# Part 9
	text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 3), 20)
	line = Polygon(part, start)
	
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, doubleGroove) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, (indentationDepth - doubleGroove) ) )
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, -(indentationDepth - doubleGroove) ) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -doubleGroove) )
			
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, doubleGroove) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -doubleGroove) )
	
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, doubleGroove) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, (indentationDepth - doubleGroove) ) )
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, height - indentationDepth) )
	
	for i in range(4):
		line.drawToRelative( (-sideLength, 0) )
		line.drawToRelative( (0, -doubleGroove) )
		line.drawToRelative( (-grooveWidth, 0) )
		line.drawToRelative( (0, doubleGroove) )
	
	line.drawToRelative( (-sideLength, 0 ) )
	line.drawTo(start)
	
	start = (line.end[0], line.end[1] + spacing + border + height)
	partNo += 1
	
	###################################################################################
	# Part 10
	text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 3), 20)
	line = Polygon(part, start)
	
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, doubleGroove) )
	line.drawToRelative( (grooveWidth, 0) )
	
	# Intendtation
	line.drawToRelative( (0, (indentationDepth - doubleGroove) ) )
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, -(indentationDepth - doubleGroove) ) )
	
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -doubleGroove) )
			
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, doubleGroove) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -doubleGroove) )
		
	line.drawToRelative( (sideLength, 0 ) )
	line.drawToRelative( (0, height) )
	
	for i in range(3):
		line.drawToRelative( (-sideLength, 0) )
		line.drawToRelative( (0, -doubleGroove) )
		line.drawToRelative( (-grooveWidth, 0) )
		line.drawToRelative( (0, doubleGroove) )
	
	line.drawToRelative( (-sideLength, 0 ) )
	line.drawTo(start)
	
	start = (line.end[0], line.end[1] + spacing + border + height)
	partNo += 1
	
	###################################################################################
	# Part 11
	text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 3), 20)
	line = Polygon(part, start)
	
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, doubleGroove) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, (indentationDepth - doubleGroove) ) )
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, -(indentationDepth - doubleGroove) ) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -doubleGroove) )
			
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, doubleGroove) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -doubleGroove) )
	
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, doubleGroove) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, (indentationDepth - doubleGroove) ) )
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, height - indentationDepth) )
	
	for i in range(4):
		line.drawToRelative( (-sideLength, 0) )
		line.drawToRelative( (0, -doubleGroove) )
		line.drawToRelative( (-grooveWidth, 0) )
		line.drawToRelative( (0, doubleGroove) )
	
	line.drawToRelative( (-sideLength, 0 ) )
	line.drawTo(start)
	
	start = (line.end[0], line.end[1] + spacing + border + height)
	partNo += 1
	
	
	###################################################################################
	# Part 12
	text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 3), 20)
	line = Polygon(part, (start[0] + interlockGrooveDepth,start[1]))
	
	line.drawToRelative( (sideLength - interlockGrooveDepth, 0) )
	line.drawToRelative( (0, doubleGroove) )
	line.drawToRelative( (grooveWidth, 0) )
	
	# Intendtation
	line.drawToRelative( (0, (indentationDepth - doubleGroove) ) )
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, -(indentationDepth - doubleGroove) ) )
	
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -doubleGroove) )
			
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, doubleGroove) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -doubleGroove) )
		
	line.drawToRelative( (sideLength - interlockGrooveDepth, 0 ) )
	
	drawEndInterlock(line)
	
	line.drawToRelative( (-sideLength + interlockGrooveDepth, 0) )
	drawGroovePattern(line, 3, True, True)
	
	#line.drawToRelative( (-sideLength + interlockGrooveDepth, 0 ) )
	drawStartInterlock(line)
	
	start = (line.end[0] - interlockGrooveDepth, line.end[1] + spacing + border + height)
	partNo += 1
	
	###################################################################################
	# Part 13 # Endpiece
	
	###################################################################################
	# Part 14 # Endpiece
	
	###################################################################################
	# Part 15
	text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 5), 20)
	line = Polygon(part, (start[0] + interlockGrooveDepth, start[1]))
	
	line.drawToRelative( (sideLength - interlockGrooveDepth, 0) )
	line.drawToRelative( (0, grooveDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth) )
	
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, grooveDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth+indentationDepth) )

	#Indentation
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, grooveDepth-indentationDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth) )
	
	line.drawToRelative( ((sideLength - interlockGrooveDepth), 0) )
	
	drawEndInterlock(line)
	
	line.drawToRelative( (-(sideLength*4 + grooveWidth*3 - interlockGrooveDepth), 0) )
	
	drawStartInterlock(line)
	
	start = (line.end[0] - interlockGrooveDepth, line.end[1] + spacing + border + height)
	partNo += 1
	
	###################################################################################
	# Part 16
	text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 5), 20)
	line = Polygon(part, start)
	
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, grooveDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth+indentationDepth) )

	#Indentation
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, grooveDepth-indentationDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth) )
	
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, grooveDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth) )
	
	line.drawToRelative( ((sideLength), 0) )
	
	line.drawToRelative( (0, grooveDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth+indentationDepth) )

	#Indentation
	line.drawToRelative( (sideLength, 0) )	
	line.drawToRelative( (0, height-indentationDepth) )

	line.drawToRelative( (-(sideLength*5 + grooveWidth*4), 0) )
	line.drawTo(start)
	
	start = (line.end[0], line.end[1] + spacing + border + height)
	partNo += 1
		###################################################################################
	# Part 17
	text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 5), 20)
	line = Polygon(part, start)
	
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, grooveDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth) )
	
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, grooveDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth+indentationDepth) )

	#Indentation
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, grooveDepth-indentationDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth) )
	
	line.drawToRelative( ((sideLength), 0) )
	line.drawToRelative( (0, height) )
	
	line.drawToRelative( (-(sideLength*4 + grooveWidth*3), 0) )
	line.drawTo(start)
	
	start = (line.end[0], line.end[1] + spacing + border + height)
	partNo += 1
	###################################################################################
	# Part 18
	text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 5), 20)
	line = Polygon(part, start)
	
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, grooveDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth+indentationDepth) )

	#Indentation
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, grooveDepth-indentationDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth) )
	
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, grooveDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth) )
	
	line.drawToRelative( ((sideLength), 0) )
	
	line.drawToRelative( (0, grooveDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth+indentationDepth) )

	#Indentation
	line.drawToRelative( (sideLength, 0) )	
	line.drawToRelative( (0, height-indentationDepth) )

	line.drawToRelative( (-(sideLength*5 + grooveWidth*4), 0) )
	line.drawTo(start)
	
	start = (line.end[0], line.end[1] + spacing + border + height)
	partNo += 1
	
	###################################################################################
	# Part 19
	text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 5), 20)
	line = Polygon(part, (start[0] + interlockGrooveDepth, start[1]))
	
	line.drawToRelative( (sideLength - interlockGrooveDepth, 0) )
	line.drawToRelative( (0, grooveDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth) )
	
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, grooveDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth+indentationDepth) )

	#Indentation
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, grooveDepth-indentationDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth) )
	
	line.drawToRelative( ((sideLength - interlockGrooveDepth), 0) )
	drawEndInterlock(line)
	
	line.drawToRelative( (-(sideLength*4 + grooveWidth*3 - interlockGrooveDepth), 0) )
	drawStartInterlock(line)
	
	start = (line.end[0], line.end[1] + spacing + border + height)
	partNo += 1
		
	###################################################################################
	# Part 20 # Endpiece
	
	part.writeSVG()
	
	return

if __name__ == '__main__': test()