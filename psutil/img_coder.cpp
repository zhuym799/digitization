#include "psutil.h"
namespace psutil
{
	Mat CMatCoder::Buffer2Mat(vector<uchar> buf)
	{
		return imdecode(buf, CV_LOAD_IMAGE_COLOR);
	}

	bool CMatCoder::Mat2Jpeg(Mat img, vector<uchar>& buf, int quality /*=95*/)
	{
		vector<int> param = vector<int>(2);
		param[0] = CV_IMWRITE_JPEG_QUALITY;
		param[1] = quality;//default(95) 0-100 
		return imencode(".jpg", img, buf, param);
	}

	bool CMatCoder::Mat2Png(Mat img, vector<uchar>& buf, int quality/*=3*/)
	{
		vector<int> param = vector<int>(2);
		param[0] = CV_IMWRITE_PNG_COMPRESSION;
		param[1] = 3;//default(3)  0-9. 
		return imencode(".png", img, buf, param);
	}
}