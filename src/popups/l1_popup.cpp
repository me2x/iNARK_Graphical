#include "l1_popup.h"
#include "ui_l1_popup.h"

L1_popup::L1_popup(QDialog* parent): QDialog(parent), ui(new Ui::L1_popup)
{
    ui->setupUi(this);
    connect(ui->Accept, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->Reject, SIGNAL(clicked()), this, SLOT(reject()));
    //data.reset(new Logical_Vertex());
    //data->name = "";
}

L1_popup::~L1_popup()
{
    delete ui;
}

void L1_popup::exec()
{
    QDialog::exec();
}

void L1_popup::accept()
{
    /*QString text = ui->lineEdit_3->text();
    data->name = text.toStdString();
    //TODO controllo name non sia gia in uso
    if (names->count(data->name) == 0){
        std::cout <<"name accepted"<<std::endl;
        data->create_L1_component(data->name);
        names->insert(data->name);
        QDialog::accept();
    }
    else 
    {
        std::cout <<"name refused"<<std::endl;
        QDialog::reject();
        
    }*/
}
std::shared_ptr<Logical_Vertex> L1_popup::get_data()
{
    return data;
}
void L1_popup::reject()
{
    QDialog::reject();
}
void L1_popup::set_data(std::shared_ptr<Logical_Vertex> data_in)
{
    this->data = data_in;
}
void L1_popup::set_names(std::shared_ptr<std::set <std::string> > names)
{
    this->names = names;
}

