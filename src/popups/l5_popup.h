#ifndef L5_POPUP_H
#define L5_POPUP_H

#include <memory>
#include <string>
#include <QWidget>
#include "../logical_vertex.h"

namespace Ui
{
class l5_popup;
}

class l5_popup : public QWidget
{
    Q_OBJECT
public:
    explicit l5_popup(QWidget* parent = 0);
    ~l5_popup();
    std::shared_ptr<L5_Vertex> get_data();
    void set_data(std::shared_ptr<L5_Vertex> data_in);
    // STUB only, never used as now since opt has no meaning (ui is empty here)
    void consolidate_data();
    void update_graphic_from_data();

private:
    Ui::l5_popup* ui;
    std::shared_ptr<L5_Vertex> data;
};

#endif // L5_POPUP_H
