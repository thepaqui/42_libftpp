#ifndef LIBFTPP_THREAD_SAFE_QUEUE_TPP
# define LIBFTPP_THREAD_SAFE_QUEUE_TPP

# include "thread_safe_queue.hpp"

template <typename TType>
void
ThreadSafeQueue<TType>::push_back(
	const TType& newElement
)
{
	std::lock_guard<std::mutex>	lock(mtx);
	dq.push_back(newElement);
}

template <typename TType>
void
ThreadSafeQueue<TType>::push_front(
	const TType& newElement
)
{
	std::lock_guard<std::mutex>	lock(mtx);
	dq.push_front(newElement);
}

template <typename TType>
TType
ThreadSafeQueue<TType>::pop_back()
{
	std::lock_guard<std::mutex>	lock(mtx);
	if (dq.empty())
		throw ThreadSafeQueueEmptyException();
	TType	ret = dq.back();
	dq.pop_back();
	return ret;
}

template <typename TType>
TType
ThreadSafeQueue<TType>::pop_front()
{
	std::lock_guard<std::mutex>	lock(mtx);
	if (dq.empty())
		throw ThreadSafeQueueEmptyException();
	TType	ret = dq.front();
	dq.pop_front();
	return ret;
}

#endif