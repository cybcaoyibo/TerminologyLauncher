#include "entry/common.h"

int main(int argc, char **args) {
	staticInit();
	if(procEventInit()) {
		printf("Another instance detected!\n");
	} else {
		staticLoggerInit();
		//TODO:
		staticLoggerFree();
		procEventFree();
	}
	staticFree();
	return 0;
}

