#include "graphic_vertex.h"
#include "handleitem.h"
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <qitemselectionmodel.h>
#include "graphic_edge.h"
Graphic_Vertex::Graphic_Vertex()
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    text = new QGraphicsTextItem(this);
    //->text->setTextInteractionFlags(Qt::TextEditorInteraction);
    text->setPos(5,5);
    text->setPlainText("Test");
    rekt = QRect (0,0,60,30);
    brush = QBrush(Qt::gray);
    arrow_target = false;
    HandleItem *topHandle = new HandleItem( this, (this->scene()), Qt::red, HandleItem::TopHandle );
    HandleItem *rightHandle = new HandleItem( this, this->scene(), Qt::red, HandleItem::RightHandle );
    HandleItem *leftHandle = new HandleItem( this, this->scene(), Qt::red, HandleItem::LeftHandle );
    HandleItem *bottomHandle = new HandleItem( this, this->scene(), Qt::red, HandleItem::BottomHandle );
    HandleItem *centerHandle = new HandleItem( this, this->scene(), Qt::red, HandleItem::CenterHandle, QList<HandleItem*>() << topHandle << rightHandle << leftHandle << bottomHandle );
    topHandle->setParentItem(this);
    rightHandle->setParentItem(this);
    leftHandle->setParentItem(this);
    bottomHandle->setParentItem(this);
    centerHandle->setParentItem(this);
}

void Graphic_Vertex::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QRectF rec = boundingRect();
    painter->fillRect(rec,brush);
    painter->drawRect(rec);
}
Graphic_Vertex::~Graphic_Vertex()
{
    delete text;
}

QRectF Graphic_Vertex::boundingRect() const
{
    return rekt;
}

void Graphic_Vertex::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    this->setSelected(false);
    emit riquadroDoubleClick();
}
void Graphic_Vertex::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    being_moved=true;
    event->accept();
    QGraphicsItem::mouseMoveEvent(event);
    redraw_lines();
    emit riquadroMosso();
}
void Graphic_Vertex::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    this->setSelected(true);
    QPointF p = mapToScene(event->pos());
    QPoint p1 = QCursor::pos();
    this->setObjectName("cliked rectangle");
    if(event->button() == Qt::LeftButton)
    {
        emit riquadroCliccatoSx();
    }
    else if (event->button() == Qt::RightButton)
        emit riquadroCliccatoDx();
}
void Graphic_Vertex::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    //event->accept();
    if(being_moved || arrow_target)
    {
    this->setSelected(false);
    being_moved = false;
    arrow_target = false;
    }
}
void Graphic_Vertex::setRect(QRectF rect)
{
    rekt = rect;
}
void Graphic_Vertex::redraw_lines()
{
    for (std::vector<std::weak_ptr<Graphic_Edge> >::iterator it = related_edges.begin(); it!= related_edges.end();)
    {
        std::weak_ptr<Graphic_Edge> tmp = (*it);
        if (std::shared_ptr<Graphic_Edge> ptr = tmp.lock())
        {
            ptr->update();
            ++it;
        }
        else
        {
            it = related_edges.erase(it);
        }
    }
}
QRectF Graphic_Vertex::rect()
{
    return rekt;
}
void Graphic_Vertex::setArrowTarget()
{
    arrow_target=true;
}
void Graphic_Vertex::setLayer(int i)
{
  switch (i)
    {
        case 0:
        {
            brush = QBrush(Qt::yellow);
            break;
        }
        case 1:
        {
            brush = QBrush(Qt::magenta);
            break;
        }
        case 2:
        {
            brush = QBrush(Qt::cyan);
            break;
        }
        case 3:
        {
            brush = QBrush(Qt::blue);
            break;
        }
        case 4:
        {
            brush = QBrush(Qt::green);
            break;
        }
        default:
        {
            brush = QBrush(Qt::gray);
            break;
        }
    }
}




#include "graphic_vertex.moc"
