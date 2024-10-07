#ifndef LIBFTPP_MEMENTO_TPP
# define LIBFTPP_MEMENTO_TPP

# include "memento.hpp"

template <typename TType>
Memento::Snapshot&
operator<<(
	Memento::Snapshot& snapshot,
	const TType& obj
)
{
	snapshot.buffer << obj;
	return snapshot;
}

template <typename TType>
Memento::Snapshot&
operator>>(
	Memento::Snapshot& snapshot,
	TType& obj
)
{
	snapshot.buffer >> obj;
	return snapshot;
}

#endif