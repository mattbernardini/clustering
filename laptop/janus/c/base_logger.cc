#define _XOPEN_SOURCE 701// needed in order to access particular function capabilites


#include "base_logger.h"
#include <stdio.h>      // sprintf, fprintf, fflush
#include <string.h>     // memset, sprintf
#include <stdbool.h>    // bool values
#include <time.h>       // struct tm, timeval, timezone
#include <errno.h>      // errno
#include <unistd.h>     // getpid()
#include <stdarg.h>     // va_list, va_start
#include <sys/time.h>   // gettimeofday, timeval (for timestamp in microsecond)


const static int MAX_BUFFER_SIZE = 1000000;
const static int MSG_BUFFER = 1000000;
const static int TIME_BUFFER = 128;
BaseLogger::BaseLogger (char * fileName)
{
	this->logFile = fopen(fileName, "a+");
}
void BaseLogger::writeToLogFile(
    LOG_LEVEL msg_level,
    char * fmt,
    ...
)
{
	/*
	 * Allocate local variables
	 */
    struct timeval tv;
    struct tm *tm = NULL;
    char msgBuffer[MSG_BUFFER],
	timeStr[TIME_BUFFER],
	fullBuffer[MAX_BUFFER_SIZE];
    /*
     * Always memset our char arrays to 0
     */
    memset(msgBuffer, '\0', sizeof(char) * MSG_BUFFER);
    memset(timeStr, '\0', sizeof(char) * TIME_BUFFER);
    memset(fullBuffer, '\0', sizeof(char) * MAX_BUFFER_SIZE);
    /*
     * Parse input and put into buffer
     * Use vsnprintf to ensure we only write the allowed
     * number of characters
     */
    va_list args;
    va_start(args, fmt);
    vsnprintf (msgBuffer, MSG_BUFFER, fmt, args);
    /*
     * Create time structs
     */
    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);
    strftime(timeStr, TIME_BUFFER, "%x", tm);
    /*
     * Merge everything into one buffer
     */
    sprintf(fullBuffer, "%s %d:%02d:%02d.%7ld [%d] %s\n",
    		timeStr,
            tm->tm_hour,
            tm->tm_min,
            tm->tm_sec,
            tv.tv_usec,
			getpid(),
			msgBuffer);
    fprintf(this->logFile, "%s", fullBuffer);
    fflush(this->logFile);
}

int BaseLogger::addSink(
    char * file_name,
	LOG_LEVEL logLevel,
	bool objectSink,
	const char *mode
)
{
	return 0;
}
int BaseLogger::addSink(
    int fd,
	LOG_LEVEL logLevel,
	bool objectSink,
	const char *mode
)
{
    return 1;
}
int BaseLogger::addSink(
    FILE * file_ptr,
	LOG_LEVEL logLevel,
	bool objectSink,
	const char *mode
)
{
    return 1;
}
