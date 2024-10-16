#ifndef LIBFTPP_OBSERVABLE_VALUE_TPP
# define LIBFTPP_OBSERVABLE_VALUE_TPP

# include "observable_value.hpp"

template <typename TType>
ObservableValue<TType>::ObservableValue()
{}

template <typename TType>
ObservableValue<TType>::ObservableValue(
	const TType& initialValue
)
: value(initialValue)
{}

template <typename TType>
void
ObservableValue<TType>::notify()
{
	for (const auto& callback : subscribers)
		if (callback)
			(*callback)();
}

template <typename TType>
ObservableValue<TType>&
ObservableValue<TType>::operator=(
	const TType& newValue
)
{
	setValue(newValue);
	return *this;
}

template <typename TType>
void
ObservableValue<TType>::setValue(
	const TType& newValue
)
{
	if (value == newValue)
		return ;

	value = newValue;
	notify();
}

template <typename TType>
const TType&
ObservableValue<TType>::getValue() const
{
	return value;
}

template <typename TType>
ObservableValue<TType>::Token
ObservableValue<TType>::subscribe(
	const Callback& lambda
)
{
	subscribers.emplace_back(lambda);
	return subscribers.size() - 1;
}

template <typename TType>
void
ObservableValue<TType>::unsubscribe(
	const Token& token
)
{
	if (token >= subscribers.size())
		throw InvalidTokenException();

	subscribers[token].reset();
}

#endif