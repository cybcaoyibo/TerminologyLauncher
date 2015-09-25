set -x
CFLAGS="-c -g -std=c99 --pedantic-error -Wall -Werror -Wfatal-errors -I /usr/local/include -I ."
gcc $CFLAGS entry/common.c
gcc $CFLAGS logger/logger.c
gcc $CFLAGS entry/entry.c
gcc $CFLAGS ../lcsc/string.c
gcc $CFLAGS ../lcsc/object.c
gcc $CFLAGS ../lcsc/comparable.c
gcc $CFLAGS ../lcsc/vector.c
windres -o res.o entry/res.rc
gcc -o TerminologyLauncher.exe -g -std=c99 --pedantic-error -Wall -Werror -Wfatal-errors common.o logger.o string.o object.o comparable.o vector.o res.o entry.o -luv -L /usr/local/lib
rm -f *.o
