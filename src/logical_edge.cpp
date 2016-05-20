#include "logical_edge.h"


void Logical_Edge::add_edge_opt(std::string from, std::string to, int from_port, int to_port)
{
    std::shared_ptr<Actual_Edge> ptr;
    ptr.reset(new Actual_Edge());
    ptr->set_start_point(from,from_port);
    ptr->set_arrival_point(to,to_port);
    edge_opts.push_back(ptr);
}
void Logical_Edge::create_edge(std::string from, std::string to, int from_port, int to_port)
{
    std::shared_ptr<Actual_Edge> ptr;
    ptr.reset(new Actual_Edge());
    ptr->set_start_point(from,from_port);
    ptr->set_arrival_point(to,to_port);
    edge_opts.push_back(ptr);
}
void Logical_Edge::print(std::ostream& comp_stream, std::ostream& opt_stream)
{

}

void Actual_Edge::print(std::ostream& out_stream)
{

}