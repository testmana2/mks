# coding=utf8

import parsing
from string import Template

# No rule for make target

unix_file = "[\w\d\./]+"
number = "\d+"

regEx = Template("^($unix_file):($number): error: (.+)")

error =parsing.Pattern(regEx.substitute(unix_file = unix_file, number = number),
								   type = 'error',
								   file = "%1",
								   line = "%2")

error.setComment('Error (usualy syntax)')

error.test("src/main.cpp:20: error: expected initializer before 's'",
				type = 'error',
				file = 'src/main.cpp',
				line = '20',
				text = "src/main.cpp:20: error: expected initializer before 's'",
				hint = "src/main.cpp:20: error: expected initializer before 's'")

print '# It is a machine generated file. Do not edit it manualy!\n'

print error.generateMkSScript('GCC')
