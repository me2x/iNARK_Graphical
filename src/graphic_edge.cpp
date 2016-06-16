#include "graphic_edge.h"
#include <boost/math/constants/constants.hpp>
#include <QPen>
#include "graphic_vertex.h"
#include <QGraphicsScene>

void Graphic_Edge::set_data(Graphic_Vertex* from, Graphic_Vertex* to, int fromport, int toport,std::shared_ptr<std::map<QGraphicsLineItem*,std::shared_ptr<Graphic_Edge>> >arrows, std::shared_ptr<Graphic_Edge> ptr)
{
    
    this->arrows=arrows;
    arrival_point = to;
    start_point = from;
    start_port = fromport;
    stop_port = toport;
    QPen blackpen = QPen(Qt::black);
    QPointF* starting_qpoint = new QPointF(start_point->x()+start_point->boundingRect().center().x(),start_point->y()+start_point->boundingRect().bottom());
    QPointF* arrival_qpoint = new QPointF (arrival_point->x()+arrival_point->boundingRect().center().x(),arrival_point->y()+arrival_point->boundingRect().top());
    QLineF newline(*arrival_qpoint,*starting_qpoint);
    line = (arrival_point->scene())->addLine(newline,blackpen);
    std::cout <<"scene is catched and its items are" <<arrival_point->scene()->items().size()<<std::endl;
    double angle = ::acos(newline.dx() / newline.length());
    if (newline.dy() >= 0)
        angle = (boost::math::constants::pi<double>() * 2) - angle;
    QPointF arrowP1 = newline.p1() + QPointF(sin(angle + boost::math::constants::pi<double>() / 3) * 10, cos(angle + boost::math::constants::pi<double>() / 3) * 10);
    QPointF arrowP2 = newline.p1() + QPointF(sin(angle + boost::math::constants::pi<double>() - boost::math::constants::pi<double>() / 3) * 10,cos(angle + boost::math::constants::pi<double>() - boost::math::constants::pi<double>() / 3) * 10);
    QLineF tmp(*arrival_qpoint,arrowP1);
    
    dx_line = (start_point->scene()->addLine(tmp,blackpen));
    QLineF tmp2(*arrival_qpoint,arrowP2);
    std::cout<<"crash test: draw line"<<std::endl;
    sx_line = (start_point->scene()->addLine(tmp2,blackpen));
    
    line->setFlag(QGraphicsItem::ItemIsSelectable);
    label_start = new QGraphicsTextItem();
    label_start->setPos(starting_qpoint->x()-(newline.dx()/2), starting_qpoint->y()-newline.dy()+(2*newline.dy()/3)-5); //stessa y ottenuta cosi. 
    label_start->setPlainText(fromport!=NO_PORT? QString::number(fromport): "");
    arrival_point->scene()->addItem(label_start);
    label_stop = new QGraphicsTextItem();
    label_stop->setPos(arrival_qpoint->x()+(newline.dx()/2), arrival_qpoint->y()+(newline.dy()/3)-5);
    label_stop->setPlainText(toport!=NO_PORT? QString::number(toport): "");
    arrival_point->scene()->addItem(label_stop);
    std::cout<< "arrow to be inserted"<<std::endl;
    std::cout << "sizw is: "<<arrows->size()<<std::endl;
    arrows->insert(std::make_pair(line,ptr));
    std::weak_ptr<Graphic_Edge> weak = ptr;
    start_point->related_edges.push_back(weak);
    arrival_point->related_edges.push_back(weak);
    std::cout<< "arrow created"<<std::endl;
    arrival_point->scene()->update();
    std::cout <<"scene is catched and its items are" <<arrival_point->scene()->items().size()<<std::endl;
    color = BLACK;
}

void Graphic_Edge::update()
{
    std::cout<<"arrow update start_point"<<std::endl;
    std::cout<<"arrow update arrival point is: "<<arrival_point<<std::endl;
    QGraphicsScene* scene = arrival_point->scene();
    std::cout<<"arrow update scenee assigned"<<std::endl;
    scene->removeItem(line); 
    scene->removeItem(sx_line);
    scene->removeItem(dx_line);
    std::shared_ptr<Graphic_Edge> ptr = arrows->at(line);
    arrows->erase(arrows->find(line));
    delete line;
    delete sx_line;
    delete dx_line;
    std::cout<<"arrow update item removed"<<std::endl;
    QPen blackpen = QPen(Qt::black);
    QPointF* starting_qpoint = new QPointF(start_point->x()+start_point->boundingRect().center().x(),start_point->y()+start_point->boundingRect().bottom());
    QPointF* arrival_qpoint = new QPointF (arrival_point->x()+arrival_point->boundingRect().center().x(),arrival_point->y()+arrival_point->boundingRect().top());
    QLineF newline(*arrival_qpoint,*starting_qpoint);
    line= scene->addLine(newline,blackpen);
    double angle = ::acos(newline.dx() / newline.length());
    if (newline.dy() >= 0)
        angle = (boost::math::constants::pi<double>() * 2) - angle;
    QPointF arrowP1 = newline.p1() + QPointF(sin(angle + boost::math::constants::pi<double>() / 3) * 10, cos(angle + boost::math::constants::pi<double>() / 3) * 10);
    QPointF arrowP2 = newline.p1() + QPointF(sin(angle + boost::math::constants::pi<double>() - boost::math::constants::pi<double>() / 3) * 10,cos(angle + boost::math::constants::pi<double>() - boost::math::constants::pi<double>() / 3) * 10);
    QLineF tmp(*arrival_qpoint,arrowP1);
    std::cout<<"crash test: draw line"<<std::endl;
    dx_line = scene->addLine(tmp,blackpen);
    QLineF tmp2(*arrival_qpoint,arrowP2);
    std::cout<<"crash test: draw line"<<std::endl;
    sx_line = scene->addLine(tmp2,blackpen);
    arrows->insert(std::make_pair(line, ptr));
    label_start->setPos(starting_qpoint->x()-newline.dx()+(2*newline.dx()/3), starting_qpoint->y()-newline.dy()+(2*newline.dy()/3)-5); //stessa y ottenuta cosi. 
    label_stop->setPos(arrival_qpoint->x()+(newline.dx()/3), arrival_qpoint->y()+(newline.dy()/3)-5);
    reset_color();
}
Graphic_Edge::~Graphic_Edge()
{
    arrows.reset();
    //do nothing with the Graphic_Vertex *, they are handled by the main interface
    //pointers of no more useful objetcs, have to be deallocated.
    delete dx_line;
    delete sx_line;
    delete label_start;
    delete label_stop;
    delete line;
}
Graphic_Edge::Graphic_Edge()
{

}
void Graphic_Edge::reset_color()
{
    switch (color)
    {
        case BLACK:
            line->setPen(QPen(Qt::black));
            break;
        case BLUE:
            line->setPen(QPen(Qt::blue));
            break;
        default:
            throw std::runtime_error("unsupported color");
    }
}
void Graphic_Edge::set_color(Graphic_Edge::iNARK_Colors c)
{
    color = c;
    reset_color();
}
