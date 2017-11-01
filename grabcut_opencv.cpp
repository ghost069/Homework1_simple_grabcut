#include "opencv2/highgui/highgui.hpp"                                                                                               
#include "opencv2/imgproc/imgproc.hpp"  

#include <iostream>
 
using namespace std;
using namespace cv; 
 
string filename1;
string filename2;
Mat image;
Mat origin;
Mat src;
string winName = "show";
string tmpName = "temp_resized.jpg";
enum{NOT_SET = 0, IN_PROCESS = 1, SET = 2};
uchar rectState;
Rect rect;
Mat mask;
const Scalar GREEN = Scalar(0,255,0);
Mat bgdModel, fgdModel;
 
void setRectInMask(){
	rect.x = max(0, rect.x);
	rect.y = max(0, rect.y);
	rect.width = min(rect.width, image.cols-rect.x);
	rect.height = min(rect.height, image.rows-rect.y);
 
}
 
static void getBinMask( const Mat& comMask, Mat& binMask ){
	binMask.create( comMask.size(), CV_8UC1 );
	binMask = comMask & 1;     
}
 
void on_mouse( int event, int x, int y, int flags, void* )
{
	Mat res;
	Mat binMask;

	switch( event ){
		case CV_EVENT_LBUTTONDOWN:
			if( rectState == NOT_SET){
				rectState = IN_PROCESS;
				rect = Rect( x, y, 1, 1 );
			cout<<"button down"<<endl;
			}
			break;
		case CV_EVENT_LBUTTONUP:
			if( rectState == IN_PROCESS ){
				cout<<"button up"<<endl;
				rect = Rect( Point(rect.x, rect.y), Point(x,y) );
				rectState = SET;
				(mask(rect)).setTo( Scalar(GC_PR_FGD));
				image=imread(tmpName,1);
				grabCut(image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);
				getBinMask( mask, binMask );
				origin=imread(filename2,1);
				resize(origin,res,Size(image.cols,image.rows));
				image.copyTo(res, binMask );//image.copyTo(imageROI，mask),作用是把mask和image重叠以后把mask中像素值为0（black）的点对应的image中的点变为透明，而保留其他点。
				namedWindow("result", 0);  
				imshow("result", res);
				imwrite("result.jpg",res);
			}
			break;
		case CV_EVENT_MOUSEMOVE:
			if( rectState == IN_PROCESS ){
				cout<<"button moving"<<" "<<x<<" "<<y<<endl;
				rect = Rect( Point(rect.x, rect.y), Point(x,y) );
				image=imread(tmpName,1);
				rectangle(image, Point( rect.x, rect.y ), Point(rect.x + rect.width, rect.y + rect.height ), GREEN, 2);
				imshow(winName, image);
			}
			break;
	}
}
 
int main(int argc, char* argv[]){
        if(argc !=3){
            cout<<"usage: grabcut [filename1(foreground)] [filename2(background)]"<<endl;
            return 0;
        }
	filename1 = argv[1];
	filename2 = argv[2];
	origin = imread( filename1, 1 );
	image = origin;
        while(image.cols>1000 || image.rows >1000){
		resize(origin,image,Size(image.cols/2,image.rows/2));
	}
	imwrite(tmpName,image);
	image=imread(tmpName,1);
        imshow(winName, image);
	mask.create(image.size(), CV_8UC1);
	rectState = NOT_SET;
	mask.setTo(GC_BGD);
	setMouseCallback(winName, on_mouse, 0);
	waitKey(0);
 
	return 0;
}
