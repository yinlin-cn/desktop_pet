#include "taici.h"
#include "ui_taici.h"
#include<QImage>
#include<QPainter>
#include<QCloseEvent>

taici::taici(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::taici)
{
    ui->setupUi(this);
    setVisible(false);
    setWindowTitle("台词设置");
}

taici::~taici()
{
    delete ui;
}

void taici::init(QString* a,QString* b,QString* c,std::filesystem::path e)
{
    for(int i=0;i<5;i++)
    {
        QLayoutItem *fieldItem = ui->formLayout_4->itemAt(i, QFormLayout::FieldRole);

        // 2. 从布局项中获取QWidget（这里是QLineEdit）
        if (fieldItem && fieldItem->widget()) {
            QLineEdit *passwordEdit = qobject_cast<QLineEdit*>(fieldItem->widget());
            if (passwordEdit) {
                xianzhi[i]=a[i];
                passwordEdit->setText(xianzhi[i]);
            }
        }
    }
    for(int i=0;i<5;i++)
    {
        QLayoutItem *fieldItem = ui->formLayout_2->itemAt(i, QFormLayout::FieldRole);

        // 2. 从布局项中获取QWidget（这里是QLineEdit）
        if (fieldItem && fieldItem->widget()) {
            QLineEdit *passwordEdit = qobject_cast<QLineEdit*>(fieldItem->widget());
            if (passwordEdit) {
                danji[i]=b[i];
                passwordEdit->setText(danji[i]);
            }
        }
    }
    for(int i=0;i<5;i++)
    {
        QLayoutItem *fieldItem = ui->formLayout_3->itemAt(i, QFormLayout::FieldRole);

        // 2. 从布局项中获取QWidget（这里是QLineEdit）
        if (fieldItem && fieldItem->widget()) {
            QLineEdit *passwordEdit = qobject_cast<QLineEdit*>(fieldItem->widget());
            if (passwordEdit) {
                tuodong[i]=c[i];
                passwordEdit->setText(tuodong[i]);
            }
        }
    }
    taicibeijing=e;
    beijing=new QImage(QString::fromStdString(taicibeijing.string()));
    update();
}


void taici::paintEvent(QPaintEvent *event) {
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





void taici::on_pushButton_clicked(bool checked)
{
    for(int i=0;i<5;i++)
    {
        QLayoutItem *fieldItem = ui->formLayout_4->itemAt(i, QFormLayout::FieldRole);

        // 2. 从布局项中获取QWidget（这里是QLineEdit）
        if (fieldItem && fieldItem->widget()) {
            QLineEdit *passwordEdit = qobject_cast<QLineEdit*>(fieldItem->widget());
            if (passwordEdit) {
                xianzhi[i]=passwordEdit->text();
            }
        }
    }
    for(int i=0;i<5;i++)
    {
        QLayoutItem *fieldItem = ui->formLayout_2->itemAt(i, QFormLayout::FieldRole);

        // 2. 从布局项中获取QWidget（这里是QLineEdit）
        if (fieldItem && fieldItem->widget()) {
            QLineEdit *passwordEdit = qobject_cast<QLineEdit*>(fieldItem->widget());
            if (passwordEdit) {
                danji[i]=passwordEdit->text();
            }
        }
    }
    for(int i=0;i<5;i++)
    {
        QLayoutItem *fieldItem = ui->formLayout_3->itemAt(i, QFormLayout::FieldRole);

        // 2. 从布局项中获取QWidget（这里是QLineEdit）
        if (fieldItem && fieldItem->widget()) {
            QLineEdit *passwordEdit = qobject_cast<QLineEdit*>(fieldItem->widget());
            if (passwordEdit) {
                tuodong[i]=passwordEdit->text();
            }
        }
    }
    emit taicichuandi(xianzhi,danji,tuodong);
}

void taici::closeEvent(QCloseEvent *event)
{
    // 1. 隐藏窗口（替代默认的销毁行为）
    this->hide();

    // 2. 忽略关闭事件（告诉Qt：我们自己处理了，不要销毁窗口）
    event->ignore();
}

