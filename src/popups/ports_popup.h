#ifndef PORTS_POPUP_H
#define PORTS_POPUP_H

#include <qt4/QtGui/QDialog>
#include <QRadioButton>
#include "../logical_vertex.h"
#include <memory>

namespace Ui
{
class Ports_Popup;
}

class Ports_Popup : public QDialog
{
    Q_OBJECT
public:        
    explicit Ports_Popup(QDialog* parent = 0);
    ~Ports_Popup();
    void exec();
    void set_data(std::shared_ptr<Logical_Vertex> from, std::shared_ptr<Logical_Vertex> to);
    std::pair<int,int> get_ports();
public slots:
    void accept();
    void reject();
private:
    Ui::Ports_Popup* ui;
    std::shared_ptr<Logical_Vertex> source;
    std::shared_ptr<Logical_Vertex> target;
    int from, to;
    QButtonGroup* bg;
    QButtonGroup* bg2;
    std::list<QRadioButton*> buttons;
};

#endif // PORTS_POPUP_H
