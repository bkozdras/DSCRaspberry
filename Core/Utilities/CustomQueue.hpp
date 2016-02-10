#pragma once

#include <queue>

template<class T>
class QueueAdapter
{
	public:
		QueueAdapter(std::queue<T>& q) : _q(q) {}
		void push_back(const T& t) { _q.push(t); }

	private:
		std::queue<T>& _q;
};

template<class T>
using CustomQueue = QueueAdapter<std::queue<T>>;
