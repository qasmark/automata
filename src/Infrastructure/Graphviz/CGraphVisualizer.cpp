#include "CGraphVisualizer.h"
#include <fstream>

void CGraphVisualizer::SetVertices(IGraphVisualizer::Vertices&& vertices)
{
	m_vertices = std::move(vertices);
}

void CGraphVisualizer::SetEdges(IGraphVisualizer::Edges&& edges)
{
	m_edges = std::move(edges);
}

void CGraphVisualizer::DrawGraph(const std::string& filename)
{
	std::ofstream output(filename);
	if (!output.is_open())
	{
		throw std::runtime_error("failed to open output file for writing");
	}

	output << "digraph G {\n";

	for (std::size_t i = 0; i < m_vertices.size(); ++i)
	{
		output << i << " [label=\"" << m_vertices[i] << "\"];\n";
	}

	for (auto&& edge : m_edges)
	{
		output << edge.from << "->" << edge.to << " [label=\"" << edge.label << "\"];\n";
	}

	output << "}\n";
}
