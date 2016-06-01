#ifndef L4_POPUP_H
#define L4_POPUP_H

#include "../logical_vertex.h"
#include <QDialog>
#include <memory>
#include <set>

namespace Ui
{
class l4_popup;
}

class l4_popup : public QDialog
{
    Q_OBJECT
public:
    explicit l4_popup(QDialog* parent = 0);
    ~l4_popup();
    void exec();
    std::shared_ptr<Logical_Vertex> get_data();
    void set_data(std::shared_ptr<Logical_Vertex> data_in);
    void set_names (std::shared_ptr<std::set <std::string> > names);
public slots:
    void accept();
    void reject();
    void add_table_line();
    void remove_table_line();
private:
    Ui::l4_popup* ui;
    std::shared_ptr<Logical_Vertex> data;
    std::shared_ptr<std::set<std::string> > names;
};

#endif // L4_POPUP_H
