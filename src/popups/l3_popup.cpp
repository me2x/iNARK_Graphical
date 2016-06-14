#include "l3_popup.h"
#include "ui_l3_popup.h"

l3_popup::l3_popup(QDialog* parent): QDialog(parent), ui(new Ui::l3_popup)
{
    ui->setupUi(this);
    connect(ui->ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->cancel, SIGNAL(clicked()), this, SLOT(reject()));
  //  data.reset(new Logical_Vertex());
    //data->name = "";
}

l3_popup::~l3_popup()
{
    delete ui;
}

void l3_popup::exec()
{
    QDialog::exec();
}

void l3_popup::accept()
{
    /*QString text = ui->lineEdit_2->text();
    std::map<int, int> slot_map;
    data->name = text.toStdString();
    //TODO controllo name non sia gia in uso
    if (names->count(data->name) == 0 && data->name != ""){
        std::cout <<"name accepted"<<std::endl;
        data->create_L3_component(data->name,0,slot_map);
        names->insert(data->name);
        QDialog::accept();
    }
    else 
    {
        std::cout <<"name refused"<<std::endl;
        QDialog::reject();
        
    }*/
}
std::shared_ptr<Logical_Vertex> l3_popup::get_data()
{
    return data;
}
void l3_popup::reject()
{
    QDialog::reject();
}
void l3_popup::set_data(std::shared_ptr<Logical_Vertex> data_in)
{
    this->data = data_in;
}
void l3_popup::set_names(std::shared_ptr<std::set <std::string> > names)
{
    this->names = names;
}

