#! /Users/mlindhol/.brew/bin/python3
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

def create_file(cor, file_nbr):

	hexdump_file = "hexdump_" + file_nbr + ".txt"
	print ("run hexdump")
	hex_dump = subprocess.run(['python3', '-m', 'hexdump', cor], stdout=subprocess.PIPE).stdout.decode('utf-8')
	print (type(hex_dump))
	with open(hexdump_file, 'a') as h_file:
	    h_file.write(hex_dump)


def remove_hexdump_files():

	if os.path.exists("hexdump_1.txt"):
		os.remove("hexdump_1.txt")
	if os.path.exists("hexdump_2.txt"):
		os.remove("hexdump_2.txt")

def run_diff(orig, own):

	print (blue + "Run diff" + endc)
	diff = subprocess.run(['diff', 'hexdump_1.txt', 'hexdump_2.txt'], stdout=subprocess.PIPE).stdout.decode('utf-8')
	print ("Diff is: " + diff)
	print ("Creating asm_diff_log...")
	diff_log = open(os.path.join("logs/", "asm_diff_log"), "a")
	if not diff:
		diff_log.write("placeholder text, no diff")
	else:
		diff_log.write(diff)
	diff_log.close

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
    print (blue + "Running tester on path: " + test_dir + endc)
else:
	sys.exit(red + "Not a valid path" + endc)

# Remove .cor files from test_dir
for file in os.listdir(test_dir):
	if file.endswith(".cor"):
		os.remove(os.path.join(test_dir, file))

# Remove existing log
if os.path.exists("logs/asm_diff_log"):
	print ("Removing asm_diff_log...")
	os.remove("logs/asm_diff_log")
	print ("asm_diff_log removed.")
	os.rmdir("logs")
	print ("logs/ removed")
elif os.path.isdir("logs/"):
	os.rmdir("logs")
	print ("logs/ removed")
else:
	print ("No logs found.")

print ("creating logs/ ...")
os.mkdir("logs")

# Loop thru all .s files in test dir
for file in os.listdir(test_dir):
	if file.endswith(".s"):
		print (yellow + "\nTesting: " + file + endc)
		cor = os.path.join(test_dir, os.path.splitext(file)[0]+ ".cor")

		# original asm
		orig = subprocess.run(['./asm_orig', test_dir + file], stdout=subprocess.PIPE).stdout.decode('utf-8')
		result_orig = check_if_cor("orig", cor)
		if (result_orig is "OK"):
			create_file(cor, "1")

		if os.path.exists(cor):
			os.remove(cor)

		# our asm
		own = subprocess.run(['../asm', test_dir + file], stdout=subprocess.PIPE).stdout.decode('utf-8')
		result_own = check_if_cor("own", cor)
		if (result_own is "OK"):
			create_file(cor, "2")

		if (result_orig is "OK" and result_own is "OK"):
			run_diff(orig, own)
		elif (result_orig is "OK" and result_own is "KO" or
			  result_orig is "KO" and result_own is "OK"):
			print ("One OK the other KO")
		else:
			print (blue + "No diff, all gud." + endc)

		# remove hexdump files
		remove_hexdump_files()

		# Create new log file with placeholder text

		#print(binascii.hexlify("batman"))
# if os.path.exists(asm_path):
# 	print (asm_path)









# Close log file at the end


# Remove .cor files

# for file in os.listdir(test_dir):
# 	if file.endswith(".cor"):
# 		os.remove(os.path.join(test_dir, file))