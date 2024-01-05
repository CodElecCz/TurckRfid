#include "twincatadsview.h"
#include "ui_twincatadsview.h"

TwinCatAdsView::TwinCatAdsView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TwinCatAdsView)
{
    ui->setupUi(this);
}

TwinCatAdsView::~TwinCatAdsView()
{
    delete ui;
}
