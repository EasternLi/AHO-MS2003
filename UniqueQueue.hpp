#pragma once

#include <cstddef>
#include <queue>
#include <vector>

class UniqueQueue {
public:
	UniqueQueue(size_t n);
	
	void push(size_t x);
	
	size_t pop();
	
	bool empty();
	
	void clear();

private:
	std::queue<size_t> queue;
	std::vector<bool> is_in_queue;
};
