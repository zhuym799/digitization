// psutil.h: ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// ��Ŀ�ض��İ����ļ�
//

#ifndef __PS_UTIL_H
#define __PS_UTIL_H
#endif

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ�ļ����ų�����ʹ�õ�����
// Windows ͷ�ļ�
#include <windows.h>

// �ڴ˴����ó�����Ҫ��������ͷ
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

#ifdef PS_API_EXPORTS  
#define PS_API __declspec(dllexport)   
#else  
#define PS_API __declspec(dllimport)   
#endif

namespace psutil
{
	/*Ĭ�϶�ֵ����ֵ*/
	const int PS_DEFAULT_BINARY_THRESHOLD = 150;
	/*�������ͶƱ��*/
	const int PS_DEFAULT_HOUGH_VOTE = 300;
	/*���̵���б�Ƕ�*/
	const int PS_DEFAULT_TOLERANT_ROTATION = 1;
	/*Ĭ�ϱ�׼BGRֵ*/
	const Vec3b PS_STANDARD_BGR[3] = { Vec3b(255,0,0),Vec3b(0,255,0), Vec3b(0,0,255) };
#ifdef __cplusplus
	extern "C" {
#endif
		class CPsException : public std::exception
		{
		private:
			exception m_inner;
			string m_msg;
		public:
			CPsException();
			CPsException(char const* const msg);
			CPsException(exception inner, char const* const msg);
			exception getInnerExp();
			string getMsg();
		};
	
		class HsvRng
		{
		public:
			vector<Vec2b> hRanges;
			Vec2b sRange;
			Vec2b vRange;	
			HsvRng(vector<Vec2b> h, Vec2b s , Vec2b v):hRanges(h), sRange(s), vRange(v)
			{
			}
		};	

		class HsvRngBGR
		{
		public:
			HsvRng bRng ;
			HsvRng gRng ;
			HsvRng rRng ;
			HsvRngBGR(HsvRng b, HsvRng g, HsvRng r) :bRng(b), gRng(g), rRng(r)
			{
			}
		};

		/************************* BGR ��ԭɫ ��HSV�ռ��µ�ȡֵ��Χ ************************************/
		/*Blue*/	
		const HsvRng PS_DEFAULT_BLUE_RANGE ({ Vec2b(100,128) }, Vec2b(100, 255), Vec2b(100, 255));
		/*Green*/
		const HsvRng PS_DEFAULT_GREEN_RANGE ({ Vec2b(35,80) }, Vec2b(80, 255), Vec2b(80, 255));
		/*Red*/	
		const HsvRng PS_DEFAULT_RED_RANGE({Vec2b(0,10),Vec2b(165,180)},Vec2b(150, 255),Vec2b(150, 255));		
		const HsvRngBGR PS_DEFAULT_BGR_RANGE_IN_HSV (PS_DEFAULT_BLUE_RANGE, PS_DEFAULT_GREEN_RANGE, PS_DEFAULT_RED_RANGE);

		/**
		* @brief : image processor
		*/
		class CImgProcessor
		{
		private:
			static vector<Rect> getMostPossibleContours(vector<Rect> rects, int error = 10);
			static Rect buildOutterRect(vector<Rect> rects, Point2i brThreshold);
		public:
			/*��ɢ����Ҷ*/
			static Mat dftImage(Mat src);

			/*����*/
			static Mat zoom(Mat src, double scale);

			/*��ָ���Ƕ���ת��˳ʱ��Ϊ��*/
			static Mat rotate(Mat src, double angle, Size dsize, Scalar padding);

			///*��ȡǰ����������*/
			/*static vector<Rect> getEdgeRect(Mat src, int grayThresh, Size blurSize);*/

			/*���ָ����ɫ��Ŀ��ߴ�*/
			static Mat pad(Mat src, Size destSize, Point topleft = Point(-1, -1), Scalar padding = Scalar::all(0));

			/*�������ȺͶԱȶ�*/
			static Mat contrastAndBrightness(Mat src, float alpha, float beta);

			/*��ȡBGR��ʽ�ı���ɫ*/
			static Vec3b getBgColor(Mat srcBGR);

			/*תΪ�Ҷ�ͼ*/
			static Mat convert2Gray(Mat srcBGR);
		};

		/**
		* @brief : improve gray image
		*/
		class  CImproveGray
		{
		private:
			static float findoutTheta(vector<float> lines, int tolerant = PS_DEFAULT_TOLERANT_ROTATION);
			static float findThetaFromObliques(vector<float> lineThetas, int tolerant = PS_DEFAULT_TOLERANT_ROTATION);
			static float findThetaFromPerpendiculars(vector<float> lineThetas, int tolerant = PS_DEFAULT_TOLERANT_ROTATION);
			/*
			 @brief��(��̬ѧ������)ȥ��
			 @param src : the source mat
			 @param morph_shape: �ں���״
			 @param morphSize: �ں˴�С
			 @param anchor: ê��
			 @return ���Mat
			 */
			static Mat denoising(Mat src, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, int morph_shape = MORPH_RECT, Size morphSize = Size(3, 3), Point anchor = Point(-1, -1));

		public:

			/*���ݻ����߻�ȡ�Ƕ�*/
			static float getAngleByHough(Mat srcGray, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, int houghVote = PS_DEFAULT_HOUGH_VOTE, int tolerant = PS_DEFAULT_TOLERANT_ROTATION);

			/*
			 @brief����ȡ��������
			 @param src: the source mat
			 @param grayThresh: �Ҷ���ֵ
			 @param blurSize: �˲�����С
			 @return ���Mat
			*/
			static PS_API vector<Rect> findTextAreas(Mat src, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, Size blurSize = Size(3, 3));

			/*
			 @brief��(��̬ѧ������)ȥ�� �������ָ���ߴ�
			 @param src : the source mat
			 @param textArea: ָ���ı�����
			 @param dSize: Ŀ��ߴ�
			 @param morphSize�� �ں˴�С
			 @return ���Mat
			 */
			static PS_API Mat denoising(Mat src, Rect textArea, Size dSize, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, Size morphSize = Size(3, 3));
			/*
			 @brief��(��̬ѧ������)ȥ�� �������ָ���ߴ�
			 @param src : the source mat
			 @param dSize: Ŀ��ߴ�
			 @param grayThresh: �Ҷ���ֵ
			 @param blurSize: �˲�����С
			 @param morphSize�� �ں˴�С
			 @return ���Mat
			 */
			static PS_API Mat denoising(Mat src, Size dSize, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, Size blurSize = Size(3, 3), Size morphSize = Size(3, 3));

			/**
			 @brief����ȡ�ı����򣬲������ָ���ߴ�
			 @param src : the source mat
			 @param textArea: ָ���ı�����
			 @param dSize: Ŀ��ߴ�
			 @return ���Mat
			*/
			static PS_API Mat excerptTextArea(Mat src, Rect textArea, Size dSize);

			static PS_API Mat textfill(Mat src, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, Size morph_size = Size(3, 3));

			/**
			 @brief���׵׺��ֵĶ�ֵͼ�������滻��ɫ��fore=trueΪ �滻ǰ��ɫ��
			 @param src : the source mat
			 @param range: ָ������
			 @param colors: ��ɫ�� ��0���滻  1Ŀ�꣩
			 @return �����ͨ��BGRͼ
			*/
			static PS_API Mat replaceColors(Mat srcBin,Rect range, Vec3b(*colors)[2]);
		};


		class CCadreArchImprove
		{
		private:
			/*�������ұ�*/
			static Mat createLookupTable(uchar divideWith);
			
		public:
			/*�Զ�����ͼƬ���� ��רӦ�Ըɲ�����ɨ���*/
			static PS_API void improve(const string& imgPath, Scalar colorPad= Scalar::all(255), int tolerant = PS_DEFAULT_TOLERANT_ROTATION, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, int houghVote = PS_DEFAULT_HOUGH_VOTE, Size blurSize = Size(3, 3), Size morphSize = Size(3, 3), Vec3b newbg = Vec3b(255, 255, 255),bool sharp = false, const HsvRngBGR bgrRng = PS_DEFAULT_BGR_RANGE_IN_HSV, const Vec3b standard[3] = PS_STANDARD_BGR);
			/*�滻 ����ɫ*/
			static PS_API Mat replace_bgColor(Mat src, Vec3b bgrReplacer = Vec3b(255, 255, 255), int grayThresh = PS_DEFAULT_BINARY_THRESHOLD);
			/*��ɫ�ؼ�����*/
			static PS_API Mat lutReduce(Mat src, uchar divideWith);

			/**
			 @brief: ��ƫ
			 @param src : the source mat
			 @param color: ���ɫ
			 @param maxAngle: �Ƕ���ֵ
			 @param grayThresh: ��ֵ���Ҷ���ֵ
			 @param houghVote: ������ͶƱ��
			 @return ���Mat
			*/
			static PS_API Mat correctRotation(Mat src, Scalar color, int tolerant = PS_DEFAULT_TOLERANT_ROTATION, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, int houghVote = PS_DEFAULT_HOUGH_VOTE);
			/**
			@brief: ��ƫ
			@param src : the source mat
			@param maxAngle: �Ƕ���ֵ
			@param grayThresh: ��ֵ���Ҷ���ֵ
			@param houghVote: ������ͶƱ��
			@return ���Mat
			*/
			static PS_API Mat correctRotation(Mat src, int tolerant = PS_DEFAULT_TOLERANT_ROTATION, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, int houghVote = PS_DEFAULT_HOUGH_VOTE);

			/**
			 @brief������ָ������(����ɫ)
			 @param  src: the source mat
			 @param range������
			 @return ���Mat
			*/
			static PS_API void erase(Mat src, Rect range);

			/*
			 @brief������������
			 @param src: the source mat
			 @param scale: ����
			 @return ���Mat
			*/
			static PS_API Mat zoom(Mat src, double scale);

			/*
			 @brief��������ָ���ߴ�
			 @param src: the source mat
			 @param dSize: Ŀ��ߴ�
			 @return ���Mat
			*/
			static PS_API Mat resize(Mat src, Size dSize);

			/*
			 @brief�����ı�ߴ������£������İ�ָ���Ƕ���ת
			 @param src: the source mat
			 @param angle: �Ƕȣ�˳ʱ��Ϊ����
			 @return ���Mat
			*/		
			static PS_API Mat rotate(Mat src, int angle, Scalar padding = Scalar::all(255), Size dSize = Size());


			static PS_API Mat contrastAndBrightness(Mat src, float alpha, float beta);

			static PS_API Mat binaryzation(Mat src, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD);

			/*��ת��ֵͼƬ��ǰ���뱳��*/
			static PS_API Mat reverseBinBgFg(Mat binary);

			/*
			 @brief����ԭͼ������ͼ�ϲ������滻����ɫ
			 @param src : the source mat
			 @param grayClean: �����ĻҶ�ͼ
			 @param newbg: �±���ɫ
			 @return ���Mat
			 */
			static PS_API Mat mergeClean(Mat srcBGR, Mat grayClean, Vec3b newbg = Vec3b(255,255,255 ));

			/*����ɫ����Hue�������Ͷȣ�Saturation�������ȣ�Value��*/
			static PS_API Mat setHSV(Mat srcBGR, uchar h, uchar s, uchar v);
		
			/*
			 @brief����ǿ3ɫ BGR
			 @param srcBGR : ���༭3ͨ��BGRͼ
			 @param HsvRngBGR: ��HSV�ռ䣬B-G-R��ɫ��H-S-V������ȡֵ��Χ
			 @param standard: ��BGR�ռ䣬ָ����B-G-Rɫֵ��׼
			 */
			static PS_API void sharpBGR(Mat& srcBGR, const HsvRngBGR bgrRng,const Vec3b standard[3] = PS_STANDARD_BGR);
			
			/*
			 @brief����ͨ���֣��ϵ� ���ߵȴ���
			 @param srcBGR : ���༭3ͨ��BGRͼ
			 @param grayThresh: ��ֵ����ֵ
			 @param morph_size: ��̬ѧ�ں˴�С��Ĭ��3*3
			 @param padding�������ȱ�ڴ�����ɫֵ,Ĭ�Ϻ�ɫ
			 */
			static PS_API void textfill(Mat& srcBGR, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, Size morph_size = Size(3,3), Vec3b padding = (0,0,0));

			/*
			 @brief���Ӳ���ͼ�����ֲ������������༭ͼ
			 @param srcBGR : ���༭3ͨ��BGRͼ
			 @param refMat: ����ͼ��3ͨ��BGRͼ
			 @param range: ������������
			 @param target����ճ������ʼ�㣨λ�ڴ��༭ͼ�ϵģ�
			 */
			static PS_API void partitionCopyPaste(Mat& srcBGR, const Mat refMat, Rect range, Point2i target);

			/**
			 @brief���滻��ɫ
			 @param src : the source mat
			 @param range: ָ������
			 @param colors: ��ɫ�� ��0���滻  1Ŀ�꣩
			 @return �����ͨ��BGRͼ
			*/
			static PS_API void replaceColors(Mat& srcBGR, Rect range, Vec3b(*colors)[2]);
		};
		class CMatCoder
		{

		public:
			
			static PS_API Mat Buffer2Mat(vector<uchar> buf);
			/*
		@bried: ��Mat����תΪjpg��ʽ������95���Ķ����ƴ�
		@param [in] img:������Mat����
		@param [out] buf:��������ƴ�	
		@param quality: ����(default 95)
		@return ת���ɰܽ��
		*/
			static PS_API bool Mat2Jpeg(Mat img, vector<uchar>& buf, int quality = 95);
			/*
			@bried: ��Mat����תΪpng��ʽ�����ƴ�
			@param [in] img:������Mat����
			@param [out] buf:��������ƴ�	
			@param quality: ����(default 3)
			*/
			static PS_API bool Mat2Png(Mat img, vector<uchar>& buf, int quality=3);
			
			
		
		};

#ifdef __cplusplus
	}
#endif
}