#include "l4_popup.h"
#include "ui_l4_popup.h"

#include <QComboBox>
#include <QSpinBox>
#include <iostream>

l4_popup::l4_popup(QWidget* parent): QWidget(parent), ui(new Ui::l4_popup)
{
    ui->setupUi(this);
    connect(ui->add,SIGNAL(clicked()),this,SLOT(add_table_line()));
    connect(ui->remove,SIGNAL(clicked()),this,SLOT(remove_table_line()));
    for (int i = 0; i< Component_Type::TYPE_SIZE;i++)
        ui->chose_layer->insertItem(i, QString::fromStdString(commons::get_component_type_name(static_cast<Component_Type>(i))));
    for (int i = 0; i< Component_Priority_Category::HANDLING_SIZE; i++)
        ui->chose_layer_2->insertItem(i, QString::fromStdString(commons::get_component_priority_type(static_cast<Component_Priority_Category>(i))));

    
    
 //   connect(ui->ok, SIGNAL(clicked()), this, SLOT(accept()));
 //   connect(ui->cancel, SIGNAL(clicked()), this, SLOT(reject()));
 //   data.reset(new Logical_Vertex());
 //   data->name = "";
}


void l4_popup::add_table_line()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    /*QComboBox* settingA = new QComboBox();
    settingA->addItem("100");
    settingA->addItem("200");
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 1, settingA );
    */
    QComboBox* settingB = new QComboBox();
    settingB->addItem("True");
    settingB->addItem("False");
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 3, settingB );
    
    QSpinBox* spin = new QSpinBox();
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 0, spin );
    
    
    QSpinBox* spin2 = new QSpinBox();
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 2, spin2 );
    spin2->setMinimum(-1);
    
    QSpinBox* spin3 = new QSpinBox();
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 1, spin3 );
    
    /* getter
     QComboBox *myCB = qobject_cast<QComboBox*>(ui->tableWidget_4->cellWidget(0,0));
     InputComboData << myCB->currentText();
     
     */
}
void l4_popup::remove_table_line()
{
    ui->tableWidget->removeRow(ui->tableWidget->rowCount()-1);
}

l4_popup::~l4_popup(){
    delete ui;
    data.reset();
}

void l4_popup::consolidate_data()
{
    std::cout<<"consolidate data enter"<<std::endl;
    std::cout<< ui->tableWidget <<std::endl;
    if(data->ports->size()!=0)
    {
        data->ports->clear(); //this let me avoid selective removal of table lines, just clear and recreate the internal fields
        std::cout<<"consolidate data size check in"<<std::endl;    
    }
    
    for (int i = 0; i< ui->tableWidget->rowCount();i++)
    {
        std::cout<<"consolidate data loop enter, row count is: "<<ui->tableWidget->rowCount()<<std::endl;
        Port port;
        QSpinBox *id = qobject_cast<QSpinBox*>(ui->tableWidget->cellWidget(i,0));
        //TODO check != 0
        int id_port = id->value();
        QSpinBox *priority = qobject_cast<QSpinBox*>(ui->tableWidget->cellWidget(i,1));
        port.priority = priority->value();
        
        QSpinBox *ass_id = qobject_cast<QSpinBox*>(ui->tableWidget->cellWidget(i,2));
        port.associate_id = ass_id->value();
        QComboBox *isMaster = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(i,3));
        port.isMaster = isMaster->currentText()=="True"?true:false;
        
        //data->ports->find(id_port) != data->ports->end? data->ports->at(id_port) = port : 
        data->ports->insert(std::make_pair(id_port,port));
    }
    //get the other data.
    //actually the Component_Priority_Category goes from 1 to 3, and not 0 to 2, so +1 has to be put in the second.
    data->component_type = ui->chose_layer->currentIndex();
    data->scheduler_type = commons::int_To_Priority_Handler(ui->chose_layer_2->currentIndex());
}

std::shared_ptr< L4_Vertex > l4_popup::get_data()
{
    return data;
}

void l4_popup::set_data(std::shared_ptr< L4_Vertex > data_in)
{
    data = data_in;
    //ui->lineEdit_2->setText(QString::fromStdString(data->name));
    update_graphic_from_data();
}

void l4_popup::update_graphic_from_data()
{
    std::cout<<data->ports->size()<<std::endl;
    ui->tableWidget->setRowCount(0);
    for (std::map<int,Port>::iterator it = data->ports->begin();it != data->ports->end();++it)
    {
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());

    QComboBox* settingB = new QComboBox();
    settingB->addItem("True");
    settingB->addItem("False");
    settingB->setCurrentIndex((*it).second.isMaster == true ? 0 : 1);
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 3, settingB );
    
    QSpinBox* spin = new QSpinBox();
    spin->setValue((*it).first);
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 0, spin );
    
    QSpinBox* spin2 = new QSpinBox();
    spin2->setMinimum(-1);
    spin2->setValue((*it).second.associate_id);
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 2, spin2 );
   
    QSpinBox* spin3 = new QSpinBox();
    
    spin3->setValue((*it).second.priority);
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 1, spin3 );
  
    }
    ui->chose_layer->setCurrentIndex(data->component_type);
    ui->chose_layer_2->setCurrentIndex(commons::Priority_Handler_To_Int(data->scheduler_type));
}
