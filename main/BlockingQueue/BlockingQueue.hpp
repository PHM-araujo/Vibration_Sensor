#ifndef BLOCKINGQUEUE_HPP
#define BLOCKINGQUEUE_HPP
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <esp_log.h>

struct AccelerometerData {
    float velocity_x;
    float acceleration_x;
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

// Declaration of the static object
extern BlockingQueue<AccelerometerData> globalQueue;

// Template function definitions
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
    ESP_LOGI("BlockingQueue", "Pushing item");
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