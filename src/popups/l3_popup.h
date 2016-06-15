#ifndef L3_POPUP_H
#define L3_POPUP_H

#include "../logical_vertex.h"
#include <QWidget>
#include <memory>
#include <set>

namespace Ui
{
class l3_popup;
}

class l3_popup: public QWidget
{
    Q_OBJECT
public:
    explicit l3_popup(QWidget* parent = 0);
    ~l3_popup();
    std::shared_ptr<L3_Vertex> get_data();
    void set_data(std::shared_ptr<L3_Vertex> data_in);
    void consolidate_data();
    void update_graphic_from_data();

public slots:
    void add_table_line();
    void remove_table_line();
private:
    Ui::l3_popup* ui;
    std::shared_ptr<L3_Vertex> data;
};



#endif // L2_POPUP_H



