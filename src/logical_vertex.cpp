#include "logical_vertex.h"

void Logical_Vertex::create_L1_component(std::string comp_name)
{
    
    std::shared_ptr<L1_Vertex> ptr;
    ptr.reset(new L1_Vertex());
    components_opt.push_back(ptr);
    name = comp_name;
    layer = Layer::FUNCTION;
}
void Logical_Vertex::create_L2_component(std::string comp_name)
{
    
    std::shared_ptr<L2_Vertex> ptr;
    ptr.reset(new L2_Vertex());
    components_opt.push_back(ptr);
    name = comp_name;
    layer = Layer::FUNCTION;
}
void Logical_Vertex::create_L3_component(std::string comp_name, int scheduler_type, std::map< int, int > slots)
{
    std::shared_ptr<L3_Vertex> ptr;
    ptr.reset(new L3_Vertex());
    ptr->scheduler_type = scheduler_type;
    ptr->slots = slots;
    components_opt.push_back(ptr);
    name = comp_name;
    layer = Layer::FUNCTION;
}
void Logical_Vertex::create_L4_component(std::string comp_name, int scheduler_type, int component_type, std::map< int, Port > ports)
{
    std::shared_ptr<L4_Vertex> ptr;
    ptr.reset(new L4_Vertex());
    ptr->ports = ports;
    ptr->component_type = component_type;
    ptr->scheduler_type = scheduler_type;
    components_opt.push_back(ptr);
    name = comp_name;
    layer = Layer::FUNCTION;
}
void Logical_Vertex::create_L5_component(std::string comp_name)
{
    
    std::shared_ptr<L5_Vertex> ptr;
    ptr.reset(new L5_Vertex());
    components_opt.push_back(ptr);
    name = comp_name;
    layer = Layer::FUNCTION;
}
void Logical_Vertex::add_L3_opt(int scheduler_type, std::map< int, int > slots)
{
    std::shared_ptr<L3_Vertex> ptr;
    ptr.reset(new L3_Vertex());
    ptr->scheduler_type = scheduler_type;
    ptr->slots = slots;
    components_opt.push_back(ptr);
}
void Logical_Vertex::add_L4_opt(int scheduler_type, int component_type, std::map< int, Port > ports)
{
    std::shared_ptr<L4_Vertex> ptr;
    ptr.reset(new L4_Vertex());
    ptr->ports = ports;
    ptr->component_type = component_type;
    ptr->scheduler_type = scheduler_type;
    components_opt.push_back(ptr);
}
void Logical_Vertex::print(std::ostream& comp_stream, std::ostream& opt_stream)
{
    if (components_opt.size()==1)
    {
        comp_stream << "<component>"<<std::endl<<"<name>"<<name<<"</name>"<<std::endl<<"</component>"<<std::endl; //has to be finished
        (components_opt.at(0))->print(comp_stream);
    }
    else
    {
        opt_stream << "<component>"<<std::endl<<"<name>"<<name<<"</name>"<<std::endl<<"</component>"<<std::endl; //has to be finished
        components_opt.at(0)->print(opt_stream);
    }
}



void L1_Vertex::print(std::ostream& out_stream)
{
    out_stream<<"L1 print"<<std::endl;
}
void L2_Vertex::print(std::ostream& out_stream)
{
    out_stream<<"L2 print"<<std::endl;
}
void L3_Vertex::print(std::ostream& out_stream)
{
    out_stream<<"L3 print"<<std::endl;
}
void L4_Vertex::print(std::ostream& out_stream)
{
    out_stream<<"L4 print"<<std::endl;
}
void L5_Vertex::print(std::ostream& out_stream)
{
    out_stream<<"L5 print"<<std::endl;
}

























