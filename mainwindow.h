#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "src/vctcontact.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnadd_clicked();
    void on_btnrem_clicked();
    void on_btnedit_clicked();
    void on_btnload_clicked();
    void on_btnsave_clicked();
    void on_btnsearch_clicked();
    void on_btnreset_clicked();
    void onHeaderSectionClicked(int logicalIndex);

private:
    Ui::MainWindow *ui;
    vectorContact contacts;
    vectorContact searchResult;
    bool searchActive = false;

    int  m_lastSortColumn = -1;
    bool m_sortAscending  = true;

    void updatetable();
};
#endif // MAINWINDOW_H
