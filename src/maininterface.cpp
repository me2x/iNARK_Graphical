#include "maininterface.h"
#include "ui_maininterface.h"
//#include <QString>
#include <QFileDialog>
//#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include "common.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <vector>
#include <QMouseEvent>


MainInterface::MainInterface(QWidget* parent): QWidget(parent), ui(new Ui::MainInterface)
{
    ui->setupUi(this);
    names.reset(new std::set<std::string>());
    //Layer and Types scopes are set up from the enum in graph/graph_common
    for (int i = 0; i< SEARCH_LAYER_END;i++)
        ui->chose_layer->insertItem(i, QString::fromStdString(commons::get_search_layer_names(static_cast<Search_Layers>(i))));
    for (int i = 0; i< SEARCH_TYPE_END; i++)
        ui->chose_type->insertItem(i, QString::fromStdString(commons::get_search_type_name(static_cast<Search_Types>(i))));

    connect(ui->Load_Button,SIGNAL(clicked(bool)),this,SLOT(load_file()));
    connect(ui->Save_Button,SIGNAL(clicked(bool)),this,SLOT(save_file()));
    connect(ui->create_lay1,SIGNAL(clicked()),this,SLOT(Layer_1_press_event()));
    connect(ui->create_lay2,SIGNAL(clicked()),this,SLOT(Layer_2_press_event()));
    connect(ui->create_lay3,SIGNAL(clicked()),this,SLOT(Layer_3_press_event()));
    connect(ui->create_lay4,SIGNAL(clicked()),this,SLOT(Layer_4_press_event()));
    connect(ui->create_lay5,SIGNAL(clicked()),this,SLOT(Layer_5_press_event()));
    
    connect(ui->Delete_Button,SIGNAL(clicked()),this,SLOT(delete_items()));
    
    connect(ui->START,SIGNAL(clicked()),this,SLOT(start_search()));
    
    
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(redraw_line()));
    timer->start(100);
    
    
    is_drawing = false;
    starting_object = nullptr;
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->setParent(ui->graphicsView);
    std::cout << "setup finished" <<std::endl;
}

MainInterface::~MainInterface()
{
delete ui;
}
void MainInterface::load_file()
{
    QString filename = QFileDialog::getOpenFileName(
      this,
      tr("Open source graph file"),
      QDir::homePath(),
      "Graph source file (*.xml);;All Files (*.*)"
    );
    if (!filename.isNull())
    {
        std::cout<<"build graph"<<std::endl;
       
    }
}

void MainInterface::save_file()
{
    QString filename = QFileDialog::getSaveFileName(
      this,
      tr("Save current source graph file"),
      QDir::homePath(),
      "Graph source file (*.xml);;All Files (*.*)"
    );
    if (!filename.isNull())
    {
        std::cout <<"save the file somehow" <<std::endl;
    }
}
//TODO logical deletion has to be added
void MainInterface::delete_edge(std::shared_ptr< Graphic_Edge > to_be_deleted)
{
    //remove from edge_map
    std::string edge_string = (edges.at(to_be_deleted)).get()->get_string();
    edges_set.erase(edge_string);
    // remove from the arrow map. graphical utility
    arrows->erase(to_be_deleted->line);
    //remove from the edges map. logical value
    edges.erase(to_be_deleted);
    //remove from the scene.
    std::cout<<"tobedeeleted counter" << to_be_deleted.use_count()<<std::endl;
    scene->removeItem(to_be_deleted->dx_line);
    scene->removeItem(to_be_deleted->sx_line);
    scene->removeItem(to_be_deleted->line);
    scene->removeItem(to_be_deleted->label_start);
    scene->removeItem(to_be_deleted->label_stop);

    to_be_deleted.reset();
}

//TODO logical deletion has to be added
void MainInterface::delete_vertex(Graphic_Vertex* to_be_deleted)
{
    scene->removeItem(to_be_deleted);
    names->erase(vertices.at(to_be_deleted)->name);
    vertices.erase(to_be_deleted);
    
    for (std::vector<std::weak_ptr<Graphic_Edge>>::iterator it = to_be_deleted->related_edges.begin(); it != to_be_deleted->related_edges.end(); ++it)
    {
        delete_edge((*it).lock());
    }
    delete to_be_deleted;
    starting_object = nullptr;
}

void MainInterface::break_line_drawing()
{
    is_drawing = false;
}

void MainInterface::component_clicked()
{
 std::cout<<"component_clicked"<<std::endl;
        QPoint p1 = QCursor::pos();
        QPoint p2 = ui->graphicsView->mapFromGlobal(p1);
        QPointF p = ui->graphicsView->mapToScene(p2);
        QGraphicsItem* item_tmp =  scene->itemAt(p.x()-3, p.y()-3); //NON HO ANCORA CAPITO XK SI PERDA 3px nella conversione. 
        //che poi la domanda fondamentale è sono 3 qua e x altrove? xk in tal caso il fix non  funziona...
        // QPoint p = ui->frame_6->mapFromGlobal(QCursor::pos());
        std::cout<<"item catched"<<std::endl;
        std::cout<<"item catched"<<item_tmp->type() <<std::endl;
        std::cout<<"item catched"<<std::endl;
        QPen blackpen = QPen(Qt::black);
        
        if (item_tmp->type() != Graphic_Vertex::Graphic_Vertex_type) //qtextgraphicitem
        {
            std::cout<<"item is not riquadro"<<std::endl;
            QList<QGraphicsItem*> items;
            QPointF point = QPointF(p.x()-3, p.y()-3);
            items = scene->items(point, Qt::IntersectsItemShape,Qt::AscendingOrder,QTransform());
            while (item_tmp->type() != Graphic_Vertex::Graphic_Vertex_type|| items.size()!= 0)
            {
            std::cout<<"item is not riquadro size:"<<items.size()<<std::endl;
            item_tmp = items.at(items.size()-1);
            items.removeLast();
            }
            if (item_tmp->type() != Graphic_Vertex::Graphic_Vertex_type)
                throw std::runtime_error("click su riquadro non trova oggetto");
        }
        Graphic_Vertex* item = qgraphicsitem_cast<Graphic_Vertex*>(item_tmp);
        std::cout<<"item is: "<<item<<std::endl;
        std::cout<<"item is: "<<item->text->toPlainText().toStdString()<<std::endl;
        if (item != nullptr && item != 0)
        {
            std::cout<<"LINE: enter item found"<<std::endl;

            if(is_drawing) //show the line popup, selection has to be done in the finalize phase, because the ports are still to be decided.
            {
                std::cout<<"LINE: enter is drawing"<<std::endl;
                is_drawing = false;
                scene->removeItem(current_line_item);
                arrival_object = item;
                std::cout<<"test: precall di portselect"<<std::endl;
                if (((vertices.at(starting_object))->layer == Layer::RESOURCE && (vertices.at(arrival_object))->layer == Layer::RESOURCE ) ||
                    ((vertices.at(starting_object))->layer == Layer::TASK && (vertices.at(starting_object))->layer == Layer::CONTROLLER))  
                {
                    //make popup and show
                    ports.reset(new Ports_Popup());
                    ports->set_data(vertices.at(starting_object),vertices.at(arrival_object));
                    std::cout<<"data set set"<<std::endl;
                    connect(ports.get(),SIGNAL(accepted()),this,SLOT(get_line_data_from_popup()));
                    connect(ports.get(),SIGNAL(rejected()),this,SLOT(break_line_drawing()));
                    std::cout<<"connection set"<<std::endl;
                    ports->exec();
                    ports->activateWindow();
                }
                else
                {
                    from_port = 0;
                    to_port = 0;
                    finalize_line();
                }
            }
            else //start the line drawing
            {
                std::cout<<"LINE: enter is not drawing"<<std::endl;
                is_drawing = true;
                starting_object = item;
                QPoint p1 = QCursor::pos();
                QPoint p2 = ui->graphicsView->mapFromGlobal(p1);
                QPointF p = ui->graphicsView->mapToScene(p2);
                QLineF newline(item->x()+item->boundingRect().center().x(),item->y()+item->boundingRect().bottom(),p.x()-3, p.y()-3);
                current_line_item = scene->addLine(newline,blackpen);
              }

            starting_object = item;
        }
        else 
            std::cout << " other type, value is: " << item_tmp->type()<<std::endl;
}

void MainInterface::create_L1_obj()
{
     std::shared_ptr<Logical_Vertex> data = l1->get_data();
     Graphic_Vertex* temp= new Graphic_Vertex(); //IMPORTANT: when removed from scene has to be freed
     temp->setLayer(0);
     scene->addItem(temp);
     connect(temp,SIGNAL(riquadroCliccatoSx()),this,SLOT(component_clicked()));
     connect(temp,SIGNAL(riquadroMosso()),this,SLOT(break_line_drawing()));
     connect(temp,SIGNAL(riquadroCliccatoDx()),this,SLOT(break_line_drawing()));
     connect(temp,SIGNAL(riquadroDoubleClick()),this,SLOT(start_update_L1_object()));
     temp->text->setPlainText(QString::fromStdString(data->name));
     vertices.insert(std::make_pair(temp,data));
}

void MainInterface::create_L2_obj()
{

}

void MainInterface::create_L3_obj()
{

}

void MainInterface::create_L4_obj()
{

}

void MainInterface::create_L5_obj()
{

}

void MainInterface::delete_items()
{
if (selected_edge != nullptr)
    {
        delete_edge(selected_edge);
    }
    else if (starting_object != 0 && starting_object != nullptr)
    {
        delete_vertex(starting_object);
    }
}
void MainInterface::get_line_data_from_popup()
{
    std::pair<int, int> portspair =  ports->get_ports();
    from_port = portspair.first;
    to_port = portspair.second;
    finalize_line();
}
void MainInterface::finalize_line()
{
    std::cout <<"drawing line"<<std::endl;
    std::weak_ptr<Graphic_Vertex> start, arrive;
    start.reset(starting_object);
    arrive.reset(arrival_object);
    Graphic_Edge edge;
    //insert in arrows done inside the function
    std::shared_ptr<Graphic_Edge> edge_ptr = edge.set_data(start,arrive,from_port,to_port,arrows);
    Logical_Edge l_edge((vertices.at(starting_object))->name,(vertices.at(arrival_object))->name,from_port,to_port);
    edges_set.insert(l_edge.get_string());
    arrival_object = nullptr;
    starting_object = nullptr;
}

void MainInterface::finalize_update_L1_object()
{

}

void MainInterface::finalize_update_L2_object()
{

}

void MainInterface::finalize_update_L3_object()
{

}

void MainInterface::finalize_update_L4_object()
{

}

void MainInterface::finalize_update_L5_object()
{

}

void MainInterface::Layer_1_press_event()
{
    std::cout<<"layer 1 press event triggered"<<std::endl;
    l1.reset(new L1_popup());
    l1->set_names(names);
       std::cout<<"layer 1 press event popup creation"<<std::endl;  
    connect(l1.get(),SIGNAL(accepted()),this,SLOT(create_L1_obj()));
    connect(l1.get(),SIGNAL(rejected()),this,SLOT(no_data()));
       std::cout<<"layer 1 press event popup connection"<<std::endl;
    l1->exec();
    l1->activateWindow();
  
    std::cout<<"layer 1 press event popup execution"<<std::endl;
}

void MainInterface::Layer_2_press_event()
{

}

void MainInterface::Layer_3_press_event()
{

}

void MainInterface::Layer_4_press_event()
{

}

void MainInterface::Layer_5_press_event()
{

}

void MainInterface::mousePressEvent(QMouseEvent* e)
{
        std::cout << "widget mouse press catched" << std::endl;
    //QWidget::mousePressEvent(e);
    if (e->button() == Qt::RightButton)
    {
        break_line_drawing();
        selected_edge = nullptr;
    }
    else
    {
        QPoint globpos = e->globalPos();
        QPoint p2 = ui->graphicsView->mapFromGlobal(globpos);
        QPointF p3 = ui->graphicsView->mapToScene(p2);
        QRectF rect = QRectF(p3.x()-5,p3.y()-5,10,10);
        QList<QGraphicsItem*> list =  scene->items(rect);
        std::cout<<"list size is: "<<list.size()<<std::endl;
        for (QList<QGraphicsItem*>::iterator it = list.begin(); it != list.end();++it)
        {
            std::cout<< "type is:" << (*it)->type()<<std::endl;
            QGraphicsLineItem* item = qgraphicsitem_cast<QGraphicsLineItem*>(*it);
            if (arrows->count(item) != 0)
                selected_edge = arrows->at(item);
        }
    }
    //QWidget::mousePressEvent(e);
}

void MainInterface::no_data()
{

}

void MainInterface::redraw_line()
{

}

void MainInterface::start_search()
{
    //get nodes names
    /*Search_Types st = static_cast<Search_Types>(ui->chose_type->currentIndex());
    Search_Layers sl = static_cast<Search_Layers>(ui->chose_layer->currentIndex());
    QString from = ui->lineEdit->text();
    QString to = ui->lineEdit_2->text();
    
    std::cout<< "type is: "<<commons::get_search_type_name(st)<<std::endl;
    std::cout<< "layer is: "<<commons::get_search_layer_names(sl)<<std::endl;
    std::cout<< "from is: "<<from.toStdString()<<std::endl;
    std::cout<< "to is: "<<to.toStdString()<<std::endl;
    */
    
}

void MainInterface::start_update_L1_object()
{

}

void MainInterface::start_update_L2_object()
{

}

void MainInterface::start_update_L3_object()
{

}

void MainInterface::start_update_L4_object()
{

}

void MainInterface::start_update_L5_object()
{

}
