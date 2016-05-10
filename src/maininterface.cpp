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
MainInterface::MainInterface(QWidget* parent): QWidget(parent), ui(new Ui::MainInterface)
{
    ui->setupUi(this);
    //Layer and Types scopes are set up from the enum in graph/graph_common
    for (int i = 0; i< SEARCH_LAYER_END;i++)
        ui->chose_layer->insertItem(i, QString::fromStdString(get_search_layer_names(static_cast<Search_Layers>(i))));
    for (int i = 0; i< SEARCH_TYPE_END; i++)
        ui->chose_type->insertItem(i, QString::fromStdString(get_search_type_name(static_cast<Search_Types>(i))));

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

void MainInterface::delete_edge(std::shared_ptr< Graphic_Edge >& to_be_deleted)
{
    std::cout<<"tobedeeleted counter" << to_be_deleted.use_count()<<std::endl;
    scene->removeItem(to_be_deleted->dx_line);
    scene->removeItem(to_be_deleted->sx_line);
    scene->removeItem(to_be_deleted->line);
    scene->removeItem(to_be_deleted->label_start);
    scene->removeItem(to_be_deleted->label_stop);
    to_be_deleted.reset();
}


void MainInterface::delete_vertex(std::shared_ptr< Graphic_Vertex >& to_be_deleted)
{
    std::cout<<"tobedeeleted counter" << to_be_deleted.use_count()<<std::endl;
    scene->removeItem(to_be_deleted.get());
    to_be_deleted.reset();
}

void MainInterface::break_line_drawing()
{

}

void MainInterface::component_clicked()
{

}

void MainInterface::create_L1_obj()
{

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

}

void MainInterface::finalize_line()
{

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
    QWidget::mousePressEvent(e);
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
