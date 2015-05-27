/*
 * Name: Object detection (show image)
 * Author: Xiaochen
 * Time: 05/27/2015
 * Version: 2.0
 * Function: 
 	1. Use HAAR features and SVM to find a specific obj in a picture.
 	2. [Update] Automatically increase the threshold to decrease the number of detected objects to one
 	3. Show the result in a new window
 * Usage: 
 	1. Compile the cpp file, get the executable file
 	2. In command: ./[executable_file] --cascade=[cascade_path] [image_path]
 *
 */

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cctype>
#include <iostream>
#include <iterator>
#include <stdio.h>

using namespace std;
using namespace cv;

void detectAndDraw( Mat& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, bool tryflip );

string cascadeName = "../../data/haarcascades/haarcascade_frontalface_alt.xml";
string nestedCascadeName = "../../data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

int main(int argc, const char** argv) {
	CvCapture* capture = 0;
	Mat frame, frameCopy, image;
	const string scaleOpt = "--scale=";
	size_t scaleOptLen = scaleOpt.length();
	const string cascadeOpt = "--cascade=";
	size_t cascadeOptLen = cascadeOpt.length();
	const string nestedCascadeOpt = "--nested-cascade";
	size_t nestedCascadeOptLen = nestedCascadeOpt.length();
	const string tryFlipOpt = "--try-flip";
	size_t tryFlipOptLen = tryFlipOpt.length();
	string inputName;
	bool tryflip = false;

	CascadeClassifier cascade, nestedCascade;
	double scale = 1;

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
		else if( argv[i][0] == '-' ) { // wrong input format
		    	cerr << "WARNING: Unknown option %s" << argv[i] << endl;
		}
		else
		    inputName.assign( argv[i] ); // image name 
    	}

	if( !cascade.load( cascadeName ) ) { // handle  the error cascade
		cerr << "ERROR: Could not load classifier cascade" << endl;
		return -1;
	}

	if( inputName.size() ) { // read in the image
		image = imread( inputName, 1 );
		if( image.empty() ) {
			cout << "ERRROR: input image is empty!" << endl;
			return -1;
		}
		cout << "SUCCESS: In image read" << endl;
	}
	else {
		cout << "ERROR: Couldn't no input image name" << endl;
		return -1;
	}

    	cvNamedWindow("result", 1);
	
	detectAndDraw(image, cascade, nestedCascade, scale, tryflip); // detect and draw
	
	waitKey(0);
    	cvDestroyWindow("result");
    	
    	return 0;
}

void detectAndDraw( Mat& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, bool tryflip )
{
	const static Scalar colors[] = { CV_RGB(0,0,255),		// colors of rectangles
					CV_RGB(0,128,255),
					CV_RGB(0,255,255),
					CV_RGB(0,255,0),
					CV_RGB(255,128,0),
					CV_RGB(255,255,0),
					CV_RGB(255,0,0),
					CV_RGB(255,0,255)};
					
	
	double t = (double)cvGetTickCount(); // count the procesing time
	
	vector<Rect> detected_object; // detected object
	Mat gray, smallImg(cvRound (img.rows / scale), cvRound(img.cols / scale), CV_8UC1);
	
	cvtColor( img, gray, CV_BGR2GRAY ); 
	resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
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
    	
	// show image in a window
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
    
	cv::imshow( "result", img ); // show the image
    	
}
