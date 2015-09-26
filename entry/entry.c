#include "entry/common.h"
#include "engine/engine.h"

int main(int argc, char **args) {
	staticInit();
	if(procEventInit()) {
		printf("Another instance detected!\n");
	} else {
		staticLoggerInit();
		staticEngineInit();
		engineRun(&staticEngine);
		staticEngineFree();
		staticLoggerFree();
		procEventFree();
	}
	staticFree();
	return 0;
}

