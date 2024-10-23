#include "ListView.hpp"

ListView::ListView(QWidget *parent)
: QListView(parent) {
}

ListView::~ListView()
{}

void ListView::keyPressEvent(QKeyEvent *e) {
    e->ignore();
}
