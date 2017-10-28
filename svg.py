import os


class SVG():
	def __init__(self, name = 'dinA4-svg', width = '210mm', height = '297mm'):
		self.name = name
		self.items = []
		self.height = height
		self.width = width
		
		
	def strarray(self):
		var = ['<meta http-equiv="refresh" content="2"><?xml version="1.0"?>\n',
			   '<svg height="%s" width="%s" >\n' % (self.height,self.width),
			   ' <g style="fill-opacity:1.0; stroke:black;\n',
			   '  stroke-width:0.1mm;">\n']
		for item in self.items: 
			var += item	
		var += ['</g>\n</svg>\n']
		return var

	def add(self, item): 
		self.items.append(item)


	def writeSVG(self, filename=None):
		if filename:
			self.svgname = filename
		else:
			self.svgname = self.name + '.html'
		file = open(self.svgname,'w')
		file.writelines(self.strarray())
		file.close()
		return



class Polygon():
	def __init__(self, svg, start, rotate = '0 0 0'):
		self.start = start
		self.svg = svg
		self.end = 0
		self.rotate = rotate
		
	def drawTo(self, end):
		self.end = end
		self.svg.add('\t<line x1="%dmm" y1="%dmm" x2="%dmm" y2="%dmm" transform="rotate(%s)" />\n' % (self.start[0], self.start[1], end[0], end[1], self.rotate))
		self.start = end
		
	def drawToRelative(self, offset):
		self.end = ((self.start[0] + offset[0]), (self.start[1]+ offset[1]))
		self.svg.add('\t<line x1="%dmm" y1="%dmm" x2="%dmm" y2="%dmm" transform="rotate(%s)" />\n' % (self.start[0], self.start[1], (self.start[0] + offset[0]), (self.start[1]+ offset[1]), self.rotate))
		newStart = ((self.start[0] + offset[0]), (self.start[1]+ offset[1]))
		self.start = newStart
			
			
			
class Text:
	def __init__(self, svg, origin, text, size=24):
		self.origin = origin
		self.text = text
		self.size = size
		svg.add('\t<text x="%dmm" y="%dmm" font-size="%d">\n %s\n\t</text>\n' % (self.origin[0], self.origin[1], self.size, self.text))

		