#pragma once

#include <cstddef>
#include <queue>
#include <vector>

namespace AHO_MS2003 {

class UniqueQueue {
public:
	explicit UniqueQueue(size_t n);
	
	void push(size_t x);
	
	[[nodiscard]] size_t front() const;
	
	void pop();
	
	[[nodiscard]] bool empty() const;
	
	void clear();

private:
	std::queue<size_t> queue;
	std::vector<bool> is_in_queue;
};

}
