#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H
#include <QTextEdit>
#include <QMainWindow>
#include <QCommandLineParser>
#include "rocdata.h"
#include "rocdatabase.h"

//class QTextEdit;
class QCustomPlot;
class RocDataBase;

class PlotWindow  : public QMainWindow
{
    Q_OBJECT
public:
    //explicit PlotWindow(QWidget *parent = 0);
    //explicit PlotWindow(QCommandLineParser& clp, QWidget *parent = 0);
    explicit PlotWindow(RocDataBase* rocData, QWidget *parent = 0);
    virtual ~PlotWindow();
signals:

public slots:

private:
    void init();
    void writeToPdf(const QString &fileName);
    void setSecondAxis(QList<double> frequencies);
    void writeRoc(RocDataBase* rocData);

    QTextEdit* textEdit;
    QCustomPlot* customPlot;
    //QCommandLineParser clp;
    //RocData rocData;
    //RocDataBase* rocDataBase;

    QVector<double> ticPositions;
    QVector<QString> ticLabels;

};

#endif // PLOTWINDOW_H
