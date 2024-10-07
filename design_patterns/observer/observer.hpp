#ifndef LIBFTPP_OBSERVER_HPP
# define LIBFTPP_OBSERVER_HPP

# include <map>
# include <vector>
# include <functional>

using Callback = std::function<void()>;

template <typename TEvent>
class Observer {
private :
	std::map<TEvent, std::vector<Callback>>	eventMap;
public :
	void	subscribe(const TEvent& event, const Callback& lambda);
	void	notify(const TEvent& event);
};

# include "observer.tpp"

#endif