#ifndef LOGICAL_EDGE_H
#define LOGICAL_EDGE_H
#include <string>
#include <memory>
#include <vector>


class Logical_Edge
{
private:
    std::string from;
    std::string to;
    int from_port;
    int to_port;
public:
    void print(std::ostream& comp_stream);
    Logical_Edge (std::string& from, std::string& to, int& from_port, int& to_port);
    std::pair< std::string, int> get_from();
    std::pair< std::string, int> get_to();
    std::string get_string();
    
};


#endif // LOGICAL_EDGE_H
