#pragma once

#include <memory>
#include <vector>

class IGraphVisualizer
{
public:
	using Vertex = std::string;
	using Vertices = std::vector<Vertex>;

	struct Edge
	{
		int from;
		int to;
		std::string label;
	};
	using Edges = std::vector<Edge>;

	virtual void SetVertices(IGraphVisualizer::Vertices&& vertices) = 0;
	virtual void SetEdges(IGraphVisualizer::Edges&& edges) = 0;
	virtual void DrawGraph(std::string const& filename) = 0;

	virtual ~IGraphVisualizer() = default;
};

using IGraphVisualizerPtr = std::unique_ptr<IGraphVisualizer>;
