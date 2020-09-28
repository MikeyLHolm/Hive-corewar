#! /Users/sadawi/.brew/bin/python3

import sys, re, os
import subprocess

if (len(sys.argv) < 2):
	print("Usage: ./diff.py [PLAYER 1] [PLAYER 2] [PLAYER 3] [PLAYER 4]")
	sys.exit()

diff = ""
i = 1
sys.argv.pop(0)
while not diff:
	result = subprocess.run(['./corewar', '-dump', str(i), *sys.argv], stdout=subprocess.PIPE, stderr=subprocess.PIPE).stdout.decode('utf-8')
	result2 = subprocess.run(['./vm_champs/corewar', '-d', str(i), *sys.argv], stdout=subprocess.PIPE, stderr=subprocess.PIPE).stdout.decode('utf-8')

	splitA = set(result.split("\n"))
	splitB = set(result2.split("\n"))

	diff = splitB.symmetric_difference(splitA)
	diff = "\n".join(diff)
	if diff:
		break
	if (len(splitA) == len(splitB) and len(splitA) < 60):
		break
	i += 1000
	os.system('clear')
	print("Checking cycle {}...".format(i))
if not diff or "Error" in diff:
	print("No differences found.")
	sys.exit()
i -= 1000
diff = ""
while not diff:
	result = subprocess.run(['./corewar', '-dump', str(i), *sys.argv], stdout=subprocess.PIPE).stdout.decode('utf-8')
	result2 = subprocess.run(['./vm_champs/corewar', '-d', str(i), *sys.argv], stdout=subprocess.PIPE).stdout.decode('utf-8')

	splitA = set(result.split("\n"))
	splitB = set(result2.split("\n"))

	diff = splitB.symmetric_difference(splitA)
	diff = "\n".join(diff)
	if diff:
		break
	i += 100
	os.system('clear')
	print("Checking cycle {}...".format(i))
i -= 100
diff = ""
while not diff:
	result = subprocess.run(['./corewar', '-dump', str(i), *sys.argv], stdout=subprocess.PIPE).stdout.decode('utf-8')
	result2 = subprocess.run(['./vm_champs/corewar', '-d', str(i), *sys.argv], stdout=subprocess.PIPE).stdout.decode('utf-8')

	splitA = set(result.split("\n"))
	splitB = set(result2.split("\n"))

	diff = splitB.symmetric_difference(splitA)
	diff = "\n".join(diff)
	if diff:
		break
	i += 10
	os.system('clear')
	print("Checking cycle {}...".format(i))
i -= 10
diff = ""
while not diff:
	result = subprocess.run(['./corewar', '-dump', str(i), *sys.argv], stdout=subprocess.PIPE).stdout.decode('utf-8')
	result2 = subprocess.run(['./vm_champs/corewar', '-d', str(i), *sys.argv], stdout=subprocess.PIPE).stdout.decode('utf-8')

	splitA = set(result.split("\n"))
	splitB = set(result2.split("\n"))

	diff = splitB.symmetric_difference(splitA)
	diff = "\n".join(diff)
	i += 1
	os.system('clear')
	print("Checking cycle {}...".format(i))
print("Difference found in cycle {}\n".format(i))
print(diff)
