#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
bool ROI_flag=false;
bool saveFlag=false;
int  _time= 0 ; //selectObject的初始值为false,
bool _initdone = false;
float Rool_angle[2]={-1,-1.0};
Point origin;
Rect selection;
#define UNKNOWN_FLOW_THRESH 1e9
//用鼠标选择目标图像区域
static vector<Scalar> colorwheel; //Scalar r,g,b

void onMouse( int event, int x, int y, int, void* )
{  
	if (event == CV_EVENT_LBUTTONDOWN && !CV_EVENT_MOUSEMOVE)  
	{  
		if(event==CV_EVENT_LBUTTONDOWN) 
		{
			if(_time==0)
			{
				origin = Point(x,y);
				selection = Rect(x,y,0,0);
				cout<<"position1 "<<x<<" "<<y<<endl;
				_time++;
				ROI_flag=false;
			}
			else if (_time==1)
			{
				selection.x = MIN(x, origin.x);
				selection.y = MIN(y, origin.y);
				selection.width = std::abs(x - origin.x);
				selection.height = std::abs(y - origin.y);
				cout<<"selection area: "<<selection<<endl;
				_time++;
				ROI_flag=true;
			}
		}
	}  
}    
//opencv coloe BGR system
void makecolorwheel(vector<Scalar> &colorwheel)  
{  //共55个方向
	int RY = 15;  
	int YG = 6;  
	int GC = 4;  
	int CB = 11;  
	int BM = 13;  
	int MR = 6;  
	
	int i;  
	
	for (i = 0; i < RY; i++) colorwheel.push_back(Scalar(255,       255*i/RY,     0));  
	for (i = 0; i < YG; i++) colorwheel.push_back(Scalar(255-255*i/YG, 255,       0));  
	for (i = 0; i < GC; i++) colorwheel.push_back(Scalar(0,         255,      255*i/GC));  
	for (i = 0; i < CB; i++) colorwheel.push_back(Scalar(0,         255-255*i/CB, 255));  
	for (i = 0; i < BM; i++) colorwheel.push_back(Scalar(255*i/BM,      0,        255));  
	for (i = 0; i < MR; i++) colorwheel.push_back(Scalar(255,       0,        255-255*i/MR));  
}  

void motionToColor(const Mat &flowx, const Mat &flowy,Mat &color)  
{  
	Mat color_tmp;
	//color.copyTo(color_tmp);
	color_tmp.create(flowx.rows, flowx.cols, CV_8UC3);  
	color.create(flowx.rows, flowx.cols, CV_8UC3);  
	double xMin, xMax,yMin,yMax;   // determine motion range:  
	cv::minMaxLoc(flowx, &xMin, &xMax, 0, 0);
	cv::minMaxLoc(flowy, &yMin, &yMax, 0, 0);
	float xVol = ::max(abs(xMin), abs(xMax) ) ;  // Find max flow to normalize fx and fy  
	float yVol = ::max(abs(yMin), abs(yMax) ) ;
	float rad = xVol * xVol + yVol * yVol;  
	float maxrad = 0 > rad ? 0 : rad; 

	Mat Rad;
	Rad.create(flowx.rows,flowx.cols,CV_32FC1);
	Rad=(flowx.mul(flowx)+flowy.mul(flowy))/maxrad;
	cv::sqrt(Rad, Rad);

	Mat Wheel;
	Wheel=Mat::zeros (55, 1, CV_32FC1);  
	color_tmp.setTo(Scalar(0,0,0));
	color.setTo(Scalar(0,0,0));

	int num=0;
	for (int i = 0; i < flowx.rows; ++i)   
	{  
		for (int j = 0; j < flowx.cols; ++j)   
		{  
	             	uchar *data = color_tmp.data + color_tmp.step[0] * i + color_tmp.step[1] * j;  
	            	float fx = flowx.at<float>(i,j ) ;  
	            	float fy = flowy.at<float>(i,j ) ;  
	            	if ((fabs(fx) >  UNKNOWN_FLOW_THRESH) || (fabs(fy) >  UNKNOWN_FLOW_THRESH))  
	            	{  
	                	data[0] = data[1] = data[2] = 0;  
	                	continue;  
	            	}  
/*	            	float angle_tmp = atan2(-fy, -fx) / CV_PI;  
                        float fk = (angle_tmp + 1.0) / 2.0 * (colorwheel.size()-1);  
            		int k0 = (int)fk; 
            		Wheel.at<float>(k0,1)=Wheel.at<float>(k0,1)+1; 
	            	for (int b = 0; b < 3; b++)   
	            	{  
		                float col0 = colorwheel[k0][b] / 255.0;  
		                col0 = Rad.at<float>(i, j)<=1?1 - Rad.at<float>(i, j) / maxrad* (1 - col0):0.75; // increase saturation with radius  
		  		data[2 - b] = (int)(255.0 * col0);  
	           	 }  */

	            	float diff_XY=abs(fx)+abs(fy);
                        if (diff_XY > 5)
                         {
	            	for (int b = 0; b < 3; b++)   
	            	{  
		  		data[2 - b] = (int)255.0 ;  
	           	 } 
	           	 num++;
                        }
                        
                        //arrowedLine(cflow, Point(x, y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)), CV_RGB(0,255,0));
                        
                        //circle(cflow, Point(x,y), 2, CV_RGB(255, 0, 0), -1);
    		}
	    	//Wheel=Wheel/(double)(flowx.rows*flowx.cols);
	}
   	double minv = 0.0, maxv = 0.0;  
    	Point minl,maxl;    
    	minMaxLoc(Wheel, &minv, &maxv,&minl,&maxl);
    	if(num<0.2*color_tmp.rows*color_tmp.cols   && num>1500)
    	{
		cout<<" num "<<num <<" "<<color_tmp.rows*color_tmp.cols <<endl;
		vector<vector<Point> > contours;
		Mat greay;
		cvtColor(color_tmp,  greay,  CV_BGR2GRAY);
		findContours(greay,contours , CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
		cout<<contours.size()<<endl;
		if (contours.size()!=0)
		{
			int tmp[2]={0};
			for (size_t c=0;c<contours.size();c++)
			{
				tmp[0]=contours.size()>tmp[0]?contours.size():tmp[0];
				tmp[1]=c;
			}
			RotatedRect box = minAreaRect(Mat(contours[tmp[1]]));
			if(box.boundingRect().area()>100  && box.boundingRect().area()<10000)
			{
				Point2f vertex[4];
				box.points(vertex);
				cout<<vertex<<endl;
				for( int i = 0; i < 4; i++ )
				line(color, vertex[i], vertex[(i+1)%4], Scalar(0, 255, 0), 2, CV_AA);
				saveFlag=true;				
			}
			//drawContours(color, contours,  -1,  Scalar(0,255,0));
			//imwrite("dd.jpg", color);	    
		}
		
    	}
		Rool_angle[0] =(float)maxl.y;
	Rool_angle[1] =(float)(maxv*100/(double)(flowx.rows*flowx.cols));

/*for( int x = 0; x < color.rows; x++ ) {
	for( int y = 0; y < color.cols; y++ ) {
		if ( color.at<Vec3b>(x, y) == Vec3b(255,255,255) ) {
			color.at<Vec3b>(x, y)[0] = 0;
			color.at<Vec3b>(x, y)[1] = 0;
			color.at<Vec3b>(x, y)[2] = 0;
		}
	}
}


Mat kernel = (Mat_<float>(3,3) <<
            1,  1, 1,
            1, -8, 1,
            1,  1, 1);
Mat imgLaplacian;
Mat sharp = color; // copy source image to another temporary one
filter2D(sharp, imgLaplacian, CV_32F, kernel);
color.convertTo(sharp, CV_32F);
Mat imgResult = sharp - imgLaplacian;
// convert back to 8bits gray scale
imgResult.convertTo(imgResult, CV_8UC3);
imgLaplacian.convertTo(imgLaplacian, CV_8UC3);

imshow( "New Sharped Image", imgResult );
moveWindow("New Sharped Image",  710,  10);*/

/*        color = imgResult; // copy back
// Create binary image from source image


Mat bw;
cvtColor(color, bw, CV_BGR2GRAY);
threshold(bw, bw, 40, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
// Perform the distance transform algorithm
imshow("Binary Image", bw);
moveWindow("Binary Image",  1200,  10);*/


/*    Mat dist;
distanceTransform(bw, dist, CV_DIST_L2, 3);
// Normalize the distance image for range = {0.0, 1.0}
// so we can visualize and threshold it
normalize(dist, dist, 0, 1., NORM_MINMAX);
imshow("Distance Transform Image", dist);
moveWindow("Distance Transform Image",  10,  500);

threshold(dist, dist, .4, 1., CV_THRESH_BINARY);
// Dilate a bit the dist image
Mat kernel1 = Mat::ones(3, 3, CV_8UC1);
dilate(dist, dist, kernel1);
imshow("Peaks", dist);
moveWindow("Peaks",  710,  500);*/


/*        Mat dist_8u;
dist.convertTo(dist_8u, CV_8U);
// Find total markers
vector<vector<Point> > contours;
findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
// Create the marker image for the watershed algorithm


Mat markers = Mat::zeros(dist.size(), CV_32SC1);
// Draw the foreground markers
for (size_t i = 0; i < contours.size(); i++)
drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i)+1), -1);
// Draw the background marker
circle(markers, Point(5,5), 3, CV_RGB(255,255,255), -1);
imshow("Markers", markers*10000);
moveWindow("Markers",  1200,  500);
*/
/*        watershed(color, markers);
Mat mark = Mat::zeros(markers.size(), CV_8UC1);
markers.convertTo(mark, CV_8UC1);
bitwise_not(mark, mark);
vector<Vec3b> colors;


for (size_t i = 0; i < contours.size(); i++)
{
	int b = theRNG().uniform(0, 255);
	int g = theRNG().uniform(0, 255);
	int r = theRNG().uniform(0, 255);
	colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
}
// Create the result image
color = Mat::zeros(markers.size(), CV_8UC3);
// Fill labeled objects with random colors


for (int i = 0; i < markers.rows; i++)
{
	for (int j = 0; j < markers.cols; j++)
	{
		int index = markers.at<int>(i,j);
		if (index > 0 && index <= static_cast<int>(contours.size()))
		color.at<Vec3b>(i,j) = colors[index-1];
		else
		color.at<Vec3b>(i,j) = Vec3b(0,0,0);
		}
}*/
 }  