'''
 Create: 	04/09/2015
 Modified:	05/27/2015
 Author: 	Xiaochen Liu
 
 Function: 	Given image path and logo name, the "get_result" will return 
 		the x-coordinate of the two boudaries of detected logo and 
 		the image width
 
 Usage: 	Put this python file in the same path with the "support" folder. 
 		Then import the python file and call "get_result" function 
'''

import time, os, copy, sys, random,threading

RESULT_HEADER = "RESULT: "
ERROR_MSG = "ERROR"

def get_result(logo_name, image_path):
	cmd = "./support/obj_detect --cascade=cascade/" + logo_name + ".xml " + image_path
	ret_val = os.popen(cmd).read().split('\n') # system call
	
	found = False
	for i in range(len(ret_val)):
		print ret_val[i]
		if ret_val[i].startswith(RESULT_HEADER):
			found = True
			return ret_val[i].split(RESULT_HEADER)[-1]
			break
	
	if found == False:
		return ERROR_MSG
	
if __name__ == '__main__':
	res = get_result("subway", "images/subway/good/1.jpg")
	print "done"
