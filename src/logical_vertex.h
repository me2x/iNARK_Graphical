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
/*
 The logic behind the pointer or not is the following: common fields between opts have to use the pointer in order
 to have the same object referred by all the internal components, if one modifies, all get the modification.
 The fields that change in every option are normal attributes. if the option gets deleted, this info is no more needed.
 */
class Logical_Vertex
{
public:
    std::vector<std::shared_ptr<Vertex_Virtual> > components_opt;
    std::shared_ptr<std::string> name;
    Layer layer;
    float x_pos;
    float y_pos;
    void print (std::ostream& comp_stream, std::ostream& opt_stream);
    void create_L1_component(std::shared_ptr<std::string> name);
    void create_L2_component(std::shared_ptr<std::string> name);
    void create_L3_component(std::shared_ptr< std::string > comp_name, Component_Priority_Category scheduler_type, std::shared_ptr< std::map< int, int > > sched_slot);
    void create_L4_component(std::shared_ptr<std::string> name, Component_Priority_Category scheduler_type, std::shared_ptr<int> component_type, std::shared_ptr<std::map<int, Port>> ports);
    void create_L5_component(std::shared_ptr<std::string> name);
    void add_L3_opt(Component_Priority_Category scheduler_type);
    void add_L4_opt(Component_Priority_Category scheduler_type);
   
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
    std::shared_ptr<std::map < int, int >> OS_slots; //id and priority //the slots name is a QT define so it cant be used.
    Component_Priority_Category scheduler_type;
    void print(std::ostream& out_stream);
};
class L4_Vertex : public Vertex_Virtual
{
public:
    std::shared_ptr<std::map < int, Port >>ports; //id is key, other values ar in the struct
    Component_Priority_Category scheduler_type;
    std::shared_ptr<int> component_type;
    void print(std::ostream& out_stream);
};
class L5_Vertex : public Vertex_Virtual
{
public:
    void print(std::ostream& out_stream);
};



#endif // LOGICAL_VERTEX_H
