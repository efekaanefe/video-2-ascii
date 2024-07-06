#include <iostream>
#include <opencv2/opencv.hpp>
/*#include <opencv2/core/core.hpp>*/
/*#include <opencv2/highgui/highgui.hpp>*/

using namespace cv;
using namespace std;

int main(){

	string filename = "video.mp4";
    VideoCapture cap(filename);

	if(!cap.isOpened()){
		cout << "Error opening video stream or file" << endl;
		return -1;
	}
   
	while(1){

		Mat frame;
		// Capture frame-by-frame
		cap >> frame;

		// If the frame is empty, break immediately
		if (frame.empty())
		  break;

		// Display the resulting frame
		imshow( "Frame", frame );

		// Press  ESC on keyboard to exit
		char c=(char)waitKey(25);
		if(c==27)
		  break;
	}
		  
	cap.release();
	destroyAllWindows();

	return 0;
}
