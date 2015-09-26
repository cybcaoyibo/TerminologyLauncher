#include "entry/common.h"
#include "logger/logger.h"

void afail(const char *file, int line) {
	string str; stringCtor(&str);
	stringAppendF(&str, "%s:%d", file, line);
	fatal(stringCStr(&str));
}

void fatal(const char *msg) {
	string str;
	stringCtorFromCStr(&str, "fatal error:\n");
	stringAppendRaw(&str, (char*)msg, strlen(msg));
	fprintf(stderr, "%s", stringCStr(&str));
	fflush(stderr);
	FILE *fp = fopen("fatal.err", "w");
	if(fp) {
		fprintf(fp, "%s", stringCStr(&str));
		fclose(fp);
	}
	ExitProcess(-1);
}

static uv_mutex_t mtxLocalTime;

int localtime_r(time_t *stamp, struct tm *info) {
	uv_mutex_lock(&mtxLocalTime);
	struct tm *new = localtime(stamp);
	int rst = 0;
	if(new) rst = -1;
	else memcpy(info, new, sizeof *info);
	uv_mutex_unlock(&mtxLocalTime);
	return rst;
}

void staticInit() {
	if(uv_mutex_init(&mtxLocalTime)) FAIL()
}

void staticFree() {
	uv_mutex_destroy(&mtxLocalTime);
}

struct logger staticLogger;

void staticLoggerInit() {
	loggerCtor(&staticLogger);
	loggerSink *sinkStdout = malloc(sizeof *sinkStdout);
	loggerSinkStdoutCtor(sinkStdout);
	loggerSinkFile *sinkFile = malloc(sizeof *sinkFile);
	loggerSinkFileCtor(sinkFile, "TerminologyLauncher.log");
	loggerAddSink(&staticLogger, sinkStdout, DEBUG_VAL ? LOG_DEBUG : LOG_INFO);
	loggerAddSink(&staticLogger, &sinkFile->superLoggerSink, DEBUG_VAL ? LOG_DEBUG : LOG_INFO);
}

void staticLoggerFree() {
	objectDtorVirtual(&staticLogger.superObject);
}

