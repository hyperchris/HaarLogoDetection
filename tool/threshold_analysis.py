'''
 Create: 	05/28/2015
 Modified:	05/29/2015
 Author: 	Xiaochen Liu
 
 Function: 	The code is used to automatically run the obj_detect.get_result to generate the detection log. It can also analyze the log file by plotting the histogram of round #.
 
 Usage: 	Put this file in the same dir with "obj_detect.py" for running. Read the description of each function to decide which you should use
'''

import time, os, copy, sys, random,threading
import obj_detect	# obj_detect
import numpy as np
import pylab as pl

# dir that stores images
GOOD_DIR = "/home/chris/Desktop/Share/LogoDetection/images/subway/good/"
ERROR_DIR = "/home/chris/Desktop/Share/LogoDetection/images/subway/error/"

# path where log file is stored
GOOD_LOG = "/home/chris/Desktop/Share/LogoDetection/log/m20/good_log_m_20m.txt"
ERROR_LOG = "/home/chris/Desktop/Share/LogoDetection/log/m20/error_log_m_20m.txt"

SHOW_IMAGE = 0  # 1 means show the detection result for each image
				
MAX_ROUND = 27	# round number larger than this will be recorded as MAX_ROUND
MAX_X = 29		# max value of x-axis

DATA_SIZE = 69	# size of good/error log dataset

# generate log with min_neighbor and round_num, return the list of round_num
def generate_log (logo_name, image_dir, show_image, max_round):
	log_fd = open(image_dir.split("/")[-2] + "_log.txt", 'w')
	round_log = []
	for root, dirs, files in os.walk(image_dir): # walk the dir
		for file_name in files:
			print "---------------------" + file_name
			res = obj_detect.get_result(logo_name, image_dir + file_name, show_image).split('/')
			log_fd.write(file_name + "\t" + res[-2] + "\t" + res[-1] + '\n') # image name, min_neighbor, round number
			round_num = int(res[-1])
			if round_num > max_round:
				round_num = max_round
			round_log.append(round_num)
	log_fd.close()
	return round_log

# plot the histogram of a log file (with two lists)
def plot_histogram (title, x_label, y_label, data_1, data_2, label_1, label_2, color_1, color_2, transparency, min_x, max_x, step):
	bins = np.arange(min_x, max_x, step)
	pl.hist(data_1, bins, histtype='stepfilled', alpha=transparency, label=label_1, color=color_1)
	pl.hist(data_2, bins, histtype='stepfilled', alpha=transparency, label=label_2, color=color_2)
	# make plot labels
	pl.title(title)
	pl.xlabel(x_label)
	pl.ylabel(y_label)
	pl.legend(loc='upper right')
	pl.show()
	return

# calculate the False Positive Rate (FPR) and False Negative Rate (FNR)
def cal_rate (good_log_path, error_log_path, thres_start, thres_end):
	good_data = []
	error_data = []

	# read file
	line = ''
	file_reader = open(good_log_path, 'r') # read good log
	while True:
		line = file_reader.readline()
		if len(line) < 2:
			break
		good_data.append(int(line.split('\t')[-1].split('\n')[0]))
	file_reader.close()
	file_reader = open(error_log_path, 'r') # read error log
	while True:
		line = file_reader.readline()
		if len(line) < 2:
			break
		error_data.append(int(line.split('\t')[-1].split('\n')[0]))
	file_reader.close()

	# test the size of dataset
	if len(good_data) != DATA_SIZE or len(error_data) != DATA_SIZE:
		print "[ERROR] Data size error!"
		print "good data size: " + str(len(good_data)) + "----------------"
		print good_data
		print "error_data size: " + str(len(error_data)) + "---------------"
		print error_data
		return 

	# calculate FPR and FNR
	print "Threshold\tFPR\t\tFNR"
	for thres in range(thres_start, thres_end):
		FP_num = 0
		FN_num = 0
		for i in range(0, DATA_SIZE):
			if good_data[i] < thres:
				FN_num += 1
			if error_data[i] >= thres:
				FP_num += 1
		fpr = 100.0 * float(FP_num)/float(DATA_SIZE)
		fnr = 100.0 * float(FN_num)/float(DATA_SIZE)
		print str(thres) + "\t\t" + str(fpr - fpr % 0.1) + "\t\t" + str(fnr - fnr % 0.1)

	return 

if __name__ == '__main__':
	# generate logs
	# good_round = generate_log("subway", GOOD_DIR, SHOW_IMAGE, MAX_ROUND)
	# error_round = generate_log("subway", ERROR_DIR, SHOW_IMAGE, MAX_ROUND)

	# show histogram 
	# plot_histogram("Distribution of Round Number", "Round number", "# of images", good_round, error_round, "good", "error", "green", "red", 0.5, 0, MAX_X, 1)

	# calculate FNR and FPR
	cal_rate(GOOD_LOG, ERROR_LOG, 1, 30)

	print "done"
