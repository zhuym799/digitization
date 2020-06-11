#ifdef _WINDOWS
#include "stdafx.h"
#endif
#include "pch.h"
#include <iostream>
#include <sstream>


#include "concretApp.h"
using namespace std;

concretApp::concretApp(HWND parent):TwainAppInterface(parent)
{
}

concretApp::~concretApp()
{
}

extern
#ifdef TWH_CMP_MSC
TW_UINT16 FAR PASCAL
#else
FAR PASCAL TW_UINT16
#endif
DSMCallbackFunc(pTW_IDENTITY _pOrigin,
	pTW_IDENTITY _pDest,
	TW_UINT32    _DG,
	TW_UINT16    _DAT,
	TW_UINT16    _MSG,
	TW_MEMREF    _pData);

TwainApp::DSMCallback * concretApp::getDSMCallbackFunction()
{
	return DSMCallbackFunc;
}

//void concretApp::printErrorText(const string text)
//{
//	cerr << "Unsupported capability" << endl;
//}
//
//void concretApp::printCommonText(const string text)
//{
//	cout << "Unsupported capability" << endl;
//}



void concretApp::fillIdentity(TW_IDENTITY & _identity)
{
	_identity.Id = 0;
	_identity.Version.MajorNum = 1;
	_identity.Version.MinorNum = 0;
	_identity.Version.Language = TWLG_CHINESE;
	_identity.Version.Country = TWCY_CHINA;
	SSTRCPY(_identity.Version.Info, sizeof(_identity.Version.Info), "1.0.0");
	_identity.ProtocolMajor = 2;
	_identity.ProtocolMinor = 4;
	_identity.SupportedGroups = DF_APP2 | DG_IMAGE | DG_CONTROL;
	SSTRCPY(_identity.Manufacturer, sizeof(_identity.Manufacturer), "Hasng");
	SSTRCPY(_identity.ProductFamily, sizeof(_identity.ProductFamily), "Digitalization");
	SSTRCPY(_identity.ProductName, sizeof(_identity.ProductName), "Digitalization of cadre archives");
}

//////////////////////////////////////////////////////////////////////////////
string concretApp::getErrorString_UnexpectedType(const TW_UINT16 _unExpected, const TW_UINT16 _unReceived)
{
	ostringstream errMsg;

	errMsg
		<< "The type is an unexpected value. "
		<< " got " << convertTWTY2String(_unReceived);

	return errMsg.str();
}

//////////////////////////////////////////////////////////////////////////////
void concretApp::printOptions()
{
	cout
		<< "\n"
		<< "Please enter an option\n"
		<< "----------------------\n"
		<< "q    - quit\n"
		<< "h    - help\n"
		<< "cdsm - connect to the dsm\n"
		<< "xdsm - disconnect from the dsm\n"
		<< "lds  - list data sources available\n"
		<< "pds# - print identity structure for data source with id#. ex: pds2\n"
		<< "cds# - connect to data source with id#. ex: cds2\n"
		<< "xds  - disconnect from data source\n"
		<< "caps - negotiate capabilities\n"
		<< "scan - start the scan\n"
		<< endl;
	return;
}

//////////////////////////////////////////////////////////////////////////////
void concretApp::printMainCaps()
{
	cout
		<< "\n"
		<< "Capabilites\n"
		<< "-----------\n"
		<< "q - done negotiating, go back to main menu\n"
		<< "h - help\n"
		<< "1 - ICAP_XFERMECH\n"
		<< "2 - ICAP_PIXELTYPE\n"
		<< "3 - ICAP_BITDEPTH\n"
		<< "4 - ICAP_XRESOLUTION\n"
		<< "5 - ICAP_YRESOLUTION\n"
		<< "6 - ICAP_FRAMES\n"
		<< "7 - ICAP_UNITS\n"
		<< endl;
	return;
}

//////////////////////////////////////////////////////////////////////////////
void concretApp::print_ICAP(const TW_UINT16 _unCap, pTW_ONEVALUE _pCap)
{
	if (0 == _pCap)
	{
		return;
	}

	cout
		<< "\n"
		<< convertCAP2String(_unCap) << "\n"
		<< "--------------\n"
		<< "Showing supported types. * indicates current setting.\n\n"
		<< "q - done\n";

	switch (_pCap->ItemType)
	{
	case TWTY_FIX32:
	{
		pTW_FIX32 pFix32 = (pTW_FIX32)&_pCap->Item;
		cout << "1 - " << pFix32->Whole << "." << pFix32->Frac << "*\n" << endl;
	}
	break;

	case TWTY_FRAME:
	{
		pTW_FRAME pframe = (pTW_FRAME)&_pCap->Item;

		cout
			<< "1 - Frame Data:*\n"
			<< "\tLeft,\tTop,\tRight,\tBottom\n"
			<< "\t" << convertFIX32ToFloat(pframe->Left) << ",\t"
			<< convertFIX32ToFloat(pframe->Top) << ",\t"
			<< convertFIX32ToFloat(pframe->Right) << ",\t"
			<< convertFIX32ToFloat(pframe->Bottom) << "\n"
			<< "\n"
			<< endl;
	}
	break;

	case TWTY_INT8:
	case TWTY_INT16:
	case TWTY_INT32:
	case TWTY_UINT8:
	case TWTY_UINT16:
	case TWTY_UINT32:
	case TWTY_BOOL:
	{
		cout << "1 - " << convertCAP_Item2String(_unCap, _pCap->Item, _pCap->ItemType) << "*" << endl;
	}
	break;

	case TWTY_STR32:
	{
		cout << "1 - " << (pTW_STR32)&_pCap->ItemType << "*" << endl;
	}
	break;

	case TWTY_STR64:
	{
		cout << "1 - " << (pTW_STR64)&_pCap->ItemType << "*" << endl;
	}
	break;

	case TWTY_STR128:
	{
		cout << "1 - " << (pTW_STR128)&_pCap->ItemType << "*" << endl;
	}
	break;

	case TWTY_STR255:
	{
		cout << "1 - " << (pTW_STR255)&_pCap->ItemType << "*" << endl;
	}
	break;

	default:
	{
		cerr << getErrorString_UnexpectedType(0, _pCap->ItemType) << endl;
	}
	break;

	}
	return;
}

//////////////////////////////////////////////////////////////////////////////
void concretApp::print_ICAP(const TW_UINT16 _unCap, pTW_ENUMERATION _pCap)
{
	if (0 == _pCap)
	{
		return;
	}

	cout
		<< "\n"
		<< convertCAP2String(_unCap) << "\n"
		<< "--------------\n"
		<< "Showing supported types. * indicates current setting.\n\n"
		<< "q - done\n";

	for (TW_UINT32 x = 0; x < _pCap->NumItems; ++x)
	{
		switch (_pCap->ItemType)
		{
		case TWTY_UINT32:
		{
			cout << x << " - " << convertCAP_Item2String(_unCap, ((pTW_UINT32)(&_pCap->ItemList))[x], _pCap->ItemType);
		}
		break;

		case TWTY_INT32:
		{
			cout << x << " - " << convertCAP_Item2String(_unCap, ((pTW_INT32)(&_pCap->ItemList))[x], _pCap->ItemType);
		}
		break;

		case TWTY_UINT16:
		{
			cout << x << " - " << convertCAP_Item2String(_unCap, ((pTW_UINT16)(&_pCap->ItemList))[x], _pCap->ItemType);
		}
		break;

		case TWTY_INT16:
		{
			cout << x << " - " << convertCAP_Item2String(_unCap, ((pTW_INT16)(&_pCap->ItemList))[x], _pCap->ItemType);
		}
		break;

		case TWTY_UINT8:
		{
			cout << x << " - " << convertCAP_Item2String(_unCap, ((pTW_UINT8)(&_pCap->ItemList))[x], _pCap->ItemType);
		}
		break;

		case TWTY_INT8:
		{
			cout << x << " - " << convertCAP_Item2String(_unCap, ((pTW_INT8)(&_pCap->ItemList))[x], _pCap->ItemType);
		}
		break;

		case TWTY_FIX32:
		{
			cout << x << " - " << ((pTW_FIX32)(&_pCap->ItemList))[x].Whole << "." << (int)((((pTW_FIX32)(&_pCap->ItemList))[x].Frac / 65536.0 + .0005) * 1000);
		}
		break;

		case TWTY_FRAME:
		{
			pTW_FRAME pframe = &((pTW_FRAME)(&_pCap->ItemList))[x];
			cout << x << " - Frame Data:\n"
				<< "\tLeft,\tTop,\tRight,\tBottom\n"
				<< "\t" << convertFIX32ToFloat(pframe->Left) << ",\t"
				<< convertFIX32ToFloat(pframe->Top) << ",\t"
				<< convertFIX32ToFloat(pframe->Right) << ",\t"
				<< convertFIX32ToFloat(pframe->Bottom);
		}
		break;

		case TWTY_STR32:
		{
			cout << "1 - " << &((pTW_STR32)(&_pCap->ItemList))[x];
		}
		break;

		case TWTY_STR64:
		{
			cout << "1 - " << &((pTW_STR64)(&_pCap->ItemList))[x];
		}
		break;

		case TWTY_STR128:
		{
			cout << "1 - " << &((pTW_STR128)(&_pCap->ItemList))[x];
		}
		break;

		case TWTY_STR255:
		{
			cout << "1 - " << &((pTW_STR255)(&_pCap->ItemList))[x];
		}
		break;


		default:
		{
			cerr << getErrorString_UnexpectedType(0,_pCap->ItemType) << endl;
		}
		break;
		}

		if (x == _pCap->CurrentIndex)
		{
			cout << "*";
		}

		cout << "\n";
	}

	cout << endl;
	return;
}
//////////////////////////////////////////////////////////////////////////////