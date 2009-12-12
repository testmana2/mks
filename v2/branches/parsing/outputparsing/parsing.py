import re
import copy
import sys

class Pattern:
	"""Regular expression pattern. Contains output format info and unit tests for self
		Allowed types are 'error', 'warning', 'compiling'
	"""
	def __init__(self, pattern, file = '', line = '-1', column = '-1', type = 'error', text = '%0', hint = '%0'):
		self.pattern = pattern
		self.file = file
		self.line = line
		self.column = column
		self.type = type
		self.text = text
		self.hint = hint
	
	def setComment(self, comment):
		self.comment = comment
	
	def processTemplate(self, template, inputString, vars):
		"""Replace %x with it's value
		Used for unit tests
		"""
		string = copy.copy(template)
		string = string.replace('%0', inputString[:-1]) # input string without \n
		for i, val in enumerate(vars):
			string = string.replace('%%%d' % (i + 1), val)
		return string
	
	
	def parse(self, string):
		"""Parse the string and return tuple (file, line, column, type, text, hint)
		Used for unit tests
		"""
		res = re.search(self.pattern, string)
		
		if res is None:
			print 'Failed pattern %s, text <%s>\n' %(self.comment, string)
			assert(None)
		
		if '--debug' in sys.argv:
			print 'For <%s>' % self.comment
			print 'Full match: <', res.string[res.start():res.end()], '>'
			for i, peace in enumerate(res.groups()):
				print 'match %d: <%s>' % (i + 1, peace)
		file = self.processTemplate(self.file, string, res.groups())
		line = self.processTemplate(self.line, string, res.groups())
		column = self.processTemplate(self.column, string, res.groups())
		type = self.type
		text = self.processTemplate(self.text, string, res.groups())
		hint = self.processTemplate(self.hint, string, res.groups())
		
		return (file, line, column, type, text, hint)
	
	def test(self, string, file = '', line = '-1', column = '-1', type = 'error', text = '', hint = ''):
		"""Do unit test
		"""
		res = self.parse(string)
		
		failed = False
		
		if res[0] != file:
			print >> sys.stderr, 'file <%s> != <%s>' % (res[0], file)
			failed = True
		if res[1] != line:
			print >> sys.stderr, 'line <%s> != <%s>' % (res[1], line)
			failed = True
		if res[2] != column:
			print >> sys.stderr, 'column <%s> != <%s>' % (res[2], column)
			failed = True
		if res[3] != type:
			print >> sys.stderr, 'type <%s> != <%s>' % (res[3], type)
			failed = True
		if res[4] != text:
			print >> sys.stderr, 'text <%s> != <%s>' % (res[4], text)
			failed = True
		if res[5] != hint:
			print >> sys.stderr, 'hint <%s> != <%s>' % (res[5], hint)
			failed = True
		assert(not failed)
	
	def generateMkSScript(self, parserName):
		template = 	"""# %s\nparser add "%s" "%s" "%s" "%s" "%s" "%s" "%s" "%s"
		""" 		
		arguments = (self.comment, parserName, self.pattern, self.file, self.column, self.line, self.type, self.text, self.hint)
		#arguments = [arg.replace('\\', '\\\\') for arg in arguments]
		arguments = [arg.replace('"', '\\"') for arg in arguments]
		result =  template % tuple(arguments)
		return result