#include "tab_popup.h"
#include "ui_tab_popup.h"

#include "l4_popup.h"
#include "l3_popup.h"

Tab_popup::Tab_popup(QDialog* parent): QDialog(parent), ui(new Ui::Tab_popup)
{
    ui->setupUi(this);
    //ui->tabWidget->insertTab(ui->tabWidget->count(), new l4_popup(),QIcon(QString("")),"new tab");
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(add_tab()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(remove_tab()));
    connect(ui->ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->cancel, SIGNAL(clicked()), this, SLOT(reject()));
    previous_tab = 0;
}

Tab_popup::~Tab_popup()
{
    delete ui;
}
void Tab_popup::set_data(Layer l, std::shared_ptr< Logical_Vertex > vtx, std::shared_ptr< std::set< std::string > > names_in, bool isUpdateParam)
{
    std::cout<<"tab_popup, set data start"<<std::endl;
    lay = l;
    std::cout<<"tab_popup, layer set "<<l<<std::endl;
    vtx_ptr = vtx;
    std::cout<<"tab_popup, logical pointer set"<<std::endl;
    
    
    
    //TODO switch
    std::cout <<"before loop: size of the components opt is: "<<vtx_ptr->components_opt.size()<<std::endl;
    switch (lay)
    {
        case FUNCTION:
        {
            ui->label_15->hide();
            ui->pushButton->hide();
            ui->pushButton_2->hide();
            break;
        }
        case TASK:
        {
            ui->label_15->hide();
            ui->pushButton->hide();
            ui->pushButton_2->hide();
            break;
        }
        case CONTROLLER:
        {
            for (int i = 0; i < vtx->components_opt.size(); i++)
            {
                std::string opt_string = "opt: "; 
                ui->tabWidget->insertTab(i, new l3_popup(),QIcon(QString("")),QString::fromStdString(opt_string));
        
                QWidget* wp = ui->tabWidget->widget(i);
                l3_popup *l3_obj = qobject_cast<l3_popup*>(wp);
                std::shared_ptr<L3_Vertex> tmp = std::static_pointer_cast<L3_Vertex>(vtx_ptr->components_opt.at(i));
                l3_obj->set_data(tmp);
            }
            break;
        }
        case RESOURCE:
        {
            for (int i = 0; i < vtx->components_opt.size(); i++)
            {
                std::string opt_string = "opt: "; 
                ui->tabWidget->insertTab(i, new l4_popup(),QIcon(QString("")),QString::fromStdString(opt_string));
        
                QWidget* wp = ui->tabWidget->widget(i);
                l4_popup *l4_obj = qobject_cast<l4_popup*>(wp);
                std::shared_ptr<L4_Vertex> tmp = std::static_pointer_cast<L4_Vertex>(vtx_ptr->components_opt.at(i));
                l4_obj->set_data(tmp);
            }
            break;
        }
        case PHYSICAL:
        {
            //should build here, but is only one i dont want the tabs to be shown so until no more spec are requested is skipped
            // hide option stuff
            ui->label_15->hide();
            ui->pushButton->hide();
            ui->pushButton_2->hide();
            break;
        }
        default:
        {
            throw std::runtime_error("popup called without layer set");
            break;
        }
        
        
        
    }
   
    std::cout<<"tab_popup, data add return name is: "<<(*(vtx_ptr->name))<<std::endl;
    names = names_in;
    isUpdate = isUpdateParam;//notsureisneeded
    ui->lineEdit_2->setText(QString::fromStdString((*(vtx_ptr->name))));
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
            if (ui->tabWidget->count() > 0)
            {
                QWidget* wp = ui->tabWidget->widget(ui->tabWidget->currentIndex());
                l3_popup *l3_obj = qobject_cast<l3_popup*>(wp);
                if (l3_obj->get_data() != nullptr)
                    l3_obj->consolidate_data();
            }    
            ui->tabWidget->insertTab(ui->tabWidget->count(), new l3_popup(),QIcon(QString("")),QString::fromStdString("opt: "));
            std::shared_ptr<L3_Vertex> new_vtx;
            new_vtx.reset(new L3_Vertex());
            
            std::shared_ptr<L3_Vertex> tmp = std::static_pointer_cast<L3_Vertex>(vtx_ptr->components_opt.at(0));
            new_vtx->OS_slots= tmp->OS_slots;
            
            QWidget* wp2 = ui->tabWidget->widget(ui->tabWidget->count()-1);
            previous_tab = ui->tabWidget->count()-1;
            l3_popup* l3_obj2 = qobject_cast<l3_popup*>(wp2);
            l3_obj2->set_data(new_vtx);
            break;
            
            
            
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
            ui->tabWidget->insertTab(ui->tabWidget->count(), new l4_popup(),QIcon(QString("")),QString::fromStdString("opt: "));
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
    std::cout <<"remove_tab: enter"<<std::endl;
    QWidget* tmp = ui->tabWidget->currentWidget();
    std::cout <<"remove_tab: select"<<std::endl;
    int curr_tab_to_rm = ui->tabWidget->currentIndex();
    if (ui->tabWidget->count() > 1)
    {
        ui->tabWidget->setCurrentIndex(curr_tab_to_rm != 0 ? curr_tab_to_rm-1:0);
        std::cout <<"remove_tab: selected tab is: "<<ui->tabWidget->currentIndex()<<std::endl;
        ui->tabWidget->removeTab(curr_tab_to_rm);
        std::cout <<"remove_tab: remove"<<std::endl;
        delete tmp;
        std::cout <<"remove_tab: free"<<std::endl;
    }
}


void Tab_popup::exec()
{
    QDialog::exec();
}
void Tab_popup::accept()
{
    //check the name using the isUpdate method
    QString text = ui->lineEdit_2->text();
    std::string old_name = *(vtx_ptr->name);
    *(vtx_ptr->name) = text.toStdString();
    std::cout << "tab_popup:accept entry point"<<std::endl;
    if ((names->count(*(vtx_ptr->name)) == 0 && *(vtx_ptr->name) != "")&& !isUpdate ||
        isUpdate && *(vtx_ptr->name) != "" && (old_name == *(vtx_ptr->name)||names->count(*(vtx_ptr->name)) == 0))
        //when updating i want to check if im reusing the same name, and its ok or a new name and this new name has to be unique
    {
        //switch out, differentiate between components with and without opts.
        std::cout << "tab_popup:enter if"<<std::endl;
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
                vtx_ptr->components_opt.clear();
                for (int i = 0; i < ui->tabWidget->count(); i++)
                {
                    std::cout<<"assign loop, iteration variable value is: "<<i<<std::endl;
                    //assegna a posizione in vettore il vertice presente nel tab.
                    QWidget* wp = ui->tabWidget->widget(i);
                    l3_popup *l3_obj= qobject_cast<l3_popup*>(wp);
                    l3_obj->consolidate_data();
                    std::cout <<"before insert: size of the components opt is: "<<vtx_ptr->components_opt.size()<<std::endl;
                    vtx_ptr->components_opt.push_back(l3_obj->get_data());
                    std::cout <<"after insert: size of the components opt is: "<<vtx_ptr->components_opt.size()<<std::endl;
                }
                break;
            }
            case RESOURCE:
            {
                vtx_ptr->components_opt.clear();
                for (int i = 0; i < ui->tabWidget->count(); i++)
                {
                    
                    std::cout<<"assign loop, iteration variable value is: "<<i<<std::endl;
                    //assegna a posizione in vettore il vertice presente nel tab.
                    QWidget* wp = ui->tabWidget->widget(i);
                    l4_popup *l4_obj = qobject_cast<l4_popup*>(wp);
                    if (i != ui->tabWidget->currentIndex())
                        l4_obj->update_graphic_from_data();
                    l4_obj->consolidate_data();
                    std::cout <<"before insert: size of the components opt is: "<<vtx_ptr->components_opt.size()<<std::endl;
                    vtx_ptr->components_opt.push_back(l4_obj->get_data());
                    std::cout <<"after insert: size of the components opt is: "<<vtx_ptr->components_opt.size()<<std::endl;
                }
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
        std::cout <<"name accepted, size of the components opt is: "<<vtx_ptr->components_opt.size()<<std::endl;
        if (isUpdate && old_name != *(vtx_ptr->name))
        {
            std::cout << "tab_popup: isUpdate && oldname diverso"<<std::endl;
            names->erase(old_name);
            names->insert(*(vtx_ptr->name));
        }
        std::cout << "tab_popup:accept"<<std::endl;
        QDialog::accept();
    }
    else 
    {
        std::cout <<"tab_popup: refuse"<<std::endl;
        QDialog::reject();
        
    }
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
        case CONTROLLER:
        {
            QWidget* wp = ui->tabWidget->widget(previous_tab);
            l3_popup *l3_obj = qobject_cast<l3_popup*>(wp);
            if (l3_obj->get_data() != nullptr)
                l3_obj->consolidate_data();
            wp = ui->tabWidget->widget(ui->tabWidget->currentIndex());
            l3_obj = qobject_cast< l3_popup* >(wp);
            if (l3_obj->get_data() != nullptr)
                l3_obj->update_graphic_from_data();
            break;   
        }
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
std::shared_ptr< Logical_Vertex > Tab_popup::get_data()
{
    return vtx_ptr;
}
