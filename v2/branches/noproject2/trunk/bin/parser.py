#!/usr/bin/python
import time
from sys import stdout, stdin
import re
print "parser started"
errPatern = r"^(.+):(\d+):(\d+:)?\s((warning)|(error)):\s(.+)$"
errExp = re.compile (errPatern)
while 1:
	s = raw_input()
	if errExp.match
	stdout.flush() 