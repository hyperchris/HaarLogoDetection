# Logo Detection
Version: 2.0

## Requirements
**OS:** Linux (Ubuntu 14.04 is recommended)     
- OpenCV2
- Python2

## Classifier Training
Follow [the link](http://abhishek4273.com/2014/03/16/traincascade-and-car-detection-using-opencv/) to get multiple Haar classifier cascades. 

## Code Layout
- **obj_detect.py:** The function that you will use
- **support/** The cpp code that supports the function of "obj_detect.py"
- **cascade/** Trained classifiers. Include AT&T, Chase, Macy's, Mobil, Subway, Walmart, and banana (Taco Bell and Starbucks will come soon...).
- **history/** Previous version of logo detection code
- **images/** Images w/ or w/o logos
- **tool/** Includes a script that can help you find the best MARGIN and THRESHOLD to have acceptable FPR and FNR. There is also an analysis result text file generated by me.
	- `log/` contains detection analysis log with the margin from 0 to 25
	- `plot_round_num/` has the images that show the distribution of round number with different margins

## Main function  
Given image path and logo name, the "get_result" function in "obj_detect.py" will return the x-coordinate of the detected logo.

## How to use
Put "obj_detect.py" in the same path with the "support" folder. Then import "obj_detect.py" into your code and call "get_result" function.

**Sample:** In python, use:  
	- $ res = get_result("walmart", "images/walmart/walmart.jpg", 1)

- Input parameters:  
	- "walmart" is the name of logo that you want to detect
	- "images/walmart/walmart.jpg" is the path of the image to process
	- 1 means show the detection result; 0 means do not show the result (Displaying the result will block the get_result function and need PRESS_ANY_KEY to continue) 

- Returned string:
	- If the program returns correctly, "res" will be in the format like **"logo_left / logo_right / image_width / min_neighbor / conf_score"**.
	- For "logo_left" and "logo_right", -1 means more than one object found; 0 means no object;
	- If something goes wrong, "res" will be "ERROR"

## Define the confidence score
In object detection, there is a score called "MIN_NEIGHBOR" which represents the similarity between the detected area and sample object. Moreover, I applied recursive image cropping to get the number of "cropping round", which can also be used to know if there is a real logo or not. In most cases, real logo will have higher round number while the false positive has less.  

**Threshold of conf_score** To remove false positive cases, we need to ignore the results with confidence scores smaller than a certain threshold. The relation between the threshold, FPR, and FNR is partially listed below. You can select a threshold according to your application scenario.
* Threshold / FPR(%) / FNR(%)
* 10 / 15.9 / 13.0
* 11 / 15.9 / 14.4
* 12 / 15.9 / 14.4
* 13 / 14.4 / 14.4
* 14 / 13.0 / 14.4
* 15 / 11.5 / 15.9
* 16 / 11.5 / 15.9
* 17 / 10.1 / 18.8
* 18 / 8.6 / 18.8
* 19 / 8.6 / 20.2
* 20 / 5.7 / 21.7
(The data is tested among 64 images in 2015 May, new result will be updated shortly)

## Update
- Chase cascade updated (with the trainining set of streetview as background)

## Citation
Please cite this paper in your publications if it helps your research 

	@inproceedings{Hu:2016:AAL:2971648.2971674,
	 author = {Hu, Yitao and Liu, Xiaochen and Nath, Suman and Govindan, Ramesh},
	 title = {ALPS: Accurate Landmark Positioning at City Scales},
	 booktitle = {Proceedings of the 2016 ACM International Joint Conference on Pervasive and Ubiquitous Computing},
	 series = {UbiComp '16},
	 year = {2016},
	 isbn = {978-1-4503-4461-6},
	 location = {Heidelberg, Germany},
	 pages = {1147--1158},
	 numpages = {12},
	 url = {http://doi.acm.org/10.1145/2971648.2971674},
	 doi = {10.1145/2971648.2971674},
	 acmid = {2971674},
	 publisher = {ACM},
	 address = {New York, NY, USA},
	 keywords = {context-aware computing, landmark localization system, machine/deep learning},
	} 

