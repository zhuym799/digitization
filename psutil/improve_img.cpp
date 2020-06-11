#include "psutil.h"
namespace psutil
{
	/*创建查找表*/
	Mat CCadreArchImprove::createLookupTable(uchar divideWith)
	{
		Mat table(1, 256, CV_8UC1);
		uchar *p = table.data;
		for (int i = 0; i < 256; ++i)
		{
			p[i] = divideWith * (i / divideWith);
		}
		return table;
	}


	/*自动提升图片质量 仅专应对干部档案扫描件*/
	void CCadreArchImprove::improve(const string & imgPath, Scalar colorPad, int tolerant, int grayThresh, int houghVote, Size blurSize, Size morphSize, Vec3b newbg, bool sharp, const HsvRngBGR bgrRng, const Vec3b standard[3])
	{
		Mat img = imread(imgPath, IMREAD_COLOR);
		img = correctRotation(img, colorPad, tolerant, grayThresh, houghVote);
		//imwrite("D:\\testResult\\rotation.jpg", img);

		Mat gray;
		cvtColor(img, gray, COLOR_BGR2GRAY);
		threshold(gray, gray, grayThresh, 255, THRESH_BINARY | THRESH_OTSU);
		gray = CImproveGray::denoising(gray, gray.size(), grayThresh, blurSize, morphSize);
		imwrite("D:\\testResult\\denoise.jpg", gray);

		Mat clean = mergeClean(img, gray, newbg);
		imwrite("D:\\testResult\\mergeClean.jpg", clean);
		if (sharp)
		{
			sharpBGR(img, bgrRng, standard);
			imwrite("D:\\testResult\\hsv.jpg", img);
		}

	}



	/*替换背景色*/
	Mat CCadreArchImprove::replace_bgColor(Mat src, Vec3b bgrReplacer, int grayThresh)
	{
		if (src.channels() != 3)
		{
			throw CPsException("待编辑图：仅接受BGR三通道的图片。");
		}
		Mat gray = Mat(src.size(), CV_8UC1);
		cvtColor(src, gray, COLOR_BGR2GRAY);

		threshold(gray, gray, grayThresh, 255, THRESH_BINARY);
		return mergeClean(src, gray, bgrReplacer);
	}

	/*颜色空间缩减*/
	Mat CCadreArchImprove::lutReduce(Mat src, uchar divideWith)
	{
		Mat outMat;
		LUT(src, createLookupTable(divideWith), outMat);
		return outMat;
	}

	/*纠偏 以指定颜色填充*/
	Mat CCadreArchImprove::correctRotation(Mat src, Scalar color, int tolerant, int grayThresh, int houghVote)
	{
		Mat gray = CImgProcessor::convert2Gray(src);
		double angle = CImproveGray::getAngleByHough(gray, grayThresh, houghVote, tolerant);
		if (angle != 0)
		{
			if (angle > 45)
			{
				angle = 90 - angle;
			}
			else if (angle < -45)
			{
				angle = 90 + angle;
			}

			//Rotate the image to recover		
			return CImgProcessor::rotate(src, angle, src.size(), color);
		}
		return src;
	}

	/*纠偏 以原图背景色填充*/
	Mat CCadreArchImprove::correctRotation(Mat src, int tolerant, int grayThresh, int houghVote)
	{
		return correctRotation(src, CImgProcessor::getBgColor(src), tolerant, grayThresh, houghVote);
	}

	/*以背景色替代指定区域*/
	void CCadreArchImprove::erase(Mat src, Rect range)
	{
		rectangle(src, range, CImgProcessor::getBgColor(src), FILLED);
	}

	/*按比例缩放*/
	Mat CCadreArchImprove::zoom(Mat src, double scale)
	{
		return CImgProcessor::zoom(src, scale);
	}

	/*缩放至指定尺寸*/
	Mat CCadreArchImprove::resize(Mat src, Size dSize)
	{
		Mat dest;
		cv::resize(src, dest, dSize, 0, 0, INTER_LINEAR);
		return dest;
	}

	///*以指定角度旋转 顺时针为负 保持原尺寸 以原图背景色填充*/
	//Mat CCadreArchImprove::rotate(Mat src, int angle)
	//{
	//	return CCadreArchImprove::rotate(src, angle, CImgProcessor::getBgColor(src), src.size());
	//}

	/*以指定角度旋转 顺时针为负  保持原尺寸 白色填充*/
	Mat CCadreArchImprove::rotate(Mat src, int angle, Scalar padding, Size dSize)
	{
		angle = angle % 360;
		if (angle == 0)
		{
			return src;
		}
		if (dSize == Size())
		{
			dSize = src.size();
		}
		return CImgProcessor::rotate(src, angle, dSize, padding);
	}


	/*调整亮度和对比度*/
	Mat CCadreArchImprove::contrastAndBrightness(Mat src, float alpha, float beta)
	{
		return CImgProcessor::contrastAndBrightness(src, alpha, beta);
	}

	/*二值化图片*/
	Mat CCadreArchImprove::binaryzation(Mat src, int grayThresh)
	{
		Mat dest = CImgProcessor::convert2Gray(src);
		threshold(dest, dest, grayThresh, 255, THRESH_BINARY);
		return dest;
	}
	/*反转二值图片的前景与背景*/
	Mat CCadreArchImprove::reverseBinBgFg(Mat binary)
	{
		Mat dest;
		bitwise_not(binary, dest);
		return binary;
	}
	Mat CCadreArchImprove::mergeClean(Mat src, Mat binary, Vec3b newbg)
	{
		if (src.channels() != 3)
		{
			throw CPsException("待编辑图：仅接受BGR三通道的图片。");
		}
		if (binary.channels() > 1)
		{
			throw CPsException("参照图：仅接受灰度图。");
		}
		for (int r = 0; r < binary.rows; r++)
		{
			uchar* ptrGray = binary.ptr<uchar>(r);
			Vec3b* ptrColor = src.ptr<Vec3b>(r);
			for (int c = 0; c < binary.cols; c++)
			{
				if (ptrGray[c] == 255)
				{
					ptrColor[c] = newbg;
				}
			}
		}
		return src;
	}


	Mat CCadreArchImprove::setHSV(Mat srcBGR, uchar h, uchar s, uchar v)
	{
		if (srcBGR.channels() != 3)
		{
			throw CPsException("待编辑图：仅接受BGR三通道的图片。");
		}

		Mat hsv;
		cvtColor(srcBGR, hsv, COLOR_BGR2HSV);
		vector<Mat> channels;
		split(hsv, channels);
		channels[0] = MIN(h, 180); //0— 180
		channels[1] = MIN(s, 255);// s;//0— 255
		channels[2] = MIN(v, 255);// v;//0— 255
		merge(channels, hsv);
		cvtColor(hsv, hsv, COLOR_HSV2BGR);
		return hsv;
	}

	void CCadreArchImprove::sharpBGR(Mat& srcBGR, const HsvRngBGR bgrRng, const Vec3b standard[3])
	{
		if (srcBGR.channels() != 3)
		{
			throw CPsException("待编辑图：仅接受BGR三通道的图片。");
		}

		Mat hsvImg;
		cvtColor(srcBGR, hsvImg, COLOR_BGR2HSV);

		/* color in HSV and BGR : 0-B  1-G  2-R */
		HsvRng colors[] = { bgrRng.bRng ,bgrRng.gRng ,bgrRng.rRng };
		for (int r = 0; r < hsvImg.rows; r++)
		{
			Vec3b* ptrHSV = hsvImg.ptr<Vec3b>(r);
			Vec3b* ptrBGR = srcBGR.ptr<Vec3b>(r);
			for (int c = 0; c < hsvImg.cols; c++)
			{
				for (int colorIdx = 0; colorIdx < 3; colorIdx++)
				{
					bool flagH = false;
					vector<Vec2b> cmpH = colors[colorIdx].hRanges;
					for (size_t idx = 0; idx < cmpH.size(); idx++)
					{
						if (ptrHSV[c][0] >= cmpH[idx][0] && ptrHSV[c][0] <= cmpH[idx][1])
						{
							flagH = true;
							break;
						}
					}

					if (flagH &&  ptrHSV[c][1] >= colors[colorIdx].sRange[0] && ptrHSV[c][1] <= colors[colorIdx].sRange[1] && ptrHSV[c][2] >= colors[colorIdx].vRange[0] && ptrHSV[c][2] <= colors[colorIdx].vRange[1])
					{
						ptrBGR[c] = standard[colorIdx];
						break;
					}
				}
			}
		}
		
	}
	void CCadreArchImprove::textfill(Mat& srcBGR, int grayThresh, Size morph_size, Vec3b padding)
	{
		if (srcBGR.channels() != 3)
		{
			throw CPsException("待编辑图：仅接受BGR三通道的图片。");
		}

		Mat gray;
		cvtColor(srcBGR, gray, COLOR_BGR2GRAY);
		gray = CImproveGray::textfill(gray, grayThresh, morph_size);
		for (int r = 0; r < gray.rows; r++)
		{
			uchar* ptrGray = gray.ptr<uchar>(r);
			Vec3b* ptrBGR = srcBGR.ptr<Vec3b>(r);
			for (int c = 0; c < gray.cols; c++)
			{
				if (ptrGray[c] == 0)
				{
					ptrBGR[c] = padding;
				}
			}
		}
	}
	void CCadreArchImprove::partitionCopyPaste(Mat & srcBGR, Mat refMat, Rect refRng, Point2i target)
	{
		if (srcBGR.channels() != 3)
		{
			throw CPsException("待编辑图：仅接受BGR三通道的图片。");
		}
		if (refMat.channels() != 3)
		{
			throw CPsException("参照图：仅接受BGR三通道的图片。");
		}

		for(int r = 0 ; r < refRng.height; r ++)		
		{
			Vec3b* ptrRef = refMat.ptr<Vec3b>(refRng.y + r);
			Vec3b* ptrColor = srcBGR.ptr<Vec3b>(target.y + r);
			for(int c = 0 ; c < refRng.width; c ++)			
			{
				ptrColor[target.x + c] = ptrRef[refRng.x + c];
			}
		}
	}
	void CCadreArchImprove::replaceColors(Mat& srcBGR, Rect range, Vec3b (* colors)[2])
	{
		if (srcBGR.channels() != 3)
		{
			throw CPsException("待编辑图：仅接受BGR三通道的图片。");
		}

		if (colors != nullptr)
		{
			for (int r = 0; r < range.height; r++)
			{
				Vec3b* ptrColor = srcBGR.ptr<Vec3b>(range.y + r);
				for (int c = 0; c < range.width; c++)
				{
					int len = sizeof(colors) / sizeof(colors[0]);
					for (int idx = 0; idx < len; idx++)
					{
						if (ptrColor[range.x + c] == colors[idx][0])
						{
							ptrColor[range.x + c] = colors[idx][1];
						}
					}
				}
			}
		}
	}
}
