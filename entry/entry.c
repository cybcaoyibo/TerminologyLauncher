#include "entry/common.h"

int main(int argc, char **args) {
	staticInit();
	staticLoggerInit();
	//TODO:
	staticLoggerFree();
	staticFree();
	return 0;
}

