#include "l3_popup.h"
#include "ui_l3_popup.h"
#include <QSpinBox>
l3_popup::l3_popup(QWidget* parent): QWidget(parent), ui(new Ui::l3_popup)
{
    ui->setupUi(this);
    connect(ui->add,SIGNAL(clicked()),this,SLOT(add_table_line()));
    connect(ui->remove,SIGNAL(clicked()),this,SLOT(remove_table_line()));
    for (int i = 0; i< Component_Priority_Category::HANDLING_SIZE; i++)
        ui->chose_layer->insertItem(i, QString::fromStdString(commons::get_component_priority_type(static_cast<Component_Priority_Category>(i))));

   //  data.reset(new Logical_Vertex());
    //data->name = "";
}

l3_popup::~l3_popup()
{
    delete ui;
}


std::shared_ptr<L3_Vertex> l3_popup::get_data()
{
    return data;
}
void l3_popup::set_data(std::shared_ptr<L3_Vertex> data_in)
{
    this->data = data_in;
    update_graphic_from_data();
}

void l3_popup::add_table_line()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
      
    QSpinBox* spin = new QSpinBox();
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 0, spin );
    
    QSpinBox* spin2 = new QSpinBox();
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 1, spin2 );
}
void l3_popup::remove_table_line()
{
    ui->tableWidget->removeRow(ui->tableWidget->rowCount()-1);
    consolidate_data();
}
void l3_popup::consolidate_data()
{
    std::cout<<"consolidate data enter"<<std::endl;
    std::cout<< ui->tableWidget <<std::endl;
    if(data->OS_slots->size()!=0)
    {
        data->OS_slots->clear(); //this let me avoid selective removal of table lines, just clear and recreate the internal fields
        std::cout<<"consolidate data size check in"<<std::endl;    
    }
    
    for (int i = 0; i< ui->tableWidget->rowCount();i++)
    {
        std::cout<<"consolidate data loop enter"<<std::endl;
        QSpinBox *id = qobject_cast<QSpinBox*>(ui->tableWidget->cellWidget(i,0));
        QSpinBox *priority = qobject_cast<QSpinBox*>(ui->tableWidget->cellWidget(i,1));
        data->OS_slots->insert(std::make_pair(id->value(),priority->value()));
    }
    //get the other data.
      data->scheduler_type = commons::int_To_Priority_Handler(ui->chose_layer->currentIndex());
}

void l3_popup::update_graphic_from_data()
{
    std::cout<<data->OS_slots->size()<<std::endl;
    ui->tableWidget->setRowCount(0);
    for (std::map<int,int>::iterator it = data->OS_slots->begin();it != data->OS_slots->end();++it)
    {
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    QSpinBox* spin = new QSpinBox();
    spin->setValue((*it).first);
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 0, spin );
    QSpinBox* spin3 = new QSpinBox();
    spin3->setValue((*it).second);
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 1, spin3 );
  
    }
    ui->chose_layer->setCurrentIndex(commons::Priority_Handler_To_Int(data->scheduler_type));
}


