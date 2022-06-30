
#include "server_tsq.h"

ThreadSafeQueue::ThreadSafeQueue(size_t max) {

    this->max = max;
}

ThreadSafeQueue::~ThreadSafeQueue() {

}

bool ThreadSafeQueue::isEmpty() {

    std::lock_guard<std::mutex> lock(this->mtx);
    return this->q.empty();
}

size_t ThreadSafeQueue::getSize() {

    std::lock_guard<std::mutex> lock(this->mtx);
    return this->q.size();
}

void ThreadSafeQueue::push(std::unique_ptr<instruction_t> && new_instruction) {

    std::lock_guard<std::mutex> lock(this->mtx);
    this->q.push(std::move(new_instruction));
    this->cv.notify_one();
    return;
}

std::unique_ptr<instruction_t> ThreadSafeQueue::pop() {

    //std::lock_guard<std::mutex> lock(this->mtx);
    std::unique_lock<std::mutex> lock(this->mtx);

    while (this->q.empty())
        this->cv.wait(lock);

    std::unique_ptr<instruction_t> next_instruction = std::move(this->q.front());
    this->q.pop();

    // si está vacía, el pop deberia bloquearse

    return next_instruction;
}
