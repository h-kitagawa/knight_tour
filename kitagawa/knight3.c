//gcc ${=CFLAGS} -Ofast -s -o knight3_7 knight3.c -lpthread
#include <pthread.h>
#include <stdio.h>
#ifndef SIZE
#define SIZE 7
#endif

#define BIT(a) (1ULL<<(a))
typedef unsigned long long int ULL;

#define RIGHTMOST (1ULL<<(SIZE-1))
#define FULL      (BIT(SIZE*SIZE)-1ULL)
const ULL end_flag = BIT((SIZE+1)*(SIZE-1));
const ULL nwcorner_flag = BIT(SIZE*(SIZE-1));
const ULL secorner_flag = BIT(SIZE-1);
const ULL ne_sww_flag = BIT((SIZE-2)*SIZE+(SIZE-3));
const ULL ne_ssw_flag = BIT((SIZE-3)*SIZE+(SIZE-2));


void main_loop(const unsigned int eh, const unsigned int v, const ULL state, ULL* sum);

void inline check_and_rec(const ULL nh, const unsigned int nv, const ULL state, ULL* sum) {
  if ( (nv<SIZE*SIZE)  && ! (state&(nh<<nv)) ) main_loop( nh, nv, state|(nh<<nv), sum );
}

void main_loop(const unsigned int eh, const unsigned int v, const ULL state, ULL* sum) {
  if ( state & end_flag ) { // reached the goal
      if ( state==FULL ) { printf("%lld\n", ++(*sum)); }
  } else {
    switch(eh) {
    case 1:
      check_and_rec(2, v+2*SIZE, state, sum);
      check_and_rec(2, v-2*SIZE, state, sum);
      check_and_rec(4, v-  SIZE, state, sum);
      check_and_rec(4, v+  SIZE, state, sum);
      break;
    case 2:
       if ((v==(SIZE-3)*SIZE) && !(state & nwcorner_flag)) {// nwcorner check
	 check_and_rec(4, (SIZE-2)*SIZE, state | nwcorner_flag,  sum);
       } else {
	 check_and_rec(1, v+2*SIZE, state, sum);
	 check_and_rec(4, v+2*SIZE, state, sum);
	 check_and_rec(4, v-2*SIZE, state, sum);
	 check_and_rec(1, v-2*SIZE, state, sum);
	 check_and_rec(8, v+  SIZE, state, sum);
	 check_and_rec(8, v-  SIZE, state, sum);
       }
      break;
    case RIGHTMOST:
      check_and_rec(RIGHTMOST>>1, v+2*SIZE, state, sum);
      check_and_rec(RIGHTMOST>>1, v-2*SIZE, state, sum);
      check_and_rec(RIGHTMOST>>2, v-  SIZE, state, sum);
      check_and_rec(RIGHTMOST>>2, v+  SIZE, state, sum);
      break;
    case RIGHTMOST>>1:
       if ((v==2*SIZE) && !(state & secorner_flag)) {// secorner check
	 check_and_rec(RIGHTMOST>>2, SIZE, state | secorner_flag,  sum);
       } else if ((v==(SIZE-3)*SIZE) && (state & ne_sww_flag)) {
	 // we have to reach the goal at the next move
	 if ( state==FULL-end_flag ) { printf("%lld\n", ++(*sum)); }
       } else {
	 check_and_rec(RIGHTMOST,    v+2*SIZE, state, sum);
	 check_and_rec(RIGHTMOST>>2, v+2*SIZE, state, sum);
	 check_and_rec(RIGHTMOST>>2, v-2*SIZE, state, sum);
	 check_and_rec(RIGHTMOST,    v-2*SIZE, state, sum);
	 check_and_rec(RIGHTMOST>>3, v+  SIZE, state, sum);
	 check_and_rec(RIGHTMOST>>3, v-  SIZE, state, sum);
       }
      break;
    case RIGHTMOST>>2:
      if ((v==SIZE) && !(state & secorner_flag)) {// secorner check
	check_and_rec(RIGHTMOST>>1, 2*SIZE, state | secorner_flag,  sum);
      } else if ((v==(SIZE-2)*SIZE) && (state & ne_ssw_flag)) {
	 // we have to reach the goal at the next move
	 if ( state==FULL-end_flag ) { printf("%lld\n", ++(*sum)); }
      } else {
	check_and_rec(RIGHTMOST>>1, v+2*SIZE, state, sum);
	check_and_rec(RIGHTMOST>>3, v+2*SIZE, state, sum);
	check_and_rec(RIGHTMOST>>3, v-2*SIZE, state, sum);
	check_and_rec(RIGHTMOST>>1, v-2*SIZE, state, sum);
	check_and_rec(RIGHTMOST>>4, v+  SIZE, state, sum);
	check_and_rec(RIGHTMOST,    v+  SIZE, state, sum);
	check_and_rec(RIGHTMOST,    v-  SIZE, state, sum);
	check_and_rec(RIGHTMOST>>4, v-  SIZE, state, sum);
      }
      break;
    default: // この中で switch しない方が速い
      if ((eh==4) && (v==(SIZE-2)*SIZE) && !(state & nwcorner_flag)) {// nwcorner check
	check_and_rec(2, (SIZE-3)*SIZE, state | nwcorner_flag,  sum);
      } else {
	check_and_rec(eh>>1, v+2*SIZE, state, sum);
	check_and_rec(eh<<1, v+2*SIZE, state, sum);
	check_and_rec(eh<<1, v-2*SIZE, state, sum);
	check_and_rec(eh>>1, v-2*SIZE, state, sum);
	check_and_rec(eh>>2, v+  SIZE, state, sum);
	check_and_rec(eh<<2, v+  SIZE, state, sum);
	check_and_rec(eh<<2, v-  SIZE, state, sum);
	check_and_rec(eh>>2, v-  SIZE, state, sum);
      }
    }
  }
}

ULL sum[5];
void* loop_1() {
  main_loop ( 16,      0, 1ULL+BIT(SIZE+2)+BIT(4), sum );
}
void* loop_2() {
  main_loop ( 16, 2*SIZE, 1ULL+BIT(SIZE+2)+BIT(2*SIZE+4), sum+1 );
}
void* loop_3() {
  main_loop (  8, 3*SIZE, 1ULL+BIT(SIZE+2)+BIT(3*SIZE+3), sum+2 );
}
void* loop_4() {
  main_loop (  2, 3*SIZE, 1ULL+BIT(SIZE+2)+BIT(3*SIZE+1), sum+3 );
}
void* loop_5() {
  main_loop (  1, 2*SIZE, 1ULL+BIT(SIZE+2)+BIT(2*SIZE), sum+4 );
}
int main(void) {
  ULL gt = 0;
  pthread_t tid1, tid2, tid3, tid4, tid5;
  printf("size %d\n", SIZE);

  pthread_create(&tid1, NULL, loop_1, NULL);
  pthread_create(&tid2, NULL, loop_2, NULL);
  pthread_create(&tid3, NULL, loop_3, NULL);
  pthread_create(&tid4, NULL, loop_4, NULL);
  pthread_create(&tid5, NULL, loop_5, NULL);

  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);
  pthread_join(tid3,NULL);
  pthread_join(tid4,NULL);
  pthread_join(tid5,NULL);

  for (int i=0;i<5;i++) {
    gt += sum[i]<<1;
    printf("%c: sum %lld\n", 'A'+i, sum[i]<<1);
  }
  printf("total %lld\n", gt);
}

