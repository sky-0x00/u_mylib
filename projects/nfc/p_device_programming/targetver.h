#pragma once

#define UTILITY_NAME	L"Ultralight Dumper for Moscow Subway"

#define VERSION_MAJOR	0
#define VERSION_MINOR	0
#define VERSION_BUILD	2			// TODO: генерировать автоматически

#define SAVING_DIR		L"..\\data\\"
#ifdef _DEBUG
#	define DUMP_FILENAME		L"uid_1_34C45591D62576.dmp"
#endif

// Включение SDKDDKVer.h обеспечивает определение самой последней доступной платформы Windows.

// Если требуется выполнить сборку приложения для предыдущей версии Windows, включите WinSDKVer.h и
// задайте для макроса _WIN32_WINNT значение поддерживаемой платформы перед включением SDKDDKVer.h.

#include <SDKDDKVer.h>

#define DATE_YEAR_CH0	__DATE__[7]
#define DATE_YEAR_CH1	__DATE__[8]
#define DATE_YEAR_CH2	__DATE__[9]
#define DATE_YEAR_CH3	__DATE__[10]

#define DATE_DAY_CH0	( __DATE__[4] == ' ' ? '0' : __DATE__[4] )
#define DATE_DAY_CH1	__DATE__[5]

#define DATE_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define DATE_MONTH_IS_FEB (__DATE__[0] == 'F')
#define DATE_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define DATE_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define DATE_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define DATE_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define DATE_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define DATE_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define DATE_MONTH_IS_SEP (__DATE__[0] == 'S')
#define DATE_MONTH_IS_OCT (__DATE__[0] == 'O')
#define DATE_MONTH_IS_NOV (__DATE__[0] == 'N')
#define DATE_MONTH_IS_DEC (__DATE__[0] == 'D')

#define DATE_MONTH_CH0	( (DATE_MONTH_IS_OCT || DATE_MONTH_IS_NOV || DATE_MONTH_IS_DEC) ? '1' : '0' )
#define DATE_MONTH_CH1				\
    (								\
        (DATE_MONTH_IS_JAN) ? '1' : \
        (DATE_MONTH_IS_FEB) ? '2' : \
        (DATE_MONTH_IS_MAR) ? '3' : \
        (DATE_MONTH_IS_APR) ? '4' : \
        (DATE_MONTH_IS_MAY) ? '5' : \
        (DATE_MONTH_IS_JUN) ? '6' : \
        (DATE_MONTH_IS_JUL) ? '7' : \
        (DATE_MONTH_IS_AUG) ? '8' : \
        (DATE_MONTH_IS_SEP) ? '9' : \
        (DATE_MONTH_IS_OCT) ? '0' : \
        (DATE_MONTH_IS_NOV) ? '1' : \
        (DATE_MONTH_IS_DEC) ? '2' : \
        /* error default */    '?'  \
    )
