#include <math.h>
#define _USE_MATH_DEFINES
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

Mat MyRotate(Mat img,double angle,unsigned char* pData) {
	pData = (unsigned char*)img.data; //���� �̹����� ������ ��
	double r = (angle * M_PI / 180); //������ ���� ����
	double Sin, Cos, x, y;
	int x_c = img.rows / 2,y_c = img.cols / 2, x_in, y_in; //ȸ�� �߽�

	Mat img_out(img.rows, img.cols, CV_8UC1, Scalar(0)); //����� �̹����� ���� ���� Mat

	for (int y_out = 0; y_out < img.rows; y_out++) {
		for (int x_out = 0; x_out < img.cols; x_out++) {
			Sin = sin(r);//sin��
			Cos = cos(r);//cos��

			//��� ���
			x = Cos * (x_out - x_c) + Sin * (y_out - y_c) + x_c; //������ x��
			y = -Sin * (x_out - x_c) + Cos * (y_out - y_c) + y_c; //������ y��

			x_in = (int)(x); 
			y_in = (int)(y);
			//int������ ����

			if (x_in < 0 || x_in > 255 || y_in < 0 || y_in > 255) { //����� �Ѿ�� ���
				img_out.data[y_out * img.cols + x_out] = 0; //�����Ͱ� 0(������)
			}
			else 
				img_out.data[y_out * img.cols + x_out] = pData[y_in * img.cols + x_in];//������ �� (�����̹����� ��)
		}
	}
	return img_out;
}


Mat MyZoom(Mat img, int x_size, int y_size, unsigned char* pData) {
	double q1, q2; //interpolated ����
	double p1, p2, p3, p4; //input pixel ����
	int a, b, n; //��ȭ��
	double x_zoom = (double)x_size/256; //Ȯ�� ����
	double y_zoom = (double)y_size / 256;
	pData = (unsigned char*)img.data;

	//Ȯ�� ���� �� �ִ밪(��ü ������)
	int x_max = (int)(256 * x_zoom);
	int y_max = (int)(256 * y_zoom);
	Mat img_out(y_max, x_max, CV_8UC1, Scalar(0)); //��� �̹��� 

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

	cout << "���ϴ� ���� �����ϼ���! -> 1:ȸ�� 2: Ȯ��\n�Է�: ";
	cin >> a;
	while (t == 0) {
		if (a != 1 && a != 2) {
			cout << "1�� 2�߿��� ������ �ּ���! -> 1:ȸ�� 2: Ȯ��\n�Է�: ";
			cin >> a;
		}
		else {
			t++;
		}
	}

	if (a == 1) {
		cout << "�󸶸�ŭ ȸ�� �ұ��? : ";
		cin >> angle;
		img_in = imread("Lena_256x256.png");
		cvtColor(img_in, img_in, cv::COLOR_RGB2GRAY);
		imshow("source img", img_in);
		img_in = MyRotate(img_in,angle,pData);
	}
	if (a == 2) {
		cout << "� ������� Ȯ�� �ұ��? (128 ~1024) x_size y_size : ";
		cin >> x_size >> y_size;
		while (t == 1) {
			if (x_size < 128 || y_size < 128 || x_size > 1024 || y_size > 1024) {
				cout << "128 ~ 1024 ������ ���ڷ� �Է����ּ���!\n";
				cout << "� ������� Ȯ�� �ұ��?(128 ~1024) x_size y_size : ";
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
