#include "TwainString.h"
#include "twainAppInterface.h"

TwainAppInterface::TwainAppInterface(HWND parent/*=NULL*/) :TwainAppCMD(parent)
{
}

TwainAppInterface::~TwainAppInterface()
{
}

//void TwainAppInterface::quit_negotiate(const pTW_CAPABILITY _pCap)
//{
//	_DSM_UnlockMemory(_pCap->hContainer);
//}





///**
//* Negotiate a capabilities between the app and the DS
//* @param[in] _pCap the capabilities to negotiate
//* @param[in] _input the value of capability
//*/
//void TwainAppInterface::negotiateCAP(const pTW_CAPABILITY _pCap,const string _input)
//{
//	
//		if ((TWON_ENUMERATION == _pCap->ConType) ||
//			(TWON_ONEVALUE == _pCap->ConType))
//		{
//			TW_MEMREF pVal = _DSM_LockMemory(_pCap->hContainer);
//
//			// print the caps current value
//			if (TWON_ENUMERATION == _pCap->ConType)
//			{
//				print_ICAP(_pCap->Cap, (pTW_ENUMERATION)(pVal));
//			}
//			else // TWON_ONEVALUE
//			{
//				print_ICAP(_pCap->Cap, (pTW_ONEVALUE)(pVal));
//			}			
//
//			/*if (quitCmd == input)
//			{
//				_DSM_UnlockMemory(_pCap->hContainer);
//				break;
//			}
//			else
//			{*/
//				int n = atoi(_input.c_str());
//				TW_UINT16  valUInt16 = 0;
//				pTW_FIX32  pValFix32 = { 0 };
//				pTW_FRAME  pValFrame = { 0 };
//
//				// print the caps current value
//				if (TWON_ENUMERATION == _pCap->ConType)
//				{
//					switch (((pTW_ENUMERATION)pVal)->ItemType)
//					{
//					case TWTY_UINT16:
//						valUInt16 = ((pTW_UINT16)(&((pTW_ENUMERATION)pVal)->ItemList))[n];
//						break;
//
//					case TWTY_FIX32:
//						pValFix32 = &((pTW_ENUMERATION_FIX32)pVal)->ItemList[n];
//						break;
//
//					case TWTY_FRAME:
//						pValFrame = &((pTW_ENUMERATION_FRAME)pVal)->ItemList[n];
//						break;
//					}
//
//					switch (_pCap->Cap)
//					{
//					case ICAP_PIXELTYPE:
//						set_ICAP_PIXELTYPE(valUInt16);
//						break;
//
//					case ICAP_BITDEPTH:
//						set_ICAP_BITDEPTH(valUInt16);
//						break;
//
//					case ICAP_UNITS:
//						set_ICAP_UNITS(valUInt16);
//						break;
//
//					case ICAP_XFERMECH:
//						set_ICAP_XFERMECH(valUInt16);
//						break;
//
//					case ICAP_XRESOLUTION:
//					case ICAP_YRESOLUTION:
//						set_ICAP_RESOLUTION(_pCap->Cap, pValFix32);
//						break;
//
//					case ICAP_FRAMES:
//						set_ICAP_FRAMES(pValFrame);
//						break;
//
//					default:
//						//cerr << "Unsupported capability" << endl;
//						printErrorText("Unsupported capability");
//						break;
//					}
//				}
//			//}
//			_DSM_UnlockMemory(_pCap->hContainer);
//		}
//		else
//		{
//			/*cerr << "Unsupported capability" << endl;*/
//			printErrorText("Unsupported capability");
//		}
//}

const char * TwainAppInterface::convertTWTY2String(const TW_UINT16 _unItem)
{
	return convertTWTY_toString(_unItem);
}

const char * TwainAppInterface::convertCAP2String(const TW_UINT16 _unCap)
{
	return convertCAP_toString(_unCap);
}

const char * TwainAppInterface::convertCAP_Item2String(const TW_UINT16 _unCap, const TW_UINT32 _unItem, const TW_UINT16 _unType)
{
	return convertCAP_Item_toString(_unCap, _unItem, _unType);
}

TW_FIX32 TwainAppInterface::convertFloatToFIX32(float floater)
{
	return FloatToFIX32(floater);
}

float TwainAppInterface::convertFIX32ToFloat(const TW_FIX32 & _fix32)
{
	return FIX32ToFloat(_fix32);
}

//TW_UINT16 TwainAppInterface::S_DSM_Entry(pTW_IDENTITY _pOrigin, pTW_IDENTITY _pDest, TW_UINT32 _DG, TW_UINT16 _DAT, TW_UINT16 _MSG, TW_MEMREF _pData)
//{
//	return _DSM_Entry(_pOrigin, _pDest, _DG, _DAT, _MSG, _pData);
//}

TW_MEMREF TwainAppInterface::S_DSM_LockMemory(TW_HANDLE _hMemory)
{
	return _DSM_LockMemory(_hMemory);
}

void TwainAppInterface::S_DSM_UnlockMemory(TW_HANDLE _hMemory)
{
	_DSM_UnlockMemory(_hMemory);
}
