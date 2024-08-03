#ifndef CIRCULAR_BYTE_ARRAY_HPP
#define CIRCULAR_BYTE_ARRAY_HPP

#include <stdlib.h>
#include <stdint.h>
#include <cstring>
#include <mutex>

class CircularByteArray {
public:
  CircularByteArray(uint32_t size);
  ~CircularByteArray();

  uint8_t append(const uint8_t* data, uint32_t size);
  uint8_t peek(uint8_t* data, uint32_t size, uint32_t offset);
  uint8_t remove(uint32_t size);
  uint32_t remain();
  uint32_t size();
  bool isFull();
  uint32_t totalSize();
  void clean();

private:
  uint32_t headerIndex;
  uint32_t tailIndex;
  uint8_t* byteArray;
  uint32_t arraySize;
  uint32_t length;
  std::mutex xMutex;
};

#endif /* CIRCULAR_BYTE_ARRAY_HPP */
