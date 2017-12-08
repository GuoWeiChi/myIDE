#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>//QT已经帮我们写好了一个控件
#include <QMenu>
#include <QMenuBar>
#include <QAction>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QString filename;
    QTextEdit *text1;
    QMenu *file;
    QMenu *edit;
    QMenu *help;
    QMenu *build;

    QAction *build_compile;
    QAction *build_run;

    QAction *file_open;
    QAction *file_exit;
    QAction *help_about;
    QAction *edit_copy;
    QAction *edit_paste;
    QAction *edit_cut;
    QAction *edit_select_all;
    QAction *file_save;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_open();
    void on_about();
    void on_exit();
    void on_copy();
    void on_paste();
    void on_cut();
    void on_select_all();
    void on_file_save();
    void on_build_compile();
    void on_build_run();
};

#endif // MAINWINDOW_H
