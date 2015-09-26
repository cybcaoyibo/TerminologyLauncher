#include "entry/common.h"
#include "engine/engine.h"
#include "logger/logger.h"

void engineCtor(engine *this) {
	objectCtor(&this->superObject);
	string verStr; stringCtor(&verStr); engineGetVersion(this, &verStr);
	loggerLogF(&staticLogger, LOG_INFO, "Engine %s Initializing...", stringCStr(&verStr));
	dtorUtil(string, &verStr);
	
}

void engineRun(engine *this) {
	//TODO:
}

void engineGetVersion(engine *this, string *out) {
	stringAppendCStr(out, "A2");
}

