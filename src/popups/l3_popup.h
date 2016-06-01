#ifndef L3_POPUP_H
#define L3_POPUP_H

#include "../logical_vertex.h"
#include <QDialog>
#include <memory>
#include <set>

namespace Ui
{
class l3_popup;
}

class l3_popup: public QDialog
{
    Q_OBJECT
public:
    explicit l3_popup(QDialog* parent = 0);
    ~l3_popup();
    void exec();
    std::shared_ptr<Logical_Vertex> get_data();
    void set_data(std::shared_ptr<Logical_Vertex> data_in);
    void set_names (std::shared_ptr<std::set <std::string> > names);
public slots:
    void accept();
    void reject();
private:
    Ui::l3_popup* ui;
    std::shared_ptr<Logical_Vertex> data;
    std::shared_ptr<std::set<std::string> > names;
};



#endif // L2_POPUP_H



