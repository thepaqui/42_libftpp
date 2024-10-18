#ifndef LIBFTPP_STRATEGY_SELECTOR_HPP
# define LIBFTPP_STRATEGY_SELECTOR_HPP

# include <vector>
# include <functional>
# include <stdexcept>

template <typename TType>
class StrategySelector {
public :
	// If a strategy is null, nothing gets executed
	using Strategy = std::function<void(TType&)>;
	// If a Condition is null, the associated strategy will never be executed
	using Condition = std::function<bool(const TType&)>;

	enum class Mode {
		FIRST,			// If data matches multiple conditions, executes only oldest added strategy. If no match, uses default strategy
		ALL,			// If data matches multiple conditions, executes all associated strategies, in order of addition, including the default strategy (always last)
		ALL_NO_DEFAULT	// If data matches multiple conditions, executes all associated strategies, in order of addition, excluding the default strategy (even if the default is the only one that can execute)
	};

	StrategySelector(Strategy newDefaultStrategy, Mode newMode = Mode::FIRST);

	void	setDefaultStrategy(Strategy newDefaultStrategy);
	void	setMode(Mode newMode);
	void	addStrategy(Strategy strategy, Condition condition);
	void	handle(TType& data);

private :
	Mode		mode;
	Strategy	defaultStrategy;

	std::vector<std::pair<Strategy, Condition>>	strategies;

public :
	/* Exceptions */

	class InvalidModeException : public std::invalid_argument {
	public :
		explicit InvalidModeException()
		: invalid_argument("StrategySelector: Invalid mode.") {}
	};
};

# include "strategy_selector.tpp"

#endif