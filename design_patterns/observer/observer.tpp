#ifndef LIBFTPP_OBSERVER_TPP
# define LIBFTPP_OBSERVER_TPP

# include "observer.hpp"

template <typename TEvent>
void
Observer<TEvent>::subscribe(
	const TEvent& event,
	const Callback& lambda
)
{
	eventMap[event].push_back(lambda);
}

template <typename TEvent>
void
Observer<TEvent>::notify(
	const TEvent& event
)
{
	if (eventMap.find(event) == eventMap.end())
		return ;
	for (auto& callback : eventMap[event])
		callback();
}

#endif