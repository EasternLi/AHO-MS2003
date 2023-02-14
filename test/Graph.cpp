#include "Graph.hpp"

#include <sstream>

Graph::Graph(int n, int m, std::vector<Node>node, std::vector<Edge> edge)
: n_(n), m_(m), node_(node), edge_(edge) {}

void Graph::prettyPrint(std::string &out) const {
	if (!info.empty()) {
		out += info;
		return;
	}
	std::stringstream ss;
	ss << "{"; {
		ss << "n: " << n_ <<"; ";
		ss << "{"; for (auto& limit : node_) {
			ss << "{";
			ss << "l:" << limit.l << ",";
			ss << "u:" << limit.u << ",";
			ss << "F:#";
			ss << "},";
		} ss << "}; ";
		ss << "m: " << m_ <<"; ";
		ss << "{"; for (auto& limit : edge_) {
			ss << "{";
			ss << "i:" << limit.i << ",";
			ss << "j:" << limit.j << ",";
			ss << "l:" << limit.l << ",";
			ss << "u:" << limit.u << ",";
			ss << "B:#";
			ss << "},";
		} ss << "}; ";
	} ss << "}";
	out += ss.str();
}
