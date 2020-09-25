#! /Users/mlindhol/.brew/bin/python3
import os, sys

# 1. is there argument?
# 2. set test directory from arg
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

# Check argument

if (len(sys.argv) < 2):
	sys.exit("USAGE: python3 mega_asm.py [path]")

test_dir = sys.argv[1]

if os.path.exists(test_dir):
    print (os.path.basename(test_dir))
    # file exists
else:
	sys.exit("Not a valid path")



