#ifndef HANDLEITEM_H
#define HANDLEITEM_H
 
#include <QGraphicsItem>
#include "graphic_vertex.h"
class Graphic_Vertex; 
class HandleItem;
 
class HandleItem : public QGraphicsItem
{
    
public:
  enum { Handler_Type = UserType + 2 };
  enum HandleRole
  {
    CenterHandle,
    RightHandle,
    TopHandle,
    LeftHandle,
    BottomHandle
  };
  int type() const
  {// Enable the use of qgraphicsitem_cast with this item.
     return Handler_Type;
  }
  HandleItem( Graphic_Vertex *item, QGraphicsScene *scene, QColor color, HandleRole role = CenterHandle, QList<HandleItem*> handles = QList<HandleItem*>() );
 
  void paint( QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget );
  QRectF boundingRect() const;
 
protected:
  void mousePressEvent( QGraphicsSceneMouseEvent *event );
  void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
  void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
  QVariant itemChange( GraphicsItemChange change, const QVariant &data );
 
private:
  Graphic_Vertex *m_item;
 
  HandleRole m_role;
  QColor m_color;
 
  QList<HandleItem*> m_handles;
 
  bool m_pressed;
};
 
#endif // HANDLEITEM_H