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
    arrows.reset(new std::map<QGraphicsLineItem*,std::shared_ptr<Graphic_Edge> >);
    from_port = NO_PORT;
    to_port = NO_PORT;
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
        std::ofstream myfile;
        std::stringstream opt;
    myfile.open (filename.toStdString());
    for (std::map<Graphic_Vertex*,std::shared_ptr<Logical_Vertex>>::iterator it = vertices.begin(); it != vertices.end();++it)
    {
        std::cout << "printing " <<std::endl;
        (*it).second->print(myfile,opt);
        std::cout << "after printing " <<std::endl;
    }
    for (std::map<std::shared_ptr<Graphic_Edge>,std::shared_ptr<Logical_Edge>>::iterator it =  edges.begin(); it !=  edges.end();++it)
    {
        std::cout << "printing " <<std::endl;
        (*it).second->print(myfile);
        std::cout << "after printing " <<std::endl;
    }
    opt << "option stream" <<std::endl;
    myfile<<opt.str();
    myfile.close(); 

        
    }
}
//TODO logical deletion has to be added
void MainInterface::delete_edge(std::shared_ptr< Graphic_Edge > to_be_deleted)
{
    //remove from edge_map
    std::string edge_string = (edges.at(to_be_deleted)).get()->get_string();
    edges_set.erase(edge_string);
    // if ports remove from used_slot_and_ports
    std::map< std::pair <std::string, int>, std::set<std::shared_ptr<Graphic_Edge>> >::iterator it = used_slot_and_ports.find(edges.at(to_be_deleted)->get_from());
    if (it != used_slot_and_ports.end())
    {
        std::cout <<"used_slot_and_ports key found, erasing"<<(*it).second.size()<<std::endl;
        (*it).second.erase(to_be_deleted);
        if (it->second.size() == 1)
        {
            std::cout <<"used_slot_and_ports check passed, blackening"<<std::endl;
            (*(*it).second.begin())->set_color(Graphic_Edge::BLACK);
        }
        else if (it->second.size() == 0)
            used_slot_and_ports.erase(it);
    }
    
    it = used_slot_and_ports.find(edges.at(to_be_deleted)->get_to());
    if (it != used_slot_and_ports.end())
    {
        (*it).second.erase(to_be_deleted);
        if (it->second.size() == 1)
            (*(*it).second.begin())->set_color(Graphic_Edge::BLACK);
        else if (it->second.size() == 0)
            used_slot_and_ports.erase(it);
    }
    
    
    // remove from the arrow map. graphical utility
    arrows->erase(to_be_deleted->line);
    //remove from the edges map. logical value
    edges.erase(to_be_deleted);
    //remove from the scene.
    std::cout<<"tobedeeleted counter internal beefore" << to_be_deleted.use_count()<<std::endl;
    scene->removeItem(to_be_deleted->dx_line);
    scene->removeItem(to_be_deleted->sx_line);
    scene->removeItem(to_be_deleted->line);
    scene->removeItem(to_be_deleted->label_start);
    scene->removeItem(to_be_deleted->label_stop);

    to_be_deleted.reset();
    std::cout<<"tobedeeleted counter inteernal afteer" << to_be_deleted.use_count()<<std::endl;
}

//TODO logical deletion has to be added
void MainInterface::delete_vertex(Graphic_Vertex* to_be_deleted)
{
    break_line_drawing();
    scene->removeItem(to_be_deleted);
    names->erase(*(vertices.at(to_be_deleted)->name.get()));
    vertices.erase(to_be_deleted);
    //IMPORTANT delete the edges first because they contains pointer to vertex that will be deleted here. to avoid segfault.
    
    
    
    for (std::vector<std::weak_ptr<Graphic_Edge>>::iterator it = to_be_deleted->related_edges.begin(); it != to_be_deleted->related_edges.end(); )
    {
        std::cout <<"inside related edges removal loop"<<std::endl;
        if (std::shared_ptr<Graphic_Edge> ptr = (*it).lock())
        {
            delete_edge(ptr);
            ++it;
        }
        else
            it = to_be_deleted->related_edges.erase(it);
        
    }
     std::cout <<"after related edges removal" <<std::endl;
    delete to_be_deleted;
    starting_object = nullptr;
}

void MainInterface::break_line_drawing()
{
    std::cout<<"enter break line drawing"<<std::endl;
    if (!is_drawing) return;
    
    scene->removeItem(current_line_item);
    starting_object->setSelected(false);
    std::cerr<<"selecteditems size: "<<scene->selectedItems().size()<<std::endl;
    starting_object = nullptr;
    is_drawing = false;
    std::cout <<"exit from break line"<<std::endl;
}

void MainInterface::component_clicked()
{
    if (selected_edge.use_count() != 0) 
    {
        selected_edge->reset_color();
        selected_edge.reset();
    }
    
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
                std::cout<<"test: precall di portselect start is: "<<
                commons::Layer_to_String(vertices.at(starting_object)->layer)<<
                "while target is: "<<
                commons::Layer_to_String(vertices.at(arrival_object)->layer)<<
                std::endl;
                if (((vertices.at(starting_object))->layer == Layer::RESOURCE && (vertices.at(arrival_object))->layer == Layer::RESOURCE ) ||
                    ((vertices.at(starting_object))->layer == Layer::TASK && (vertices.at(arrival_object))->layer == Layer::CONTROLLER))  
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
                    from_port = NO_PORT;
                    to_port = NO_PORT;
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

            
        }
        else 
            std::cout << " other type, value is: " << item_tmp->type()<<std::endl;
}

void MainInterface::create_L1_obj()
{
create_object(FUNCTION);
}
void MainInterface::create_L2_obj()
{
create_object(TASK);
}
void MainInterface::create_L3_obj()
{
create_object(CONTROLLER);
}
void MainInterface::create_L4_obj()
{
create_object(RESOURCE);
}
void MainInterface::create_L5_obj()
{
create_object(PHYSICAL);
}
void MainInterface::delete_items()
{    
    if (selected_edge.use_count() != 0)
    {
        std::cout<<"deleting edgee"<<std::endl;
        std::cout<<"tobedeeleted counter before" << selected_edge.use_count()<<std::endl;
        delete_edge(selected_edge);
        selected_edge.reset();
        std::cout<<"tobedeeleted counter afteer" << selected_edge.use_count()<<std::endl;
    }
    else if (starting_object != nullptr)
    {
        std::cout<<"deleting veertex"<<std::endl;
        delete_vertex(starting_object);
        std::cout <<"out from delete vertex"<<std::endl;
    }
}
void MainInterface::get_line_data_from_popup()
{
    std::pair<int, int> portspair =  ports->get_ports();
    from_port = portspair.first;
    to_port = portspair.second;
    if (from_port != NO_PORT && to_port != NO_PORT)
        finalize_line();
    else
        break_line_drawing();
}
void MainInterface::finalize_line()
{
    std::cout <<"drawing line"<<std::endl;
    std::shared_ptr<Logical_Edge> l_edge_ptr;
    l_edge_ptr.reset(new Logical_Edge(*((vertices.at(starting_object))->name.get()),*((vertices.at(arrival_object))->name.get()),from_port,to_port));
    std::shared_ptr<Graphic_Edge> edge;
    edge.reset(new Graphic_Edge()); //should not be an issue, all the insert are internal, and if not done the counter decrease at the exit and the object is freed
    bool flag_is_inserted = false;
    if (edges_set.find(l_edge_ptr->get_string())==edges_set.end())
    {
        edges_set.insert(l_edge_ptr->get_string());
        //insert in arrows done inside the function
        edge->set_data(starting_object,arrival_object,from_port,to_port,arrows,edge);
        std::cout <<"graph edge created"<<std::endl;
        flag_is_inserted = edges.insert(std::make_pair(edge,l_edge_ptr)).second;
    }
    else
        std::cout<<"edge already exists"<<l_edge_ptr->get_string()<<std::endl;
    
    std::map<std::pair <std::string, int>,std::set<std::shared_ptr<Graphic_Edge>>>::iterator it = used_slot_and_ports.find(l_edge_ptr->get_from());
    if (it != used_slot_and_ports.end() && flag_is_inserted)
    {
        if ((*it).second.size() == 1)
            (*(*it).second.begin())->set_color(Graphic_Edge::BLUE);
        edge->set_color(Graphic_Edge::BLUE);
        used_slot_and_ports.at(it->first).insert(edge);
     //doppio uso della porta from, colora i due archi partenti da li   ;
    }
    else if(from_port != NO_PORT && flag_is_inserted)
    {
        //crea set e inserisci arco.
        std::set<std::shared_ptr<Graphic_Edge>> tmp_set;
        tmp_set.insert(edge);
        used_slot_and_ports.insert(std::make_pair(l_edge_ptr->get_from(),tmp_set));        
    }
    it = used_slot_and_ports.find(l_edge_ptr->get_to());
    if (it != used_slot_and_ports.end() && flag_is_inserted)
    {
        if ((*it).second.size() == 1)
            (*(*it).second.begin())->set_color(Graphic_Edge::BLUE);
        edge->set_color(Graphic_Edge::BLUE);
        used_slot_and_ports.at(it->first).insert(edge);
     
    }
    else if(to_port != NO_PORT && flag_is_inserted)
    {
        std::set<std::shared_ptr<Graphic_Edge>> tmp_set;
        tmp_set.insert(edge);
        used_slot_and_ports.insert(std::make_pair(l_edge_ptr->get_to(),tmp_set));        
    }
    
    
    
    arrival_object->setSelected(false);
    arrival_object->setArrowTarget();
    std::cout << "arrival object is: "<<arrival_object<<std::endl;
    arrival_object = nullptr;
    starting_object->setSelected(false);
    starting_object = nullptr;
    if (selected_edge.use_count() != 0) 
    {
        selected_edge->reset_color();
        selected_edge.reset();
    }
    std::cout << "fine finalize line, item selected count is:" <<scene->selectedItems().count()<<std::endl;
    std::cout << "arrival object is: "<<arrival_object<<std::endl;
    from_port = NO_PORT;
    to_port = NO_PORT;
    //ports.reset();
    std::cout << "exit point from finalize line"<<std::endl;
}

void MainInterface::Layer_1_press_event()
{
create_button_press(FUNCTION);
}
void MainInterface::Layer_2_press_event()
{
create_button_press(TASK);
}
void MainInterface::Layer_3_press_event()
{
create_button_press(CONTROLLER);
}
void MainInterface::Layer_4_press_event()
{
create_button_press(RESOURCE);
}
void MainInterface::Layer_5_press_event()
{
create_button_press(PHYSICAL);
}
void MainInterface::mousePressEvent(QMouseEvent* e)
{
        std::cout << "widget mouse press catched" << std::endl;
    //QWidget::mousePressEvent(e);
    if (e->button() == Qt::RightButton)
    {
        break_line_drawing();
        if (selected_edge.use_count()!=0)
        {
            selected_edge->reset_color();
            selected_edge.reset();
        }
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
            {
                if (selected_edge.use_count() != 0)
                {
                    selected_edge->reset_color();
                }
                selected_edge = arrows->at(item);
                selected_edge->line->setPen(QPen(Qt::red)); 
                
                
            }
        }
    }
    //QWidget::mousePressEvent(e);
}
void MainInterface::no_data()
{

}
void MainInterface::redraw_line()
{
    if (!is_drawing) return;
    //std::cout<<"inside redraw line" <<std::endl;
    
    QPen blackpen = QPen(Qt::black);
      //std::cout << "curr line != 0" <<std::endl;
    QPoint p1 = QCursor::pos();
    QPoint p2 = ui->graphicsView->mapFromGlobal(p1);
    QPointF p = ui->graphicsView->mapToScene(p2);
    scene->removeItem(current_line_item);
        
    QLineF newline(starting_object->x()+starting_object->boundingRect().center().x(),starting_object->y()+starting_object->boundingRect().bottom(),p.x()-3, p.y()-3);
    current_line_item =  scene->addLine(newline,blackpen);
       
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
    update_object(FUNCTION);
}
void MainInterface::start_update_L2_object()
{
    update_object(TASK);
}
void MainInterface::start_update_L3_object()
{
    update_object(CONTROLLER);
}
void MainInterface::start_update_L4_object()
{
    update_object(RESOURCE);
}
void MainInterface::start_update_L5_object()
{
    update_object(PHYSICAL);
}
void MainInterface::create_object(Layer l)
{
    std::shared_ptr<Logical_Vertex> data = tab_pop->get_data();
    Graphic_Vertex* temp= new Graphic_Vertex(); //IMPORTANT: when removed from scene has to be freed
    temp->setLayer(commons::Layer_to_int(l)-1);
    scene->addItem(temp);
    connect(temp,SIGNAL(riquadroCliccatoSx()),this,SLOT(component_clicked()));
    connect(temp,SIGNAL(riquadroMosso()),this,SLOT(break_line_drawing()));
    connect(temp,SIGNAL(riquadroCliccatoDx()),this,SLOT(break_line_drawing()));
     switch (l)
    {
       case FUNCTION:
            {
                connect(temp,SIGNAL(riquadroDoubleClick()),this,SLOT(start_update_L1_object()));
                break;
            }
            case TASK:
            {
                connect(temp,SIGNAL(riquadroDoubleClick()),this,SLOT(start_update_L2_object()));
                break;
            }
            case CONTROLLER:
            {
                connect(temp,SIGNAL(riquadroDoubleClick()),this,SLOT(start_update_L3_object()));
                break;
            }
            case RESOURCE:
            {
                connect(temp,SIGNAL(riquadroDoubleClick()),this,SLOT(start_update_L4_object()));
                break;
            }
            case PHYSICAL:
            {
                connect(temp,SIGNAL(riquadroDoubleClick()),this,SLOT(start_update_L5_object()));
                break;
            }
            default:
            {
                throw std::runtime_error("create object called without layer set");
                break;
            } 
    }
    temp->text->setPlainText(QString::fromStdString(*(data->name)));
    vertices.insert(std::make_pair(temp,data));
}
void MainInterface::create_button_press(Layer l)
{
if (selected_edge.use_count() != 0)
    {
        selected_edge->reset_color();
        selected_edge.reset();
    }
    std::cout<<"layer 3 press event triggered"<<std::endl;
    tab_pop.reset(new Tab_popup());
    std::shared_ptr<Logical_Vertex> vtx;
    vtx.reset(new Logical_Vertex());
    
    std::shared_ptr< std::string > comp_name;
    comp_name.reset(new std::string(""));
    switch (l)
    {
       case FUNCTION:
            {
                vtx->create_L1_component(comp_name);
                tab_pop->set_data(PHYSICAL,vtx,names,false);
                connect(tab_pop.get(),SIGNAL(accepted()),this,SLOT(create_L1_obj()));
                break;
            }
            case TASK:
            {
                vtx->create_L2_component(comp_name);
                tab_pop->set_data(PHYSICAL,vtx,names,false);
                connect(tab_pop.get(),SIGNAL(accepted()),this,SLOT(create_L2_obj()));
                break;
            }
            case CONTROLLER:
            {
                std::shared_ptr< std::map< int, int > > sched_slot;
                sched_slot.reset(new std::map< int, int>());
                vtx->create_L3_component(comp_name,0,sched_slot);
                tab_pop->set_data(CONTROLLER,vtx,names,false);
                connect(tab_pop.get(),SIGNAL(accepted()),this,SLOT(create_L3_obj()));
                break;
            }
            case RESOURCE:
            {
                std::shared_ptr< std::map< int, Port > > ports;
                ports.reset(new std::map< int, Port>());
                vtx->create_L4_component(comp_name,0,0,ports);
                tab_pop->set_data(RESOURCE,vtx,names,false);
                connect(tab_pop.get(),SIGNAL(accepted()),this,SLOT(create_L4_obj()));
                break;
            }
            case PHYSICAL:
            {
                vtx->create_L5_component(comp_name);
                tab_pop->set_data(PHYSICAL,vtx,names,false);
                connect(tab_pop.get(),SIGNAL(accepted()),this,SLOT(create_L5_obj()));
                break;
            }
            default:
            {
                throw std::runtime_error("create object called without layer set");
                break;
            } 
    }
    connect(tab_pop.get(),SIGNAL(rejected()),this,SLOT(no_data()));
    std::cout<<"layer 3 press event popup connection"<<std::endl;
    tab_pop->exec();
    tab_pop->activateWindow();
  
    std::cout<<"layer 3 press event popup execution"<<std::endl;
}
void MainInterface::finalize_update()
{
    //only graphical things, logical are done on the tab_pop->accept
    starting_object->text->setPlainText(QString::fromStdString(*(tab_pop->get_data()->name)));
    starting_object = nullptr;
}
void MainInterface::update_object(Layer l)
{
if (selected_edge.use_count() != 0)
    {
        selected_edge->reset_color();
        selected_edge.reset();
    }
    
    std::cout<<"update event called"<<std::endl;
    std::cout<<"update event called"<<starting_object<<std::endl;
    tab_pop.reset(new Tab_popup());
    try{
        tab_pop->set_data(l,vertices.at(starting_object),names,true);
    }
    catch (std::out_of_range e)
    {
        return;
    }
    scene->removeItem(current_line_item);
    is_drawing = false;
    connect(tab_pop.get(),SIGNAL(accepted()),this,SLOT(finalize_update()));
    connect(tab_pop.get(),SIGNAL(rejected()),this,SLOT(no_data()));
    tab_pop->exec();
    tab_pop->activateWindow();
  
    std::cout<<"layer 4 update  event popup execution"<<std::endl;
}
