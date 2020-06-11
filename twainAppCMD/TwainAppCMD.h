/***************************************************************************
* Copyright ?2007 TWAIN Working Group:  
*   Adobe Systems Incorporated, AnyDoc Software Inc., Eastman Kodak Company, 
*   Fujitsu Computer Products of America, JFL Peripheral Solutions Inc., 
*   Ricoh Corporation, and Xerox Corporation.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the TWAIN Working Group nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY TWAIN Working Group ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL TWAIN Working Group BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
***************************************************************************/

/**
 * @file TwainAppCMD.h
 * TWAIN CMD Application.
 * A TWAIN Application communicates with the DSM to acquire images. 
 * The goal of the application is to acquire data from a Source.  
 * However, applications cannot contact the Source directly.  All requests for
 * data, capability information, error information, etc. must be handled 
 * Through the Source Manager.
 * @author JFL Peripheral Solutions Inc.
 * @date April 2007
 */

#ifndef __TWAINAPPCMD_H__
#define __TWAINAPPCMD_H__

#pragma once

//#ifdef _WINDOWS
//  #ifndef __AFXWIN_H__
//    #error include 'stdafx.h' before including this file for PCH
//  #endif
//#endif

#include "TwainApp.h"

#ifndef TWH_CMP_MSC
  typedef void* HWND;
#endif

using namespace std;

#ifdef TWNDS_OS_LINUX
  #include <semaphore.h> 
#endif

/**
* The Main Application Class.
*/
class TWAIN_APP_API TwainAppCMD : public TwainApp
{
public:
/**
* Constructor for the main application.
* Initialize the application.
* @param[in] parent handle to Window
*/
  TwainAppCMD(HWND parent = NULL);

/**
* Deconstructor for the main application TwainAppCMD.  
* Clean up any alocated memory. 
*/
  ~TwainAppCMD();

/**
* Fills an identity structure with this applications identity details.
* Initialize the Apps identity so the DSM can uniquely identify it.
* @param[out] _identity a structure that will get filled with Applications details
*/
  virtual void fillIdentity(TW_IDENTITY& _identity);

/**
* Override so we can Initilize
* @param[in] _dsID the ID of the datasource to load
*/
  virtual void loadDS(const TW_INT32 _dsID);

/**
* Unloads the currently loaded datasource.  If successful moves the 
* app to state 3.
*/
  virtual void unloadDS();

/**
* Print a list of Data Sources.  Prints to stdout a quick list of the 
* Data Sources the app knows about.
*/
  void printAvailableDataSources();

/**
* Prints an identity structure that uniquely identifies an Application 
* or Data Source out in detail to stdout.
* @param[in] _ident the TW_IDENTITY structure to print
*/
  void printIdentStruct(const TW_IDENTITY& _ident);

/**
* Goes through the list of known identity structures and prints the one
* with an ID that matches the passed in id.
* @param[in] _identityID The ID of the identity structure to print
*/
  void printIdentityStruct(const TW_UINT32 _identityID);

/**
* Initialize default capabilities.
* Negotiates the default capabilities with the currently opened data source.
*/
  void initCaps();
/**
* Free allocated capabilities.
*/
  void uninitCaps();
/**
* Tries to starts the scanning process.  Must be in state 6 to start.  
* If successfully finishes scan moves app to state 5.
*/
  void startScan();
  /******************************** BEGIN OF NEGOTIATE CAP ***************************************/
/**
* Sets the CAP_XFERCOUNT capability
* @param[in] _count the value to set
*/
  void set_CAP_XFERCOUNT(const TW_INT16 _count,bool reread = false);
  
/**
* Sets the ICAP_UNITS capability
* @param[in] _val the value to set
*/
  void set_ICAP_UNITS(const TW_UINT16 _val, bool reget = false);

/**
* Sets the ICAP_PIXELTYPE capability
* @param[in] _pt the value to set
*/
  void set_ICAP_PIXELTYPE(const TW_UINT16 _pt, bool reget = false);

/**
* Sets the ICAP_RESOLUTION specified in _Cap
* @param[in] _Cap the RESOLUTION cap to set, either ICAP_XRESOLUTION or ICAP_YRESOLUTION
* @param[in] _pVal the value to set the cap to.
*/
  void set_ICAP_RESOLUTION(const TW_UINT16 _Cap, const pTW_FIX32 _pVal, bool reget = false);

/**
* Sets the ICAP_XFERMECH capability
* @param[in] _mech the mechanism to use. (TWSX_NATIVE, TWSX_FILE, TWSX_MEMORY)
*/
  void set_ICAP_XFERMECH(const TW_UINT16 _mech, bool reget = false);

/**
* Sets the ICAP_IMAGEFILEFORMAT capability
* @param[in] _fileformat the compression to use. (TWFF_TIFF, TWFF_PICT, TWFF_BMP, TWFF_XBM, TWFF_JFIF, TWFF_FPX, TWFF_TIFFMULTI, TWFF_PNG, TWFF_SPIFF, TWFF_EXIF)
*/
  void set_ICAP_IMAGEFILEFORMAT(const TW_UINT16 _fileformat, bool reget = false);

/**
* Sets the ICAP_COMPRESSION capability
* @param[in] _comp the compression to use. (TWCP_NONE, TWCP_PACKBITS, TWCP_GROUP4, TWCP_JPEG, ...)
*/
  void set_ICAP_COMPRESSION(const TW_UINT16 _comp, bool reget = false);

/**
* Sets the ICAP_FRAMES capability
* @param[in] _pFrame the frame data to set
*/
  void set_ICAP_FRAMES(const pTW_FRAME _pFrame, bool reget = false);

/**
* Sets the ICAP_BITDEPTH capability using a TW_ENUMERATION struct.
* @param[in] _nVal the bit depth to set
*/
  void set_ICAP_BITDEPTH(const TW_UINT16 _nVal, bool reget = false);

  /********************* below: Capabilities added by JiaFang in Jan 2019 ********************************/
  /**
* Sets the ICAP_AUTODISCARDBLANKPAGES capability
* @param[in] _value the auto discard blank pages indication data to set
*/
  void set_ICAP_AUTODISCARDBLANKPAGES(const TW_INT32 _value, bool reget = false);
  /**
* Sets the CAP_AUTOMATICSENSEMEDIUM capability
* @param[in] _flag indicate automatic sense medium or not
*/
  void set_CAP_AUTOMATICSENSEMEDIUM(const TW_BOOL _flag, bool reget = false);
  /**
* Sets the CAP_FEEDERENABLED capability
* @param[in] _flag indicate feeder enabled or not 
*/
  void set_CAP_FEEDERENABLED(const TW_BOOL _flag, bool reget = false);

  /**
* Sets the CAP_AUTOSCAN capability
* @param[in] _flag indicate auto scan or not
*/
  void set_CAP_AUTOSCAN(const TW_BOOL _flag, bool reget = false);
  /**
* Sets the CAP_AUTOMATICCAPTURE capability
* @param[in] _value the number of images to automatically capture to set
*/
  void set_CAP_AUTOMATICCAPTURE(const TW_INT32 _value, bool reget = false);
  /**
* Sets the CAP_TIMEBEFOREFIRSTCAPTURE capability
* @param[in] _value the number of milliseconds (before the first picture is to be taken, or the first image is to be scanned) to set
*/
  void set_CAP_TIMEBEFOREFIRSTCAPTURE(const TW_INT32 _value, bool reget = false);
  /**
* Sets the CAP_TIMEBETWEENCAPTURES capability
* @param[in] _value the milliseconds (to wait between pictures taken, or images scanned) to set
*/
  void set_CAP_TIMEBETWEENCAPTURES(const TW_INT32 _value, bool reget = false);

  /**
* Sets the CAP_DUPLEXENABLED capability
* @param[in] _flag indicate the scanner scans both sides of a paper or only one side
*/
  void set_CAP_DUPLEXENABLED(const TW_BOOL _flag, bool reget = false);
  /**
* Sets the ICAP_AUTOMATICCOLORENABLED capability
* @param[in] _flag indication whether automatically detects the pixel type of the image
*/
  void set_ICAP_AUTOMATICCOLORENABLED(const TW_BOOL _flag, bool reget = false);
  /**
* Sets the ICAP_AUTOMATICCOLORNONCOLORPIXELTYPE capability
* @param[in] _value the non-color pixel type to set
*/
  void set_ICAP_AUTOMATICCOLORNONCOLORPIXELTYPE(const TW_UINT16 _value, bool reget = false);
  /**
* Sets the ICAP_AUTOBRIGHT capability
* @param[in] _flag indicate whether auto-brightness
*/
  void set_ICAP_AUTOBRIGHT(const TW_BOOL _flag, bool reget = false);
  /**
* Sets the ICAP_BRIGHTNESS capability
* @param[in] _pVal the brightness data to set
*/
  void set_ICAP_BRIGHTNESS(const  pTW_FIX32 _pVal, bool reget = false);
  /**
* Sets the ICAP_CONTRAST capability
* @param[in] _pVal the contrast data to set
*/
  void set_ICAP_CONTRAST(const pTW_FIX32 _pVal, bool reget = false);
  /**
* Sets the ICAP_UNDEFINEDIMAGESIZE capability
* @param[in] _flag indication whether  application accepts undefined image size
*/
  void set_ICAP_UNDEFINEDIMAGESIZE(const TW_BOOL _flag, bool reget = false);
  /**
* Sets the ICAP_AUTOMATICBORDERDETECTION capability
* @param[in] _flag indication AutoBorder detection
*/
  void set_ICAP_AUTOMATICBORDERDETECTION(const TW_BOOL _flag, bool reget = false);
  /**
* Sets the ICAP_AUTOMATICROTATE capability
* @param[in] _flag indicate whether automatically rotate the image to the correct position
*/
  void set_ICAP_AUTOMATICROTATE(const TW_BOOL _flag, bool reget = false);

  /**
* Sets the ICAP_AUTOMATICDESKEW capability
* @param[in] _flag indicate Automatic deskew
*/
  void set_ICAP_AUTOMATICDESKEW(const TW_BOOL _flag, bool reget = false);
  /**
* Sets the ICAP_AUTOSIZE capability
* @param[in] _value the size(s) mechanism of output image dimensions to set.
*/
  void set_ICAP_AUTOSIZE(const TW_UINT16 _value, bool reget = false);
  /**
* Sets the ICAP_SUPPORTEDSIZES capability
* @param[in] _value the supported size to set
*/
  void set_ICAP_SUPPORTEDSIZES(const TW_UINT16 _value, bool reget = false);
  /**
* Sets the ICAP_MAXFRAMES capability
* @param[in] _value the maximum number of frames to set
*/
  void set_ICAP_MAXFRAMES(const TW_UINT16 _value, bool reget = false);
  /**
* Sets the ICAP_ORIENTATION capability
* @param[in] _value the image's "top"that is aligned with data to set
*/
  void set_ICAP_ORIENTATION(const TW_UINT16 _value, bool reget = false);
  /**
* Sets the ICAP_ROTATION capability
* @param[in] _pVal the rotation degrees to set
*/
  void set_ICAP_ROTATION(const pTW_FIX32 _pVal, bool reget = false);
  /******************************** END OF  NEGOTIATE CAP ***************************************/

  /******************************** BEGIN OF  GET CAP CURRENT VALUE ***************************************/
/**
* Gets the current setting for the ICAP_UNITS cap
* @param[out] _val set to the caps value
* @return true on succes
*/
  bool getICAP_UNITS(TW_UINT16& _val);

/**
* Gets the current setting for the CAP_XFERCOUNT cap
* @param[out] _val set to the caps value
* @return true on succes
*/
  bool getCAP_XFERCOUNT(TW_INT16& _val);

/**
* Gets the current setting for the ICAP_XFERMECH cap
* @param[out] _val a TW_UINT16 holding the current transfer method (TWSX_NATIVE, etc...)
* @return true on succes
*/
  bool getICAP_XFERMECH(TW_UINT16& _val);

/**
* Gets the current setting for the ICAP_IMAGEFILEFORMAT cap
* @param[out] _val a TW_UINT16 holding the current compression method (TWFF_TIFF, TWFF_PICT, TWFF_BMP, TWFF_XBM, TWFF_JFIF, TWFF_FPX, TWFF_TIFFMULTI, TWFF_PNG, TWFF_SPIFF, TWFF_EXIF)
* @return true on succes
*/
  bool getICAP_IMAGEFILEFORMAT(TW_UINT16& _val);

/**
* Gets the current setting for the ICAP_COMPRESSION cap
* @param[out] _val a TW_UINT16 holding the current compression method (TWCP_NONE, TWCP_PACKBITS, TWCP_GROUP4, TWCP_JPEG, ...)
* @return true on succes
*/
  bool getICAP_COMPRESSION(TW_UINT16& _val);

/**
* Gets the current setting for the ICAP_PIXELTYPE cap
* @param[out] _val a TW_UINT16 holding the current pixel type (TWPT_BW, etc...)
* @return true on succes
*/
  bool getICAP_PIXELTYPE(TW_UINT16& _val);

/**
* Gets the current setting for the ICAP_BITDEPTH cap
* @param[out] _val a TW_UINT16 holding the current bit depth
* @return true on succes
*/
  bool getICAP_BITDEPTH(TW_UINT16& _val);

/**
* Gets the current setting for the ICAP_XRESOLUTION cap
* @param[out] _xres a TW_FIX32 holding the current x resolution
* @return true on succes
*/
  bool getICAP_XRESOLUTION(TW_FIX32& _xres);

/**
* Gets the current setting for the ICAP_YRESOLUTION cap
* @param[out] _yres a TW_FIX32 holding the current y resolution
* @return true on succes
*/
  bool getICAP_YRESOLUTION(TW_FIX32& _yres);

  /**************************** Below: Capabilities added by JiaFang in Jan 2019 ***************************/
  /**
* Gets the current setting for the ICAP_AUTODISCARDBLANKPAGES cap
* @param[out] _value a TW_FIX32 holding the current auto discard blank pages value.
* @return true on succes
*/
  bool getICAP_AUTODISCARDBLANKPAGES(TW_INT32& _value);
  /**
* Gets the current setting for the CAP_AUTOMATICSENSEMEDIUM cap
* @param[out] _flag a TW_BOOL holding the current automatic sense medium or not flag.
* @return true on succes
*/
  bool getCAP_AUTOMATICSENSEMEDIUM(TW_BOOL& _flag);
  /**
* Gets the current setting for the CAP_FEEDERENABLED cap
* @param[out] _flag a TW_BOOL holding the current feeder enabled or not flag.
* @return true on succes
*/
  bool getCAP_FEEDERENABLED(TW_BOOL& _flag);

  /**
* Gets the current setting for the CAP_PAPERDETECTABLE cap
* @param[out] _flag a TW_BOOL holding the current paper detectable or not flag.
* @return true on succes
*/
  bool getCAP_PAPERDETECTABLE(TW_BOOL& _flag);
  /**
* Gets the current setting for the CAP_FEEDERLOADED cap
* @param[out] _flag a TW_BOOL holding the current feeder loaded or not flag.
* @return true on succes
*/
  bool getCAP_FEEDERLOADED(TW_BOOL& _flag);

  /**
* Gets the current setting for the CAP_AUTOSCAN cap
* @param[out] _flag a TW_BOOL holding the current auto scan or not flag;
* @return true on succes
*/
  bool getCAP_AUTOSCAN(TW_BOOL& _flag);

  /**
* Gets the current setting for the CAP_AUTOMATICCAPTURE cap
* @param[out] _value a TW_INT32 holding the current number of images to automatically capture.
* @return true on succes
*/
  bool getCAP_AUTOMATICCAPTURE(TW_INT32& _value);
  /**
* Gets the current setting for the CAP_TIMEBEFOREFIRSTCAPTURE cap
* @param[out] _yres a TW_INT32  holding the current number of milliseconds before the first picture is to be taken, or the first image is to be scanned.
* @return true on succes
*/
  bool getCAP_TIMEBEFOREFIRSTCAPTURE(TW_INT32& _value);
  /**
* Gets the current setting for the CAP_TIMEBETWEENCAPTURES cap
* @param[out] _yres a TW_INT32 holding the current milliseconds to wait between pictures taken, or images scanned.
* @return true on succes
*/
  bool getCAP_TIMEBETWEENCAPTURES(TW_INT32& _value);
  /**
* Gets the current setting for the CAP_DUPLEX cap
* @param[out] _yres a TW_UINT16 holding the current  indication whether the scanner supports duplex.
* @return true on succes
*/
  bool getCAP_DUPLEX(TW_UINT16& _value);
  /**
* Gets the current setting for the CAP_DUPLEXENABLED cap
* @param[out] _flag a TW_BOOL holding the current indication whether the scanner scans both sides of a paper or only one side.
* @return true on succes
*/
  bool getCAP_DUPLEXENABLED(TW_BOOL& _flag);

  /**
* Gets the current setting for the ICAP_AUTOMATICCOLORENABLED cap
* @param[out] _flag a TW_BOOL holding the current indication whether automatically detects the pixel type of the image and returns either a color image or a non-color image.
* @return true on succes
*/
  bool getICAP_AUTOMATICCOLORENABLED(TW_BOOL& _flag);
  /**
* Gets the current setting for the ICAP_AUTOMATICCOLORNONCOLORPIXELTYPE cap
* @param[out] _value a TW_UINT16 holding the current  non-color pixel type to use when automatic color is enabled.
* @return true on succes
*/
  bool getICAP_AUTOMATICCOLORNONCOLORPIXELTYPE(TW_UINT16& _value);

  /**
* Gets the current setting for the ICAP_AUTOBRIGHT cap
* @param[out] _flag a TW_BOOL holding the current auto-brightness function
* @return true on succes
*/
  bool getICAP_AUTOBRIGHT(TW_BOOL& _flag);
  /**
* Gets the current setting for the ICAP_BRIGHTNESS cap
* @param[out] _value a TW_FIX32 holding the current brightness
* @return true on succes
*/
  bool getICAP_BRIGHTNESS(TW_FIX32& _value);
  /**
* Gets the current setting for the ICAP_CONTRAST cap
* @param[out] _yres a TW_FIX32 holding the current contrast
* @return true on succes
*/
  bool getICAP_CONTRAST(TW_FIX32& _value);
  /**
* Gets the current setting for the ICAP_UNDEFINEDIMAGESIZE cap
* @param[out] _flag a TW_BOOL holding the current indication whether  application accepts undefined image size.
* @return true on succes
*/
  bool getICAP_UNDEFINEDIMAGESIZE(TW_BOOL& _flag);
  /**
* Gets the current setting for the ICAP_AUTOMATICBORDERDETECTION cap
* @param[out] _flag a TW_BOOL holding the current  state of the AutoBorder detection.
* @return true on succes
*/
  bool getICAP_AUTOMATICBORDERDETECTION(TW_BOOL& _flag);
  /**
* Gets the current setting for the ICAP_AUTOMATICROTATE cap
* @param[out] _flag a TW_BOOL holding the current  intelligent features within the Source to automatically rotate the image to the correct position.
* @return true on succes
*/
  bool getICAP_AUTOMATICROTATE(TW_BOOL& _flag);
  /**
* Gets the current setting for the ICAP_AUTOMATICDESKEW cap
* @param[out] _flag a TW_BOOL holding the current  capability to enable or disable Automatic deskew.
* @return true on succes
*/
  bool getICAP_AUTOMATICDESKEW(TW_BOOL& _flag);
  /**
* Gets the current setting for the ICAP_AUTOSIZE cap
* @param[out] _value a TW_UINT16 holding the current size(s) mechanism of output image dimensions to match.
* @return true on succes
*/
  bool getICAP_AUTOSIZE(TW_UINT16& _value);

  /**
* Gets the current setting for the ICAP_SUPPORTEDSIZES cap
* @param[out] _value a TW_UINT16 holding the current  frame size. 
* @return true on succes
*/
  bool getICAP_SUPPORTEDSIZES(TW_UINT16& _value);
  /**
* Gets the current setting for the ICAP_MAXFRAMES cap
* @param[out] _num a TW_UINT16 holding the current maximum number of frames the Source can provide or the application can accept per page.
* @return true on succes
*/
  bool getICAP_MAXFRAMES(TW_UINT16& _num);
  /**
* Gets the current setting for the ICAP_ORIENTATION cap
* @param[out] _orientation a TW_UINT16 holding the current  image's "top"that is aligned with.
* @return true on succes
*/
  bool getICAP_ORIENTATION(TW_UINT16& _orientation);
  /**
* Gets the current setting for the ICAP_ROTATION cap
* @param[out] _degree a TW_FIX32 holding the current degrees the Source can/should rotate the scanned image data prior to transfer. 
* @return true on succes
*/
  bool getICAP_ROTATION(TW_FIX32& _degree);
  /******************************** END OF  GET CAP CURRENT VALUE ***************************************/

  ////////////////
  // CAPABILITIES

  TW_CAPABILITY   m_CAP_XFERCOUNT;        /**< Number of images the application is willing to accept this session. */
  TW_CAPABILITY   m_ICAP_XFERMECH;        /**< Transfer mechanism - used to learn options and set-up for upcoming transfer. */
  TW_CAPABILITY   m_ICAP_IMAGEFILEFORMAT; /**< File format saved when using File Xfer Mechanism. */
  TW_CAPABILITY   m_ICAP_COMPRESSION;     /**< Compression method used for upcoming transfer. */
  TW_CAPABILITY   m_ICAP_UNITS;           /**< Unit of measure (inches, centimeters, etc). */
  TW_CAPABILITY   m_ICAP_PIXELTYPE;       /**< The type of pixel data (B/W, gray, color, etc). */
  TW_CAPABILITY   m_ICAP_BITDEPTH;        /**< Pixel bit depth for Current value of ICAP_PIXELTYPE. */
  TW_CAPABILITY   m_ICAP_XRESOLUTION;     /**< Current/Available optical resolutions for x-axis. */
  TW_CAPABILITY   m_ICAP_YRESOLUTION;     /**< Current/Available optical resolutions for y-axis */
  TW_CAPABILITY   m_ICAP_FRAMES;          /**< Size and location of frames on page. */

	/********************* below: Capabilities added by JiaFang in Jan 2019 ********************************/
  TW_CAPABILITY   m_ICAP_AUTODISCARDBLANKPAGES;          /**< Discards blank pages.*/
  TW_CAPABILITY   m_CAP_AUTOMATICSENSEMEDIUM;            /**< Configures a Source to check for paper in the Automatic Document Feeder. */

  TW_CAPABILITY   m_CAP_FEEDERENABLED;                   /**< If TRUE, Source’s feeder is available*/
  TW_CAPABILITY   m_CAP_PAPERDETECTABLE;                 /**< Determines whether source can detect documents on the ADF or flatbed*/
  TW_CAPABILITY   m_CAP_FEEDERLOADED;                    /**< If TRUE, Source has documents loaded in feeder (MSG_GET only)*/
  TW_CAPABILITY   m_CAP_AUTOSCAN;                        /**< Enables the source’s automatic document scanning process.*/

  TW_CAPABILITY   m_CAP_AUTOMATICCAPTURE;                /**< Specifies the number of images to automatically capture.*/
  TW_CAPABILITY   m_CAP_TIMEBEFOREFIRSTCAPTURE;          /**< Selects the number of seconds before the first picture taken*/
  TW_CAPABILITY   m_CAP_TIMEBETWEENCAPTURES;             /**< Selects the hundredths of a second to wait between pictures taken*/

  TW_CAPABILITY   m_CAP_DUPLEX;                          /**< Indicates whether the scanner supports duplex*/
  TW_CAPABILITY   m_CAP_DUPLEXENABLED;                   /**< Enables the user to set the duplex option to be TRUE or FALSE. */

  TW_CAPABILITY   m_ICAP_AUTOMATICCOLORENABLED;          /**< Detects the pixel type of the image and returns either a color image or a non-color image specified by ICAP_AUTOMATICCOLORNONCOLORPIXELTYPE.*/
  TW_CAPABILITY   m_ICAP_AUTOMATICCOLORNONCOLORPIXELTYPE;/**< Specifies the non-color pixel type to use when automatic color is enabled.*/

  TW_CAPABILITY   m_ICAP_AUTOBRIGHT;                     /**< Enable Source’s Auto-brightness function*/
  TW_CAPABILITY   m_ICAP_BRIGHTNESS;                     /**< Source brightness values*/
  TW_CAPABILITY   m_ICAP_CONTRAST;                       /**< Source contrast values*/

  TW_CAPABILITY   m_ICAP_UNDEFINEDIMAGESIZE;             /**< The application will accept undefined image size*/
  TW_CAPABILITY   m_ICAP_AUTOMATICBORDERDETECTION;       /**< Turns automatic border detection on and off.*/
  TW_CAPABILITY   m_ICAP_AUTOMATICROTATE;                /**< When TRUE, depends on source to automatically rotate the image.*/
  TW_CAPABILITY   m_ICAP_AUTOMATICDESKEW;                /**< Turns automatic skew correction on and off.*/
  TW_CAPABILITY   m_ICAP_AUTOSIZE;                       /**< Force the output image dimensions to match either the current value of ICAP_SUPPORTEDSIZES or any of its current allowed values.*/

  TW_CAPABILITY   m_ICAP_SUPPORTEDSIZES;                 /**< Fixed frame sizes for typical page sizes*/
  TW_CAPABILITY   m_ICAP_MAXFRAMES;                      /**< Maximum number of frames possible per page*/

  TW_CAPABILITY   m_ICAP_ORIENTATION;                    /**< Defines which edge of the paper is the top: Portrait or Landscape*/
  TW_CAPABILITY   m_ICAP_ROTATION;                       /**< Source can, or should, rotate image this number of degrees*/

  
#ifdef TWNDS_OS_LINUX
  sem_t m_TwainEvent;
#endif

protected:
};

#endif //  __TWAINAPPCMD_H__
