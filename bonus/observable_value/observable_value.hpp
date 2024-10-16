#ifndef LIBFTPP_OBSERVABLE_VALUE_HPP
# define LIBFTPP_OBSERVABLE_VALUE_HPP

# include <exception>
# include <vector>
# include <optional>
# include <functional>

using Callback = std::function<void()>;

template <typename TType>
class ObservableValue {
private :
	TType									value;
	std::vector<std::optional<Callback>>	subscribers;

	void	notify();

public :
	using Token = size_t;

	ObservableValue();
	ObservableValue(const TType& initialValue);
	ObservableValue&	operator=(const TType& newValue);
	void				setValue(const TType& newValue);

	const TType&	getValue() const;

	Token	subscribe(const Callback& lambda);
	void	unsubscribe(const Token& token);

	/* Exceptions */

	class InvalidTokenException : public std::exception {
	public :
		const char* what() const noexcept { return "ObservableValue: Ivalid token."; }
	};
};

# include "observable_value.tpp"

#endif