#include "CInputOutputAdapter.h"
#include <fstream>
#include <iostream>
#include <sstream>

MealyAutomaton CInputOutputAdapter::GetMealy(std::string const& filename)
{
	Spreadsheet spreadsheet = GetDataFromFile(filename);

	std::vector<std::string> states;
	std::vector<std::string> inputSymbols;
	MealyMoves moves;

	std::vector<std::string> elements;

	for (int rowIndex = 0; rowIndex < spreadsheet.size(); ++rowIndex)
	{
		for (int columnIndex = 0; columnIndex < spreadsheet[rowIndex].size(); ++columnIndex)
		{
			if (rowIndex == 0 && columnIndex == 0)
			{
				continue;
			}

			if (rowIndex == 0)
			{
				states.push_back(spreadsheet[rowIndex][columnIndex]);
				continue;
			}

			if (columnIndex == 0)
			{
				inputSymbols.push_back(spreadsheet[rowIndex][columnIndex]);
				continue;
			}

			std::stringstream ss(spreadsheet[rowIndex][columnIndex]);
			std::string element;
			while (std::getline(ss, element, STATE_AND_SIGNAL_SEPARATOR))
			{
				elements.push_back(element);
			}

			moves.insert({ { spreadsheet[0][columnIndex], spreadsheet[rowIndex][0] },
				{ elements[0], elements[1] } });

			elements.clear();
		}
	}

	return {
		.states = states,
		.inputSymbols = inputSymbols,
		.moves = moves,
	};
}

MooreAutomaton CInputOutputAdapter::GetMoore(std::string const& filename)
{
	Spreadsheet spreadsheet = GetDataFromFile(filename);

	std::vector<std::string> states;
	std::vector<std::string> inputSymbols;
	std::unordered_map<std::string, std::string> stateSignals;
	MooreMoves moves;

	for (int rowIndex = 0; rowIndex < spreadsheet.size(); ++rowIndex)
	{
		for (int columnIndex = 0; columnIndex < spreadsheet[rowIndex].size(); ++columnIndex)
		{
			if (rowIndex == 0)
			{
				continue;
			}

			if (rowIndex == 1)
			{
				if (columnIndex == 0)
				{
					continue;
				}
				states.push_back(spreadsheet[rowIndex][columnIndex]);
				stateSignals.insert({ spreadsheet[rowIndex][columnIndex], spreadsheet[rowIndex - 1][columnIndex] });
				continue;
			}

			if (columnIndex == 0)
			{
				inputSymbols.push_back(spreadsheet[rowIndex][columnIndex]);
				continue;
			}

			moves.insert({ { spreadsheet[1][columnIndex], spreadsheet[rowIndex][0] }, spreadsheet[rowIndex][columnIndex] });
		}
	}

	return {
		.states = states,
		.inputSymbols = inputSymbols,
		.stateSignals = stateSignals,
		.moves = moves,
	};
}

FiniteAutomaton CInputOutputAdapter::GetFinite(std::string const& filename)
{
	Spreadsheet spreadsheet = GetDataFromFile(filename);

	std::vector<StateWithFinalIndication> states;
	std::vector<std::string> inputSymbols;
	MooreMoves moves;

	for (int rowIndex = 0; rowIndex < spreadsheet.size(); ++rowIndex)
	{
		for (int columnIndex = 0; columnIndex < spreadsheet[rowIndex].size(); ++columnIndex)
		{
			if (rowIndex == 0 && columnIndex == 0 || rowIndex == 1)
			{
				continue;
			}

			if (rowIndex == 0)
			{
				bool isFinal = spreadsheet[rowIndex][columnIndex] == FINAL_STATE;
				states.push_back({ spreadsheet[rowIndex + 1][columnIndex],
					isFinal });
				continue;
			}

			if (columnIndex == 0)
			{
				inputSymbols.push_back(spreadsheet[rowIndex][columnIndex]);
				continue;
			}

			if (spreadsheet[rowIndex][columnIndex] == EMPTY_MOVE)
			{
				continue;
			}

			moves.insert({ { spreadsheet[1][columnIndex], spreadsheet[rowIndex][0] }, spreadsheet[rowIndex][columnIndex] });
		}
	}

	return {
		.states = states,
		.inputSymbols = inputSymbols,
		.moves = moves
	};
}

CInputOutputAdapter::Spreadsheet CInputOutputAdapter::GetDataFromFile(std::string const& filename)
{
	std::ifstream input(filename);
	if (!input.is_open())
	{
		throw std::runtime_error("failed to open input file for reading");
	}

	Spreadsheet spreadsheet;
	std::string row;
	std::string cell;

	while (std::getline(input, row))
	{
		std::istringstream iss(row);
		std::vector<std::string> rows;

		while (std::getline(iss, cell, CSV_SEPARATOR))
		{
			rows.push_back(cell);
		}

		spreadsheet.push_back(rows);
	}

	auto const firstRowSize = spreadsheet[0].size();
	auto const secondRowSize = spreadsheet[1].size();
	if (firstRowSize < secondRowSize)
	{
		for (std::size_t i = firstRowSize; i < secondRowSize; ++i)
		{
			spreadsheet[0].emplace_back("");
		}
	}

	return spreadsheet;
}
