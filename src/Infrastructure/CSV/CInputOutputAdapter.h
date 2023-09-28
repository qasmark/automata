#pragma once

#include "../../App/IInputOutputAdapter.h"

class CInputOutputAdapter : public IInputOutputAdapter
{
public:
	MealyAutomaton GetMealy(std::string const& filename) override;
	MooreAutomaton GetMoore(std::string const& filename) override;
	FiniteAutomaton GetFinite(std::string const& filename) override;

private:
	using Spreadsheet = std::vector<std::vector<std::string>>;

	static auto const CSV_SEPARATOR = ';';
	static auto const STATE_AND_SIGNAL_SEPARATOR = '/';
	constexpr static auto const FINAL_STATE = "F";
	constexpr static auto const EMPTY_MOVE = "-";

	static Spreadsheet GetDataFromFile(std::string const& filename);
};
