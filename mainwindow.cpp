#include "mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    text1 = new QTextEdit;
    QFont f;
    f.setPixelSize(30);
    text1->setFont(f);//设置text1字体大小为30
    QColor c;
    c.setRgb(255,0,0);
    text1->setTextColor(c);//设置颜色为红色
    QColor c2;
    c2.setRgb(0,0,0);
    text1->setTextBackgroundColor(c2);
    this->setCentralWidget(text1);//将这个TextEdit放到对话框的中间


    file=this->menuBar()->addMenu("文件");//在菜单栏加入一个菜单项
    edit=this->menuBar()->addMenu("编译");
    build = this->menuBar()->addMenu("构建");
    help=this->menuBar()->addMenu("帮助");

    file_open = new QAction("打开",this);//建立一个action
    file_open->setShortcut(tr("Ctrl+o"));
    file_exit = new QAction("退出",this);

    file->addAction(file_open);//将file_open这个ACtion加入到file菜单下面
    file->addSeparator();

    file->addAction(file_exit);
    file_exit->setShortcut(tr("Ctrl+z"));

    help_about=new QAction("关于",this);
    help->addAction(help_about);
    help_about->setShortcut(tr("Ctrl+h"));

    edit_copy = new QAction("拷贝",this);
    edit->addAction(edit_copy);
    edit_copy->setShortcut(tr("Ctrl+c"));

    edit_paste=new QAction("粘贴",this);
    edit->addAction(edit_paste);

    edit_cut=new QAction("剪切",this);
    edit->addAction(edit_cut);

    edit_select_all=new QAction("全选",this);
    edit->addAction(edit_select_all);

    file_save = new QAction("保存",this);
    file_save->setShortcut(tr("Ctrl+s"));
    file->addAction(file_save);

    build_compile = new QAction("编译",this);
    build->addAction(build_compile);

    build_run = new QAction("运行",this);
    build->addAction(build_run);

    //QT消息槽机制
    connect(file_open,SIGNAL(triggered()),this,SLOT(on_open()));//第一个参数是触发这个事件的控件
                                                                //第二个参数对于ACTION,固定写SIGNAL(triggered)
                                                                //第三个参数的固定写this                                                              //第四个参数指定点击Action后会执行哪个函数
    connect(help_about,SIGNAL(triggered()),this,SLOT(on_about()));
    connect(file_exit,SIGNAL(triggered()),this,SLOT(on_exit()));
    connect(edit_paste,SIGNAL(triggered()),this,SLOT(on_paste()));
    connect(edit_cut,SIGNAL(triggered()),this,SLOT(on_cut()));
    connect(edit_select_all,SIGNAL(triggered()),this,SLOT(on_select_all()));
    connect(file_save,SIGNAL(triggered()),this,SLOT(on_file_save()));
    connect(build_compile,SIGNAL(triggered()),this,SLOT(on_build_compile()));
    connect(build_run,SIGNAL(triggered()),this,SLOT(on_build_run()));
}


MainWindow::~MainWindow()
{
    delete text1;
}

void MainWindow::on_open()
{
    filename = QFileDialog::getOpenFileName();//打开一个标准对话框,返回值是用户选择的文件信息
    //函数返回用户选择的路径+文件名
    if(filename.isEmpty())
        return;
    QString content;
    //filename.toStdString().data();//将QSting转化为const char *
    FILE *p = fopen(filename.toStdString().data(),"r");
    if(p==NULL)//如果打开失败,那么提示用户
    {
        QMessageBox::information(this,"提示","文件打开出错");
    }
    else
    {
        char buf[1024] = {0};
        while(!feof(p))
        {
            memset(buf,0,sizeof(buf));
            fgets(buf,sizeof(buf),p);
            content += buf;//将buf的内容追加到content后面
        }
        fclose(p);
        text1->setText(content);//将QString里的字符串放入text1里面
    }
}

void MainWindow::on_about()
{
    QMessageBox::information(this,"关于","池国维:版权所有\n2014-2100");
}

void MainWindow::on_exit()
{
    exit(0);
}

void MainWindow::on_copy()
{
    text1->copy();
}

void MainWindow::on_paste()
{
    text1->paste();
}

void MainWindow::on_cut()
{
    text1->cut();
}

void MainWindow::on_select_all()
{
    text1->selectAll();
}

void MainWindow::on_file_save()
{
    filename = QFileDialog::getSaveFileName();
     if(filename.isEmpty())
     {
         return;
     }

     FILE *p=fopen(filename.toStdString().data(),"w");
     if(p==NULL)
     {
         QMessageBox::information(this,"错误","打开文件失败");
     }
     else
     {
         //text1->toPlainText().toStdString().data();//将用户在text1控件中输入的信息转化为char*

         fputs(text1->toPlainText().toStdString().data(),p);

     }
     fclose(p);
}


void MainWindow::on_build_compile()
{
    QString content = "#include <stdio.h>\n";
    FILE *p = fopen(filename.toStdString().data(),"r");
    if(p==NULL)
        return;
    while(!feof(p))
    {
        char buf[1024] = {0};
        fgets(buf,sizeof(buf),p);
        content += buf;
    }
    fclose(p);

    content.replace("整数","int").replace("主函数","main").replace("开始","{").replace("打印","printf(\"hello world\")").replace("返回","return").replace("结束","}").replace("等待","getchar()");

    QString destname = filename;

    destname.replace(".e",".c");
    p=fopen(destname.toStdString().data(),"w");
    fputs(content.toStdString().data(),p);
    fclose(p);

    //如果是一个新的文件,要提示用户是否把页面保存
    if(filename==NULL)
    {
        if(QMessageBox::information(NULL,"提示","文件还未保存,是否保存",QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            on_file_save();
        }
        return;
    }
    else
    {
        //编译前先把文件保存
        FILE *p=fopen(filename.toStdString().data(),"w");
        if(p==NULL)
        {
            QMessageBox::information(this,"错误","文件保存失败");
            return;
        }
        else
        {
            //text1->toPlainText().toStdString().data();//将用户在text1控件中输入的信息转化为char*

            fputs(text1->toPlainText().toStdString().data(),p);

        }
        fclose(p);
    }



    QString srcname = destname;
    srcname.replace(".c",".exe");
    QString command = "gcc -o "+srcname+" "+destname;
    system(command.toStdString().data());
}

void MainWindow::on_build_run()
{
    QString destname=filename;
    destname.replace(".e",".exe");//将filename中的.c替换为.exe
    system(destname.toStdString().data());
}
