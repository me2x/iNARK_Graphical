#include "ports_popup.h"
#include "ui_ports_popup.h"
#include <iostream>
#include <QButtonGroup>
#include "../logical_vertex.h"
#include <memory>
void Ports_Popup::exec()
{
    QDialog::exec();
}
Ports_Popup::Ports_Popup(QDialog* parent): QDialog(parent), ui(new Ui::Ports_Popup)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    bg = new QButtonGroup();
    bg->setExclusive(true);
    bg2 = new QButtonGroup();
    bg2->setExclusive(true);
}
void Ports_Popup::set_data(std::shared_ptr<Logical_Vertex> from, std::shared_ptr<Logical_Vertex> to)
{

    if (from->layer == RESOURCE) //dont need to check the target, they are already checked outside
    {
        std::shared_ptr<L4_Vertex> l4_ptr_from = std::static_pointer_cast<L4_Vertex>(from->components_opt.at(0));
        std::shared_ptr<L4_Vertex> l4_ptr_to= std::static_pointer_cast<L4_Vertex>(to->components_opt.at(0));
        std::cout<<"set data start"<<std::endl;
        std::cout << "from ports size: "<<l4_ptr_from->ports->size()<<" and to port size: "<<l4_ptr_to->ports->size()<<std::endl;
        for (std::map<int, Port>::iterator it =  l4_ptr_from->ports->begin(); it != l4_ptr_from->ports->end(); ++it)
        {
            std::cout<<"adding qradiobutton"<<std::endl;
            if ((*it).second.isMaster)
            {
                QRadioButton* radio_ptr = new QRadioButton();
                radio_ptr->setText(QString::number((*it).first));
                bg->addButton(radio_ptr);
                ui->frame_3->layout()->addWidget(radio_ptr);
                buttons.push_back(radio_ptr);
            }
        }
        for (std::map<int, Port>::iterator it =  l4_ptr_to->ports->begin(); it != l4_ptr_to->ports->end(); ++it)
        {
            if (!(*it).second.isMaster)
            {
                std::cout<<"adding qradiobutton to"<<std::endl;
                QRadioButton* radio_ptr = new QRadioButton();
                radio_ptr->setText(QString::number((*it).first));
                bg2->addButton(radio_ptr);
                ui->frame_4->layout()->addWidget(radio_ptr);
                buttons.push_back(radio_ptr);
            }
        }
    }
    else if (from->layer == TASK) //dont need to check the target, they are already checked outside
    {
        std::shared_ptr<L3_Vertex> l3_ptr_to= std::static_pointer_cast<L3_Vertex>(to->components_opt.at(0));
        for (std::map<int, int>::iterator it =  l3_ptr_to->OS_slots->begin(); it != l3_ptr_to->OS_slots->end(); ++it)
        {
            std::cout<<"adding qradiobutton to"<<std::endl;
            QRadioButton* radio_ptr = new QRadioButton();
            radio_ptr->setText(QString::number((*it).first));
            bg2->addButton(radio_ptr);
            ui->frame_4->layout()->addWidget(radio_ptr);
            buttons.push_back(radio_ptr);
        }
    }
    ui->frame_3->update();
    ui->frame_4->update();
    std::cout<<"set data end"<<std::endl;
}
Ports_Popup::~Ports_Popup()
{
    for (std::list<QRadioButton*>::iterator it =  buttons.begin(); it != buttons.end(); ++it)
    {
        delete (*it);
    }
    delete ui;
}
void Ports_Popup::accept()
{
    std::cout<<"checked sourc button: "<<(bg->buttons().size() != 0 ? (bg->checkedButton() != 0?bg->checkedButton()->text().toInt():-1): 0)<<std::endl;
    std::cout<<"checked trgt button: "<<(bg2->buttons().size() != 0 ? (bg2->checkedButton() != 0?bg2->checkedButton()->text().toInt():-1): 0)<<std::endl;
    
    from = bg->buttons().size() != 0 ? (bg->checkedButton() != 0?bg->checkedButton()->text().toInt():-1): 0;
    to = bg2->buttons().size() != 0 ? (bg2->checkedButton() != 0?bg2->checkedButton()->text().toInt():-1): 0;
    if (to == -1 || from == -19)
        QDialog::reject();
    else
        QDialog::accept();
}
void Ports_Popup::reject()
{
    QDialog::reject();
}

std::pair< int, int > Ports_Popup::get_ports()
{
    return std::make_pair(from,to);
}
