#ifndef LIBFTPP_THREAD_SAFE_QUEUE_HPP
# define LIBFTPP_THREAD_SAFE_QUEUE_HPP

# include <deque>
# include <mutex>
# include <stdexcept>

template <typename TType>
class ThreadSafeQueue {
private :
	std::mutex			mtx;
	std::deque<TType>	dq;

public :
	void	push_back(const TType& newElement);
	void	push_front(const TType& newElement);
	TType	pop_back();
	TType	pop_front();

	/* Exceptions */
	class ThreadSafeQueueEmptyException : public std::runtime_error {
	public :
		explicit ThreadSafeQueueEmptyException()
		: std::runtime_error("ThreadSafeQueue: Queue is empty.") {}
	};
};

# include "thread_safe_queue.tpp"

#endif