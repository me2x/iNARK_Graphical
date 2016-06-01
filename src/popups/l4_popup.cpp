#include "l4_popup.h"
#include "ui_l4_popup.h"

#include <QComboBox>
#include <QSpinBox>
#include <iostream>

l4_popup::l4_popup(QDialog* parent): QDialog(parent), ui(new Ui::l4_popup)
{
    ui->setupUi(this);
    connect(ui->add,SIGNAL(clicked()),this,SLOT(add_table_line()));
    connect(ui->remove,SIGNAL(clicked()),this,SLOT(remove_table_line()));
    connect(ui->ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->cancel, SIGNAL(clicked()), this, SLOT(reject()));
    data.reset(new Logical_Vertex());
    data->name = "";
}


void l4_popup::add_table_line()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    QComboBox* settingA = new QComboBox();
    settingA->addItem("100");
    settingA->addItem("200");
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 1, settingA );
    
    QComboBox* settingB = new QComboBox();
    settingB->addItem("True");
    settingB->addItem("False");
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 3, settingB );
    
    QSpinBox* spin = new QSpinBox();
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 0, spin );
    
    QSpinBox* spin2 = new QSpinBox();
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 2, spin2 );
   
    /* getter
     QComboBox *myCB = qobject_cast<QComboBox*>(ui->tableWidget_4->cellWidget(0,0));
     InputComboData << myCB->currentText();
     
     */
}
void l4_popup::remove_table_line()
{
    ui->tableWidget->removeRow(ui->tableWidget->rowCount()-1);
}


void l4_popup::accept()
{
    std::map<int,Port> ports_map;
     for (int i = 0; i< ui->tableWidget->rowCount();i++)
    {
        Port port;
        QSpinBox *id = qobject_cast<QSpinBox*>(ui->tableWidget->cellWidget(i,0));
        //TODO check != 0
        int id_port = id->value();
        QComboBox *priority = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(i,1));
        port.priority = priority->currentText().toInt();
        
        QSpinBox *ass_id = qobject_cast<QSpinBox*>(ui->tableWidget->cellWidget(i,2));
        port.associate_id = ass_id->value();
        QComboBox *isMaster = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(i,3));
        port.isMaster = isMaster->currentText()=="True"?true:false;
        
        ports_map.insert(std::make_pair(id_port,port));
    }
    QString text = ui->lineEdit_2->text();
    data->name = text.toStdString();
    //TODO controllo name non sia gia in uso
    
    if (names->count(data->name) == 0 && data->name != ""){
        data->create_L4_component(data->name,1,1,ports_map);
        std::cout <<"name accepted"<<std::endl;
        QDialog::accept();
    }
    else 
    {
        std::cout <<"name refused"<<std::endl;
        QDialog::reject();
        
    }
   
}
l4_popup::~l4_popup(){
    delete ui;
}

void l4_popup::reject()
{
    std::cout <<"refuse button"<<std::endl;
    QDialog::reject();
}

void l4_popup::exec()
{
    QDialog::exec();
}

std::shared_ptr< Logical_Vertex > l4_popup::get_data()
{
    return data;
}

void l4_popup::set_names(std::shared_ptr< std::set< std::string > > names)
{
    this->names = names;
}
void l4_popup::set_data(std::shared_ptr< Logical_Vertex > data_in)
{
    data = data_in;
    ui->lineEdit_2->setText(QString::fromStdString(data->name));
    std::shared_ptr<L4_Vertex> vtx = std::static_pointer_cast<L4_Vertex>( data->components_opt.at(0));
    for (std::map<int,Port>::iterator it = vtx->ports.begin();it != vtx->ports.end();++it)
    {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    QComboBox* settingA = new QComboBox();
    settingA->addItem("100");
    settingA->addItem("200");
    settingA->setCurrentIndex((*it).second.priority == 100 ? 0 : 1);
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 1, settingA );
    
    QComboBox* settingB = new QComboBox();
    settingB->addItem("True");
    settingB->addItem("False");
    settingB->setCurrentIndex((*it).second.isMaster == true ? 0 : 1);
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 3, settingB );
    
    QSpinBox* spin = new QSpinBox();
    spin->setValue((*it).first);
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 0, spin );
    
    QSpinBox* spin2 = new QSpinBox();
    spin2->setValue((*it).second.associate_id);
    ui->tableWidget->setCellWidget ( ui->tableWidget->rowCount()-1, 2, spin2 );
   
    }
}