#include "entry/common.h"
#include "logger/logger.h"

static void loggerDtor(object *th) {
	logger *this = downCastUtil(logger, superObject, th);
	for(size_t i = 0; i < vectorSize(&this->sinks); i++) {
		loggerSink *now = *vectorDereference(&this->sinks, i);
		objectDtorVirtual(&now->superObject);
		free(now->superObject.base);
	}
	dtorUtil(vector, &this->sinks);
	uv_mutex_destroy(&this->mtx);
	objectDtorSpecial(th);
}

void loggerCtor(logger *this) {
	objectCtor(&this->superObject);
	this->superObject.base = this;
	static objectVT VT = {.dtor = &loggerDtor};
	this->superObject.VT = &VT;
	vectorCtor(&this->sinks);
	if(uv_mutex_init(&this->mtx)) FAIL()
	this->minLevel = 0xff;
}

void loggerAddSink(logger *this, loggerSink *sink, uint8_t minLevel) {
	sink->minLevel = minLevel;
	uv_mutex_lock(&this->mtx);
	if(minLevel < this->minLevel) this->minLevel = sink->minLevel;
	vectorPushBack(&this->sinks, sink);
	uv_mutex_unlock(&this->mtx);
}

static bool loggerCheckLevel(logger *this, uint8_t level) {
	uv_mutex_lock(&this->mtx);
	bool rst = level >= this->minLevel;
	uv_mutex_unlock(&this->mtx);
	return rst;
}

static const char const *lvlStrs[] = {"VERBOSE", "DEBUG", "INFO", "WARN", "ERROR"};
#define nLvlStrs 5

void loggerLog(logger *this, uint8_t level, string *msg) {
	if(!loggerCheckLevel(this, level)) return;
	string str; stringCtor(&str);
	time_t stamp = time(0);
	struct tm info; memset(&info, 0, sizeof(info));
	localtime_r(&stamp, &info);
	stringAppendF(&str, "[%04d-%02d-%02d %02d:%02d:%02d][%" PRIu32 "][%s]: ", info.tm_year, info.tm_mon, info.tm_mday, info.tm_hour, info.tm_min, info.tm_sec, GetCurrentThreadId(), (level >= nLvlStrs) ? "UNKNOWN" : lvlStrs[level]);
	stringAppend(&str, msg);
	uv_mutex_lock(&this->mtx);
	for(size_t i = 0; i < vectorSize(&this->sinks); i++) {
		loggerSink *now = *vectorDereference(&this->sinks, i);
		if(level >= now->minLevel) {
			now->VT->addLine(now, &str);
		}
	}
	uv_mutex_unlock(&this->mtx);
	dtorUtil(string, &str);
}

void loggerLogF(logger *this, uint8_t level, const char *fmt, ...) {
	if(!loggerCheckLevel(this, level)) return;
	va_list args;
	va_start(args, fmt);
	int sz = vsnprintf(0, 0, fmt, args);
	if(sz < 0) {
		va_end(args);
		loggerLogF(this, level, "(vsnprintf failed)");
		return;
	}
	va_end(args);
	va_start(args, fmt);
	char *buff = malloc(sz + 1);
	vsnprintf(buff, sz + 1, fmt, args);
	string tmp; stringCtorFromCStrNTransfer(&tmp, buff, sz, true);
	loggerLog(this,level, &tmp);
	dtorUtil(string, &tmp);
	va_end(args);
}

static void loggerSinkStdoutAddLine(struct loggerSink *this, string *str) {
	fwrite(stringData(str), stringSize(str), 1, stdout);
	putchar('\n');
	fflush(stdout);
}

void loggerSinkStdoutCtor(loggerSink *this) {
	objectCtor(&this->superObject);
	this->superObject.base = this;
	static loggerSinkVT VT = {.addLine = &loggerSinkStdoutAddLine};
	this->VT = &VT;
}

static void loggerSinkFileDtor(object *th) {
	loggerSinkFile *this = downCastUtil(loggerSinkFile, superLoggerSink, downCastUtil(loggerSink, superObject, th));
	if(this->fp) fclose(this->fp);
	objectDtorSpecial(th);
}

static void loggerSinkFileAddLine(struct loggerSink *th, string *str) {
	loggerSinkFile *this = downCastUtil(loggerSinkFile, superLoggerSink, th);
	if(this->fp) {
		fwrite(stringData(str), stringSize(str), 1, this->fp);
		putc('\n', this->fp);
		fflush(this->fp);
	}
}

void loggerSinkFileCtor(loggerSinkFile *this, const char *fn) {
	objectCtor(&this->superLoggerSink.superObject);
	this->superLoggerSink.superObject.base = this;
	static loggerSinkVT _VT1 = {.addLine = &loggerSinkFileAddLine};
	this->superLoggerSink.VT = &_VT1;
	static objectVT _VT2 = {.dtor = &loggerSinkFileDtor};
	this->superLoggerSink.superObject.VT = &_VT2;
	this->fp = fopen(fn, "a");
}

