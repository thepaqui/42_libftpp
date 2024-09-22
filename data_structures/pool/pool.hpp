#ifndef LIBFTPP_POOL_HPP
# define LIBFTPP_POOL_HPP

# include <vector>
# include <memory>
# include <new>

template <typename TType>
class Pool {
private :
	// Unique pointers will automatically free all TTypes when Pool goes out of scope
	std::vector<std::unique_ptr<TType>>	allocatedObjects;
	std::vector<TType*>					availableObjects;
	size_t								totalAllocated = 0;

	void	growPool(const size_t& newSize);
	void	shrinkPool(const size_t& newSize);

	void	returnObject(TType* obj);

public :
	void	resize(const size_t& numberOfObjectStored);

	class Object {
	private :
		Pool<TType>*	pool;
		TType*			object;

		Object(Pool<TType>* p_pool, TType* p_object);

	public :
		~Object();
		TType*	operator->();
	};

	template <typename... TArgs>
	Object	acquire(TArgs&&... p_args)
	{
		if (availableObjects.empty())
			throw NoAvailableObjectsException();

		TType*	obj = availableObjects.back();
		availableObjects.pop_back();

		// Placement new reconstructs an object in the same memory location
		new(obj) TType(std::forward<TArgs>(p_args)...);

		Object	objectWrapper(this, obj);
		return objectWrapper;
	}

	/* Exceptions */
	class NoAvailableObjectsException : public std::exception {
	public :
		const char*	what() const noexcept { return "Pool: No available objects."; }
	};

	class NoAvailableObjectsToRemoveException : public std::exception {
	public :
		const char*	what() const noexcept { return "Pool: No available objects to remove."; }
	};

	class NoAllocatedObjectsToRemoveException : public std::exception {
	public :
		const char*	what() const noexcept { return "Pool: No allocated objects to remove."; }
	};
};

#endif