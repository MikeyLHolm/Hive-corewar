#!/usr/bin/env python3
import os, subprocess, sys
import hexdump

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
def check_if_cor(caller, cor):

	if os.path.exists(cor):
		print (green + caller + " OK" + endc)
		return ("OK")
	else:
		print (red + caller + " KO" + endc)
		return ("KO")

def create_hexdump_file(cor, file_nbr):

	hexdump_file = "hexdump_" + file_nbr + ".txt"
	hex_dump = subprocess.run(['python3', '-m', 'hexdump', cor], stdout=subprocess.PIPE).stdout.decode('utf-8')
	with open(hexdump_file, 'a') as h_file:
	    h_file.write(hex_dump)

def one_ko(file, msg):
	print (red + msg + endc)
	if not os.path.exists("logs/asm_diff_log"):
		print ("creating asm_diff_log...")
	diff_log = open(os.path.join("logs/", "asm_diff_log"), "a")
	diff_log.write("diff in " + file)
	diff_log.write(msg)
	diff_log.close

def remove_hexdump_files():

	if os.path.exists("hexdump_1.txt"):
		os.remove("hexdump_1.txt")
	if os.path.exists("hexdump_2.txt"):
		os.remove("hexdump_2.txt")

def run_diff(file):

	diff = subprocess.run(['diff', 'hexdump_1.txt', 'hexdump_2.txt'], stdout=subprocess.PIPE).stdout.decode('utf-8')
	if diff:
		if not os.path.exists("logs/asm_diff_log"):
			print ("creating asm_diff_log...")
		diff_log = open(os.path.join("logs/", "asm_diff_log"), "a")
		diff_log.write("diff in " + file)
		diff_log.write(diff)
		diff_log.close
		print (red + diff + endc)
	else:
		print (blue + "all good, no diff" + endc)

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
    print (blue + "running tester on path: " + test_dir + endc)
else:
	sys.exit(red + "not a valid path" + endc)

# Remove .cor files from test_dir
for file in os.listdir(test_dir):
	if file.endswith(".cor"):
		os.remove(os.path.join(test_dir, file))

# Remove existing log
if os.path.exists("logs/asm_diff_log"):
	print ("removing asm_diff_log...")
	os.remove("logs/asm_diff_log")
	print ("asm_diff_log removed.")
	os.rmdir("logs")
	print ("logs/ removed")
elif os.path.isdir("logs/"):
	os.rmdir("logs")
	print ("logs/ removed")
else:
	print ("no logs found.")

print ("creating logs/ ...")
os.mkdir("logs")

# Loop thru all .s files in test dir
for file in os.listdir(test_dir):
	if file.endswith(".s"):
		print (yellow + "______________________________________" + endc)
		print (yellow + "\ntesting: " + file + endc)
		cor = os.path.join(test_dir, os.path.splitext(file)[0]+ ".cor")

		# original asm
		orig = subprocess.run(['./asm_orig', test_dir + file], stdout=subprocess.PIPE).stdout.decode('utf-8')
		result_orig = check_if_cor("orig", cor)
		if (result_orig is "OK"):
			create_hexdump_file(cor, "1")

		if os.path.exists(cor):
			os.remove(cor)

		# our asm
		own = subprocess.run(['../asm', test_dir + file], stdout=subprocess.PIPE).stdout.decode('utf-8')
		result_own = check_if_cor("own", cor)
		if (result_own is "OK"):
			create_hexdump_file(cor, "2")

		if (result_orig is "OK" and result_own is "OK"):
			run_diff(file)
		elif (result_orig is "OK" and result_own is "KO"):
			one_ko(file, own)
		elif (result_orig is "KO" and result_own is "OK"):
			one_ko(file, orig)
		else:
			print (blue + "no diff, all gud." + endc)

		remove_hexdump_files()

		print (yellow + "______________________________________" + endc)

print (blue + "\n*** asm_tester for " + test_dir + " completed!  ***")
print ("*** check /logs/asm_diff_log for results ***\n" + endc)
