#! /Users/mlindhol/.brew/bin/python3
import os, sys

# DONE:
# 1. is there argument?
# 2. set test directory from arg

# NOT DONE:
# 3. remove *.cor from testdir and Diff from main dir.
# 4. for each .s in test_dir:
# 	./asm --> hexdump
# 	Print OK//fail
# 	./asm_orig --> hexdump2
# 	Print OK//fail
# 	if both OK then compare DIFFS hexdump vs hexdump2
# 		if DIFF then add to diff_log
# 	Print OK / DIFF FOUND
# 5. finish.

# -----------------------------------------------------

# Check if argument

if (len(sys.argv) is not 2):
	sys.exit("USAGE: python3 mega_asm.py [path]")

# Set test directory and verify existance

test_dir = sys.argv[1] + "/"

if os.path.exists(test_dir):
    print ("Running tester on path: " + test_dir)
else:
	sys.exit("Not a valid path")

# Remove .cor files from test_dir

for file in os.listdir(test_dir):
	if file.endswith(".cor"):
		os.remove(os.path.join(test_dir, file))

# Remove existing log