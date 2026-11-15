#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Suboptimal alignment
struct ExampleSuboptimal {
  char c; // 1
  // padding 3
  int i;  // 4
  char d; // 1
  // padding 7
  double x; // 8
};

// Optimal alignment
struct ExampleOptimal {
  double x; // 0
  int i;    // 8
  char c;   // 12
  char d;   // 13
  // padding 2
};

int main() {
  printf("Char size: %zu\n", sizeof(char));
  printf("Char alignment: %zu\n", alignof(char));

  // Expect 1

  printf("Short size: %zu\n", sizeof(short));
  printf("Short alignment: %zu\n", alignof(short));

  // Expect 2

  printf("Int size: %zu\n", sizeof(int));
  printf("Int alignment: %zu\n", alignof(int));

  // Expect 4

  printf("Long size: %zu\n", sizeof(long));
  printf("Long alignment: %zu\n", alignof(long));

  // Expect 4

  printf("Double size: %zu\n", sizeof(double));
  printf("Double alignment: %zu\n", alignof(double));

  // Expect 8

  printf("Pointer size: %zu\n", sizeof(void *));
  printf("Pointer alignment: %zu\n", alignof(void *));

  // Expect 8

  printf("Max align size: %zu\n", sizeof(max_align_t));
  printf("Max align alignment: %zu\n", alignof(max_align_t));

  // Expect 8

  /////////////////////////////////////////////////////////////////////////////////////////////

  printf("Suboptimal struct size: %zu\n", sizeof(struct ExampleSuboptimal));

  // Expect 24 with padding

  printf("Offset of subotimal first char: %zu\n", offsetof(struct ExampleSuboptimal, c));

  // Expect  0

  printf("Offset of suboptimal int: %zu\n", offsetof(struct ExampleSuboptimal, i));

  // Expect 4

  printf("Offset of suboptimal second char: %zu\n", offsetof(struct ExampleSuboptimal, d));

  // Expect 8

  printf("Offset of suboptimal double: %zu\n", offsetof(struct ExampleSuboptimal, x));

  // Expect 16

  /////////////////////////////////////////////////////////////////////////////////////////////

  printf("Optimal struct size: %zu\n", sizeof(struct ExampleOptimal));

  // Expect 16 with padding

  printf("Offset of optimal double: %zu\n", offsetof(struct ExampleOptimal, x));

  // Expect 0

  printf("Offset of optimal int: %zu\n", offsetof(struct ExampleOptimal, i));

  // Expect 8

  printf("Offset of optimal first char: %zu\n", offsetof(struct ExampleOptimal, c));

  // Expect 12

  printf("Offset of optimal second char: %zu\n", offsetof(struct ExampleOptimal, d));

  // Expect 13

  /////////////////////////////////////////////////////////////////////////////////////////////

  void *one_byte_p = malloc(1);
  printf("1 byte pointer %p, it's address 16 remainder is %lu\n", one_byte_p,
         (uintptr_t)one_byte_p % 16);

  // Expect 0

  void *three_byte_p = malloc(3);
  printf("3 byte pointer %p, it's address 16 remainder is %lu\n", three_byte_p,
         (uintptr_t)three_byte_p % 16);

  // Expect 0

  void *tw_four_byte_p = malloc(24);
  printf("24 byte pointer %p, it's address 16 remainder is %lu\n", tw_four_byte_p,
         (uintptr_t)tw_four_byte_p % 16);

  // Expect 0

  void *hundred_byte_p = malloc(100);
  printf("100 byte pointer %p, it's address 16 remainder is %lu\n", hundred_byte_p,
         (uintptr_t)hundred_byte_p % 16);

  // Expect 0
}
