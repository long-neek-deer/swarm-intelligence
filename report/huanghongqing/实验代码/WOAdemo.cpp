// WOAdemo.cpp: 定义控制台应用程序的入口点。
//
// @author 黄红清
// @email huanghqdx@163.com
#include "stdafx.h"
#include "WOA.h"

class WOAshow :public WOA {
	using WOA::WOA;//构造函数继承
	int height, width;
public:
	double run_with_show() {
		height = (int)(y_max - y_min + 1), width = (int)(x_max - x_min + 1);
		//获取每个位置的函数值
		Mat bgv(height, width, CV_64FC1);
		double max_value = DBL_MIN, min_value = DBL_MAX;
		for (int x = x_min; x <= x_max; x++) {
			for (int y = y_min; y <= y_max; y++) {
				double value = loss_func(Point(x, y));
				if (value > max_value) max_value = value;
				if (value < min_value) min_value = value;
				bgv.at<double>(trans(x,y)) = value;
			}
		}
		//将每个位置函数值画成热力图
		Mat bg(height, width, CV_8UC3, Scalar(0, 0, 0));
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				double value = bgv.at<double>(x, y);
				double proportion = (value - min_value) / (max_value - min_value);
				double red, green, blue;
				if (proportion <= 0.3671875)
					red = 0;
				else if (proportion < 0.6171875)
					red = 1017.8*proportion - 373.5;
				else if (proportion <= 0.8671875)
					red = 255;
				else
					red = -1018.7*proportion + 1138.1;
				if (proportion <= 0.1171875)
					green = 0;
				else if (proportion < 0.3671875)
					green = 1017.8*proportion - 119.04;
				else if (proportion <= 0.6171875)
					green = 255;
				else if (proportion < 0.8671875)
					green = -1017.8*proportion + 882.89;
				else
					green = 0;
				if (proportion < 0.1171875)
					blue = 1024 * proportion + 135;
				else if (proportion <= 0.3671875)
					blue = 255;
				else if (proportion < 0.6171875)
					blue = -1017.8*proportion + 628.43;
				else
					blue = 0;
				bg.at<Vec3b>(x, y)[0] = (uchar)floor(blue);
				bg.at<Vec3b>(x, y)[1] = (uchar)floor(green);
				bg.at<Vec3b>(x, y)[2] = (uchar)floor(red);
			}
		}
		
		paint_whales(bg);
		while (cur_it < max_it) {

			next_iteration();
			//next_iteration_modify();
			paint_whales(bg);
		}
		return loss_func(best_whale);
	}
private:
	Point trans(Point& p) { return Point(p.x + width / 2, height / 2 - p.y);}
	Point trans(Point2d& p) { return Point(p.x + width / 2, height / 2 - p.y); }
	Point trans(int x, int y) { return Point(x + width / 2, height / 2 - y);}
	void paint_whales(Mat&bg_orgin) {
		Mat bg; bg_orgin.copyTo(bg);
		Point rec_c = trans(best_whale);
		int rec_a = 100;
		rectangle(bg, Point(rec_c.x - rec_a / 2, rec_c.y - rec_a / 2), Point(rec_c.x + rec_a / 2, rec_c.y + rec_a / 2), Scalar(255, 255, 255), 6, LINE_AA, 0);
		for (int i = 0; i < whales_num; i++) {
			circle(bg, trans(whales[i]), 1, Scalar(255, 255, 255), 10, LINE_AA, 0);
		}
		imwrite("C:\\Users\\hhq\\Desktop\\WOA\\e"+to_string(cur_it)+".jpg", bg);
		imshow("WOA", bg);
		if (waitKey(100*DELAY) > 0)
			return ;
	}
};

double func(Point2d p) {
	double x = p.x, y = p.y;
	//return x * x + y * y;
	//return fabs(x) > fabs(y) ? fabs(x) : fabs(y);
	//return 4 * pow(x / 300.0, 2) - 2.1*pow(x / 300.0, 4) + 1.0 / 3 * pow(x / 300.0, 6) + x*y / 90000 - 4 * pow(y / 300.0, 2) + 4 * pow(y / 300.0, 4);
	return x * x/90000.0 - 10 * cos(2 * PI*x/300.0) + y * y/90000.0 - 10 * cos(2 * PI*y/300.0);
}

int main()
{
	WOAshow woa(-300, 300, -300, 300, 25, 11, func);
	cout << woa.run_with_show();

	getchar();
    return 0;
}

/*
const int DELAY = 5;
Mat image(200, 200, CV_8UC3, Scalar(0, 0, 0));
for (int i = 0; i < 100; i++)
{
image = image + Scalar((double)(105 / 100), (double)(105 / 100), (double)(105 / 100));
imshow("win", image);
if (waitKey(DELAY) >= 0)
return 0;
}
waitKey();
return 0;
*/