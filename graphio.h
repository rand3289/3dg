#ifndef INCLUDED_GRAPHIO_H
#define INCLUDED_GRAPHIO_H

#include <fstream>
#include <vector>
#include <string>

struct GraphNode {
   unsigned int id;
   int label; 
   GraphNode(unsigned int idNode, int labelNode): id(idNode), label(labelNode) {}
};

struct GraphEdge{
   unsigned int from;
   unsigned int to; 
   int label;
   GraphEdge(unsigned int fromNode, unsigned int toNode, int labelNode): from(fromNode), to(toNode), label(labelNode) {}
};


// this class reads and writes digraph dot files
// https://en.wikipedia.org/wiki/DOT_(graph_description_language)
class GraphIO {
	std::fstream file;
	int parseLabel(std::vector<std::string>& tokens, unsigned int start);
	void parseLine(std::string& line, std::vector<GraphNode>& nodesOut, std::vector<GraphEdge>& edgesOut);
public:
	GraphIO();
	~GraphIO();
	std::string initGraph(const std::string& fileName = ""); // returns graph name
	void writeNode(unsigned int id, const std::string& label);
	void writeEdge(unsigned int from, unsigned int to, const std::string& label);
	void closeGraph();
	bool parse(const std::string& fileName, std::vector<GraphNode>& nodesOut, std::vector<GraphEdge>& edgesOut);
};


#endif // INCLUDED_GRAPHIO_H
