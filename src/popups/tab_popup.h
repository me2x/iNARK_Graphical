#ifndef TAB_POPUP_H
#define TAB_POPUP_H

#include <QDialog>
#include <common.h>
#include "../logical_vertex.h"
#include <string>
#include <set>
#include <memory>
namespace Ui
{
class Tab_popup;
}

class Tab_popup : public QDialog
{
    Q_OBJECT
public:
    explicit Tab_popup(QDialog* parent = 0);
    ~Tab_popup();
    void exec();
    void set_data(Layer l, std::shared_ptr<Logical_Vertex> vtx, std::shared_ptr<std::set <std::string> > names,bool isUpdateParam);
    std::shared_ptr<Logical_Vertex> get_data();
public slots:
    void accept();
    void reject();
private slots:
    void add_tab();
    void remove_tab();
    void tabSelected();
private:
    Ui::Tab_popup* ui;
    Layer lay;
    std::shared_ptr<Logical_Vertex> vtx_ptr;
    std::shared_ptr<Vertex_Virtual> virtual_vtx_ptr;
    std::shared_ptr<std::set <std::string> > names;
    int previous_tab;
    bool isUpdate;
};

#endif // TAB_POPUP_H
