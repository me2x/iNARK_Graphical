#include "graphic_vertex.h"

void Graphic_Vertex::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
}
Graphic_Vertex::~Graphic_Vertex()
{
    delete text;
}

QRectF Graphic_Vertex::boundingRect() const
{
}

void Graphic_Vertex::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
QGraphicsItem::mouseDoubleClickEvent(event);
}
void Graphic_Vertex::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
QGraphicsItem::mouseMoveEvent(event);
}
void Graphic_Vertex::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
QGraphicsItem::mousePressEvent(event);
}
void Graphic_Vertex::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
QGraphicsItem::mouseReleaseEvent(event);
}
void Graphic_Vertex::setRect(QRectF rect)
{

}
void Graphic_Vertex::redraw_lines()
{

}
QRectF Graphic_Vertex::rect()
{

}
void Graphic_Vertex::setArrowTarget()
{

}
void Graphic_Vertex::setLayer(int i)
{

}




#include "graphic_vertex.moc"
