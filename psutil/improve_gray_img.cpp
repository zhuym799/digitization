#include "psutil.h"
#include <list>
namespace psutil
{
	/**
	 @brief get angle by hough lines
	 @param src : the source mat
	 @param houghVote: vote quantity for hough lines
	 @param tolerant: 容许的角度偏差
	 @return angle in double( + - )
	*/
	float CImproveGray::getAngleByHough(Mat src, int grayThresh, int houghVote, int tolerant)
	{
		if (grayThresh > 255 || grayThresh < 0)
		{
			throw CPsException("二值化阈值的取值范围为：0~255");
		}
		float angle = 0;

		// Dft image		
		Mat magMat = CImgProcessor::dftImage(src);
		Mat magImg;
		magMat.convertTo(magImg, CV_8UC1, 255, 0);
		//imwrite("D:\\testResult\\dftImage.jpg", magImg);

		// Turn into binary image	
		threshold(magImg, magImg, grayThresh, 255, THRESH_BINARY);
		//imwrite("D:\\testResult\\before_hough.jpg", magImg);
		vector<Vec2f> lines; // will hold the results of the detection
		size_t qty = 0;
		while (houghVote > 0 && houghVote < MAX(magImg.cols, magImg.rows))
		{
			// Standard Hough Line Transform	
			HoughLines(magImg, lines, 1, CV_PI / 180, houghVote, 0, 0); // runs the actual detection
			qty = lines.size();

			if (qty >= 3)
			{
				break;
			}
			houghVote -= 10;
		}

		if (qty > 0)
		{
			qty = MIN(qty, 3);
			vector<float> thetas;
			for (size_t i = 0; i < qty; i++)
			{
				float th = lines[i][1];
				if (th != 0 && th != (float)(CV_PI / 2))
				{
					thetas.push_back(th);
				}
			}
			if (thetas.size() > 0)
			{
				angle = findoutTheta(thetas, tolerant);
				if (angle != 0 && angle != (float)(CV_PI / 2))
				{
					angle = atan(src.rows*tan(angle) / src.cols);
					angle = (float)(angle * 180 / CV_PI);
				}
			}
		}
		return angle;
	}

	/*查找文字轮廓的矩形边界*/
	vector<Rect> CImproveGray::findTextAreas(Mat srcGray, int grayThresh, Size blurSize)
	{
		// 文字轮廓的矩形边界
		vector<Rect> boundRect;

		/// 找到轮廓
		int maxW = MIN(srcGray.rows, srcGray.cols);
		Mat blurMat;
		while (blurSize.width > 0 && blurSize.width < maxW)
		{
			vector<vector<Point>> contours;
			vector<Vec4i> hierarchy;
			blur(srcGray, blurMat, blurSize);
			imwrite("D:\\testResult\\blur" + to_string(blurSize.width) + ".jpg", blurMat);
			threshold(blurMat, blurMat, grayThresh, 255, THRESH_BINARY);
			imwrite("D:\\testResult\\before_drawContours" + to_string(blurSize.width) + ".jpg", blurMat);

			findContours(blurMat, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
			/// 多边形逼近轮廓 + 获取矩形边界框
			vector<vector<Point> > contours_poly(contours.size());
			for (size_t i = 0; i < contours.size(); i++)
			{				
				approxPolyDP(Mat(contours[i]), contours_poly[i], 1, true);
				Rect binding = boundingRect(Mat(contours_poly[i]));
				if (binding.width > blurMat.cols * 2 / 3)
				{
					boundRect.push_back(binding);
				}				
			}

			/*通过调整滤波尺寸 控制矩形的数量 以获取可能的文字轮廓区域*/
			int h = 0;
			for (size_t i = 0; i < boundRect.size(); i++)
			{
				h += boundRect[i].height;
			}

			if (h > blurMat.rows * 2 / 3)
			{
				// draw bound rects
				Mat drawing(blurMat.size(), CV_8UC3, Scalar::all(255));
				for (size_t i = 0; i < boundRect.size(); i++) // contours[0] is external
				{
					Scalar color = Scalar(rand() % 255, rand() % 255, rand() % 255);
					rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
				}
				imwrite("D:\\testResult\\drawContours" + to_string(blurSize.width) + ".jpg", drawing);
				break;
			}
			blurSize.width += 10;
			blurSize.height += 10;
		}
		return boundRect;
	}

	/*分析霍夫曼直线，找出角度*/
	float CImproveGray::findoutTheta(vector<float> lineThetas, int tolerant)
	{
		float  theta = findThetaFromPerpendiculars(lineThetas, tolerant);
		if (theta == 0)
		{
			theta = findThetaFromObliques(lineThetas, tolerant);
		}
		return theta;
	}

	/*根据垂直线查找角度*/
	float CImproveGray::findThetaFromPerpendiculars(vector<float> lineThetas, int tolerant)
	{
		float theta = 0;

		const float degress45 = (float)(CV_PI / 4);
		const float degree90 = (float)(CV_PI / 2);
		float tolerantPi = (float)(tolerant * CV_PI / 180);
		size_t cnt = 0; // cnt of thetas

		for (size_t i = 0; i < lineThetas.size(); i++)
		{
			float thetaI = lineThetas[i];
			if (thetaI == 0 || thetaI == degree90)
			{
				continue;
			}
			for (size_t j = i + 1; j < lineThetas.size(); j++)
			{
				float thetaJ = lineThetas[j];
				if (thetaJ == 0 || thetaJ == degree90)
				{
					break;
				}
				if (abs(abs(thetaI - thetaJ) - degree90) < tolerantPi)
				{
					theta += MIN(thetaI, thetaJ);
					cnt++;
					break;
				}
			}
		}

		if (cnt > 0)
		{
			theta = theta / cnt;
			//theta = theta < degress45 ? theta : theta - CV_PI / 2;
			theta = theta < degree90 ? theta : (float)(theta - CV_PI);
		}
		return theta;
	}

	/*根据斜线查找角度*/
	float CImproveGray::findThetaFromObliques(vector<float> lineThetas, int tolerant)
	{
		float theta = 0;
		size_t cnt = 0; // cnt of thetas 
		float tolerantPi = (float)(tolerant * CV_PI / 180); // 允许的偏差
		for (size_t i = 0; i < lineThetas.size(); i++)
		{
			if (abs(lineThetas[i]) <= tolerantPi || (float)(abs(lineThetas[i] - CV_PI / 2) <= tolerantPi))
			{
				continue;
			}
			theta += lineThetas[i];
			cnt++;
		}
		if (cnt > 0)
		{
			theta = theta / cnt;
			theta = theta < (float)(CV_PI / 2) ? theta : (float)(theta - CV_PI);
		}
		return theta;
	}

	/**
	@brief: (形态学闭运算)去除噪点
	@param src: 源Mat
	@param morph_shape: 内核形状
	@param morphSize: 内核大小
	@param anchor: 锚点(默认在内核中心位置)
	@return: 结果Mat
	*/
	Mat CImproveGray::denoising(Mat src, int grayThresh, int morph_shape, Size morph_size, Point anchor)
	{
		Mat dest;
		threshold(src, dest, grayThresh, 255, THRESH_BINARY);
		Mat element = getStructuringElement(morph_shape, morph_size, anchor);
		morphologyEx(dest, dest, MORPH_CLOSE, element);
		return dest;
	}

	/**
	@brief: (形态学闭运算)去除噪点，并扩充至指定目标尺寸，以白色填充
	@param src: 源Mat
	@param textArea: 源图片上的文字区域矩形框
	@param dSize: 目标Mat尺寸
	@param morphSize: 形态学内核大小
	@return: 结果Mat
	*/
	Mat CImproveGray::denoising(Mat src, Rect textArea, Size dSize, int grayThresh, Size morphSize)
	{
		Mat dest = denoising(src(textArea), grayThresh, MORPH_RECT, morphSize);
		return CImgProcessor::pad(dest, dSize, textArea.tl(), Scalar::all(255));
	}

	/**
	@brief: (形态学闭运算)去除噪点，并扩充至指定目标尺寸，以白色填充
	@param src: 源Mat
	@param dSize: 目标Mat尺寸
	@param grayThresh : 二值化阈值
	@param blur: 滤波器的大小
	@param morphSize: 形态学内核大小
	@return: 结果Mat
	*/
	Mat CImproveGray::denoising(Mat srcGray, Size dSize, int grayThresh, Size blurSize, Size morphSize)
	{
		Mat clean;
		vector<Rect> rects = findTextAreas(srcGray, grayThresh, blurSize);
		if (rects.size() > 0)
		{
			clean = Mat(srcGray.size(), CV_8UC1, Scalar::all(255));
			for (size_t i = 0; i < rects.size(); i++)
			{
				srcGray(rects[i]).copyTo(clean(rects[i]));
			}
		}
		else
		{
			clean = srcGray;
		}
		imwrite("D:\\testResult\\clean.jpg", clean);
		Mat dest = denoising(clean, grayThresh, MORPH_RECT, morphSize);
		if (dest.size() != dSize)
		{
			dest = CImgProcessor::pad(dest, dSize, Point(-1, -1), Scalar::all(255));
		}
		return dest;
	}

	/**
	@brief：截取文本区域，并填充至指定尺寸
	@param src : the source mat
	@param textArea: 指定文本区域
	@param dSize: 目标尺寸
	@return 结果Mat
	*/
	Mat CImproveGray::excerptTextArea(Mat src, Rect textArea, Size dSize)
	{
		return CImgProcessor::pad(src(textArea), dSize, textArea.tl(), Scalar::all(255));
	}
	Mat CImproveGray::textfill(Mat src, int grayThresh, Size morph_size)
	{
		Mat dest;
		threshold(src, dest, grayThresh, 255, THRESH_BINARY | THRESH_OTSU);
		// 开运算(Opening)
		Mat element = getStructuringElement(MORPH_RECT, morph_size);
		morphologyEx(dest, dest, MORPH_OPEN, element);
		return dest;
	}

	
	Mat CImproveGray::replaceColors(Mat srcBin, Rect range, Vec3b(*colors)[2])
	{
		if (srcBin.channels() != 1)
		{
			throw CPsException("待编辑图：仅接受单通道的二值图片。");
		}
		Mat dest;
		cvtColor(srcBin, dest, COLOR_GRAY2BGR);

		CCadreArchImprove::replaceColors(dest, range, colors);
		return dest;
	}
}


