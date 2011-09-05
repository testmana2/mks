import parsing

# parser add <name> <regular expression> <file name> <column> <row> <pattern type> <pattern text> <full text>
# def __init__(self, pattern, file = '', line = '-1', column = '-1', type = 'error', text = '%0', hint = '%0'):

# WARNING: Failure to find
missingFile = parsing.Pattern( r"^WARNING:\s+(Failure to find:\s+([^\n]+))", file = '%2', type = 'error', text = '%1' )
missingFile.setComment( 'WARNING: Failure to find file' )

missingFile.test( 'WARNING: Failure to find: file.ext',
					file = 'file.ext',
					type = 'error',
					text = 'Failure to find: file.ext',
					hint = 'WARNING: Failure to find: file.ext' )

# Project MESSAGE
message = parsing.Pattern( r"^Project MESSAGE:\s+([^\n]+)", type = 'warning', text = '%1' )
message.setComment( 'Project MESSAGE' )

message.test( 'Project MESSAGE: this is my qmake project message',
					type = 'warning',
					text = 'this is my qmake project message',
					hint = 'Project MESSAGE: this is my qmake project message' )

# Reading
reading = parsing.Pattern( r"^\s*Reading\s+([^\n]+)", file = '%1', type = 'warning' )
reading.setComment( 'Reading' )

reading.test( 'Reading /home/pasnox/Temporaire/qtsol/sub1/sub1.pro',
				file = '/home/pasnox/Temporaire/qtsol/sub1/sub1.pro',
				type = 'warning',
				text = 'Reading /home/pasnox/Temporaire/qtsol/sub1/sub1.pro',
				hint = 'Reading /home/pasnox/Temporaire/qtsol/sub1/sub1.pro' )

# Warning: potential duplicate alias detected
duplicateAlias = parsing.Pattern( r"^([^\n]+):\s+Warning:\s+(potential duplicate alias detected:\s+'[^\n]+')", file = '%1', type = 'warning', text = '%2' )
duplicateAlias.setComment( 'Warning: potential duplicate alias detected' )

duplicateAlias.test( 'src/resources/resources.qrc: Warning: potential duplicate alias detected: \'license.gpl\'',
						file = 'src/resources/resources.qrc',
						type = 'warning',
						text = 'potential duplicate alias detected: \'license.gpl\'',
						hint = 'src/resources/resources.qrc: Warning: potential duplicate alias detected: \'license.gpl\'' )

# Generation of script file
print '# It is a machine generated file. Do not edit it manualy!'
print ''
print missingFile.generateMkSScript( 'QMake' )
print message.generateMkSScript( 'QMake' )
print reading.generateMkSScript( 'QMake' )
print duplicateAlias.generateMkSScript( 'QMake' )
