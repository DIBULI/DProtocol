#include "dprotocol/circular_byte_array.hpp"
#include <cassert>
#include <iostream>
#include <string.h>

int main() {
  CircularByteArray cba(11);

  char data0[] = "HelloWorld";
  char data1[11];

  uint8_t res = cba.append((uint8_t *)data0, 11);
  assert(cba.size() == 11);
  assert(res == 0);

  res = cba.peek((uint8_t *)data1, 10, 0);
  assert(cba.size() == 11);
  assert(res == 0);
  assert(strcmp(data1, "HelloWorld") == 0);

  res = cba.append((uint8_t *)data0, 1);
  assert(cba.size() == 11);
  assert(res == 1);

  res = cba.remove(5);
  assert(cba.size() == 6);
  assert(res == 0);

  res = cba.append((uint8_t *)data0, 5);
  assert(cba.size() == 11);
  assert(res == 0);
  
  res = cba.peek((uint8_t *)data1, 11, 0);
  assert(cba.size() == 11);
  assert(res == 0);
  assert(strcmp(data1, "World\0Hello") == 0);

  return 0;
}