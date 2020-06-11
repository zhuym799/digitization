// psutil.h: 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件
//

#ifndef __PS_UTIL_H
#define __PS_UTIL_H
#endif

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>

// 在此处引用程序需要的其他标头
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
	/*默认二值化阈值*/
	const int PS_DEFAULT_BINARY_THRESHOLD = 150;
	/*霍夫成线投票数*/
	const int PS_DEFAULT_HOUGH_VOTE = 300;
	/*容忍的歪斜角度*/
	const int PS_DEFAULT_TOLERANT_ROTATION = 1;
	/*默认标准BGR值*/
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

		/************************* BGR 三原色 在HSV空间下的取值范围 ************************************/
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
			/*离散傅里叶*/
			static Mat dftImage(Mat src);

			/*缩放*/
			static Mat zoom(Mat src, double scale);

			/*按指定角度旋转，顺时针为负*/
			static Mat rotate(Mat src, double angle, Size dsize, Scalar padding);

			///*获取前景的最大外框*/
			/*static vector<Rect> getEdgeRect(Mat src, int grayThresh, Size blurSize);*/

			/*填充指定颜色至目标尺寸*/
			static Mat pad(Mat src, Size destSize, Point topleft = Point(-1, -1), Scalar padding = Scalar::all(0));

			/*调整亮度和对比度*/
			static Mat contrastAndBrightness(Mat src, float alpha, float beta);

			/*获取BGR格式的背景色*/
			static Vec3b getBgColor(Mat srcBGR);

			/*转为灰度图*/
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
			 @brief：(形态学闭运算)去噪
			 @param src : the source mat
			 @param morph_shape: 内核形状
			 @param morphSize: 内核大小
			 @param anchor: 锚点
			 @return 结果Mat
			 */
			static Mat denoising(Mat src, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, int morph_shape = MORPH_RECT, Size morphSize = Size(3, 3), Point anchor = Point(-1, -1));

		public:

			/*根据霍夫线获取角度*/
			static float getAngleByHough(Mat srcGray, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, int houghVote = PS_DEFAULT_HOUGH_VOTE, int tolerant = PS_DEFAULT_TOLERANT_ROTATION);

			/*
			 @brief：获取文字区域
			 @param src: the source mat
			 @param grayThresh: 灰度阈值
			 @param blurSize: 滤波器大小
			 @return 结果Mat
			*/
			static PS_API vector<Rect> findTextAreas(Mat src, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, Size blurSize = Size(3, 3));

			/*
			 @brief：(形态学闭运算)去噪 并填充至指定尺寸
			 @param src : the source mat
			 @param textArea: 指定文本区域
			 @param dSize: 目标尺寸
			 @param morphSize： 内核大小
			 @return 结果Mat
			 */
			static PS_API Mat denoising(Mat src, Rect textArea, Size dSize, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, Size morphSize = Size(3, 3));
			/*
			 @brief：(形态学闭运算)去噪 并填充至指定尺寸
			 @param src : the source mat
			 @param dSize: 目标尺寸
			 @param grayThresh: 灰度阈值
			 @param blurSize: 滤波器大小
			 @param morphSize： 内核大小
			 @return 结果Mat
			 */
			static PS_API Mat denoising(Mat src, Size dSize, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, Size blurSize = Size(3, 3), Size morphSize = Size(3, 3));

			/**
			 @brief：截取文本区域，并填充至指定尺寸
			 @param src : the source mat
			 @param textArea: 指定文本区域
			 @param dSize: 目标尺寸
			 @return 结果Mat
			*/
			static PS_API Mat excerptTextArea(Mat src, Rect textArea, Size dSize);

			static PS_API Mat textfill(Mat src, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, Size morph_size = Size(3, 3));

			/**
			 @brief：白底黑字的二值图基础上替换颜色（fore=true为 替换前景色）
			 @param src : the source mat
			 @param range: 指定区域
			 @param colors: 颜色对 （0待替换  1目标）
			 @return 结果三通道BGR图
			*/
			static PS_API Mat replaceColors(Mat srcBin,Rect range, Vec3b(*colors)[2]);
		};


		class CCadreArchImprove
		{
		private:
			/*创建查找表*/
			static Mat createLookupTable(uchar divideWith);
			
		public:
			/*自动提升图片质量 仅专应对干部档案扫描件*/
			static PS_API void improve(const string& imgPath, Scalar colorPad= Scalar::all(255), int tolerant = PS_DEFAULT_TOLERANT_ROTATION, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, int houghVote = PS_DEFAULT_HOUGH_VOTE, Size blurSize = Size(3, 3), Size morphSize = Size(3, 3), Vec3b newbg = Vec3b(255, 255, 255),bool sharp = false, const HsvRngBGR bgrRng = PS_DEFAULT_BGR_RANGE_IN_HSV, const Vec3b standard[3] = PS_STANDARD_BGR);
			/*替换 背景色*/
			static PS_API Mat replace_bgColor(Mat src, Vec3b bgrReplacer = Vec3b(255, 255, 255), int grayThresh = PS_DEFAULT_BINARY_THRESHOLD);
			/*颜色控件缩减*/
			static PS_API Mat lutReduce(Mat src, uchar divideWith);

			/**
			 @brief: 纠偏
			 @param src : the source mat
			 @param color: 填充色
			 @param maxAngle: 角度阈值
			 @param grayThresh: 二值化灰度阈值
			 @param houghVote: 霍夫线投票数
			 @return 结果Mat
			*/
			static PS_API Mat correctRotation(Mat src, Scalar color, int tolerant = PS_DEFAULT_TOLERANT_ROTATION, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, int houghVote = PS_DEFAULT_HOUGH_VOTE);
			/**
			@brief: 纠偏
			@param src : the source mat
			@param maxAngle: 角度阈值
			@param grayThresh: 二值化灰度阈值
			@param houghVote: 霍夫线投票数
			@return 结果Mat
			*/
			static PS_API Mat correctRotation(Mat src, int tolerant = PS_DEFAULT_TOLERANT_ROTATION, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, int houghVote = PS_DEFAULT_HOUGH_VOTE);

			/**
			 @brief：擦除指定区域(填充白色)
			 @param  src: the source mat
			 @param range：区域
			 @return 结果Mat
			*/
			static PS_API void erase(Mat src, Rect range);

			/*
			 @brief：按比例缩放
			 @param src: the source mat
			 @param scale: 比例
			 @return 结果Mat
			*/
			static PS_API Mat zoom(Mat src, double scale);

			/*
			 @brief：缩放至指定尺寸
			 @param src: the source mat
			 @param dSize: 目标尺寸
			 @return 结果Mat
			*/
			static PS_API Mat resize(Mat src, Size dSize);

			/*
			 @brief：不改变尺寸的情况下，由中心按指定角度旋转
			 @param src: the source mat
			 @param angle: 角度（顺时针为负）
			 @return 结果Mat
			*/		
			static PS_API Mat rotate(Mat src, int angle, Scalar padding = Scalar::all(255), Size dSize = Size());


			static PS_API Mat contrastAndBrightness(Mat src, float alpha, float beta);

			static PS_API Mat binaryzation(Mat src, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD);

			/*反转二值图片的前景与背景*/
			static PS_API Mat reverseBinBgFg(Mat binary);

			/*
			 @brief：将原图与清晰图合并，并替换背景色
			 @param src : the source mat
			 @param grayClean: 清晰的灰度图
			 @param newbg: 新背景色
			 @return 结果Mat
			 */
			static PS_API Mat mergeClean(Mat srcBGR, Mat grayClean, Vec3b newbg = Vec3b(255,255,255 ));

			/*调整色调（Hue）、饱和度（Saturation）、亮度（Value）*/
			static PS_API Mat setHSV(Mat srcBGR, uchar h, uchar s, uchar v);
		
			/*
			 @brief：增强3色 BGR
			 @param srcBGR : 待编辑3通道BGR图
			 @param HsvRngBGR: 在HSV空间，B-G-R三色在H-S-V分量的取值范围
			 @param standard: 在BGR空间，指定的B-G-R色值标准
			 */
			static PS_API void sharpBGR(Mat& srcBGR, const HsvRngBGR bgrRng,const Vec3b standard[3] = PS_STANDARD_BGR);
			
			/*
			 @brief：连通文字（断点 断线等处理）
			 @param srcBGR : 待编辑3通道BGR图
			 @param grayThresh: 二值化阈值
			 @param morph_size: 形态学内核大小，默认3*3
			 @param padding：用于填补缺口处的颜色值,默认黑色
			 */
			static PS_API void textfill(Mat& srcBGR, int grayThresh = PS_DEFAULT_BINARY_THRESHOLD, Size morph_size = Size(3,3), Vec3b padding = (0,0,0));

			/*
			 @brief：从参照图拷贝局部，复制至待编辑图
			 @param srcBGR : 待编辑3通道BGR图
			 @param refMat: 参照图：3通道BGR图
			 @param range: 待拷贝的区域
			 @param target：待粘贴的起始点（位于待编辑图上的）
			 */
			static PS_API void partitionCopyPaste(Mat& srcBGR, const Mat refMat, Rect range, Point2i target);

			/**
			 @brief：替换颜色
			 @param src : the source mat
			 @param range: 指定区域
			 @param colors: 颜色对 （0待替换  1目标）
			 @return 结果三通道BGR图
			*/
			static PS_API void replaceColors(Mat& srcBGR, Rect range, Vec3b(*colors)[2]);
		};
		class CMatCoder
		{

		public:
			
			static PS_API Mat Buffer2Mat(vector<uchar> buf);
			/*
		@bried: 将Mat对象转为jpg格式（质量95）的二进制串
		@param [in] img:待操作Mat对象
		@param [out] buf:缓存二进制串	
		@param quality: 质量(default 95)
		@return 转换成败结果
		*/
			static PS_API bool Mat2Jpeg(Mat img, vector<uchar>& buf, int quality = 95);
			/*
			@bried: 将Mat对象转为png格式二进制串
			@param [in] img:待操作Mat对象
			@param [out] buf:缓存二进制串	
			@param quality: 质量(default 3)
			*/
			static PS_API bool Mat2Png(Mat img, vector<uchar>& buf, int quality=3);
			
			
		
		};

#ifdef __cplusplus
	}
#endif
}