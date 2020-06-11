#include "psutil.h"
#include <map>

namespace psutil
{
	vector<Rect> CImgProcessor::getMostPossibleContours(vector<Rect> rects, int error)
	{
		vector<Rect> result;
		size_t max = 0;
		//typedef pair<Rect, int> PAIR;
		map<int, vector<Rect>> statistics;
		for (size_t i = 0; i < rects.size(); i++)
		{
			int intX = rects[i].x;
			map<int, vector<Rect>>::iterator iter = statistics.find(intX);
			if (iter == statistics.end())
			{
				//没找到
				statistics[intX] = vector<Rect>();
			}
			statistics[intX].push_back(rects[i]);
		}

		map<int, vector<Rect>>::iterator iter = statistics.begin();
		while (iter != statistics.end())
		{
			if ((iter->second).size() > max)
			{
				result = iter->second;
				max = result.size();
			}
			iter++;
		}
		return result;
	}
	Rect CImgProcessor::buildOutterRect(vector<Rect> rects, Point2i brThreshold)
	{
		Rect result;
		if (rects.size() > 0)
		{
			Rect largest(brThreshold, Point2i(0, 0));
			for (size_t i = 0; i < rects.size(); i++)
			{
				if (rects[i].x < largest.x)
				{
					largest.x = rects[i].x;
				}
				if (rects[i].y < largest.y)
				{
					largest.y = rects[i].y;
				}

				int differ = rects[i].br().x - largest.br().x;
				if (differ > 0)
				{					
					 largest.width += differ;
				}

				differ = rects[i].br().y - largest.br().y;
				if (differ > 0)
				{
					largest.height += differ;
				}				
			}
			result = largest;
		}
		return result;
	}
	Mat CImgProcessor::dftImage(Mat src)
	{
		int M = getOptimalDFTSize(src.rows);
		int N = getOptimalDFTSize(src.cols);
		Mat padded;
		copyMakeBorder(src, padded, 0, M - src.rows, 0, N - src.cols, BORDER_CONSTANT, Scalar::all(0));

		Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
		Mat complexImg;
		merge(planes, 2, complexImg);

		dft(complexImg, complexImg);

		// compute log(1 + sqrt(Re(DFT(img))**2 + Im(DFT(img))**2))
		split(complexImg, planes);
		magnitude(planes[0], planes[1], planes[0]);
		Mat dest = planes[0];
		dest += Scalar::all(1);
		log(dest, dest);

		// crop the spectrum, if it has an odd number of rows or columns
		dest = dest(Rect(0, 0, dest.cols & -2, dest.rows & -2));

		int cx = dest.cols / 2;
		int cy = dest.rows / 2;

		// rearrange the quadrants of Fourier image
		// so that the origin is at the image center
		Mat tmp;
		Mat q0(dest, Rect(0, 0, cx, cy));
		Mat q1(dest, Rect(cx, 0, cx, cy));
		Mat q2(dest, Rect(0, cy, cx, cy));
		Mat q3(dest, Rect(cx, cy, cx, cy));

		q0.copyTo(tmp);
		q3.copyTo(q0);
		tmp.copyTo(q3);

		q1.copyTo(tmp);
		q2.copyTo(q1);
		tmp.copyTo(q2);

		normalize(dest, dest, 0, 1, NORM_MINMAX);
		return dest;
	}

	Mat CImgProcessor::zoom(Mat src, double scale)
	{
		/*要缩小图像，一般推荐使用CV_INTER_AREA来插值；若要放大图像，推荐使用CV_INTER_LINEAR。
		void resize(InputArray src, OutputArray dst, Size dsize, double fx=0, double fy=0, int interpolation=INTER_LINEAR )*/

		int	interpolation = INTER_LINEAR;
		if (scale < 1)
		{
			interpolation = INTER_AREA;
		}
		Mat dest;
		cv::resize(src, dest, Size(), scale, scale, interpolation);
		return dest;
	}

	/*按指定角度旋转，保持原图尺寸*/
	Mat CImgProcessor::rotate(Mat src, double angle, Size dsize, Scalar padding)
	{
		Point center(src.cols / 2, src.rows / 2);
		Mat rotMat = getRotationMatrix2D(center, angle, 1.0);
		Mat dest;
		warpAffine(src, dest, rotMat, dsize, 1, 0, padding);
		return dest;
	}

/*扩充至目标尺寸*/
	Mat CImgProcessor::pad(Mat src, Size destSize, Point topleft, Scalar padding)
	{
		destSize.width = max(src.cols, destSize.width);
		destSize.height = max(src.rows, destSize.height);
		Mat padded;
		// 若位指定起始位置，则置以中心
		if (topleft == Point(-1, -1))
		{
			topleft = Point((destSize.height - src.rows) / 2, (destSize.width - src.cols) / 2);
		}
		int bottom = destSize.height - src.rows - topleft.y;
		int right = destSize.width - src.cols - topleft.x;
		copyMakeBorder(src, padded, topleft.y, bottom, topleft.x, right, BORDER_CONSTANT, padding);
		return padded;
	}

	/*调整亮度 和 对比度*/
	Mat CImgProcessor::contrastAndBrightness(Mat src, float alpha, float beta)
	{
		Mat new_image = Mat::zeros(src.size(), src.type());
		for (int y = 0; y < src.rows; y++)
		{
			for (int x = 0; x < src.cols; x++)
			{
				for (int c = 0; c < 3; c++)
				{
					new_image.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(src.at<Vec3b>(y, x)[c]) + beta);
				}
			}
		}
		return new_image;
	}

	/*获取背景色 */
	Vec3b CImgProcessor::getBgColor(Mat src)
	{
		CPsException exp;
		if (src.channels() < 3)
		{
			throw CPsException("仅接受BGR三通道的图片。");
		}
		Mat gray(src.size(), CV_8UC1);
		cvtColor(src, gray, COLOR_BGR2GRAY);
		threshold(gray, gray, 0, 255, THRESH_BINARY | THRESH_OTSU);

		int cnt = 0;
		vector<long> sum(3);
		for (int y = 0; y < src.rows; y++)
		{
			uchar* ptrGray = gray.ptr<uchar>(y);
			Vec3b* ptrColor = src.ptr<Vec3b>(y);
			for (int x = 0; x < src.cols; x++)
			{
				if (ptrGray[x] == 255)
				{
					cnt++;
					for (int c = 0; c < 3; c++)
					{
						sum[c] += ptrColor[x][c];
					}
				}
			}
		}
		if (cnt > 0)
		{
			Vec3b avg;
			for (int c = 0; c < 3; c++)
			{
				avg[c] = (uchar)(sum[c] / cnt);
			}
			return avg;
		}
		throw CPsException("未能检测出背景色。");;
	}

	/*转为灰度图*/
	Mat CImgProcessor::convert2Gray(Mat src)
	{
		Mat gray(src.size(), CV_8UC1);
		if (src.channels() == 3)
		{
			cvtColor(src, gray, COLOR_BGR2GRAY);
		}
		else
		{
			gray = src;
		}
		return gray;
	}
}