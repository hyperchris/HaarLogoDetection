/*
 * Name: Object detection
 * Author: Xiaochen
 * Time: 05/27/2015
 * Version: 2.0
 * Function: 
 	1. Use HAAR features and SVM to find a specific obj in a picture.
 	2. [Update] Automatically increase the threshold to decrease the number of detected objects to one
 	3. Return the position of detected object in the image
 * Usage: 
 	1. Compile the cpp file, get the executable file
 	2. Then use python code (in the root dir) to call the function
 *
 */

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cctype>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <string>
#include <stdlib.h>

using namespace std;
using namespace cv;

#define ERROR -1
#define SPLITTER '/'
#define RESULT_HEADER "RESULT: "
#define ERROR_HEADER "ERROR: "
#define MARGIN 20 // this margin works best
#define MAX_SIZE 1440.0
// #define THRES

string cascadeName = "../cascade/subway.xml";

struct coordinate {
	int x_left;
	int x_right;
	int thres;
	int conf;
};

coordinate detectObj(Mat& img, CascadeClassifier& cascade, double scale, int display);
void showImage(Mat& img, vector<Rect> detected_object, double scale, int display);
int calConfScore(Mat& img, CascadeClassifier& cascade, vector<Rect> detected_object);
cv::Rect getCroppingRec (Mat& img, int x, int y, int width, int height);

int main(int argc, const char** argv) {
	const string scaleOpt = "--scale=";
	size_t scaleOptLen = scaleOpt.length();
	const string cascadeOpt = "--cascade=";
	size_t cascadeOptLen = cascadeOpt.length();
	const string displayOpt = "--display=";
	size_t displayOptLen = cascadeOpt.length();

	Mat image;
	string inputName;
	CascadeClassifier cascade;
	double scale = 1;
	int display = 0;

    for( int i = 1; i < argc; i++ ) { // get inputs
		cout << "Processing " << i << " " <<  argv[i] << endl;
		if( cascadeOpt.compare( 0, cascadeOptLen, argv[i], cascadeOptLen ) == 0 ) { // read cascade file
		    	cascadeName.assign( argv[i] + cascadeOptLen );
		    	cout << "  from which we have cascadeName= " << cascadeName << endl;
		}
		else if( scaleOpt.compare( 0, scaleOptLen, argv[i], scaleOptLen ) == 0 ) { // read the scale
			if( !sscanf( argv[i] + scaleOpt.length(), "%lf", &scale ) || scale < 1 )
				scale = 1;
			cout << " from which we read scale = " << scale << endl;
		}
		else if( displayOpt.compare( 0, displayOptLen, argv[i], displayOptLen ) == 0 ) { // read the Display_or_not option
			if( !sscanf( argv[i] + displayOpt.length(), "%d", &display ) || !(display == 0 || display == 1))
				display = 0;
			cout << " from which we read display = " << display << endl;
		}
		else if( argv[i][0] == '-' ) { // wrong input format
		    	cerr << "WARNING: Unknown option %s" << argv[i] << endl;
		}
		else
		    inputName.assign( argv[i] ); // image name 
    	}

	if( !cascade.load( cascadeName ) ) { // handle the error cascade
		cerr << ERROR_HEADER << "Could not load classifier cascade" << endl;
		return -1;
	}

	if( inputName.size() ) { // read in the image
		image = imread( inputName, 1 );
		if( image.empty() ) {
			cout << ERROR_HEADER << "input image is empty!" << endl;
			return -1;
		}
		cout << "SUCCESS: In image read" << endl;
	}
	else {
		cout << ERROR_HEADER << "\n ERROR: Couldn't no input image name" << endl;
		return -1;
	}
	
	coordinate ret_val = detectObj(image, cascade, scale, display); // detect the object
    	
    // print the result
    cout << RESULT_HEADER << ret_val.x_left << SPLITTER << ret_val.x_right << SPLITTER << image.cols << SPLITTER << ret_val.thres << SPLITTER << ret_val.conf << endl; 
    	
    return 0;
}

/* Detect the object */
coordinate detectObj (Mat& img, CascadeClassifier& cascade, double scale, int display) {
	double t = (double)cvGetTickCount(); // count the procesing time
	
	vector<Rect> detected_object; // detected object
	if (img.cols > MAX_SIZE || img.rows > MAX_SIZE) // resize if the image is too big
		scale = (img.cols/MAX_SIZE > img.rows/MAX_SIZE) ? (img.cols/MAX_SIZE) : (img.rows/MAX_SIZE);
	Mat gray, smallImg(cvRound (img.rows / scale), cvRound(img.cols / scale), CV_8UC1);
	
	cvtColor(img, gray, CV_BGR2GRAY);  // change the image to grayscale
	resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR); 
	equalizeHist( smallImg, smallImg );

	int min_neighbors = 1;
	bool over_estimated = false;
	
	/** iteration to find the best image threshold **/			
	while (true) {
		vector<Rect> temp_obj;
		cascade.detectMultiScale( smallImg, temp_obj, // object detection
					1.1, min_neighbors, 			
					0 | CV_HAAR_SCALE_IMAGE,
					Size(30, 30) );
		
		printf("Current min_neighbors is %d, number of objects is %d\n", min_neighbors, (int)temp_obj.size());
		if ((int)temp_obj.size() > 0) { // if can find something....
			if ((int)temp_obj.size() == 1 || over_estimated) { // find the only object! [Return]
				detected_object = temp_obj;
				printf("obj found! The min_neighbors is %d\n", min_neighbors);
				if (over_estimated)
					printf("More than one object is detected!");
				break;
			}
			else // more than one obj detected... try higher threshold
				min_neighbors++;
		} else {	// no obj found with this min_neighbors
			if (min_neighbors == 1) {
				detected_object = temp_obj;
				printf("no objects detected!\n");
				break;
			} else {
				min_neighbors--;
				over_estimated = true;
			}
		}
	}
	
    t = (double)cvGetTickCount() - t;
    printf("detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.)); // print processing time

    coordinate ret_val = {0, 0, 0, 0};
    if ((int)detected_object.size() == 1) { // 1 return
		vector<Rect>::const_iterator r = detected_object.begin();
		ret_val.x_left = (int)(r->x * scale);
		ret_val.x_right = (int)((r->x + r->width - 1) * scale);
		ret_val.thres = min_neighbors;
		ret_val.conf = calConfScore(smallImg, cascade, detected_object);			// calculate conf score
	}
	else if ((int)detected_object.size() > 1) { // error (more than one detected)
		ret_val.x_left = ERROR;
		ret_val.x_right = ERROR;
	}
    
    if (ret_val.conf >= 20)			// display the detection result
		showImage(img, detected_object, scale, display);

    return ret_val;
}

/* Display the image with the detection result */
void showImage (Mat& img, vector<Rect> detected_object, double scale, int display) {
	if (display != 1) {
		return;
	}		

	const static Scalar colors[] = { CV_RGB(0,0,255),		// colors of rectangles
					CV_RGB(0,128,255),
					CV_RGB(0,255,255),
					CV_RGB(0,255,0),
					CV_RGB(255,128,0),
					CV_RGB(255,255,0),
					CV_RGB(255,0,0),
					CV_RGB(255,0,255)};

	int i = 0;
	for( vector<Rect>::const_iterator r = detected_object.begin(); r != detected_object.end(); r++, i++ ) {
		Point center;
		Scalar color = colors[i%8];

		rectangle( img, 	// draw rectangle
			cvPoint(cvRound(r->x * scale), 
			cvRound(r->y * scale)),
			cvPoint(cvRound((r->x + r->width - 1) * scale), 
			cvRound((r->y + r->height - 1) * scale)), color, 3, 8, 0);
	}
    
	cvNamedWindow("result", 1);
	Mat smallImg(cvRound (img.rows / scale), cvRound(img.cols / scale), CV_8UC1);
	resize(img, smallImg, smallImg.size(), 0, 0, INTER_LINEAR); 
	cv::imshow("result", smallImg); // show the image

	waitKey(0);
    cvDestroyWindow("result");	// return

    return;
}

/* Calculate the confindence score */
/* currently I just use round # as conf_score */
int calConfScore(Mat& img, CascadeClassifier& cascade, vector<Rect> detected_object) {
	cout << "Calculating conf score..." << endl;

	if (detected_object.size() != 1)	// no object or more than one object
		return 0;

	// First round cropping 
	vector<Rect>::const_iterator r = detected_object.begin();
	cv::Rect croppingRec = getCroppingRec(img, (int)r->x, (int)r->y, (int)r->width, (int)r->height); // Setup a rectangle (x, y, width, height)
	cv::Mat temp_img = img(croppingRec);	// get the cropped image

	int round = 1;
	int min_neighbors = 1;
	while(true) {
		vector<Rect> temp_obj;
		cascade.detectMultiScale( temp_img, temp_obj, // object detection
					1.1, min_neighbors, 			
					0 | CV_HAAR_SCALE_IMAGE,
					Size(30, 30) );

		if (temp_obj.size() < 1)	// exit if no object is found
			break;
		else if (temp_obj.size() > 1) {
			min_neighbors++;
		}
		else {				// crop the image
			round++;
			// cout << "Round " << round << " min_neighbors " << min_neighbors << endl; // for debugging ....
			r = temp_obj.begin();
			cv::Rect temp_croppingRec = getCroppingRec(temp_img, (int)r->x, (int)r->y, (int)r->width, (int)r->height); // Setup a rectangle (x, y, width, height)
			temp_img = temp_img(temp_croppingRec);	// get the cropped image
		}
	}
	return round;
}

cv::Rect getCroppingRec (Mat& img, int x, int y, int width, int height) {
	int x_res = (x - MARGIN < 0) ? 0: (x - MARGIN);	// 
	int y_res = (y - MARGIN < 0) ? 0: (y - MARGIN);	// 
	int width_res = (x + width + MARGIN > img.cols) ? (img.cols - x_res - 1) : (x + width + MARGIN - x_res - 1); 	// 
	int height_res = (y + height + MARGIN > img.rows) ? (img.rows - y_res - 1) : (y + height + MARGIN - y_res - 1); // 
	// cout << "[Rec] x:" << x_res << " y:" << y_res << " width_res:" << width_res << " height_res:" << height_res << endl; // for debugging...
	// cout << "[Img] cols:" << img.cols << " rows:" << img.rows << endl; // for debugging...
	return Rect(x_res, y_res, width_res, height_res);
}
