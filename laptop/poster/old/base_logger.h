#include <stdarg.h>
#include <stdio.h>
#include <vector>
#include <tuple>
#include <forward_list>

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
typedef struct _sink {
    FILE * file;
    enum LOG_LEVEL level_to_log_above;
    char * message_front;
}sink;
typedef struct _faucet {

}faucet;
class BaseLogger
{
    private:
        std::forward_list<sink*> * sinks;
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
			LOG_LEVEL level_to_log,
			const char mode[] = "a+"
        );
        int addSink(
            int fd,
			LOG_LEVEL level_to_log,
			const char *mode = "a+"
        );
        int addSink(
            FILE * filePtr,
			LOG_LEVEL logLevel,
			const char *mode = "a+"
        );
};

#endif
