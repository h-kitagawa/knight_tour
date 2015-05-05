#!/bin/bash
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
TEST=`(time ./knight3_6  ) 2>&1 >/dev/null| tail -n3 | head -n1|sed s/real//`
echo "n=6: $TEST"

