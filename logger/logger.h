#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

typedef struct logger {
	vector sinks;
	uv_mutex_t mtx;
	uint8_t minLevel;
	object superObject;
} logger;

struct loggerSink;

typedef struct loggerSinkVT {
	void(*addLine)(struct loggerSink *this, string *str);
} loggerSinkVT;

typedef struct loggerSink {
	loggerSinkVT *VT;
	uint8_t minLevel;
	object superObject;
} loggerSink;

void loggerCtor(logger *this);
void loggerAddSink(logger *this, loggerSink *sink, uint8_t minLevel);
void loggerLog(logger *this, uint8_t level, string *str);
void loggerLogF(logger *this, uint8_t level, const char *fmt, ...);

#define LOG_VERBOSE 0
#define LOG_DEBUG 1
#define LOG_INFO 2
#define LOG_WARN 3
#define LOG_ERROR 4

#endif

