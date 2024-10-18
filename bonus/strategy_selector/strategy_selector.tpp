#ifndef LIBFTPP_STRATEGY_SELECTOR_TPP
# define LIBFTPP_STRATEGY_SELECTOR_TPP

# include "strategy_selector.hpp"

template <typename TType>
StrategySelector<TType>::StrategySelector(
	Strategy newDefaultStrategy,
	Mode newMode
)
: mode(newMode), defaultStrategy(newDefaultStrategy)
{}

template <typename TType>
void
StrategySelector<TType>::setMode(
	Mode newMode
)
{
	if (newMode != Mode::FIRST && newMode != Mode::ALL && newMode != Mode::ALL_NO_DEFAULT)
		throw InvalidModeException();
	mode = newMode;
}

template <typename TType>
void
StrategySelector<TType>::setDefaultStrategy(
	Strategy newDefaultStrategy
)
{
	defaultStrategy = newDefaultStrategy;
}

template <typename TType>
void
StrategySelector<TType>::addStrategy(
	Strategy strategy,
	Condition condition
)
{
	strategies.emplace_back(strategy, condition);
}

template <typename TType>
void
StrategySelector<TType>::handle(
	TType& data
)
{
	for (const auto& [strategy, condition] : strategies) {
		if (condition && condition(data)) {
			if (strategy)
				strategy(data);
			if (mode == Mode::FIRST)
				return ;
		}
	}

	if (mode != Mode::ALL_NO_DEFAULT && defaultStrategy)
		defaultStrategy(data);
}

#endif