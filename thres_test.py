import time, os, copy, sys, random,threading
import obj_detect

GOOD_DIR = "/home/chris/Desktop/Share/LogoDetection/images/subway/good/"
ERROR_DIR = "/home/chris/Desktop/Share/LogoDetection/images/subway/error/"

DIR_NAME = GOOD_DIR

if __name__ == '__main__':
	log_fd = open(DIR_NAME.split("/")[-2] + "_log.txt", 'w')
	for root, dirs, files in os.walk(DIR_NAME):
		for file_name in files:
			print "---------------------" + file_name
			res = obj_detect.get_result("subway", DIR_NAME + file_name, 0).split('/')
			log_fd.write(file_name + "\t" + res[-2] + "\t" + res[-1] + '\n')
	log_fd.close()
	print "done"
