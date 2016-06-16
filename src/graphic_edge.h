#ifndef GRAPHIC_EDGE_H
#define GRAPHIC_EDGE_H

#include <memory>
#include <QGraphicsItem>
//#include <QGraphicsTextItem>

#define NO_PORT -1
class Graphic_Vertex;
class Graphic_Edge
{
    public:
        enum iNARK_Colors{
            BLUE,
            BLACK
        };
        Graphic_Edge();
        void set_data(Graphic_Vertex* from, Graphic_Vertex* to, int fromport, int toport,std::shared_ptr<std::map<QGraphicsLineItem*,std::shared_ptr<Graphic_Edge>> >arrows,std::shared_ptr<Graphic_Edge>ptr); //get data. draw first.
        ~Graphic_Edge();
        void update(); //delete old qgraphicsitem, create new ones.
        void set_color(iNARK_Colors c);//set the edge default color. black for normal, blue for double edges that share a port/slot
        void reset_color();//to be called when the edge is deselected, restore the "base" color chosen by set_color
        //scene only uses old styled pointers. 
        //thats why old styled pointers are used and delete functions have to be used.
        //shared pointers would only introduce overhead
        QGraphicsLineItem *line,*sx_line,*dx_line;
        QGraphicsTextItem *label_start,*label_stop;
    private:
        //IMPORTANT: THIS HAVE TO BE MANUALLY HANDLED
        std::shared_ptr<std::map<QGraphicsLineItem*,std::shared_ptr<Graphic_Edge>> >arrows;
        int start_port,stop_port;
        Graphic_Vertex* start_point;
        Graphic_Vertex* arrival_point;
        iNARK_Colors color;
};

#endif // GRAPHIC_EDGE_H
