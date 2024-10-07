#ifndef LIBFTPP_STATE_MACHINE_HPP
# define LIBFTPP_STATE_MACHINE_HPP

# include <vector>
# include <map>
# include <functional>
# include <algorithm>
# include <exception>

using Callback = std::function<void()>;

template <typename TState>
using TransitionPair = std::pair<TState, TState>;

template <typename TState>
class StateMachine {
private :
	TState	currentState = static_cast<TState>(0);

	std::vector<TState>							states;
	std::map<TState, Callback>					actions;
	std::map<TransitionPair<TState>, Callback>	transitions;

	bool	isStateKnown(const TState& state);

public :
	void	addState(const TState& state);
	void	addTransition(const TState& startState, const TState& finalState, const Callback& lambda);
	void	addAction(const TState& state, const Callback& lambda);

	void	transitionTo(const TState& state);
	void	update();

	/* Exceptions */
	class UnknownStateException : public std::exception {
	public :
		const char*	what() const noexcept { return "StateMachine: Unknown state."; }
	};

	class UndefinedTransitionException : public std::exception {
	public :
		const char*	what() const noexcept { return "StateMachine: Undefined transition."; }
	};

	class UndefinedActionException : public std::exception {
	public :
		const char*	what() const noexcept { return "StateMachine: Undefined action."; }
	};
};

# include "state_machine.tpp"

#endif