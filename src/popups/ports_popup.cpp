#include "ports_popup.h"
#include "ui_ports_popup.h"
#include <iostream>
#include <QButtonGroup>
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
 /*   source = from;
    target = to;
    std::cout<<"set data start"<<std::endl;
    std::cout << "from ports size: "<<from->ports.size()<<" and to port size: "<<to->ports.size()<<std::endl;
    for (std::list<Port*>::iterator it =  source->ports.begin(); it != source->ports.end(); ++it)
    {
        std::cout<<"adding qradiobutton"<<std::endl;
        QRadioButton* radio_ptr = new QRadioButton();
        radio_ptr->setText(QString::number((*it)->id));
        bg->addButton(radio_ptr);
        ui->frame->layout()->addWidget(radio_ptr);
        buttons.push_back(radio_ptr);
        
    }
        for (std::list<Port*>::iterator it =  target->ports.begin(); it != target->ports.end(); ++it)
    {
        std::cout<<"adding qradiobutton to"<<std::endl;
        QRadioButton* radio_ptr = new QRadioButton();
        radio_ptr->setText(QString::number((*it)->id));
        bg2->addButton(radio_ptr);
        ui->frame_2->layout()->addWidget(radio_ptr);
        buttons.push_back(radio_ptr);
    }
    ui->frame->update();
    std::cout<<"set data end"<<std::endl;
    */
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
