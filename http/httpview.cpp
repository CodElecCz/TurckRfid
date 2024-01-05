#include "httpview.h"
#include "ui_httpview.h"

HttpView::HttpView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HttpView)
{
    ui->setupUi(this);
}

HttpView::~HttpView()
{
    delete ui;
}

void HttpView::SetTime(const QString& time)
{
    ui->labelTimeText->setText(time);
}

void HttpView::SetTag(const QString& tag)
{
    ui->labelTagText->setText(tag);
}

void HttpView::SetCounter(uint32_t counter)
{
    ui->labelCounterText->setText(QString::number(counter));
}

void HttpView::SetStatus(const QString& status)
{
    ui->labelStatusText->setText(status);
}
