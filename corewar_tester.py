#! /Users/sadawi/.brew/bin/python3

import sys, os
import subprocess
import random

blue = '\033[94m'
endc = '\033[0m'
green = '\033[92m'
red = '\033[91m'
yellow = '\033[93m'

if (len(sys.argv) != 2):
	print("Usage: ./corewar_tester.py [FOLDER]")
	sys.exit()

cor_files = []
for file in os.listdir(sys.argv[1]):
	cor_files.append(sys.argv[1] + "/" + str(file))

for i in range(100):
	args = []
	for j in range(random.randint(1, 4)):
		args.append(random.choice(cor_files))
	print("Testing with players {}".format(args))
	try:
		result = subprocess.run(['./diff_finder.py', *args], stdout=subprocess.PIPE, timeout=5).stdout.decode('utf-8')
	except subprocess.TimeoutExpired:
		print(green + "Success! No diff found before timeout." + endc)
		continue
	if "No differences found.\n" in result:
		print(green + "Success!" + endc)
	else:
		print(red + "Diff found!")
		print(result + endc)

