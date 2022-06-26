
#include "server_tsq.h"

ThreadSafeQueue::ThreadSafeQueue(size_t max) {

    this->max = max;
}

ThreadSafeQueue::~ThreadSafeQueue() {

}

bool ThreadSafeQueue::isEmpty() {

    return this->q.empty();
}

size_t ThreadSafeQueue::getSize() {

    return this->q.size();
}

void ThreadSafeQueue::push(std::unique_ptr<instruction_t> && new_instruction) {

    std::lock_guard<std::mutex> lock(this->mtx);
    this->q.push(std::move(new_instruction));
    return;
}

std::unique_ptr<instruction_t> ThreadSafeQueue::pop() {

    std::lock_guard<std::mutex> lock(this->mtx);
    std::unique_ptr<instruction_t> next_instruction = std::move(this->q.front());
    this->q.pop();
    return next_instruction;
}
