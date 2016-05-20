#ifndef LOGICAL_EDGE_H
#define LOGICAL_EDGE_H
#include <string>
#include <memory>
#include <vector>

//to use the same mechanic of the vertexes, the Logical class will contain the vector of pointer and will handle the logic of the print (if size == 1 then print on normal output else create the options)
class Logical_Edge
{
public:
    std::vector<std::shared_ptr<Actual_Edge> > edge_opts;
    void print(std::ostream& comp_stream, std::ostream& opt_stream);
    void create_edge(std::string from, std::string to, int from_port, int to_port);
    void add_edge_opt(std::string from, std::string to, int from_port, int to_port);
};


class Actual_Edge{
private:
    std::string from;
    std::string to;
    int from_port;
    int to_port;
public:
    set_start_point(std::string name, int port);
    set_arrival_point(std::string name, int port);
    void print(std::ostream& out_stream);    
};

#endif // LOGICAL_EDGE_H
