#include "l2_popup.h"
#include "ui_l2_popup.h"

l2_popup::l2_popup(QDialog* parent): QDialog(parent), ui(new Ui::l2_popup)
{
    ui->setupUi(this);
     //data.reset(new Logical_Vertex());
    //data->name = "";
}

l2_popup::~l2_popup()
{
    delete ui;
}

void l2_popup::exec()
{
    QDialog::exec();
}

void l2_popup::accept()
{
    /*QString text = ui->lineEdit->text();
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
        
    }*/
}
std::shared_ptr<Logical_Vertex> l2_popup::get_data()
{
    return data;
}
void l2_popup::reject()
{
    QDialog::reject();
}
void l2_popup::set_data(std::shared_ptr<Logical_Vertex> data_in)
{
    this->data = data_in;
}
void l2_popup::set_names(std::shared_ptr<std::set <std::string> > names)
{
    this->names = names;
}

