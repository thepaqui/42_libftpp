#ifndef LIBFTPP_STRATEGY_SELECTOR_HPP
# define LIBFTPP_STRATEGY_SELECTOR_HPP

# include <vector>
# include <functional>

template <typename TType>
class StrategySelector {
public:
	using Strategy = std::function<void(TType&)>;
	using Condition = std::function<bool(const TType&)>;

	void addStrategy(Strategy strategy, Condition condition) {
		strategies.emplace_back(strategy, condition);
	}

	void handle(TType& data) {
		for (const auto& [strategy, condition] : strategies) {
			if (strategy && condition && condition(data)) {
				strategy(data);
				return;
			}
		}

		if (defaultStrategy)
			defaultStrategy(data);
	}

private:
	Strategy	defaultStrategy;

	std::vector<std::pair<Strategy, Condition>>	strategies;
};

# include "strategy_selector.tpp"

#endif