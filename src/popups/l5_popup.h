#ifndef L5_POPUP_H
#define L5_POPUP_H

#include <memory>
#include <string>
#include <set>
#include <QDialog>
#include "../logical_vertex.h"

namespace Ui
{
class l5_popup;
}

class l5_popup : public QDialog
{
    Q_OBJECT
public:
    explicit l5_popup(QDialog* parent = 0);
    ~l5_popup();
    void exec();
    std::shared_ptr<Logical_Vertex> get_data();
    void set_data(std::shared_ptr<Logical_Vertex> data_in);
    void set_names (std::shared_ptr<std::set <std::string> > names);
public slots:
    void accept();
    void reject();
private:
    Ui::l5_popup* ui;
    std::shared_ptr<Logical_Vertex> data;
    std::shared_ptr<std::set<std::string> > names;
};

#endif // L5_POPUP_H
