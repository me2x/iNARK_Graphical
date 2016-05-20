#include <iostream>
#include "common.h"
#include <memory>
#include <vector>
#include <map>

#ifndef LOGICAL_VERTEX_H
#define LOGICAL_VERTEX_H

class Vertex_Virtual
{
public:
    virtual void print (std::ostream& out_stream) = 0;
    virtual ~Vertex_Virtual() {};
};

struct Port
{
    bool isMaster;
    int associate_id;
    int priority;    
};

class Logical_Vertex
{
    std::vector<std::shared_ptr<Vertex_Virtual> > components_opt;
    std::string name;
    Layer layer;
    void print (std::ostream& comp_stream, std::ostream& opt_stream);
    void create_L1_component(std::string name);
    void create_L2_component(std::string name);
    void create_L3_component(std::string name, int scheduler_type, std::map<int, int> slots);
    void create_L4_component(std::string name, int scheduler_type, int component_type, std::map<int, Port> ports);
    void create_L5_component(std::string name);
    void add_L3_opt(int scheduler_type, std::map<int, int> slots);
    void add_L4_opt(int scheduler_type, int component_type, std::map<int, Port> ports);
};


class L1_Vertex : public Vertex_Virtual
{
public:
    void print(std::ostream& out_stream);
};
class L2_Vertex : public Vertex_Virtual
{
public:
    void print(std::ostream& out_stream);
};
class L3_Vertex : public Vertex_Virtual
{
public:
    std::map<int, int> slots; //id and priority
    int scheduler_type;
    void print(std::ostream& out_stream);
};
class L4_Vertex : public Vertex_Virtual
{
public:
    std::map<int, Port>ports; //id is key, other values ar in the struct
    int scheduler_type;
    int component_type;
    void print(std::ostream& out_stream);
};
class L5_Vertex : public Vertex_Virtual
{
public:
    void print(std::ostream& out_stream);
};



#endif // LOGICAL_VERTEX_H
