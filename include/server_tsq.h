#pragma once

#include <memory> 
#include <queue>
#include <mutex>
#include <condition_variable>

#include "common_utils.h"

class ThreadSafeQueue {

 private:
    std::queue<std::unique_ptr<instruction_t>> q;
    std::mutex mtx;
    //std:condition_variable cv;

    size_t max;

 public:
    ThreadSafeQueue(size_t max);
    ~ThreadSafeQueue();

    bool isEmpty();
    size_t getSize();
    void push(std::unique_ptr<instruction_t> && new_instruction);
    std::unique_ptr<instruction_t> pop();
};
