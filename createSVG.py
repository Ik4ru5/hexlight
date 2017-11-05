#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import math
from math import floor
from pprint import pprint
from svg import SVG, Polygon, Text

# Border and Spacing configuration
border = 10 # Border to the edge in mm
spacing = 1 # Space between parts in mm

# Hexagon dimenstions
sideLength = 50.0 # Length of a side of the hexagon in mm
height = 50.0 # Height of the lamp in mm
materialThickness = 3 # Thickness of the material in mm

# Groove dimenstions
grooveWidth = materialThickness * 2 # Width of the groove
grooveDepth = (height / 3) * 2 # Depth of a groove where pieces are interlocked
doubleGroove = height - grooveDepth # Depth of a double groove for the parts in the middel

# Indentation configuration
indentationDepth = (height / 2)
indentationOffset = 5 # How fare should the slante groove be slanted
indentationWidth = (sideLength - 2 * indentationOffset)

# Interlocking groove configuration
numberOfGrooves = 6 # must be devisiable by two
interlockGrooveDepth = 4 # TODO: calculate best value

showText = True

def drawEndInterlock(line):
	for groove in range(numberOfGrooves / 2):
		line.drawToRelative( (0, (height / numberOfGrooves) - 0.0125) )
		line.drawToRelative( (interlockGrooveDepth, 0) ) # raus
		line.drawToRelative( (0, (height / numberOfGrooves) + 0.0125) )
		line.drawToRelative( (-interlockGrooveDepth, 0) )

def drawStartInterlock(line):
	line.drawToRelative((-interlockGrooveDepth,0))
	for groove in range(numberOfGrooves / 2):
		line.drawToRelative( (0, -(height / numberOfGrooves) - 0.0125) )
		print (height / numberOfGrooves)
		line.drawToRelative( (interlockGrooveDepth, 0) )
		line.drawToRelative( (0, -(height / numberOfGrooves) + 0.0125) )
		if groove != (numberOfGrooves / 2) - 1:
			line.drawToRelative( (-interlockGrooveDepth, 0) )

def drawGroovePattern(line, count, interlocked = False, double = False):
	if double:
		depth = doubleGroove
	else:
		depth = grooveDepth
	oldDepth = depth
	
	for i in range(count):
		if (i == 0 or i == count - 1) and interlocked:
			oldDepth = depth
			depth = height / 2
		else:
			depth = oldDepth
			
		line.drawToRelative( (0, -depth) ) #doubleGroove
		line.drawToRelative( (-grooveWidth, 0) )
		line.drawToRelative( (0, depth) ) #doubleGroove
		line.drawToRelative( (-sideLength, 0) )
	
			
def drawEndPiece(endPiece):
	endPiece.drawToRelative((sideLength+2*interlockGrooveDepth + 2*materialThickness, 0))
		
	for groove in range(numberOfGrooves / 2):
		endPiece.drawToRelative((0, (height / numberOfGrooves) - 0.0125))
		endPiece.drawToRelative((-interlockGrooveDepth, 0))
		endPiece.drawToRelative((0, (height / numberOfGrooves) + 0.0125))
		if groove != (numberOfGrooves / 2) - 1:
			endPiece.drawToRelative((interlockGrooveDepth, 0))
		
	endPiece.drawToRelative((-sideLength - 2*materialThickness, 0))
	
	for groove in range(numberOfGrooves / 2):
		endPiece.drawToRelative((0, -(height / numberOfGrooves) - 0.0125))
		endPiece.drawToRelative((-interlockGrooveDepth, 0))
		endPiece.drawToRelative((0, -(height / numberOfGrooves) + 0.0125))
		if groove != (numberOfGrooves / 2) - 1:
			endPiece.drawToRelative((interlockGrooveDepth, 0))


def test():	
	part = SVG('test', "1220mm", "610mm")
	
	partNo = 0
	start = (border, border)
	###################################################################################
	# Part 1
	if showText:
		text = Text(part, ((start[0] + 13), (start[1] + height/2 + height*partNo ) ), "Part: %i" % (partNo + 1), 20)
	line = Polygon(part, (start[0]+ interlockGrooveDepth, start[1]) )
	
	# Top
	line.drawToRelative( ( ((sideLength + grooveWidth) * 2), 0 ) )
	
	#Indentation
	line.drawToRelative( (indentationOffset, indentationDepth) )
	line.drawToRelative( (indentationWidth, 0) )
	line.drawToRelative( (indentationOffset, -indentationDepth) )
	
	line.drawToRelative( ((sideLength + grooveWidth), 0) )
	
	endPiece = Polygon(part, (line.end[0] + spacing, line.end[1]))
	
	# end
	drawEndInterlock(line)
	
	line.drawToRelative( (-sideLength, 0) )
	
	# bottom
	drawGroovePattern(line, 3, True)
	#line.drawToRelative((-interlockGrooveDepth,0))
	
	# start
	drawStartInterlock(line)
	
	partNo += 1
	start = (line.end[0], line.end[1] + spacing + height)
	
	###################################################################################
	# Endpiece
	drawEndPiece(endPiece)
	
	###################################################################################
	# Part 2
	if showText:
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
	
	drawGroovePattern(line, 4, False)

	line.drawTo(start)
	
	start = (line.end[0], line.end[1] + spacing + height)
	partNo += 1
	
	###################################################################################
	# Part 3
	if showText:
		text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 1), 20)
	line = Polygon(part, start)
	
	# Top
	line.drawToRelative( ((sideLength * 2 + 2 * grooveWidth) , 0 ) )
	
	#Indentation
	line.drawToRelative( (indentationOffset, indentationDepth) )
	line.drawToRelative( (indentationWidth, 0) )
	line.drawToRelative( (indentationOffset, -indentationDepth) )
	
	line.drawToRelative( ((sideLength + grooveWidth), 0) )
	
	endPiece = Polygon(part, (line.end[0] + spacing, line.end[1]))
	
	line.drawToRelative( (0, height) )
	line.drawToRelative( (-sideLength, 0) )
	
	# bottom
	drawGroovePattern(line, 3, False)
	
	# start
	line.drawTo(start)
	
	start = (line.end[0], line.end[1] + spacing + height)
	partNo += 1
	
	###################################################################################
	# Endpiece
	drawEndPiece(endPiece)
	
	
	###################################################################################
	# Part 4
	if showText:
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
	
	drawGroovePattern(line, 4, False)
	
	line.drawTo(start)
	
	start = (line.end[0], line.end[1] + spacing + height)
	partNo += 1
	
	###################################################################################
	# Part 5 # interlocking
	if showText:
		text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 1), 20)
	line = Polygon(part, (start[0],start[1]))
	
	# Top
	line.drawToRelative( ( ((sideLength + grooveWidth) * 2), 0 ) )
	
	#Indentation
	line.drawToRelative( (indentationOffset, indentationDepth) )
	line.drawToRelative( (indentationWidth, 0) )
	line.drawToRelative( (indentationOffset, -indentationDepth) )
	
	line.drawToRelative( ((sideLength + grooveWidth), 0) )
	
	endPiece = Polygon(part, (line.end[0] + spacing, line.end[1]))
	
	# end
	drawEndInterlock(line)
	line.drawToRelative( (-sideLength, 0) )
	
	# bottom
	drawGroovePattern(line, 3, True)
		
	# start
	drawStartInterlock(line)
	
	start = (line.end[0], line.end[1] + spacing + height)
	partNo += 1
	
	###################################################################################
	# Endpiece
	drawEndPiece(endPiece)
	
	###################################################################################
	# Part 8 # interlocking
	if showText:
		text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 3), 20)
	line = Polygon(part, (start[0], start[1]))
	
	line.drawToRelative( (sideLength+grooveWidth, 0) )
	#line.drawToRelative( (0, doubleGroove) )
	#line.drawToRelative( (grooveWidth, 0) )
	
	# Intendtation
	line.drawToRelative( (indentationOffset, (indentationDepth) ) )
	line.drawToRelative( (sideLength - indentationOffset, 0) )
	line.drawToRelative( (0, -(indentationDepth - doubleGroove) ) )
	
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -doubleGroove) )
			
	line.drawToRelative( (sideLength + grooveWidth, 0) )
	##line.drawToRelative( (0, doubleGroove) )
	#line.drawToRelative( (grooveWidth, 0) )
	#line.drawToRelative( (0, -doubleGroove) )
		
	line.drawToRelative( (sideLength, 0 ) )
	
	endPiece = Polygon(part, (line.end[0] + spacing, line.end[1]))
	
	# end
	drawEndInterlock(line)

	line.drawToRelative( (-sideLength, 0) )
	
	drawGroovePattern(line, 3, True, True)
	
	# start
	drawStartInterlock(line)
	
	#line.drawTo(start)
	
	start = (line.end[0], line.end[1] + spacing + height)
	partNo += 1
	
	###################################################################################
	# Endpiece
	drawEndPiece(endPiece)
	
	###################################################################################
	# Part 9
	if showText:
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
	
	start = (line.end[0], line.end[1] + spacing + height)
	partNo += 1
	
	###################################################################################
	# Part 10
	if showText:
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
	
	endPiece = Polygon(part, (line.end[0] + spacing, line.end[1]))
	
	line.drawToRelative( (0, height) )
	
	for i in range(3):
		line.drawToRelative( (-sideLength, 0) )
		line.drawToRelative( (0, -doubleGroove) )
		line.drawToRelative( (-grooveWidth, 0) )
		line.drawToRelative( (0, doubleGroove) )
	
	line.drawToRelative( (-sideLength, 0 ) )
	line.drawTo(start)
	
	start = (line.end[0], line.end[1] + spacing + height)
	partNo += 1
	
	
	###################################################################################
	# Endpiece
	drawEndPiece(endPiece)
	
	###################################################################################
	# Part 11
	if showText:
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
	
	start = (line.end[0], line.end[1] + spacing + height)
	partNo += 1
	
	
	###################################################################################
	# Part 12
	if showText:
		text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 3), 20)
	line = Polygon(part, (start[0],start[1]))
	
	line.drawToRelative( (sideLength + grooveWidth, 0) )
	#line.drawToRelative( (0, doubleGroove) )
	#line.drawToRelative( (grooveWidth, 0) )
	
	# Intendtation
	line.drawToRelative( (indentationOffset, (indentationDepth) ) )
	line.drawToRelative( (sideLength - indentationOffset, 0) )
	line.drawToRelative( (0, -(indentationDepth - doubleGroove) ) )
	
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -doubleGroove) )
			
	line.drawToRelative( (sideLength + grooveWidth, 0) )
	#line.drawToRelative( (0, doubleGroove) )
	#line.drawToRelative( (grooveWidth, 0) )
	#line.drawToRelative( (0, -doubleGroove) )
		
	line.drawToRelative( (sideLength, 0 ) )
	
	endPiece = Polygon(part, (line.end[0] + spacing, line.end[1]))
	
	drawEndInterlock(line)
	
	line.drawToRelative( (-sideLength, 0) )
	drawGroovePattern(line, 3, True, True)
	
	#line.drawToRelative( (-sideLength + interlockGrooveDepth, 0 ) )
	drawStartInterlock(line)
	
	start = (line.end[0], line.end[1] + spacing + height)
	partNo += 1
	
	###################################################################################
	# Endpiece
	drawEndPiece(endPiece)
	
	###################################################################################
	# Part 15
	if showText:
		text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 5), 20)
	line = Polygon(part, (start[0], start[1]))
	
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, height / 2) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -height / 2) )
	
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, grooveDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth+indentationDepth) )

	#Indentation
	line.drawToRelative( (sideLength + grooveWidth, 0) )
	#line.drawToRelative( (0, grooveDepth-indentationDepth) )
	#line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -height / 2) )
	
	line.drawToRelative( ((sideLength), 0) )
	
	drawEndInterlock(line)
	
	line.drawToRelative( (-(sideLength*4 + grooveWidth*3), 0) )
	
	drawStartInterlock(line)
	
	start = (line.end[0], line.end[1] + spacing + height)
	partNo += 1
	
	start = (border + 5 * sideLength + 5 * grooveWidth + 3*interlockGrooveDepth + 2*materialThickness, border)
	###################################################################################
	# Part 16
	if showText:
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
	
	start = (line.end[0], line.end[1] + spacing + height)
	partNo += 1

	###################################################################################
	# Part 17
	if showText:
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
	
	start = (line.end[0], line.end[1] + spacing + height)
	partNo += 1
	###################################################################################
	# Part 18
	if showText:
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
	
	start = (line.end[0], line.end[1] + spacing + height)
	partNo += 1
	
	###################################################################################
	# Part 19
	if showText:
		text = Text(part, ((start[0] + 13), (start[1] + height/2 ) ), "Part: %i" % (partNo + 5), 20)
	line = Polygon(part, (start[0], start[1]))
	
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, height / 2) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -height / 2) )
	
	line.drawToRelative( (sideLength, 0) )
	line.drawToRelative( (0, grooveDepth) )
	line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -grooveDepth+indentationDepth) )

	#Indentation
	line.drawToRelative( (sideLength + grooveWidth, 0) )
	#line.drawToRelative( (0, grooveDepth-indentationDepth) )
	#line.drawToRelative( (grooveWidth, 0) )
	line.drawToRelative( (0, -height / 2) )
	
	line.drawToRelative( ((sideLength), 0) )
	drawEndInterlock(line)
	
	line.drawToRelative( (-(sideLength*4 + grooveWidth*3), 0) )
	drawStartInterlock(line)
	
	start = (line.end[0], line.end[1] + spacing + height)
	partNo += 1
		
	###################################################################################
	# Part 20 # Endpiece
	
	part.writeSVG()
	
	return

if __name__ == '__main__': test()