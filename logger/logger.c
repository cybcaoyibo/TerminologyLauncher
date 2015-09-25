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
}

void loggerAddSink(logger *this, loggerSink *sink) {
	uv_mutex_lock(&this->mtx);
	vectorPushBack(&this->sinks, sink);
	uv_mutex_unlock(&this->mtx);
}

void loggerLog(logger *this, string *msg) {
	string str; stringCtor(&str);
	time_t stamp = time(0);
	struct tm info; memset(&info, 0, sizeof(info));
	localtime_r(&stamp, &info);
	stringAppendF(&str, "[%04d-%02d-%02d %02d:%02d:%02d][%" PRIu32 "]: ", info.tm_year, info.tm_mon, info.tm_mday, info.tm_hour, info.tm_min, info.tm_sec, GetCurrentThreadId());
	stringAppend(&str, msg);
	uv_mutex_lock(&this->mtx);
	for(size_t i = 0; i < vectorSize(&this->sinks); i++) {
		loggerSink *now = *vectorDereference(&this->sinks, i);
		now->VT->addLine(now, &str);
	}
	uv_mutex_unlock(&this->mtx);
	dtorUtil(string, &str);
}

int loggerLogF(logger *this, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	int sz = vsnprintf(0, 0, fmt, args);
	if(sz < 0) {
		va_end(args);
		loggerLogF(this, "(vsnprintf failed)");
		return sz;
	}
	va_end(args);
	va_start(args, fmt);
	char *buff = malloc(sz + 1);
	vsnprintf(buff, sz + 1, fmt, args);
	string tmp; stringCtorFromCStrNTransfer(&tmp, buff, sz, true);
	loggerLog(this, &tmp);
	dtorUtil(string, &tmp);
	va_end(args);
	return sz;
}

