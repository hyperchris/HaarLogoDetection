import time, os, copy, sys, random,threading
import obj_detect
import numpy as np
import pylab as pl

GOOD_DIR = "/home/chris/Desktop/Share/LogoDetection/images/subway/good/"
ERROR_DIR = "/home/chris/Desktop/Share/LogoDetection/images/subway/error/"

SHOW_IMAGE = 0

if __name__ == '__main__':
	good_thres = []
	error_thres = []

	good_round = []
	error_round = []
	
	DIR_NAME = GOOD_DIR
	log_fd = open(DIR_NAME.split("/")[-2] + "_log.txt", 'w')
	for root, dirs, files in os.walk(DIR_NAME):
		for file_name in files:
			print "---------------------" + file_name
			res = obj_detect.get_result("subway", DIR_NAME + file_name, SHOW_IMAGE).split('/')
			# log_fd.write(file_name + "\t" + res[-2] + "\t" + res[-1] + '\n')
			good_thres.append(int(res[-2]))
			good_round.append(int(res[-1]))
	log_fd.close()

	DIR_NAME = ERROR_DIR
	log_fd = open(DIR_NAME.split("/")[-2] + "_log.txt", 'w')
	for root, dirs, files in os.walk(DIR_NAME):
		for file_name in files:
			print "---------------------" + file_name
			res = obj_detect.get_result("subway", DIR_NAME + file_name, SHOW_IMAGE).split('/')
			# log_fd.write(file_name + "\t" + res[-2] + "\t" + res[-1] + '\n')
			error_thres.append(int(res[-2]))
			error_round.append(int(res[-1]))
	log_fd.close()
	
	''' Plot the distribution of Threshold '''
	bins = np.arange(0, 20, 2)
	pl.hist(good_thres, bins, histtype='stepfilled', alpha=0.6, label='good')
	pl.hist(error_thres, bins, histtype='stepfilled', alpha=0.6, label='error')
	# make plot labels
	pl.title('Distribution of Threshold')
	pl.xlabel('range')
	pl.ylabel('# of images')
	pl.legend(loc='upper right')
	pl.show()

	''' Plot the distribution of Round # '''
	bins = np.arange(0, 20, 2)
	pl.hist(good_round, bins, histtype='stepfilled', alpha=0.6, label='good')
	pl.hist(error_round, bins, histtype='stepfilled', alpha=0.6, label='error')
	# make plot labels
	pl.title('Distribution of Round #')
	pl.xlabel('range')
	pl.ylabel('# of images')
	pl.legend(loc='upper right')
	pl.show()

	print "done"
