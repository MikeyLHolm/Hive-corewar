#! /Users/sadawi/.brew/bin/python3

import sys, re
import subprocess

if (len(sys.argv) < 3):
	print("Usage: ./diff.py [PLAYER 1] [PLAYER 2] [DUMP CYCLE]")
	sys.exit()

result = subprocess.run(['./corewar', '-dump', sys.argv[3], sys.argv[1], sys.argv[2]], stdout=subprocess.PIPE).stdout.decode('utf-8')
result2 = subprocess.run(['./vm_champs/corewar', '-d', sys.argv[3], sys.argv[1], sys.argv[2]], stdout=subprocess.PIPE).stdout.decode('utf-8')

splitA = set(result.split("\n"))
splitB = set(result2.split("\n"))

diff = splitB.symmetric_difference(splitA)
diff = "\n".join(diff)

if not diff:
	print("No differences found.")
else:
	print(diff)
