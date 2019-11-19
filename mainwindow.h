#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "point.h"
#include "neurona.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_CargarArchivo_clicked();

    void on_SOM_clicked();

    void on_cruz_clicked();

    void on_estrella_clicked();


    void on_euclidiana_clicked();

    void on_hamming_clicked();

    void on_limpiar_clicked();

    void on_SOM_PROBAR_clicked();

private:
    Ui::MainWindow *ui;
    QVector<QColor> myColors;
    QVector<Point> pointVector;
    QVector<Neurona> malla;
    bool tipoVecindad = true;
    bool tipoDistancia = true;
    int dimensionDelProblema;

    void crearGraficas();
    void inicializaMalla();
    QVector<double> neuronaGanadora(QVector<double> x);
    QVector<double> peorNeurona(QVector<double> x);
    QVector<int> vecindario(int pos);
    void actualizaPesos(QVector<int> v, int i);

    void pintarCalor(QVector<double> x);
    void pintarError(QVector<double> dist, QVector<double> iter);

    void clear();

    void sleep(int msec);
};

#endif // MAINWINDOW_H
