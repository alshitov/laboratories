#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

#include <QtCore>
#include <QtWidgets>

class ResultsWindow : public QListWidget
{
    Q_OBJECT

public:
    ResultsWindow();

signals:
    void close_signal();

public slots:
    void receive_res(std::string&);

private:
    void add_entry(std::string& result);

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // RESULTSWINDOW_H
