#include "l5_popup.h"
#include "ui_l5_popup.h"
#include <iostream>

l5_popup::l5_popup(QWidget* parent): QWidget(parent), ui(new Ui::l5_popup)
{
    ui->setupUi(this);
 //   data.reset(new Logical_Vertex());
 //   data->name = "";
}

l5_popup::~l5_popup()
{
    delete ui;
}

std::shared_ptr<L5_Vertex> l5_popup::get_data()
{
    return data;
}
void l5_popup::set_data(std::shared_ptr<L5_Vertex> data_in)
{
    this->data = data_in;
}

void l5_popup::consolidate_data()
{

}
void l5_popup::update_graphic_from_data()
{

}
