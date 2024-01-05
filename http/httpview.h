#ifndef HTTPVIEW_H
#define HTTPVIEW_H

#include <QWidget>

namespace Ui {
class HttpView;
}

class HttpView : public QWidget
{
    Q_OBJECT

public:
    explicit HttpView(QWidget *parent = nullptr);
    ~HttpView();

    void SetTime(const QString& time);
    void SetTag(const QString& tag);
    void SetCounter(uint32_t counter);
    void SetStatus(const QString& status);

private:
    Ui::HttpView *ui;
};

#endif // HTTPVIEW_H
