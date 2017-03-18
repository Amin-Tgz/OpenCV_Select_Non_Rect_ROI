#include "opencv2/opencv.hpp" 
#include <iostream>

using namespace cv;
using namespace std;

/*~~~~~~~~~~~~~~~~~~*/
char ky;
bool got_roi = false;
Point points_array[4];
Mat src, ROI_Img,backup,ROI_MASK;
Rect2d ROI_Select;
int width_roi = 0, height_roi = 0,min_x,min_y,max_x,max_y;
Rect ROI_RECT ;
vector< vector<Point> >  co_ordinates;
/*~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~*/
//Callback for mousclick event, the x-y coordinate of mouse button-down 
//are stored array of points [points_array].

void mouse_click(int event, int x, int y, int flags, void *param)
{

static int count=0;
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		{
			switch (count)	// number of set Point 
			{
				case 0:
					cout << "Select top-right    point" << endl;
					break;
				case 1:
					cout << "Select bottom-right point" << endl;
					break;
				case 2:
					cout << "Select bottom-left  point" << endl << endl;
					break;
				default:
					break;
			}

			if (!got_roi) // you are not select ROI yet!
			{
				points_array[count] = Point(x,y);
				circle(src, points_array[count], 2, Scalar(0, 255, 0), 2);	//show points on image
				imshow("My_Win", src);
				count++;
				if (count == 4) // if select 4 point finished
				{
					cout << "ROI x & y points :" << endl;
					cout << points_array[0] << endl;
					cout << points_array[1] << endl;
					cout << points_array[2] << endl;
					cout << points_array[3] << endl;
					cout << endl << "ROI Saved You can continue with double press any keys except 'c' " << endl <<"once press 'c' or 'C' to clear points and retry select ROI " << endl << endl;
					ky = waitKey(0) & 0xFF;
					
					if (ky == 99 || ky == 67)  // c or C to clear
					{
						backup.copyTo(src);
						points_array[0] = Point(0, 0);
						points_array[1] = Point(0, 0);
						points_array[2] = Point(0, 0);
						points_array[3] = Point(0, 0);
						imshow("My_Win", src);
						count = 0;
						cout << endl << endl << endl << "@---------------------	 Clear Points!	------------------@ " << endl << endl << endl  ;
					}
					else // user accept points & dosn't want to clear them
					{
						min_x = std::min(points_array[0].x, points_array[3].x);	//find rectangle for minimum ROI surround it!
						max_x = std::max(points_array[1].x, points_array[2].x);
						min_y = std::min(points_array[0].y, points_array[1].y);
						max_y = std::max(points_array[3].y, points_array[2].y);
						height_roi = max_y - min_y;
						width_roi = max_x - min_x;
						ROI_RECT = Rect(min_x, min_y, width_roi, height_roi);
						got_roi = true;
						co_ordinates.push_back(vector<Point>());
						co_ordinates[0].push_back(points_array[0]);
						co_ordinates[0].push_back(points_array[1]);
						co_ordinates[0].push_back(points_array[2]);
						co_ordinates[0].push_back(points_array[3]);

					}
				}
			}
			else { // if got_roi se true => select roi before
				cout << endl << "You Select ROI Before " << endl << "if you want to clear point press 'c' or double press other keys to continue" << endl << endl;
			}
			break;
		}
	}

}
/*~~~~~~~~~~~~~~~~~~*/

int main()
{
	// replace all "My_Win" with your window name
	/*~~~~~~~~~~~~~~~~~~*/
	namedWindow("My_Win", 1);
	/*~~~~~~~~~~~~~~~~~~*/

	VideoCapture input_video("Video_path");

	// Set source imafe as [src]
	/*~~~~~~~~~~~~~~~~~~*/
	input_video >> src;
	imshow("My_Win", src);
	src.copyTo(backup);
	setMouseCallback("My_Win", mouse_click, 0);
	waitKey(0);
	Mat mask(src.rows, src.cols, CV_8UC1, cv::Scalar(0));
	drawContours(mask, co_ordinates, 0, Scalar(255), CV_FILLED, 8);
	/*~~~~~~~~~~~~~~~~~~*/
	
	while (1)
	{
		input_video >> src;
		
		/*~~~~~~~~~~~~~~~~~~*/	
		//Need to copy Select ROI as MASK
		src.copyTo(ROI_MASK, mask);
		//Creat a rectangle around the Mask to reduce size of mask
		ROI_Img = ROI_MASK(ROI_RECT);
		/*~~~~~~~~~~~~~~~~~~*/

		//Show Image
		imshow("My_Win", ROI_Img);

		// Do remaining processing here on capture roi for every frame
		if(char (waitKey(1)& 0xFF) == 27) break;
			
		
	}
}