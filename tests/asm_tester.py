#! /Users/mlindhol/.brew/bin/python3
import os, subprocess, sys

# DONE:
# 1. is there argument?
# 2. set test directory from arg
# 3. remove *.cor from testdir and Diff from main dir.

# NOT DONE:
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

# Inside main loop, checks if .cor file has been made out of .s file
def check_if_cor(file, test_dir, caller):

	f = os.path.splitext(file)[0]+ ".cor"
	if os.path.exists(os.path.join(test_dir, f)):
		print (green + caller + " OK" + endc)
		return ("OK")
	else:
		print (red + caller + " KO" + endc)
		return ("KO")

# Check if argument
if (len(sys.argv) is not 2):
	sys.exit("USAGE: python3 asm_tester.py [path]")

# Set Colors and vars
asm_path = "../asm"
asm_orig_path = "./asm_orig "

blue = '\033[94m'
endc = '\033[0m'
green = '\033[92m'
red = '\033[91m'
yellow = '\033[93m'

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
if os.path.exists("logs/asm_diff_log"):
	print ("Removing asm_diff_log...")
	os.remove("logs/asm_diff_log")
	print ("asm_diff_log removed.")
else:
	print ("No logs found.")

# Create new log file with placeholder text
print ("Creating asm_diff_log...")
diff_log = open(os.path.join("logs/", "asm_diff_log"), "a")
diff_log.write("asm_diff_log init...ready to rock!")

# Loop thru all .s files in test dir
for file in os.listdir(test_dir):
	if file.endswith(".s"):
		print (yellow + "Testing: " + file + endc)
		orig = subprocess.run(['./asm_orig', test_dir + file], stdout=subprocess.PIPE).stdout.decode('utf-8')
		result_orig = check_if_cor(file, test_dir, "orig")


		# own = subprocess.run(['../asm', test_dir + file], stdout=subprocess.PIPE).stdout.decode('utf-8')
		# run asm_orig > hexdump_orig
		# if error running FAIL otherwise OK
		# run ../asm > hexdump
		# if error running FAIL otherwise OK
		# if both OK, then diff hexdump and hexdump_orig
		#
		#
		# subprocess.run(['diff', 'hexdump', 'hexdump_orig'])
		#
		#
		#
		#

if os.path.exists(asm_path):
	print (asm_path)









# Close log file at the end

diff_log.close()

# Remove .cor files

# for file in os.listdir(test_dir):
# 	if file.endswith(".cor"):
# 		os.remove(os.path.join(test_dir, file))