#pragma once

#include <QListView>
#include <QKeyEvent>
#include <QTreeView>

class ListView : public QListView {

protected:
    void keyPressEvent(QKeyEvent *e) override;

public:
    ListView(QWidget *parent = nullptr);
    ~ListView();

};
