#include "dprotocol/circular_byte_array.hpp"



CircularByteArray::CircularByteArray(uint32_t size) {
  byteArray = (uint8_t *) malloc(size * sizeof(uint8_t));
  arraySize = size;
  headerIndex = 0;
  tailIndex = 0;
  // length = headerIndex < tailIndex ? headerIndex + size - tailIndex : headerIndex - tailIndex;
  length = 0;
}

CircularByteArray::~CircularByteArray() {
  delete byteArray;
}

uint8_t CircularByteArray::append(const uint8_t* data, uint32_t size) {
  std::lock_guard<std::mutex> lock(xMutex);
  if (size > remain()) {
    return 1;
  } else {
    uint32_t lengthToTheEnd;
    if (tailIndex > headerIndex) {
      memcpy(this->byteArray + headerIndex, data, size);
      headerIndex += size;
    } else {
      lengthToTheEnd = arraySize - headerIndex;
      if (lengthToTheEnd >= size) {  // if the remaining array (to the end) capacity can store all the request data
        memcpy(this->byteArray + headerIndex, data, size);
        headerIndex += size;
        headerIndex %= arraySize;
      } else {  // if the remaining array (to the end) capacity can  not store all the request data
        memcpy(this->byteArray + headerIndex, data, lengthToTheEnd);
        memcpy(this->byteArray, data + lengthToTheEnd, size - lengthToTheEnd);
        headerIndex = size - lengthToTheEnd;
      }
    }
    
    length += size;
    return 0;
  }
}

uint8_t CircularByteArray::peek(uint8_t* data, uint32_t size, uint32_t offset) {
  std::lock_guard<std::mutex> lock(xMutex);
  uint32_t lengthToTheEnd;
  if (size + offset > length) {
    return 1;
  } else {
    if (tailIndex + offset <= headerIndex) {
      memcpy(data, this->byteArray + this->tailIndex + offset, size);
    } else {
      if (tailIndex + offset + size <= arraySize) {
        memcpy(data, this->byteArray + this->tailIndex + offset, size);
      } else { // if the remaining array (to the end) capacity can not retrieve all the request data
        if (arraySize > (tailIndex + offset)) {
          lengthToTheEnd = arraySize - (tailIndex + offset);
          memcpy(data, this->byteArray + this->tailIndex + offset, lengthToTheEnd);
          memcpy(data + lengthToTheEnd, this->byteArray, size - lengthToTheEnd);
        } else {
          memcpy(data, this->byteArray + this->tailIndex + offset - arraySize, size);
        }
      }
    }
  }

  return 0;
}

uint8_t CircularByteArray::remove(uint32_t size) {
  std::lock_guard<std::mutex> lock(xMutex);

  if (size > length) {
    return 1;
  } else {
    if (headerIndex >= tailIndex) {
      tailIndex += size;
    } else {
      uint32_t lengthToTheEnd = arraySize - tailIndex;
      if (lengthToTheEnd >= size) {
        tailIndex += size;
        tailIndex %= arraySize;
      } else {
        tailIndex = size - lengthToTheEnd;
      }
    }
  }

  length -= size;
  return 0;
}

uint32_t CircularByteArray::remain() {
  return arraySize - length;
}

uint32_t CircularByteArray::size() {
  return length;
}

uint32_t CircularByteArray::totalSize() {
  return arraySize;
}

void CircularByteArray::clean() {
  std::lock_guard<std::mutex> lock(xMutex);
  headerIndex = 0;
  tailIndex = 0;
  length = 0;
}

bool CircularByteArray::isFull() {
  return this->length == this->arraySize;
}