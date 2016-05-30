#include "logical_edge.h"
#include <iostream>
#include <boost/lexical_cast.hpp>

void Logical_Edge::print(std::ostream& comp_stream)
{
    comp_stream << "print edge" <<std::endl;
}

std::pair< std::string, int > Logical_Edge::get_from()
{
    return std::make_pair(from,from_port);
}
std::pair< std::string, int > Logical_Edge::get_to()
{
    return std::make_pair(to,to_port);
}
Logical_Edge::Logical_Edge(std::string& from, std::string& to, int& from_port, int& to_port)
{
    this->from = from;
    this->to = to;
    this->from_port = from_port;
    this->to_port = to_port;
}
std::string Logical_Edge::get_string()
{
    std::string st;
    st=from+to+boost::lexical_cast<std::string>(from_port)+boost::lexical_cast<std::string>(to_port);
    return st;
}
