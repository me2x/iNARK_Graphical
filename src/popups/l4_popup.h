#ifndef L4_POPUP_H
#define L4_POPUP_H

#include "../logical_vertex.h"
#include <QWidget>
#include <memory>
#include <set>

namespace Ui
{
class l4_popup;
}

class l4_popup : public QWidget
{
    Q_OBJECT
public:
    explicit l4_popup(QWidget* parent = 0);
    ~l4_popup();
    std::shared_ptr<L4_Vertex> get_data();
    void set_data(std::shared_ptr<L4_Vertex> data_in); 
    void consolidate_data();
    void update_graphic_from_data();
    
public slots:
    void add_table_line();
    void remove_table_line();
private:
    Ui::l4_popup* ui;
    std::shared_ptr<L4_Vertex> data;
};

#endif // L4_POPUP_H
