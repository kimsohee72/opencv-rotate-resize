#include <math.h>
#define _USE_MATH_DEFINES
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

Mat MyRotate(Mat img,double angle,unsigned char* pData) {
	pData = (unsigned char*)img.data; //원래 이미지의 데이터 값
	double r = (angle * M_PI / 180); //변경할 각도 비율
	double Sin, Cos, x, y;
	int x_c = img.rows / 2,y_c = img.cols / 2, x_in, y_in; //회전 중심

	Mat img_out(img.rows, img.cols, CV_8UC1, Scalar(0)); //출력할 이미지의 값을 넣을 Mat

	for (int y_out = 0; y_out < img.rows; y_out++) {
		for (int x_out = 0; x_out < img.cols; x_out++) {
			Sin = sin(r);//sin값
			Cos = cos(r);//cos값

			//행렬 계산
			x = Cos * (x_out - x_c) + Sin * (y_out - y_c) + x_c; //변경할 x값
			y = -Sin * (x_out - x_c) + Cos * (y_out - y_c) + y_c; //변경할 y값

			x_in = (int)(x); 
			y_in = (int)(y);
			//int값으로 변경

			if (x_in < 0 || x_in > 255 || y_in < 0 || y_in > 255) { //사이즈를 넘어가는 경우
				img_out.data[y_out * img.cols + x_out] = 0; //데이터값 0(검정색)
			}
			else 
				img_out.data[y_out * img.cols + x_out] = pData[y_in * img.cols + x_in];//데이터 값 (원래이미지의 값)
		}
	}
	return img_out;
}


Mat MyZoom(Mat img, int x_size, int y_size, unsigned char* pData) {
	double q1, q2; //interpolated 변수
	double p1, p2, p3, p4; //input pixel 변수
	int a, b, n; //변화값
	double x_zoom = (double)x_size/256; //확대 비율
	double y_zoom = (double)y_size / 256;
	pData = (unsigned char*)img.data;

	//확대 했을 때 최대값(전체 사이즈)
	int x_max = (int)(256 * x_zoom);
	int y_max = (int)(256 * y_zoom);
	Mat img_out(y_max, x_max, CV_8UC1, Scalar(0)); //출력 이미지 

	for (int y_out = 0; y_out < img_out.rows; y_out++)
	{
		for (int x_out = 0; x_out < img_out.cols; x_out++) {
			
			a = x_out / x_zoom - (int)(x_out / x_zoom);
			b = y_out / y_zoom - (int)(y_out / y_zoom);
			n = (int)(y_out / y_zoom) * img.rows + (int)(x_out / x_zoom);


			p1= pData[n], p2= pData[n + 1], p3= pData[n + img.cols], p4 = pData[n + img.cols + 1];
			q1 = ((1 - a) *p1 + a * p2);
			q2 = ((1 - a) * p3 + a * p4);

			img_out.data[y_out * img_out.cols + x_out] = (1-b)*q1 + b*q2;
		}
	}

	return img_out;
}


int main()
{
	int a, angle, x_size,y_size,t=0;
	Mat img_in;
	unsigned char* pData = (unsigned char *)img_in.data;

	cout << "원하는 것을 선택하세요! -> 1:회전 2: 확대\n입력: ";
	cin >> a;
	while (t == 0) {
		if (a != 1 && a != 2) {
			cout << "1과 2중에서 선택해 주세요! -> 1:회전 2: 확대\n입력: ";
			cin >> a;
		}
		else {
			t++;
		}
	}

	if (a == 1) {
		cout << "얼마만큼 회전 할까요? : ";
		cin >> angle;
		img_in = imread("Lena_256x256.png");
		cvtColor(img_in, img_in, cv::COLOR_RGB2GRAY);
		imshow("source img", img_in);
		img_in = MyRotate(img_in,angle,pData);
	}
	if (a == 2) {
		cout << "어떤 사이즈로 확대 할까요? (128 ~1024) x_size y_size : ";
		cin >> x_size >> y_size;
		while (t == 1) {
			if (x_size < 128 || y_size < 128 || x_size > 1024 || y_size > 1024) {
				cout << "128 ~ 1024 사이의 숫자로 입력해주세요!\n";
				cout << "어떤 사이즈로 확대 할까요?(128 ~1024) x_size y_size : ";
				cin >> x_size >> y_size;
			}
			else
				t++;
		}
		img_in = imread("Lena_256x256.png");
		cvtColor(img_in, img_in, cv::COLOR_RGB2GRAY);
		imshow("source img", img_in);
		img_in = MyZoom(img_in,x_size,y_size, pData);
	}

    imshow("output image", img_in);

    waitKey(0);

    return 0;
}
