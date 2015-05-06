#!/bin/bash
set_START() {
        local dummy
        read START dummy < /proc/uptime
}

get_ELAPS() {
        local dummy
        read END dummy < /proc/uptime
        let ELAPS=${END/./}0-${START/./}0
}

g++ ${CFLAGS} -g  -std=c++11 -o knight5_5 -DSIZE=5 knight5.cc -lpthread || exit 1
g++ ${CFLAGS} -s -std=c++11 -Ofast -o knight5_6 -DSIZE=6 knight5.cc -lpthread
g++ ${CFLAGS} -s -std=c++11 -Ofast -o knight5_7 knight5.cc -lpthread
TEST=`./knight5_5 |tail -1 | sed 's/total //'`
if [[ "$TEST" -ne 56  ]]; then
    echo "n=5: FAILED"; exit 1
else
    echo "n=5: PASSED"
fi
#rm knight5_5
g++ ${CFLAGS} -s -std=c++11 -Ofast -S -DSIZE=6 knight5.cc # assembly

set_START
TEST=`./knight5_6 |tail -1 | sed 's/total //'`
get_ELAPS
if [[ "$TEST" -ne 0  ]]; then
    echo "n=6: FAILED"; exit 1
else
    echo "n=6: PASSED. elapsed time: $ELAPS ms"
fi

echo "To test n=7, execute ./knight5_7 "
