#include <stdio.h>

int main(int argc, char **argv) {
  printf("%p\n", __builtin_return_address(0));
  printf("Program=%s Argument[0]=%s\n", argv[0], argv[1]);
}
