#include "l5_popup.h"
#include "ui_l5_popup.h"
#include <iostream>

l5_popup::l5_popup(QDialog* parent): QDialog(parent), ui(new Ui::l5_popup)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(reject()));
    data.reset(new Logical_Vertex());
    data->name = "";
}

l5_popup::~l5_popup()
{
    delete ui;
}

void l5_popup::exec()
{
    QDialog::exec();
}

void l5_popup::accept()
{
    QString text = ui->lineEdit->text();
    data->name = text.toStdString();
    //TODO controllo name non sia gia in uso
    if (names->count(data->name) == 0){
        std::cout <<"name accepted"<<std::endl;
        data->create_L2_component(data->name);
        names->insert(data->name);
        QDialog::accept();
    }
    else 
    {
        std::cout <<"name refused"<<std::endl;
        QDialog::reject();
        
    }
}
std::shared_ptr<Logical_Vertex> l5_popup::get_data()
{
    return data;
}
void l5_popup::reject()
{
    QDialog::reject();
}
void l5_popup::set_data(std::shared_ptr<Logical_Vertex> data_in)
{
    this->data = data_in;
}
void l5_popup::set_names(std::shared_ptr<std::set <std::string> > names)
{
    this->names = names;
}

