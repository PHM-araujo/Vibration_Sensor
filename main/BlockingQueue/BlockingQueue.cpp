#include "BlockingQueue.hpp"

BlockingQueue<AccelerometerData> processing_queue(30); 
BlockingQueue<ProcessedData> sending_queue(30); 
