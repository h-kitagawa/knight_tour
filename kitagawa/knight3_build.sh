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

gcc ${CFLAGS} -g  -o knight3_5 -DSIZE=5 knight3.c -lpthread
TEST=`./knight3_5 |tail -1 | sed 's/total //'`
if [[ "$TEST" -ne 56  ]]; then
    echo "n=5: FAILED"; exit 1
else
    echo "n=5: PASSED"
fi
#rm knight3_5
gcc ${CFLAGS} -s -Ofast -o knight3_6 -DSIZE=6 knight3.c -lpthread
gcc ${CFLAGS} -s -Ofast -S -DSIZE=6 knight3.c # assembly

set_START
TEST=`./knight3_6 |tail -1 | sed 's/total //'`
get_ELAPS
if [[ "$TEST" -ne 0  ]]; then
    echo "n=6: FAILED"; exit 1
else
    echo "n=6: PASSED. elapsed time: $ELAPS ms"
fi

gcc ${CFLAGS} -s -Ofast -o knight3_7 knight3.c -lpthread
echo "To test n=7, execute ./knight3_7 "
