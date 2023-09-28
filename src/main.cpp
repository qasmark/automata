#include "./App/CVisualizerService.h"
#include "Infrastructure/CSV/CInputOutputAdapter.h"
#include "Infrastructure/Graphviz/CGraphVisualizer.h"
#include <iostream>

inline const std::string_view MEALY_TYPE = "mealy";
inline const std::string_view MOORE_TYPE = "moore";
inline const std::string_view FINITE_TYPE = "finite";

enum class AutomatonType
{
	Mealy,
	Moore,
	Finite,
};

struct Args
{
	AutomatonType automatonType;
	std::string inputFilename;
	std::string outputFilename;
};

Args ParseArgs(int argc, char** argv)
{
	if (argc != 4)
	{
		throw std::invalid_argument("usage: " + std::string(argv[0]) + " [mealy|moore|finite] [input csv filename] [output dot filename]");
	}

	AutomatonType type;
	auto const strType = std::string_view(argv[1]);
	if (strType == MEALY_TYPE)
	{
		type = AutomatonType::Mealy;
	}
	else if (strType == MOORE_TYPE)
	{
		type = AutomatonType::Moore;
	}
	else if (strType == FINITE_TYPE)
	{
		type = AutomatonType::Finite;
	}
	else
	{
		throw std::invalid_argument("invalid automaton type, choose 'mealy', 'moore' or 'finite'");
	}

	return {
		.automatonType = type,
		.inputFilename = argv[2],
		.outputFilename = argv[3],
	};
}

int main(int argc, char** argv)
{
	try
	{
		auto const args = ParseArgs(argc, argv);

		CVisualizerService service(
			std::make_unique<CInputOutputAdapter>(),
			std::make_unique<CGraphVisualizer>());

		switch (args.automatonType)
		{
		case AutomatonType::Mealy:
			service.DrawMealy(args.inputFilename, args.outputFilename);
			break;
		case AutomatonType::Moore:
			service.DrawMoore(args.inputFilename, args.outputFilename);
			break;
		case AutomatonType::Finite:
			service.DrawFinite(args.inputFilename, args.outputFilename);
			break;
		}
	}
	catch (std::exception const& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
