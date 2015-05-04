//gcc ${=CFLAGS} -Ofast -o knight knight.c;
#include<stdio.h>
#define SIZE 7
#define OFFSET 8
typedef unsigned long long int ULL;

ULL total = 0;
const unsigned int h_in_flag = (1<<SIZE)-1;
const ULL end_flag =  1ULL<<((OFFSET+1)*(SIZE-1));
const ULL full = ( ( 1ULL<<(OFFSET*SIZE))-1ULL ) \
      / ( (1ULL<<OFFSET)-1ULL ) * ((1ULL<<SIZE)-1ULL);

#define check_and_rec(nh, dv) { \
  if ( ((nh) & h_in_flag) && (v+(dv)>=0) && (v+(dv)<SIZE*OFFSET) ) { \
    const ULL s = ((ULL)(nh)) << (v+(dv)); \
    if ( ! (state&s) ) main_loop(nh, v+(dv), state|s); \
  } \
};

void main_loop(const unsigned int eh, const int v, const ULL state) {
  if ( state & end_flag ) {
      if ( state==full ) { total+=2; printf("%lld\n", total); }
  } else {
    check_and_rec(eh<<2, +OFFSET);
    check_and_rec(eh<<2, -OFFSET);
    check_and_rec(eh>>2, -OFFSET);
    check_and_rec(eh>>2, +OFFSET);
    check_and_rec(eh<<1, +2*OFFSET);
    check_and_rec(eh<<1, -2*OFFSET);
    check_and_rec(eh>>1, -2*OFFSET);
    check_and_rec(eh>>1, +2*OFFSET);
  }
}
 
int main(void) {
  printf("size %d\n", SIZE);
  main_loop(4,OFFSET,1ULL+(4ULL<<OFFSET));
  printf("total %lld\n", total);
}

