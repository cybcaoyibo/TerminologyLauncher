set -x
CFLAGS="-c -g -std=c99 --pedantic-error -Wall -Werror -Wfatal-errors -I /usr/local/include -I . -DDEBUG"
gcc $CFLAGS entry/common.c
gcc $CFLAGS logger/logger.c
gcc $CFLAGS entry/entry.c
gcc $CFLAGS engine/engine.c
gcc $CFLAGS ../lcsc/string.c
gcc $CFLAGS ../lcsc/object.c
gcc $CFLAGS ../lcsc/comparable.c
gcc $CFLAGS ../lcsc/vector.c
windres -o res.o entry/res.rc
gcc -o TerminologyLauncher.exe -g -std=c99 --pedantic-error -Wall -Werror -Wfatal-errors common.o logger.o string.o object.o comparable.o vector.o res.o entry.o engine.o /usr/local/lib/libuv.a -L /usr/local/lib -lws2_32 -luserenv -liphlpapi -lpsapi
rm -f *.o
