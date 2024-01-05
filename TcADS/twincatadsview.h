#ifndef TWINCATADSVIEW_H
#define TWINCATADSVIEW_H

#include <QWidget>

namespace Ui {
class TwinCatAdsView;
}

class TwinCatAdsView : public QWidget
{
    Q_OBJECT

public:
    explicit TwinCatAdsView(QWidget *parent = nullptr);
    ~TwinCatAdsView();

private:
    Ui::TwinCatAdsView *ui;
};

#endif // TWINCATADSVIEW_H
