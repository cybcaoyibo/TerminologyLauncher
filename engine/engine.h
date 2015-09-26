#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

typedef struct engine {
	object superObject;
} engine;

void engineCtor(engine *this);
void engineRun(engine *this);
void engineGetVersion(engine *this, string *out);

#endif

