#include "UniqueQueue.hpp"

UniqueQueue::UniqueQueue(size_t n) : queue(), is_in_queue(n) {}

void UniqueQueue::push(size_t x) {
	if (is_in_queue[x])return;
	is_in_queue[x] = true;
	queue.push(x);
}

size_t UniqueQueue::pop() {
	auto ret = queue.front();
	queue.pop();
	is_in_queue[ret] = false;
	return ret;
}

bool UniqueQueue::empty() { return queue.empty(); }

void UniqueQueue::clear() {
	queue = {};
	std::fill(is_in_queue.begin(), is_in_queue.end(), false);
}
