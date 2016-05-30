#ifndef L1_POPUP_H
#define L1_POPUP_H

#include "../logical_vertex.h"
#include <QDialog>
#include <memory>
#include <set>
namespace Ui
{
class L1_popup;
}

class L1_popup : public QDialog
{
    Q_OBJECT
public:
    explicit L1_popup(QDialog* parent = 0);
    ~L1_popup();
    void exec();
    std::shared_ptr<Logical_Vertex> get_data();
    void set_data(std::shared_ptr<Logical_Vertex> data_in);
    void set_names (std::shared_ptr<std::set <std::string> > names);
public slots:
    void accept();
    void reject();
private:
    Ui::L1_popup* ui;
    std::shared_ptr<Logical_Vertex> data;
    std::shared_ptr<std::set<std::string> > names;
};

#endif // L1_POPUP_H
