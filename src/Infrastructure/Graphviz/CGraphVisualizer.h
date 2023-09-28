#pragma once

#include "../../App/IGraphVisualizer.h"

class CGraphVisualizer : public IGraphVisualizer
{
public:
	void SetVertices(IGraphVisualizer::Vertices&& vertices) override;
	void SetEdges(IGraphVisualizer::Edges&& edges) override;
	void DrawGraph(std::string const& filename) override;

private:
	IGraphVisualizer::Vertices m_vertices;
	IGraphVisualizer::Edges m_edges;
};
