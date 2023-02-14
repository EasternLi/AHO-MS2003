#include "UniqueQueue.hpp"

AHO_MS2003::UniqueQueue::UniqueQueue(size_t n) : queue(), is_in_queue(n) {}

void AHO_MS2003::UniqueQueue::push(size_t x) {
	if (is_in_queue[x])return;
	is_in_queue[x] = true;
	queue.push(x);
}

size_t AHO_MS2003::UniqueQueue::front() const { return queue.front(); }

void AHO_MS2003::UniqueQueue::pop() {
	is_in_queue[queue.front()] = false;
	queue.pop();
}

bool AHO_MS2003::UniqueQueue::empty() const { return queue.empty(); }

void AHO_MS2003::UniqueQueue::clear() {
	queue = {};
	std::fill(is_in_queue.begin(), is_in_queue.end(), false);
}
