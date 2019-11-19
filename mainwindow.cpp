#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->cruz->setChecked(true);
    ui->euclidiana->setChecked(true);
    ui->customPlot->xAxis->setTicks(false);
    ui->customPlot->yAxis->setTicks(false);
    myColors = {QColor(0,87,255),QColor(0,184,255),QColor(0,249,255),
                  QColor(0,255,172),QColor(0,255,64),QColor(75,255,0),
                  QColor(255,254,0),QColor(255,154,0),
                QColor(255,104,0),QColor(255,0,0)};
    ui->inputMalla->setText("14");
    ui->inputF->setText("0.7");
    ui->inputV->setText("2");
    ui->inputG->setText("1000");

    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    ui->errorPlot->setBackground(QBrush(gradient));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_CargarArchivo_clicked()
{
    pointVector.clear();
    QString filename =  QFileDialog::getOpenFileName(
          this,
          "Open Document",
          QDir::currentPath(),
          "All files (*.*) ;; Document files (*.doc *.rtf);; PNG files (*.png)");

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    }

    QStringList wordList;
    QString line;
    while (!file.atEnd()) {
        line = file.readLine();
        wordList = line.split('\t');

        Point dat = Point(wordList);
        pointVector.append(dat);

    }
    dimensionDelProblema = wordList.length() - 1;
    qDebug() << "Archivo Cargado";
}

void MainWindow::on_SOM_clicked()
{
    crearGraficas();
    inicializaMalla();

    int G = ui->inputG->text().toInt();

    for(int i = 0; i < pointVector.size(); i++)
    {
        for(int g = 0; g < G; g++)
        {
            QVector<double> ganador = neuronaGanadora(pointVector[i].Data);
            QVector<int> neighborhood = vecindario(ganador[0]);
            actualizaPesos(neighborhood,i);
        }
        pintarCalor(pointVector[i].Data);
        ui->clase->setText(QString::number(i) + "     Clase " + QString::number(pointVector[i].Class));
    }
}


void MainWindow::on_cruz_clicked()
{
    if(tipoVecindad)
    {
        ui->cruz->setChecked(false);
        tipoVecindad = false;
        ui->estrella->setChecked(true);
    }
    else
    {
        ui->cruz->setChecked(true);
        tipoVecindad = true;
        ui->estrella->setChecked(false);
    }
}

void MainWindow::on_estrella_clicked()
{
    if(tipoVecindad)
    {
        ui->cruz->setChecked(false);
        tipoVecindad = false;
        ui->estrella->setChecked(true);
    }
    else
    {
        ui->cruz->setChecked(true);
        tipoVecindad = true;
        ui->estrella->setChecked(false);
    }
}

void MainWindow::on_euclidiana_clicked()
{
    if(tipoDistancia)
    {
        ui->euclidiana->setChecked(false);
        tipoDistancia = false;
        ui->hamming->setChecked(true);
    }
    else
    {
        ui->euclidiana->setChecked(true);
        tipoDistancia = true;
        ui->hamming->setChecked(false);
    }
}

void MainWindow::on_hamming_clicked()
{
    if(tipoDistancia)
    {
        ui->euclidiana->setChecked(false);
        tipoDistancia = false;
        ui->hamming->setChecked(true);
    }
    else
    {
        ui->euclidiana->setChecked(true);
        tipoDistancia = true;
        ui->hamming->setChecked(false);
    }
}

void MainWindow::crearGraficas()
{
    int size = ui->inputMalla->text().toInt();
    ui->customPlot->xAxis->setRange(0, size+1);
    ui->customPlot->yAxis->setRange(0, size+1);
    int x = 1;
    int y = 1;
    QPen myPen;

    int tam = 60 - ((size - 14)*2);
    for(int i = 0; i < size*size; i++)
    {
        QVector<double> x_,y_;
        x_.append(x);
        y_.append(y);
        myPen.setColor(myColors[unsigned(0)]);
        ui->customPlot->addGraph();
        ui->customPlot->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,tam));
        ui->customPlot->graph(i)->setData(x_,y_);
        ui->customPlot->graph(i)->setPen(myPen);
        x++;
        if(x > size)
        {
            x = 1;
            y++;
        }
    }
    ui->customPlot->replot();
    ui->customPlot->update();
}

void MainWindow::inicializaMalla()
{
    int size = ui->inputMalla->text().toInt();
    for(int i = 0; i < size*size; i++)
    {
        Neurona ne = Neurona(dimensionDelProblema);
        qDebug() << ne.weights;
        malla.append(ne);
    }
    qDebug() << malla.size();
}

QVector<double> MainWindow::neuronaGanadora(QVector<double> x)
{
    int pos = 0;
    double distancia = 99999999999;
    QVector<double> result;
    for(int i = 0; i < malla.size(); i++)
    {
        if(tipoDistancia)
        {
            if(malla[i].distanciaEuclidiana(x) < distancia)
            {
                distancia = malla[i].distanciaEuclidiana(x);
                pos = i;
            }
        }
        else
        {
            if(malla[i].distanciaHamming(x) < distancia)
            {
                distancia = malla[i].distanciaHamming(x);
                pos = i;
            }
        }
    }
    result.append(pos);
    result.append(distancia);
    return result;
}

QVector<double> MainWindow::peorNeurona(QVector<double> x)
{
    int pos = 0;
    double distancia = -1;
    QVector<double> result;
    for(int i = 0; i < malla.size(); i++)
    {
        if(tipoDistancia)
        {
            if(malla[i].distanciaEuclidiana(x) > distancia)
            {
                distancia = malla[i].distanciaEuclidiana(x);
                pos = i;
            }
        }
        else
        {
            if(malla[i].distanciaHamming(x) > distancia)
            {
                distancia = malla[i].distanciaHamming(x);
                pos = i;
            }
        }
    }
    result.append(pos);
    result.append(distancia);
    return result;
}

QVector<int> MainWindow::vecindario(int pos)
{
    int size = ui->inputMalla->text().toInt();
    int vSize = ui->inputV->text().toInt();
    QVector<int> posiciones;
    int fila = pos/size;
    int columna = pos - (fila*size);

    posiciones.append(pos);
    for(int i = 1; i <= vSize; i++)
    {
        if(columna+i < size)
        {
            posiciones.append(pos+i);
        }
        if(columna-i >= 0)
        {
            posiciones.append(pos-i);
        }
        if(fila+i < size)
        {
            posiciones.append(pos+(size*i));
        }
        if(fila-i >= 0)
        {
            posiciones.append(pos-(size*i));
        }
        if(!tipoVecindad)
        {
            if(columna+i < size and fila+i < size)
            {
                posiciones.append((pos+i)+(size*i));
            }
            if(columna+i < size and fila-i >= 0)
            {
                posiciones.append((pos+i)-(size*i));
            }
            if(columna-i >= 0 and fila-i >= 0)
            {
                posiciones.append((pos-i)-(size*i));
            }
            if(columna-i >= 0 and fila+i < size)
            {
                posiciones.append((pos-i)+(size*i));
            }
        }
    }
    return posiciones;
}

void MainWindow::actualizaPesos(QVector<int> v,int e)
{
    double n = ui->inputF->text().toDouble();
    for(int i = 0; i < v.size(); i++)
    {
        malla[v[i]].actualiza(pointVector[e].Data,n);
    }
}

void MainWindow::pintarCalor(QVector<double> x)
{
    QPen myPen;

    QVector<double> tempG = neuronaGanadora(x);
    QVector<double> tempP = peorNeurona(x);
    double distancia;
    double eleccion;
    for(int i = 0; i < malla.size(); i++)
    {
        if(tipoDistancia)
        {
            distancia = malla[i].distanciaEuclidiana(x);
        }
        else
        {
            distancia = malla[i].distanciaHamming(x);
        }

        eleccion = (distancia - tempG[1]) / (tempP[1] - tempG[1]);
        if(eleccion >= 0 and eleccion < 0.1)
        {
            myPen.setColor(myColors[0]);
        }
        if(eleccion >= 0.1 and eleccion < 0.2)
        {
            myPen.setColor(myColors[1]);
        }
        if(eleccion >= 0.2 and eleccion < 0.3)
        {
            myPen.setColor(myColors[2]);
        }
        if(eleccion >= 0.3 and eleccion < 0.4)
        {
            myPen.setColor(myColors[3]);
        }
        if(eleccion >= 0.4 and eleccion < 0.5)
        {
            myPen.setColor(myColors[4]);
        }
        if(eleccion >= 0.5 and eleccion < 0.6)
        {
            myPen.setColor(myColors[5]);
        }
        if(eleccion >= 0.6 and eleccion < 0.7)
        {
            myPen.setColor(myColors[6]);
        }
        if(eleccion >= 0.7 and eleccion < 0.8)
        {
            myPen.setColor(myColors[7]);
        }
        if(eleccion >= 0.8 and eleccion < 0.9)
        {
            myPen.setColor(myColors[8]);
        }
        if(eleccion >= 0.9 and eleccion <= 1)
        {
            myPen.setColor(myColors[9]);
        }
        ui->customPlot->graph(i)->setPen(myPen);
    }
    ui->customPlot->replot();
    ui->customPlot->update();
}

void MainWindow::clear()
{
    pointVector.clear();
    malla.clear();
    ui->customPlot->clearGraphs();
}

void MainWindow::sleep(int msec)
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, &QEventLoop::quit);
    loop.exec();
}

void MainWindow::on_limpiar_clicked()
{
    clear();
    ui->customPlot->replot();
    ui->customPlot->update();
}

void MainWindow::on_SOM_PROBAR_clicked()
{
    for(int i = 0; i < pointVector.size(); i++)
    {
        sleep(700);
        QVector<double> ganador = neuronaGanadora(pointVector[i].Data);
        ui->clase->setText(QString::number(i) + "     Clase " + QString::number(pointVector[i].Class));
        pintarCalor(pointVector[i].Data);
        qDebug() << pointVector[i].Data << ganador[1];
    }
}
