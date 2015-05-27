# Logo Detection
Version: 2.0

##Requirements
**OS:** Linux (Ubuntu 14.04 is recommended) 
**Environment:**  
- OpenCV
- Python

##Code Layout
- **obj_detect.py:** The function that you will use
- **support/** The cpp code that supports the function of "obj_detect.py"
- **cascade/** Trained classifiers. Include AT&T, Chase, Macy's, Mobil, Subway, Walmart, and banana (Taco Bell and Starbucks will come soon...).
- **history/** Previous version of logo detection code
- **show_img/** Visually shows the result of logo detection

##Main function  
Given image path and logo name, the "get_result" function in "obj_detect.py" will return the x-coordinate of the two boudaries of detected logo and the image width.

##How to use
Put "obj_detect.py" in the same path with the "support" folder. Then import the python file and call "get_result" function.

**Sample:** In python, use:  
	res = get_result("subway", "support/test_img/test.jpg")

# "subway" is logo name, the second arg is image's path

# The returned string will be stored in "res":
	* If the program returns correctly, "res" will be like "logo_left/logo_right/image_width".
	* If something goes wrong, "res" will be "ERROR"

##Next step
An confidence score will be given to help eliminating false positives.