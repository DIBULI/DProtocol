#include "dprotocol/circular_byte_array.hpp"



CircularByteArray::CircularByteArray(uint32_t size) {
  byteArray = (uint8_t *) pvPortMalloc(sizeof(uint8_t) * size);
  arraySize = size;
  headerIndex = 0;
  tailIndex = 0;
  length = 0;
  xMutex = xSemaphoreCreateBinary();
  if (xMutex != NULL) {
    xSemaphoreGive(xMutex);
  }
}

CircularByteArray::~CircularByteArray() {
  vPortFree(byteArray);
}

uint8_t CircularByteArray::append(const uint8_t* data, uint32_t size) {
  taskENTER_CRITICAL();
  if (size > remain()) {
    taskEXIT_CRITICAL();
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
    taskEXIT_CRITICAL();
    return 0;
  }
}

uint8_t CircularByteArray::peek(uint8_t* data, uint32_t size, uint32_t offset) {
  taskENTER_CRITICAL();
  uint32_t lengthToTheEnd;
  if (size + offset > length) {
    taskEXIT_CRITICAL();
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
  taskEXIT_CRITICAL();
  return 0;
}

uint8_t CircularByteArray::remove(uint32_t size) {
  taskENTER_CRITICAL();

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
  taskEXIT_CRITICAL();

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
  taskENTER_CRITICAL();
  headerIndex = 0;
  tailIndex = 0;
  length = 0;
  taskEXIT_CRITICAL();
}

bool CircularByteArray::isFull() {
  return this->length == this->arraySize;
}