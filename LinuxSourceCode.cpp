#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <zbar.h>

#include <string.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cstdlib>

using namespace cv;
using namespace std;
using namespace zbar;  

 
int main( int argc, char** argv )
{
	int n, i;
	
	if( argc < 2)
    {
		cout <<"Usage: app.exe image.jpg [x] (if [x] is set to 1 image window is shown)" << endl;
		system("pause");
		return -1;
    }

	int displayOK;

	if(argc > 2)
	{	
		if(!strcmp(argv[2],"1"))
			displayOK = 1;
	}
	else
	{
		displayOK = 0;
	}

	//set output file
	string INname;
	INname = argv[1];
	
	ofstream OUTfis;
	
	size_t lastindex = INname.find_last_of("."); 
	string OUTname = INname.substr(0, lastindex);
	OUTname = OUTname + ".TAG";
	
	OUTfis.open(OUTname.c_str());

	//scanner setup
	ImageScanner scanner;  
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);  
       
	Mat imgIN;
	// Read the image - as grayscale
    imgIN = imread(INname, 0); 

	// Check for invalid input
	if(! imgIN.data )                              
    {
        cout <<  "Could not open or find the image" << endl ;
		system("pause");
        return -1;
    }

	int width = imgIN.cols;  
    int height = imgIN.rows;

	// wrap image data  
	uchar *raw = (uchar *)imgIN.data;
	Image image(width, height, "Y800", raw, width * height);

	// scan the image for barcodes  
	scanner.scan(image);

	Mat imgOUT;
	if(displayOK)
	{
		cvtColor(imgIN,imgOUT,CV_GRAY2RGB);
	}

	string TAG;
	 
	// extract results  
	for(Image::SymbolIterator symbol = image.symbol_begin();  symbol != image.symbol_end(); ++symbol) 
	{
        TAG = symbol->get_data();
		if(TAG.size() != 14)
			continue;

		//print tag
		cout << TAG << endl;
		OUTfis << TAG << endl;

		//define vector of points
		vector<Point> vp;
		if(displayOK)
		{
			n = symbol->get_location_size();

			//get bar code location in image
			for(i=0; i<n; i++)
			{
				vp.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i))); 
            }

			//build rectangle around the bar code
			RotatedRect r = minAreaRect(vp);
			Point2f pts[4];  
			r.points(pts);

			//draw lines
			for(i=0; i<4; i++)
			{  
                line(imgOUT, pts[i], pts[(i+1)%4], Scalar(255,0,0), 3);  
			}
		}
	}  

	//display information
	if(displayOK)
	{
		namedWindow( "TAG Identification", WINDOW_NORMAL );
		imshow("TAG Identification",imgOUT);
	}
	  
   // clean up  
   image.set_data(NULL, 0);  
   waitKey();
 }  
