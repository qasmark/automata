#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct InitialStateAndInputSymbol
{
	std::string state;
	std::string symbol;
};

struct KeyHash
{
	std::size_t operator()(InitialStateAndInputSymbol const& key) const
	{
		return std::hash<std::string>()(key.state) ^ (std::hash<std::string>()(key.symbol) << 1);
	}
};

struct KeyEqual
{
	bool operator()(InitialStateAndInputSymbol const& lhs, InitialStateAndInputSymbol const& rhs) const
	{
		return lhs.state == rhs.state && lhs.symbol == rhs.symbol;
	}
};

struct DestinationStateAndSignal
{
	std::string state;
	std::string signal;
};

using MealyMoves = std::unordered_map<InitialStateAndInputSymbol, DestinationStateAndSignal, KeyHash, KeyEqual>;
using MooreMoves = std::unordered_map<InitialStateAndInputSymbol, std::string, KeyHash, KeyEqual>;

struct MealyAutomaton
{
	std::vector<std::string> states;
	std::vector<std::string> inputSymbols;
	MealyMoves moves;
};

struct MooreAutomaton
{
	std::vector<std::string> states;
	std::vector<std::string> inputSymbols;
	std::unordered_map<std::string, std::string> stateSignals;
	MooreMoves moves;
};

struct StateWithFinalIndication
{
	std::string state;
	bool isFinal;
};

struct FiniteAutomaton
{
	std::vector<StateWithFinalIndication> states;
	std::vector<std::string> inputSymbols;
	MooreMoves moves;
};

class IInputOutputAdapter
{
public:
	virtual MealyAutomaton GetMealy(std::string const& filename) = 0;
	virtual MooreAutomaton GetMoore(std::string const& filename) = 0;
	virtual FiniteAutomaton GetFinite(std::string const& filename) = 0;

	virtual ~IInputOutputAdapter() = default;
};

using IInputOutputAdapterPtr = std::unique_ptr<IInputOutputAdapter>;
