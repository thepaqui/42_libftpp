#ifndef LIBFTPP_STATE_MACHINE_TPP
# define LIBFTPP_STATE_MACHINE_TPP

# include "state_machine.hpp"

template <typename TState>
bool
StateMachine<TState>::isStateKnown(
	const TState& state
)
{
	return (std::find(states.begin(), states.end(), state) != states.end());
}

template <typename TState>
void
StateMachine<TState>::addState(
	const TState& state
)
{
	if (!isStateKnown(state))
		states.push_back(state);
}

template <typename TState>
void
StateMachine<TState>::addTransition(
	const TState& startState,
	const TState& finalState,
	const Callback& lambda
)
{
	if (!isStateKnown(startState) || !isStateKnown(finalState))
		throw UnknownStateException();

	const TransitionPair<TState>	tmp(startState, finalState);
	transitions[tmp] = lambda;
}

template <typename TState>
void
StateMachine<TState>::addAction(
	const TState& state,
	const Callback& lambda
)
{
	if (!isStateKnown(state))
		throw UnknownStateException();

	actions[state] = lambda;
}

template <typename TState>
void
StateMachine<TState>::transitionTo(
	const TState& state
)
{
	if (!isStateKnown(state))
		throw UnknownStateException();

	const TransitionPair<TState>	tmp(currentState, state);
	currentState = state;

	if (transitions.find(tmp) == transitions.end())
		throw UndefinedTransitionException();

	transitions[tmp]();
}

template <typename TState>
void
StateMachine<TState>::update()
{
	if (actions.find(currentState) == actions.end())
		throw UndefinedActionException();

	actions[currentState]();
}

#endif