#include "logical_vertex.h"

void Logical_Vertex::create_L1_component(std::shared_ptr< std::string > comp_name)
{
    
    std::shared_ptr<L1_Vertex> ptr;
    ptr.reset(new L1_Vertex());
    components_opt.push_back(ptr);
    name = comp_name;
    layer = Layer::FUNCTION;
}
void Logical_Vertex::create_L2_component(std::shared_ptr<std::string> comp_name)
{
    
    std::shared_ptr<L2_Vertex> ptr;
    ptr.reset(new L2_Vertex());
    components_opt.push_back(ptr);
    name = comp_name;
    layer = Layer::TASK;
}
void Logical_Vertex::create_L3_component(std::shared_ptr< std::string > comp_name, Component_Priority_Category scheduler_type, std::shared_ptr< std::map< int, int > > slots)
{
    std::shared_ptr<L3_Vertex> ptr;
    ptr.reset(new L3_Vertex());
    ptr->scheduler_type = scheduler_type;
    ptr->OS_slots = slots;
    components_opt.push_back(ptr);
    name = comp_name;
    layer = Layer::CONTROLLER;
}
void Logical_Vertex::create_L4_component(std::shared_ptr< std::string > comp_name, Component_Priority_Category scheduler_type, int component_type, std::shared_ptr< std::map< int, Port > > ports)
{
    std::shared_ptr<L4_Vertex> ptr;
    ptr.reset(new L4_Vertex());
    ptr->ports = ports;
    ptr->component_type = component_type;
    ptr->scheduler_type = scheduler_type;
    components_opt.push_back(ptr);
    name = comp_name;
    layer = Layer::RESOURCE;
}
void Logical_Vertex::create_L5_component(std::shared_ptr<std::string> comp_name)
{
    
    std::shared_ptr<L5_Vertex> ptr;
    ptr.reset(new L5_Vertex());
    components_opt.push_back(ptr);
    name = comp_name;
    layer = Layer::PHYSICAL;
}
void Logical_Vertex::add_L3_opt(Component_Priority_Category scheduler_type)
{
    std::shared_ptr<L3_Vertex> ptr;
    ptr.reset(new L3_Vertex());
    ptr->scheduler_type = scheduler_type;
    std::shared_ptr<L3_Vertex> tmp = std::static_pointer_cast<L3_Vertex>(components_opt.at(0));
    ptr->OS_slots = tmp->OS_slots;
    components_opt.push_back(ptr);
}
void Logical_Vertex::add_L4_opt(Component_Priority_Category scheduler_type, int component_type)
{
    std::shared_ptr<L4_Vertex> ptr;
    ptr.reset(new L4_Vertex());
    std::shared_ptr<L4_Vertex> tmp = std::static_pointer_cast<L4_Vertex>(components_opt.at(0));
    ptr->ports = tmp->ports;
    ptr->component_type = component_type;
    ptr->scheduler_type = scheduler_type;
    components_opt.push_back(ptr);
}
void Logical_Vertex::print(std::ostream& comp_stream, std::ostream& opt_stream)
{
    std::cout <<"enter print"<<std::endl;
    std::cout <<"comp opt sizee is: "<<components_opt.size()<<std::endl;
    if (components_opt.size()==1)
    {
        comp_stream << "\t\t<component>"<<std::endl<<"\t\t\t<name>"<<(*name)<<"</name>"<<std::endl;
        comp_stream << "\t\t\t<layer>"<<commons::Layer_to_int(layer)<<"</layer>"<<std::endl;
        comp_stream << "\t\t\t<x_pos>"<<x_pos<<"</x_pos>"<<std::endl;
        comp_stream << "\t\t\t<y_pos>"<<y_pos<<"</y_pos>"<<std::endl;
        (components_opt.at(0))->print(comp_stream);
        comp_stream <<"\t\t</component>"<<std::endl; //has to be finished
        
    }
    else
    {
        comp_stream << "\t\t<component>"<<std::endl<<"\t\t\t<name>"<<(*name)<<"</name>"<<std::endl; //has to be finished
        comp_stream << "\t\t\t<layer>"<<commons::Layer_to_int(layer)<<"</layer>"<<std::endl;
        comp_stream << "\t\t\t<x_pos>"<<x_pos<<"</x_pos>"<<std::endl;
        comp_stream << "\t\t\t<y_pos>"<<y_pos<<"</y_pos>"<<std::endl;
        comp_stream << "\t\t\t<opt>" << (*name)<<"_opt" <<"</opt>"<<std::endl;
        comp_stream <<"\t\t</component>"<<std::endl;
        opt_stream <<  "\t\t<opt>" << std::endl;
        opt_stream << "\t\t\t<name>" << (*name)<<"_opt"<<"</name>" <<std::endl;
        for( std::vector<std::shared_ptr<Vertex_Virtual> >::iterator it = components_opt.begin(); it != components_opt.end();++it)
        {
            opt_stream << "\t\t\t<enum>" <<std::endl;
            (*it)->print(opt_stream);
            opt_stream << "\t\t\t</enum>" <<std::endl;
            
        }
        opt_stream <<"\t\t</opt>"<<std::endl;
    }
}



void L1_Vertex::print(std::ostream& out_stream)
{
    //out_stream<<"L1 print"<<std::endl;
}
void L2_Vertex::print(std::ostream& out_stream)
{
    //out_stream<<"L2 print"<<std::endl;
}
void L3_Vertex::print(std::ostream& out_stream)
{
    out_stream<<"\t\t\t\t<priority_handling>"<<commons::Priority_Handler_To_Int(scheduler_type)<<"</priority_handling>"<<std::endl;
    out_stream<<"\t\t\t\t<slots>"<<std::endl;
    for(std::map<int,int>::iterator it = OS_slots->begin(); it != OS_slots->end(); ++it)
    {
        out_stream<<"\t\t\t\t\t<slot>"<<std::endl;
        if (it->second!= NO_PRIORITY)
            out_stream<<"\t\t\t\t\t\t<priority>"<<it->second<<"</priority>"<<std::endl;
        out_stream<<"\t\t\t\t\t\t<id>"<<it->first<<"</id>"<<std::endl;
        out_stream<<"\t\t\t\t\t</slot>"<<std::endl;
    }
    out_stream<<"\t\t\t\t</slots>"<<std::endl;
}
void L4_Vertex::print(std::ostream& out_stream)
{
    out_stream<<"\t\t\t\t<priority_handling>"<<commons::Priority_Handler_To_Int(scheduler_type)<<"</priority_handling>"<<std::endl;
    out_stream<<"\t\t\t\t<type>"<<component_type<<"</type>"<<std::endl;
    out_stream<<"\t\t\t\t<ports>"<<std::endl;
    for(std::map<int,Port>::iterator it = ports->begin(); it != ports->end(); ++it)
    {
        out_stream<<"\t\t\t\t\t<port>"<<std::endl;
        if (it->second.priority!= NO_PRIORITY)
            out_stream<<"\t\t\t\t\t\t<priority>"<<it->second.priority<<"</priority>"<<std::endl;
        out_stream<<"\t\t\t\t\t\t<isMaster>"<<it->second.isMaster<<"</isMaster>"<<std::endl;
        if (it->second.associate_id!= -1) //min of the spinbox, means no associate id for this port.
            out_stream<<"\t\t\t\t\t\t<associatedPort>"<<it->second.associate_id<<"</associatedPort>"<<std::endl;
        out_stream<<"\t\t\t\t\t\t<id>"<<it->first<<"</id>"<<std::endl;
        out_stream<<"\t\t\t\t\t</port>"<<std::endl;
    }
    out_stream<<"\t\t\t\t</ports>"<<std::endl;
    
}
void L5_Vertex::print(std::ostream& out_stream)
{
    //out_stream<<"L5 print"<<std::endl;
}

























