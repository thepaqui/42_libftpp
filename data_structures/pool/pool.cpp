#include "pool.hpp"

// Pool

template <typename TType>
void
Pool<TType>::growPool(
	const size_t& newSize
)
{
	size_t	objectsToAllocate = newSize - totalAllocated;

	while (objectsToAllocate > 0) {
		auto	newObject = std::make_unique<TType>();

		availableObjects.push_back(newObject.get());
		allocatedObjects.push_back(std::move(newObject));

		totalAllocated++;
		objectsToAllocate--;
	}
}

template <typename TType>
void
Pool<TType>::shrinkPool(
	const size_t& newSize
)
{
	size_t	objectsToRemove = totalAllocated - newSize;

	while (objectsToRemove > 0 && !allocatedObjects.empty()) {
		if (availableObjects.empty())
			throw NoAvailableObjectsToRemoveException();

		TType*	obj = availableObjects.back();
		availableObjects.pop_back();

		auto it = std::find_if(
			allocatedObjects.begin(),
			allocatedObjects.end(),
			[obj](const std::unique_ptr<TType>& ptr) { return ptr.get() == obj; }
		);

		if (it != allocatedObjects.end()) {
			allocatedObjects.erase(it);
			totalAllocated--;
			objectsToRemove--;
		}
		// If a TType is in the available but not the allocated,
		// we remove it anyway cause it's not supposed to be there.
		// FYI, this sould never happen.
	}
	if (objectsToRemove > 0)
		throw NoAllocatedObjectsToRemoveException();
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
	// Pool should never be null here, the check is only for cleanliness
	if (pool)
		pool->returnObject(object);
}

template <typename TType>
TType*
Pool<TType>::Object::operator->()
{
	return object;
}
