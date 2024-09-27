#ifndef LIBFTPP_SINGLETON_TPP
# define LIBFTPP_SINGLETON_TPP

# include "singleton.hpp"

template <typename TType>
TType*
Singleton<TType>::instance()
{
	if (!obj)
		throw NoInstanceException();
	return obj.get();
}

template <typename TType>
template <typename ... TArgs>
void
Singleton<TType>::instantiate(
	TArgs&&... p_args
)
{
	if (obj)
		throw AlreadyAnInstanceException();
	obj.reset(new TType(std::forward<TArgs>(p_args)...));
}

#endif