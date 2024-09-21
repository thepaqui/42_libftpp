#include "pool.hpp"

// Pool

template <typename TType>
void
Pool<TType>::growPool(
	const size_t& newSize
)
{
	size_t	objectsToAllocate = newSize - totalAllocated;
	size_t	blockSize = 5; // Allocating in blocks of 5 to reduce overhead
	while (objectsToAllocate > 0) {
		size_t	allocateNow = std::min(blockSize, objectsToAllocate);
		Block	newBlock = { std::make_unique<TType[]>(allocateNow), allocateNow };

		for (size_t i = 0; i < allocateNow; i++) {
			availableObjects.push_back(&newBlock.data[i]);
		}

		allocatedBlocks.push_back(std::move(newBlock));
		totalAllocated += allocateNow;
		objectsToAllocate -= allocateNow;
	}
}

template <typename TType>
void
Pool<TType>::shrinkPool(
	const size_t& newSize
)
{
	size_t	objectsToRemove = totalAllocated - newSize;

	while (objectsToRemove > 0 && !)
}

template <typename TType>
void
Pool<TType>::resize(
	const size_t& numberOfObjectStored
)
{
	if (numberOfObjectStored > totalAllocated)
		growPool(numberOfObjectStored);
	else if (numberOfObjectStored < totalAllocated)
		shrinkPool(numberOfObjectStored);
}

template <typename TType>
void
Pool<TType>::returnObject(
	TType* obj
)
{
	// An object should never pass through here while it is already in availableObjects
	if (obj)
		availableObjects.push_back(obj);
}

// Pool::Object

// Constructor
template <typename TType>
Pool<TType>::Object::Object(
	Pool<TType>* p_pool,
	TType* p_object
)
: pool(p_pool), object(p_object)
{}

// Destructor
template <typename TType>
Pool<TType>::Object::~Object()
{
	if (pool)
		pool->returnObject(object);
}

template <typename TType>
TType*
Pool<TType>::Object::operator->()
{
	return object;
}
