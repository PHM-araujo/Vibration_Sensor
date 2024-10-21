#ifndef BLOCKINGQUEUE_HPP
#define BLOCKINGQUEUE_HPP
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

struct AccelerometerData {
    float acceleration_x;
    float acceleration_y;
    float acceleration_z;
};

struct ProcessedData {  
    float acceleration_x;
    float acceleration_y;
    float acceleration_z;
};

template <typename T>
class BlockingQueue {
public:
    BlockingQueue(size_t max_size);
    ~BlockingQueue();

    void push(const T& item);
    T pop();
    void clear();

private:
    QueueHandle_t queue_;
};

extern BlockingQueue<ProcessedData> sending_queue;
extern BlockingQueue<AccelerometerData> processing_queue;

template <typename T>
BlockingQueue<T>::BlockingQueue(size_t max_size) {
    queue_ = xQueueCreate(max_size, sizeof(T));
}

template <typename T>
BlockingQueue<T>::~BlockingQueue() {
    vQueueDelete(queue_);
}

template <typename T>
void BlockingQueue<T>::push(const T& item) {
    xQueueSend(queue_, &item, portMAX_DELAY);
}

template <typename T>
T BlockingQueue<T>::pop() {
    T item;
    xQueueReceive(queue_, &item, portMAX_DELAY);
    return item;
}

template <typename T>
void BlockingQueue<T>::clear() {
    xQueueReset(queue_);
}

#endif // BLOCKINGQUEUE_HPP