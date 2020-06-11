#include "psutil.h"
#include <list>
namespace psutil
{
	/**
	 @brief get angle by hough lines
	 @param src : the source mat
	 @param houghVote: vote quantity for hough lines
	 @param tolerant: ����ĽǶ�ƫ��
	 @return angle in double( + - )
	*/
	float CImproveGray::getAngleByHough(Mat src, int grayThresh, int houghVote, int tolerant)
	{
		if (grayThresh > 255 || grayThresh < 0)
		{
			throw CPsException("��ֵ����ֵ��ȡֵ��ΧΪ��0~255");
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

	/*�������������ľ��α߽�*/
	vector<Rect> CImproveGray::findTextAreas(Mat srcGray, int grayThresh, Size blurSize)
	{
		// ���������ľ��α߽�
		vector<Rect> boundRect;

		/// �ҵ�����
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
			/// ����αƽ����� + ��ȡ���α߽��
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

			/*ͨ�������˲��ߴ� ���ƾ��ε����� �Ի�ȡ���ܵ�������������*/
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

	/*����������ֱ�ߣ��ҳ��Ƕ�*/
	float CImproveGray::findoutTheta(vector<float> lineThetas, int tolerant)
	{
		float  theta = findThetaFromPerpendiculars(lineThetas, tolerant);
		if (theta == 0)
		{
			theta = findThetaFromObliques(lineThetas, tolerant);
		}
		return theta;
	}

	/*���ݴ�ֱ�߲��ҽǶ�*/
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

	/*����б�߲��ҽǶ�*/
	float CImproveGray::findThetaFromObliques(vector<float> lineThetas, int tolerant)
	{
		float theta = 0;
		size_t cnt = 0; // cnt of thetas 
		float tolerantPi = (float)(tolerant * CV_PI / 180); // �����ƫ��
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
	@brief: (��̬ѧ������)ȥ�����
	@param src: ԴMat
	@param morph_shape: �ں���״
	@param morphSize: �ں˴�С
	@param anchor: ê��(Ĭ�����ں�����λ��)
	@return: ���Mat
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
	@brief: (��̬ѧ������)ȥ����㣬��������ָ��Ŀ��ߴ磬�԰�ɫ���
	@param src: ԴMat
	@param textArea: ԴͼƬ�ϵ�����������ο�
	@param dSize: Ŀ��Mat�ߴ�
	@param morphSize: ��̬ѧ�ں˴�С
	@return: ���Mat
	*/
	Mat CImproveGray::denoising(Mat src, Rect textArea, Size dSize, int grayThresh, Size morphSize)
	{
		Mat dest = denoising(src(textArea), grayThresh, MORPH_RECT, morphSize);
		return CImgProcessor::pad(dest, dSize, textArea.tl(), Scalar::all(255));
	}

	/**
	@brief: (��̬ѧ������)ȥ����㣬��������ָ��Ŀ��ߴ磬�԰�ɫ���
	@param src: ԴMat
	@param dSize: Ŀ��Mat�ߴ�
	@param grayThresh : ��ֵ����ֵ
	@param blur: �˲����Ĵ�С
	@param morphSize: ��̬ѧ�ں˴�С
	@return: ���Mat
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
	@brief����ȡ�ı����򣬲������ָ���ߴ�
	@param src : the source mat
	@param textArea: ָ���ı�����
	@param dSize: Ŀ��ߴ�
	@return ���Mat
	*/
	Mat CImproveGray::excerptTextArea(Mat src, Rect textArea, Size dSize)
	{
		return CImgProcessor::pad(src(textArea), dSize, textArea.tl(), Scalar::all(255));
	}
	Mat CImproveGray::textfill(Mat src, int grayThresh, Size morph_size)
	{
		Mat dest;
		threshold(src, dest, grayThresh, 255, THRESH_BINARY | THRESH_OTSU);
		// ������(Opening)
		Mat element = getStructuringElement(MORPH_RECT, morph_size);
		morphologyEx(dest, dest, MORPH_OPEN, element);
		return dest;
	}

	
	Mat CImproveGray::replaceColors(Mat srcBin, Rect range, Vec3b(*colors)[2])
	{
		if (srcBin.channels() != 1)
		{
			throw CPsException("���༭ͼ�������ܵ�ͨ���Ķ�ֵͼƬ��");
		}
		Mat dest;
		cvtColor(srcBin, dest, COLOR_GRAY2BGR);

		CCadreArchImprove::replaceColors(dest, range, colors);
		return dest;
	}
}


