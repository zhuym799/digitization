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
 * @file TwainApp_ui.h
 * User interface for TWAIN Application
 * @author JFL Peripheral Solutions Inc.
 * @date April 2007
 */



 /**
 * @file twainAppInterface.h
 * TWAIN Application
 * on the base of TwainApp_ui.h
 * edit by JiaFang on Dec 2018
 */

#ifndef __TWAINAPP_INTERFACE_H__
#define __TWAINAPP_INTERFACE_H__

#pragma once

 //#ifdef _WINDOWS
 //  #ifndef __AFXWIN_H__
 //    #error include 'stdafx.h' before including this file for PCH
 //  #endif
 //#endif

#include "TwainAppCMD.h"
class TWAIN_APP_API TwainAppInterface :public TwainAppCMD
{
//private:
//		string quitCmd;
public:

	/**
	* Constructor for the main application.
	* Initialize the application.
	* @param[in] parent handle to Window
	*/
	TwainAppInterface(HWND parent = NULL);

	/**
	* Deconstructor for the main application TwainAppInterface.
	* Clean up any alocated memory.
	*/
	virtual ~TwainAppInterface();

	//virtual void quit_negotiate(const pTW_CAPABILITY _pCap);


	///**
	//* Negotiate a capabilities between the app and the DS
	//* @param[in] _pCap the capabilities to negotiate
	//*/
	//virtual void negotiateCAP(const pTW_CAPABILITY _pCap, const string input);

	/**
	* Returns an error message that can be used when a type is of an unexpected type.
	* @param[in] _unExpected the expected type
	* @param[in] _unReceived the received type
	* @return the error string
	*/
	virtual string getErrorString_UnexpectedType(const TW_UINT16 _unExpected, const TW_UINT16 _unReceived) = 0;


	/*virtual void printErrorText(const string text) = 0;
	virtual void printCommonText(const string text) = 0;*/
	/**
	* prints the main menu
	*/
	virtual void printOptions() = 0;

	/**
	* prints the main capabilities menu
	*/
	virtual void printMainCaps() = 0;

	/**
	* prints the ICAP_xxxxx menu for type TW_ONEVALUE
	* @param[in] _unCap The cap to print
	* @param[in] _pCap pointer to the TW_ONEVALUE returned by the source for this cap
	*/
	virtual void print_ICAP(const TW_UINT16 _unCap, pTW_ONEVALUE _pCap) = 0;

	/**
	* @overload void print_ICAP(const TW_UINT16 _unCap, pTW_ENUMERATION _pCap)
	* prints the ICAP_xxxxx menu for type TW_ENUMERATION
	* @param[in] _unCap The cap to print
	* @param[in] _pCap pointer to the TW_ENUMERATION returned by the source for this cap
	*/
	virtual void print_ICAP(const TW_UINT16 _unCap, pTW_ENUMERATION _pCap) = 0;

protected:

	/**
	* converts the integer TWTY value into string form
	* @param[in] _unItem the TWTY value
	* @return the equivalent string
	*/
	static const char* convertTWTY2String(const TW_UINT16 _unItem);


	/**
	* converts the integer CAP value into string form
	* @param[in] _unCap the cap
	* @return the equivalent cap string
	*/
	static const char* convertCAP2String(const TW_UINT16 _unCap);

	/**
	* converts the integer CAP transfer method into string form
	* @param[in] _unCap the cap
	* @param[in] _unItem the value
	* @param[in] _unType the TWAIN Type of the item
	* @return the equivalent string
	*/
	static const char* convertCAP_Item2String(const TW_UINT16 _unCap, const TW_UINT32 _unItem, const TW_UINT16 _unType);

	//////////////////////////////////////////////////////////////////////////////
	static TW_FIX32 convertFloatToFIX32(float floater);

	//////////////////////////////////////////////////////////////////////////////
	static float convertFIX32ToFloat(const TW_FIX32& _fix32);
public :
	///**
	//* Initialize and register the entry point into the DSM.
	//* @param[in] _pOrigin Identifies the source module of the message. This could
	//*           identify an Application, a Source, or the Source Manager.
	//*
	//* @param[in] _pDest Identifies the destination module for the message.
	//*           This could identify an application or a data source.
	//*           If this is NULL, the message goes to the Source Manager.
	//*
	//* @param[in] _DG The Data Group.
	//*           Example: DG_IMAGE.
	//*
	//* @param[in] _DAT The Data Attribute Type.
	//*           Example: DAT_IMAGEMEMXFER.
	//*
	//* @param[in] _MSG The message.  Messages are interpreted by the destination module
	//*           with respect to the Data Group and the Data Attribute Type.
	//*           Example: MSG_GET.
	//*
	//* @param[in,out] _pData A pointer to the data structure or variable identified
	//*           by the Data Attribute Type.
	//*           Example: (TW_MEMREF)&ImageMemXfer
	//*                   where ImageMemXfer is a TW_IMAGEMEMXFER structure.
	//*
	//* @return a valid TWRC_xxxx return code.
	//*          Example: TWRC_SUCCESS.
	//*/
	//static TW_UINT16 S_DSM_Entry(pTW_IDENTITY _pOrigin,
	//	pTW_IDENTITY _pDest,
	//	TW_UINT32    _DG,
	//	TW_UINT16    _DAT,
	//	TW_UINT16    _MSG,
	//	TW_MEMREF    _pData);

	/**
	* Lock global memory from being updated by others. return a pointer to the
	* memory so we can update it.
	* @param[in] _hMemory TW_HANDLE to the memory to update.
	* @return TW_MEMREF pointer to the memory.
	*/
	static TW_MEMREF S_DSM_LockMemory(TW_HANDLE _hMemory);

	/**
	* Unlock global memory after locking. to allow updating by others.
	* @param[in] _hMemory TW_HANDLE to memory returned by _DSM_Alloc
	*/
	static void S_DSM_UnlockMemory(TW_HANDLE _hMemory);
};
#endif // __TWAINAPP_INTERFACE_H__
