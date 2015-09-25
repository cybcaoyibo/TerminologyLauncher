#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

typedef struct logger {
	vector sinks;
	uv_mutex_t mtx;
	object superObject;
} logger;

struct loggerSink;

typedef struct loggerSinkVT {
	void(*addLine)(struct loggerSink *this, string *str);
} loggerSinkVT;

typedef struct loggerSink {
	loggerSinkVT *VT;
	object superObject;
} loggerSink;

void loggerCtor(logger *this);
void loggerAddSink(logger *this, loggerSink *sink);
void loggerLog(logger *this, string *str);
int loggerLogF(logger *this, const char *fmt, ...);

#endif

