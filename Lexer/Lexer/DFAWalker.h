#pragma once
#include <functional>
#include <string>
#include <vector>
#include <stdexcept>

struct DFA
{
	DFA(std::vector<std::vector<int>> table, std::vector<int> terminals, std::vector<std::function<bool(char)>> transition�onditions)
		: table(table)
		, terminals(terminals)
		, transition�onditions(transition�onditions)
	{
		if (table.at(0).size() != transition�onditions.size())
		{
			throw std::runtime_error("���������� ��������� �� ��������� � ����������� ������� ���� ���������");
		}
	}

	std::vector<std::vector<int>> table;
	std::vector<int> terminals;
	std::vector<std::function<bool(char)>> transition�onditions;
};

class DFAWalker
{
public:
	DFAWalker(const DFA& dfa);

	bool IsGrammarWord(std::string const& word);

	bool GoToNextState(char ch);

	bool IsFinishedState();

private:
	int m_state = 0;
	DFA m_dfa;
};
