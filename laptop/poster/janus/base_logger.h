#include <stdarg.h>
#include <stdio.h>
#include <vector>
#include <tuple>

#ifndef BASE_LOGGER_H
#define BASE_LOGGER_H
enum LOG_LEVEL
{
	FATAL,	// 0
	ERROR,	// 1
	TRACE,	// 2
	DEBUG,	// 3
	INFO,	// 4
	ALL,	// 5
	OFF		// 6
};
class BaseLogger
{
    private:

    public:
		FILE * logFile;
        BaseLogger (char * fileName);
        void writeToLogFile(
            LOG_LEVEL msgLevel,
            char * fmt,
            ...
        );
        int addSink(
            char * fileName,
			LOG_LEVEL logLevel,
			bool objectSink,
			const char mode[] = "a+"
        );
        int addSink(
            int fd,
			LOG_LEVEL logLevel,
			bool objectSink,
			const char *mode = "a+"
        );
        int addSink(
            FILE * filePtr,
			LOG_LEVEL logLevel,
			bool objectSink,
			const char *mode = "a+"
        );
};

#endif
