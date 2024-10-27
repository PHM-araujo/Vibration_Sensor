#include "BlockingQueue.hpp"

BlockingQueue<AccelerometerData> processing_queue(5); 
BlockingQueue<ProcessedData> sending_queue(5); 
