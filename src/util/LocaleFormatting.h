/*+--------------------------------------------------------------------+
| Company : CTO-INFORMATIK AG                                        |
| Project : Stonehenge                                               |
| Package : SHBaseObjects                                            |
+--------------------------------------------------------------------+ */
/** @brief  : 
** @file   : SHLocaleFormatting.h
** @author : UlR
** @date   : 12.06.2008
*/


#ifndef SH_LOCALEFORMATTING_H
#define SH_LOCALEFORMATTING_H

#include "SHWinApp.h"
#include <ostream>
#include <locale>
#include <string>
#include <iomanip>
#include <time.h>


#if _MSC_VER > 1000
#pragma once
#endif

// according to "Internationalization using standard C++" 
// http://www.angelikalanger.com/Articles/Cuj/Internationalization/I18N.html
// extedned to make the format string configurable according to apache std lib
// http://stdcxx.apache.org/doc/stdlibug/36-3.html

// unit test "LocaleFormatting_tests" in SHUnitTests.cpp

// get the right locale according to the PointLine language setting and the Operating System locale
std::locale GetSupportedLocale()
{
	SHVariable* pV1 = SHWinApp::GetVariable("LangCurrent");
	const std::string plLang   = (pV1 == 0 ? "" : static_cast<const char*>(pV1->AsString()));
	const std::locale osLocale = std::locale("");

	// for now we just use the users locale, but in the future we could limit the selection based on the 
	// PointLine language. To make sure the language and the locale fit together.

	return osLocale;
}

// stream manipulator to set the date format with strftime() format strings
// note : the base class is implementation specific!
class SH_CLASS_DECL setfmt : public std::_Smanip<const char*>
{
public:
	setfmt(const char* fmt) : std::_Smanip<const char*>(setfmt_, fmt) {}
private:
	static const int datfmtIdx;                                 

	static void setfmt_(std::ios_base& str, const char* fmt)
	{
		str.pword(datfmtIdx) = const_cast<char*>(fmt);           
	}

	template<class Ostream>
	friend Ostream & operator<<(Ostream &os, const struct tm &date);
};

// output formatter for good old c-struct tm
template<class Ostream>
Ostream & operator<<(Ostream &os, const struct tm &date)
{
	typedef typename Ostream::char_type					char_t;
	typedef typename Ostream::traits_type				traits_t;
	typedef std::ostreambuf_iterator<char_t, traits_t>  outIter_t;

	std::ios_base::iostate err = 0;
	char_t* fmt = 0;

	try 
	{
		typename std::basic_ostream<char_t, traits_t>::sentry opfx(os);

		if(opfx)
		{
			std::locale loc = os.getloc();

			// get the format
			char *patt      = static_cast<char*>(os.pword(setfmt.datfmtIdx)); 
			if(!patt || !strlen(patt))
				patt = "%c"; // use default if no format was provided
			std::size_t len = std::strlen(patt);
			fmt				= new char_t[len + 1];
			std::use_facet<std::ctype<char_t> >(loc).widen(patt, patt+len, fmt);

			// write the formatted string
			const std::time_put<char_t, outIter_t> &fac = std::use_facet<std::time_put<char_t, outIter_t> >(loc);
			outIter_t nextpos = fac.put(os, os, os.fill(), &date, fmt, fmt + len); 
			if(nextpos.failed())
				err = std::ios_base::badbit;
			os.width(0);
		}
	}
	catch(...)
	{
		delete[] fmt;
		bool flag = false;
		try 
		{
			os.setstate(std::ios_base::failbit);
		}
		catch(std::ios_base::failure) 
		{
			flag = true;
		}
		if(flag)
			throw;
	}

	delete[] fmt;
	if(err)
		os.setstate(err);

	return os;
}

// output formatter for WIN32 API SYSTEMTIME structure
template<class Ostream>
Ostream & operator<<(Ostream &os, const SYSTEMTIME &st)
{
	struct tm aDate;

	aDate.tm_year = st.wYear - 1900;
	aDate.tm_mon  = st.wMonth - 1;
	aDate.tm_mday = st.wDay;
	aDate.tm_hour = st.wHour;
	aDate.tm_min  = st.wMinute;
	aDate.tm_sec  = st.wSecond;
	
	os << aDate;

	return os;
}

// output formatter for WIN32 API FILETIME structure
template<class Ostream>
Ostream & operator<<(Ostream &os, const FILETIME &ft)
{
	SYSTEMTIME stUTC, stLocal;
	// Convert the last-write time to local time.
	FileTimeToSystemTime(&ft, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	os << stLocal;

	return os;
}



#endif
