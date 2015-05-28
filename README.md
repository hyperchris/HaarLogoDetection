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

##Main function  
Given image path and logo name, the "get_result" function in "obj_detect.py" will return the x-coordinate of the two boudaries of detected logo and the image width.

##How to use
Put "obj_detect.py" in the same path with the "support" folder. Then import the python file and call "get_result" function.

**Sample:** In python, use:  
	* res = get_result("walmart", "images/walmart/walmart.jpg", 1)

- Parameter:  
	- "walmart" is the name of logo that you want to detect
	- "images/walmart/walmart.jpg" is the path of the image to process
	- 1 means show the detection result; 0 means do not show the result (Displaying the result will block the get_result function and need PRESS_ANY_KEY to continue) 

- The returned string will be stored in "res":
	- If the program returns correctly, "res" will be like "logo_left/logo_right/image_width/conf_score".
	- If something goes wrong, "res" will be "ERROR"

##Next step
An confidence score will be given to help eliminating false positives.