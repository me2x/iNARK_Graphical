#include "handleitem.h"
#include "graphic_vertex.h"
 
#include <QPainter>
#include <QGraphicsScene>
#include <qgraphicssceneevent.h>
#include <QPointF>
 
#include <cmath>
#include <iostream>
 
HandleItem::HandleItem( Graphic_Vertex *item, QGraphicsScene *scene, QColor color, HandleItem::HandleRole role, QList<HandleItem*> handles ) : QGraphicsItem( 0, scene )
{
  m_role = role;
  m_color = color;
 
  m_item = item;
  m_handles = handles;
 
  m_pressed = false;
  setZValue( 100 );
 
  setFlag( ItemIsMovable );
  setFlag(ItemSendsGeometryChanges);
//  std::cout<<"handler created"<<std::endl;
//  std::cout<<"scene is: "<<this->scene()->objectName().toStdString()<<std::endl;
//  std::cout<<"item is: "<<this->m_item->text->toPlainText().toStdString() <<std::endl;
  
}
 
void HandleItem::paint( QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
  paint->setPen( m_color );
  paint->setBrush( m_color );
 //std::cout<<"handler painted"<<std::endl;
  QRectF rect = boundingRect();
  QVector<QPointF> points;
 
  switch( m_role )
  {
  case RightHandle:
    points << rect.center()+QPointF(3,0) << rect.center()+QPointF(-3,-5) << rect.center()+QPointF(-3,5);
    paint->drawConvexPolygon( QPolygonF(points) );
    break;
  case LeftHandle:
    points << rect.center()+QPointF(-3,0) << rect.center()+QPointF(3,-5) << rect.center()+QPointF(3,5);
    paint->drawConvexPolygon( QPolygonF(points) );
    break;
  case TopHandle:
    points << rect.center()+QPointF(0,-3) << rect.center()+QPointF(-5,3) << rect.center()+QPointF(5,3);
    paint->drawConvexPolygon( QPolygonF(points) );
    break;
  case BottomHandle:
    points << rect.center()+QPointF(0,3) << rect.center()+QPointF(-5,-3) << rect.center()+QPointF(5,-3);
    paint->drawConvexPolygon( QPolygonF(points) );
    break;
  }
}
 
 
QRectF HandleItem::boundingRect() const
{
  switch( m_role )
  {
  case LeftHandle:
    {
    QPointF point(m_item->boundingRect().left() - pos().x(), m_item->boundingRect().top() + m_item->boundingRect().height()/2);
    return QRectF( point-QPointF(3, 5), QSize( 6, 10 ) );
    }
  case RightHandle:
    {
    QPointF point(m_item->boundingRect().right() - pos().x(), m_item->boundingRect().top() + m_item->boundingRect().height()/2);
    return QRectF( point-QPointF(3, 5), QSize( 6, 10 ) );
    }
  case TopHandle:
    {
    QPointF point(m_item->boundingRect().left() + m_item->boundingRect().width()/2, m_item->boundingRect().top() - pos().y());
    return QRectF( point-QPointF(5, 3), QSize( 10, 6 ) );
    }
  case BottomHandle:
    {
    QPointF point(m_item->boundingRect().left() + m_item->boundingRect().width()/2, m_item->boundingRect().bottom() - pos().y());
    return QRectF( point-QPointF(5, 3), QSize( 10, 6 ) );
    }
  }
 
  return QRectF();
}
 
QVariant HandleItem::itemChange( GraphicsItemChange change, const QVariant &data )
{
  if( change == ItemPositionChange && m_pressed )
  {
 
    QPointF movement;
    QPointF newData = data.toPointF();
    QRectF newRect = m_item->rect();
 
    switch( m_role )
    {
    case LeftHandle:
      {
      // Snap the movement to the X direction
      newData.setY(0);
      movement = newData - pos();
      // Resize the rectangle
//      std::cout<< "rect width is: "<<m_item->rect().width() <<" and text bounding rect width is: "<<m_item->text->boundingRect().width()<<std::endl;
      m_item->rect().width() < m_item->text->boundingRect().width()+20?newRect.setLeft(m_item->rect().left()-1) :newRect.setLeft(m_item->rect().left() + movement.x());
      m_item->setRect(newRect);
 
      break;
      }
    case RightHandle:
      {
       // Snap the movement to the X direction
      newData.setY(0);
      movement = newData - pos();
      // Resize the rectangle
      m_item->rect().width() + movement.x() < m_item->text->boundingRect().width()+20?newRect.setRight(m_item->rect().right()+1) :newRect.setRight(m_item->rect().right() + movement.x());
      m_item->setRect(newRect);
      break;
      }
    case TopHandle:
      {
      // Snap the movement to the Y direction
      newData.setX(0);
      movement = newData - pos();
      // Resize the rectangle
      m_item->rect().height() < 25?newRect.setTop(m_item->rect().top() - 1):newRect.setTop(m_item->rect().top() + movement.y());
      m_item->setRect(newRect);
      break;
      }
    case BottomHandle:
      {
       // Snap the movement to the Y direction
      newData.setX(0);
      movement = newData - pos();
      // Resize the rectangle, preventing the collapse and the lock
      m_item->rect().height() < 25?newRect.setBottom(m_item->rect().bottom() + 1):newRect.setBottom(m_item->rect().bottom() + movement.y());
      m_item->setRect(newRect);
 
      break;
      }
    } // end switch
    m_item->text->setPos(m_item->boundingRect().topLeft().x()+2,m_item->boundingRect().topLeft().y()+2);
   // std::cout<< "number of selected items in the end of item change: "<<this->scene()->selectedItems().size()<<std::endl;
    m_item->redraw_lines();
    m_item->setSelected(false);
    this->scene()->update(this->scene()->sceneRect());
    return QGraphicsItem::itemChange( change, newData);
  } // end if pressed
 
  return QGraphicsItem::itemChange( change, data );
}
 
void HandleItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  m_pressed = true;  
  event->accept();
  //QGraphicsItem::mousePressEvent( event );
}
 
void HandleItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  m_pressed = false;
  event->accept();
  QGraphicsItem::mouseReleaseEvent( event );
}

void HandleItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    event->accept();
    QGraphicsItem::mouseMoveEvent(event);
}
