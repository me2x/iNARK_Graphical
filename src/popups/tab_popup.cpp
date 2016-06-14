#include "tab_popup.h"
#include "ui_tab_popup.h"

#include "l4_popup.h"

Tab_popup::Tab_popup(QDialog* parent): QDialog(parent), ui(new Ui::Tab_popup)
{
    ui->setupUi(this);
    //ui->tabWidget->insertTab(ui->tabWidget->count(), new l4_popup(),QIcon(QString("")),"new tab");
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(add_tab()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(remove_tab()));
    previous_tab = 0;
}

Tab_popup::~Tab_popup()
{
    delete ui;
}
void Tab_popup::set_data(Layer l, std::shared_ptr< Logical_Vertex > vtx, std::shared_ptr< std::set< std::string > > names_in)
{
    std::cout<<"tab_popup, set data start"<<std::endl;
    lay = l;
    std::cout<<"tab_popup, layer set "<<std::endl;
    vtx_ptr = vtx;
    std::cout<<"tab_popup, logical pointer set"<<std::endl;
    add_tab();
    std::cout<<"tab_popup, data add return"<<std::endl;
    names = names_in;
}

void Tab_popup::add_tab()
{
    switch (lay)
    {
        case FUNCTION:
        {
            //ui->tabWidget->insertTab(ui->tabWidget->count(), new l1_popup(),QIcon(QString("")),"new tab");
            //set_data
            break;
        }
        case TASK:
        {
            //ui->tabWidget->insertTab(ui->tabWidget->count(), new l2_popup(),QIcon(QString("")),"new tab");
            break;
        }
        case CONTROLLER:
        {
            //ui->tabWidget->insertTab(ui->tabWidget->count(), new l3_popup(),QIcon(QString("")),"new tab");
            break;
        }
        case RESOURCE:
        {
            if (ui->tabWidget->count() > 0)
            {
                QWidget* wp = ui->tabWidget->widget(ui->tabWidget->currentIndex());
                l4_popup *l4_obj = qobject_cast<l4_popup*>(wp);
                if (l4_obj->get_data() != nullptr)
                    l4_obj->consolidate_data();
            }    
            ui->tabWidget->insertTab(ui->tabWidget->count(), new l4_popup(),QIcon(QString("")),"new tab");
            std::shared_ptr<L4_Vertex> new_vtx;
            new_vtx.reset(new L4_Vertex());
            
            std::shared_ptr<L4_Vertex> tmp = std::static_pointer_cast<L4_Vertex>(vtx_ptr->components_opt.at(0));
            new_vtx->ports = tmp->ports;
            
            QWidget* wp2 = ui->tabWidget->widget(ui->tabWidget->count()-1);
            previous_tab = ui->tabWidget->count()-1;
            l4_popup* l4_obj2 = qobject_cast<l4_popup*>(wp2);
            l4_obj2->set_data(new_vtx);
            break;
        }
        case PHYSICAL:
        {
            //ui->tabWidget->insertTab(ui->tabWidget->count(), new l5_popup(),QIcon(QString("")),"new tab");
            break;
        }
        default:
        {
            throw std::runtime_error("popup called without layer set");
            break;
        }
    }
    
    
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}

void Tab_popup::remove_tab()
{
    //geets the pointer and deletes the widget. in this way all the shared pointers
    //inside the widget should be updated
    QWidget* tmp = ui->tabWidget->currentWidget();
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
    delete tmp;
}


void Tab_popup::exec()
{
    QDialog::exec();
}
void Tab_popup::accept()
{
    QDialog::accept();
}
void Tab_popup::reject()
{
    QDialog::reject();
}

void Tab_popup::tabSelected(){
    //save data from the popup tablewidget and report them in the pointed object.
    //then proceed with updating of the tab1
    //then show the tab
    //use ui->tabWidget->currentIndex() to get the index of the new one and
    //create an integer in order to trace the previous tab open.
    std::cout << "tabpopup tab selected event"<< ui->tabWidget->currentIndex()<<"and prebious is" << previous_tab <<std::endl;
    switch (lay)
    {
        case RESOURCE:
        {
            QWidget* wp = ui->tabWidget->widget(previous_tab);
            l4_popup *l4_obj = qobject_cast<l4_popup*>(wp);
            if (l4_obj->get_data() != nullptr)
                l4_obj->consolidate_data();
            wp = ui->tabWidget->widget(ui->tabWidget->currentIndex());
            l4_obj = qobject_cast< l4_popup* >(wp);
            if (l4_obj->get_data() != nullptr)
                l4_obj->update_graphic_from_data();
            break;   
        }
        default:
        {
            std::cout<<"other layer tabselected"<<std::endl;
            break;
        }
    }
    previous_tab = ui->tabWidget->currentIndex();
}