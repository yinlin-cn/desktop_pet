#include "form.h"
#include "ui_form.h"
#include<filesystem>
#include<qscreen.h>
#include<QCloseEvent>
#include<QDir>
#include <QFileDialog>
#include<QString>
#include<QPainter>
Form::Form(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);
    setVisible(false);
    setWindowFlags(
        Qt::Window);
    centerOnScreen();
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_clicked()));
    setWindowTitle("设置");
};
Form::~Form()
{
    delete ui;
}

/*void Form::duqvshujv()
{
    std::ifstream ss(mulu);
    bool a,b,c;
    ss>>a>>b>>c;
    ailimove1=a;
    playwav1=b;
    ifshow1=c;
    qDebug()<<ailimove1<<playwav1<<ifshow1;
}*/

 void Form::init()
{
    qDebug()<<ailimove1<<playwav1<<ifshow1;
    qDebug() << "进入init()，准备设置复选框";
    ui->checkBox->setChecked(ailimove1);
    ui->checkBox_2->setChecked(ifshow1);
    ui->checkBox_3->setChecked(playwav1);
    ui->checkBox_4->setChecked(xianshibofangqi);
    ui->checkBox_8->setChecked(useownwenjianjia);
    switch(bofangmoshi)
    {
    case 0:{ui->checkBox_5->setChecked(true);break;}
    case 1:{ui->checkBox_6->setChecked(true);break;}
    case 3:{ui->checkBox_7->setChecked(true);break;}
    }
    std::string a=zidingwenjianjiadizhi.string();
    QString qStr = QString::fromUtf8(a.c_str());
    ui->lineEdit->setText(qStr);
}

void Form::centerOnScreen()
{
    // 获取屏幕的几何信息（支持多屏幕，取主屏幕）
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    // 计算窗口居中时的坐标
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;

    this->move(x, y);  // 移动到屏幕中心
}

void Form::on_pushButton_clicked()
{
    if(ui->checkBox->isChecked())
        ailimove1=true;
    else
        ailimove1=false;
    if(ui->checkBox_2->isChecked())
        ifshow1=true;
    else
        ifshow1=false;
    if(ui->checkBox_3->isChecked())
        playwav1=true;
    else
        playwav1=false;
    if(ui->checkBox_4->isChecked())
        xianshibofangqi=true;
    else
        xianshibofangqi=false;
    if(ui->checkBox_8->isChecked())
        useownwenjianjia=true;
    else
        useownwenjianjia=false;
    if(ui->checkBox_5->isChecked())
        bofangmoshi=0;
    if(ui->checkBox_6->isChecked())
        bofangmoshi=1;
    if(ui->checkBox_7->isChecked())
        bofangmoshi=3;
    QString lujing=ui->lineEdit->text();
    std::string lujing1=lujing.toStdString();
    std::filesystem::path cun(lujing1);
    zidingwenjianjiadizhi=cun;
    emit shezhisignal(ailimove1,playwav1,ifshow1,xianshibofangqi,useownwenjianjia,bofangmoshi,zidingwenjianjiadizhi);
    hide();
}

void Form::shezhislot(bool a,bool b,bool c,bool d,bool e,int f,std::filesystem::path g,std::filesystem::path h)
{
    ailimove1=a;
    ifshow1=b;
    playwav1=c;
    useownwenjianjia=e;
    xianshibofangqi=d;
    bofangmoshi=f;
    zidingwenjianjiadizhi=g;
    tupianlujing=h;
    QString qlujing=QString::fromStdString(tupianlujing.string());
    beijing=new QImage(qlujing);
}

void Form::closeEvent(QCloseEvent *event)
{
    // 1. 隐藏窗口（替代默认的销毁行为）
    this->hide();

    // 2. 忽略关闭事件（告诉Qt：我们自己处理了，不要销毁窗口）
    event->ignore();
}

void Form::on_checkBox_5_clicked(bool checked)
{
    if(bofangmoshi==0)
    {
        ui->checkBox_5->setChecked(true);
        return;
    }
    bofangmoshi=0;
    ui->checkBox_6->setChecked(false);
    ui->checkBox_7->setChecked(false);
    ui->checkBox_5->setChecked(true);
}


void Form::on_checkBox_6_clicked(bool checked)
{
    if(bofangmoshi==1)
    {
        ui->checkBox_6->setChecked(true);
        return;
    }
    bofangmoshi=1;
    ui->checkBox_5->setChecked(false);
    ui->checkBox_7->setChecked(false);
    ui->checkBox_6->setChecked(true);
}


void Form::on_checkBox_7_clicked(bool checked)
{
    if(bofangmoshi==3)
    {
        ui->checkBox_7->setChecked(true);
        return;
    }
    bofangmoshi=3;
    ui->checkBox_5->setChecked(false);
    ui->checkBox_6->setChecked(false);
    ui->checkBox_7->setChecked(true);
}


void Form::on_pushButton_2_clicked(bool checked)
{
    QString dirPath = QFileDialog::getExistingDirectory(
        this,                  // 父窗口（对话框模态显示在该窗口上）
        tr("选择目标文件夹"),   // 对话框标题
        QDir::currentPath(),   // 初始目录（这里用程序当前运行目录，可自定义）
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        // 选项：只显示文件夹，不解析符号链接（避免跳转）
        );

    // 判断用户是否选择了文件夹（未选择则dirPath为空）
    if (!dirPath.isEmpty()) {
        qDebug() << "选中的文件夹路径：" << dirPath;
        zidingwenjianjiadizhi=dirPath.toStdString();
        ui->lineEdit->setText(dirPath);
    } else {
        qDebug() << "用户取消了选择";
        return;
    }
}


void Form::paintEvent(QPaintEvent *event) {
    // 先调用父类的paintEvent，确保其他绘制正常
    QWidget::paintEvent(event);

    // 缩放图片（按窗口当前大小，保持比例扩展）
    QImage scaledPix = beijing->scaled(
        this->size(),  // 窗口当前大小
        Qt::KeepAspectRatioByExpanding,
        Qt::SmoothTransformation
        );

    // 绘制缩放后的图片，居中对齐（截取中间部分）
    QPainter painter(this);
    // 计算绘制位置：居中显示（图片可能比窗口大，只画中间区域）
    int x = (this->width() - scaledPix.width()) / 2;
    int y = (this->height() - scaledPix.height()) / 2;
    painter.drawImage(x, y, scaledPix);
}

void Form::on_pushButton_3_clicked(bool checked)
{
    emit xianshi();
}

