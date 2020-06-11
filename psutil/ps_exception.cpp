#include "psutil.h"
namespace psutil
{	
	CPsException::CPsException():std::exception()
	{
	}
	CPsException::CPsException(char const* const msg) : m_msg(msg), std::exception(msg)
	{	
	}
	CPsException::CPsException(exception inner, char const* const msg) : m_inner(inner), m_msg(msg), std::exception(msg)
	{
	}
	exception CPsException::getInnerExp()
	{
		return m_inner;
	}
	string CPsException::getMsg()
	{
		return m_msg;
	}
}