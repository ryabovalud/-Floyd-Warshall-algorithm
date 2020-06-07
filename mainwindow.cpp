#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStandardItemModel"
#include "QStandardItem"
#include "math.h"

QStandardItemModel *model_C = new QStandardItemModel;
QStandardItemModel *model_T = new QStandardItemModel;
QStandardItemModel *model_H = new QStandardItemModel;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->info_2->setReadOnly(1);
    ui->way->setReadOnly(1);
    ui->long_2->setReadOnly(1);
    ui->C_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->T_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->H_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->time->setText("0.5");
}

MainWindow::~MainWindow()
{
    delete ui;
}

QEventLoop loop;
QTimer timer;
int x = 10,
    y = 10;
int v=0;//количество вершин
int **C;//матрица длин дуг
int **T;//матрица длин кратчайших путей
int **H;//матрица самих путей
int *way;//кратчайший путь
double t = 0;//секунды на паузу
int koll=0;//количество вершин в пути

void MainWindow::func_way(int from, int to)//восстанавливаем путь
{
   if(from == H[from-1][to-1])
   {
       koll ++;
       way[koll] = to;
   }
   else
   {
       koll ++;
       way[koll] = H[from-1][to-1];
       func_way (H[from-1][to-1], to);
   }
}

void MainWindow::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event);
    QString time_str=ui->time->text();
    t = time_str.toDouble();//секунды на паузу

  QPainter painter(this);
  painter.begin(this);
  painter.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::FlatCap));
   int N=v;
  float x[N],y[N];//координаты точки - центра i-ой вершины
  float alpha = 2*M_PI/N;
  int x0 = 150, y0 = 170, R=100,//радиус графа
                                        r= 10;

     for(int i = 1; i<=N ; i++) //рисуем N вершин покругу
     {
          x[i]= x0+R*cos(i*alpha);

          y[i]= y0+R*sin(i*alpha);
       QPoint center(x[i],y[i]);
        painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));
         painter.drawEllipse(center,r,r);
         painter.drawText(x[i]-r/2,y[i]+r/2,QString::number(i));
     }

//РИСУЕМ ГРАФ
     int from=0,to=0;
     for (int j =1;j<=N;j++)//столбцы
     {
         for (int i=1;i<=N;i++)//строки
         {
            if(C[i-1][j-1]>0)
            {
                from=i;
                to=j;
                //рисуем линии
                painter.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::FlatCap));
                float x_1,x_2,y_1,y_2;
                x_1= x[from]+r*cos(to*alpha);y_1= y[from]+r*sin(to*alpha);
                x_2= x[to]+r*cos(from*alpha);y_2= y[to]+r*sin(from*alpha);
                painter.drawLine(x_1, y_1, x_2, y_2);

                //рисуем стрелочку

                           const float ostr = 0.25;// острота стрелки
                           float x = x_2 - x_1;
                           float y = y_2 - y_1;
                           float lons =  12;// длина лепестков
                           float ugol = atan2(y, x);// угол наклона линии
                           QPolygon polygon;
                           polygon << QPoint(x_2, y_2)
                                   << QPoint(x_2 - lons * cos(ugol - ostr), y_2 - lons * sin(ugol - ostr))
                                   << QPoint(x_2 - lons * cos(ugol + ostr), y_2 - lons * sin(ugol + ostr))
                                      ;
                          painter.setBrush(QBrush(Qt::red
                                                   , Qt::SolidPattern));
                           painter.drawPolygon(polygon, Qt::OddEvenFill );
                           painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));
                           painter.drawText(x_2- 30 * cos(ugol - ostr) ,y_2 - 30* sin(ugol - ostr) ,QString::number(C[from-1][to-1]));

            }

         }



      }

     //Рисуем на графе кратчайший путь
     if(koll)
     {
         for(int i=0;i<koll;i++)
         {
             from=way[i];
             to=way[i+1];
             //рисуем линии
             painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::FlatCap));
             float x_1,x_2,y_1,y_2;
             x_1= x[from]+r*cos(to*alpha);y_1= y[from]+r*sin(to*alpha);
             x_2= x[to]+r*cos(from*alpha);y_2= y[to]+r*sin(from*alpha);
             painter.drawLine(x_1, y_1, x_2, y_2);
            //рисуем стрелочку

                        const float ostr = 0.25;// острота стрелки
                        float x = x_2 - x_1;
                        float y = y_2 - y_1;
                        float lons =  12;// длина лепестков
                        float ugol = atan2(y, x);// угол наклона линии
                        QPolygon polygon;
                        polygon << QPoint(x_2, y_2)
                                << QPoint(x_2 - lons * cos(ugol - ostr), y_2 - lons * sin(ugol - ostr))
                                << QPoint(x_2 - lons * cos(ugol + ostr), y_2 - lons * sin(ugol + ostr))
                                   ;
                       painter.setBrush(QBrush(Qt::blue
                                                , Qt::SolidPattern));
                        painter.drawPolygon(polygon, Qt::OddEvenFill );
                        painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::FlatCap));
                        painter.drawText(x_2- 30 * cos(ugol - ostr) ,y_2 - 30* sin(ugol - ostr) ,QString::number(C[from-1][to-1]));


         }


      }

}

//КНОПКИ

/*Загрузить граф*/
void MainWindow::on_info_clicked()
{
    QString time_str=ui->time->text();
    t = time_str.toDouble();//секунды на паузу
    //почистим всё на всякий случай
    ui->long_2->clear();
    ui->info_2->clear();
    ui->way->clear();
    ui->comboBox_FROM->clear();
    ui->comboBox_TO->clear();
    model_C->clear();
    model_T->clear();
    model_H->clear();
    koll=0;
    v=0;
    repaint();

    //Считывание информации из файла
        ui->info_2->clear();
/*C:\\Users\\brrri\\OneDrive\\Study\\Alg Diskr Math\\Kyrsach\\Floyd\\*/
    QFile file(":/GRAF.txt"); // создаем объект класса QFile
    if (!file.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наши файлы для чтения и для записи
    {
        ui->info_2->insert("CANT OPEN FILE");// если это сделать невозможно
    }
    else
    { //Если файл открыт для записи успешно
        QTextStream in(&file);
        in >> v; // Считываем v - количество вершин
        //приготовим память для матрицы длин дуг
        C=new int* [v];
          for (int i=0;i<v;i++)
             C[i]=new int[v];
        for (int i=0;i<v;i++)
            for(int j=0;j<v;j++)
            {
                in>>C[i][j];
            }
         file.close();
    }

        //Заполнение таблицы С на форме
        QStandardItem *item;
        model_C->clear();

        //Заголовки столбцов
        QStringList horizontalHeader;
        for(int i=1;i<=v;i++) horizontalHeader.append(QString::number(i));

        //Заголовки строк
        QStringList verticalHeader;
        for(int i=1;i<=v;i++) verticalHeader.append(QString::number(i));

        model_C->setHorizontalHeaderLabels(horizontalHeader);
        model_C->setVerticalHeaderLabels(verticalHeader);

        for(int i=0;i<v;i++)
          for(int j=0;j<v;j++)
          {
           item = new QStandardItem(QString::number(C[i][j]));
           model_C->setItem(i, j, item);
          }

        ui->C_table->setModel(model_C);
        ui->C_table->resizeRowsToContents();
        ui->C_table->resizeColumnsToContents();


        ui->comboBox_FROM->clear();
        ui->comboBox_TO->clear();
        for(int i=0;i<v;i++)
            ui->comboBox_FROM->insertItem(i,QString::number(i+1));
        for(int i=0;i<v;i++)
        {
            ui->comboBox_TO->insertItem(i,QString::number(i+1));
        }
        update();
}

//Прокладываем путь
void MainWindow::on_pushButton_PTY_clicked()
{
    QString time_str=ui->time->text();
    t = time_str.toDouble();//секунды на паузу
    ui->long_2->clear();
    ui->way->clear();
    int from = ui->comboBox_FROM->currentText().toInt();
    int to = ui->comboBox_TO->currentText().toInt();
    if(from!=to)
    {
        T=new int* [v];//Матрица длин путей
       for (int i=0;i<v;i++)
           T[i]=new int[v];
       for(int i=0;i<v;i++)
          for(int j=0;j<v;j++)
          {
              T[i][j]=0;
          }

       H=new int* [v];//Матрица самих путей
       for (int i=0;i<v;i++)
           H[i]=new int[v];

       for(int i=0;i<v;i++)
          for(int j=0;j<v;j++)
          {
              H[i][j]=0;
          }

       //Заполнение таблицы T на форме

       QStandardItem *item;
       model_T->clear();

       //Заголовки столбцов
       QStringList horizontalHeader_T;
       for(int i=1;i<=v;i++) horizontalHeader_T.append(QString::number(i));

       //Заголовки строк
       QStringList verticalHeader_T;
       for(int i=1;i<=v;i++) verticalHeader_T.append(QString::number(i));

       model_T->setHorizontalHeaderLabels(horizontalHeader_T);
       model_T->setVerticalHeaderLabels(verticalHeader_T);

       //Заполнение таблицы H на форме
       {
       model_H->clear();

       //Заголовки столбцов
       QStringList horizontalHeader_H;
       for(int i=1;i<=v;i++) horizontalHeader_H.append(QString::number(i));

       //Заголовки строк
       QStringList verticalHeader_H;
       for(int i=1;i<=v;i++) verticalHeader_H.append(QString::number(i));

       model_H->setHorizontalHeaderLabels(horizontalHeader_H);
       model_H->setVerticalHeaderLabels(verticalHeader_H);
       }

       //файл для записи данных
       QFile file_text(":/GRAF_text.txt");
       if (file_text.open(QIODevice::WriteOnly | QIODevice::Truncate))//Почистили файл
               file_text.close();
       file_text.open(QIODevice::WriteOnly);
       QTextStream out(&file_text);

       for(int i=0;i<v;i++)
       {
          for(int j=0;j<v;j++)
          {
              model_C->item(i, j)->setBackground(Qt::red);
              ui->C_table->setModel(model_C);
              ui->C_table->resizeRowsToContents();
              ui->C_table->resizeColumnsToContents();
              T[i][j]=C[i][j];//инициализация
              item = new QStandardItem(QString::number(T[i][j]));
              model_T->setItem(i, j, item);
              ui->T_table->setModel(model_T);
              ui->T_table->resizeRowsToContents();
              ui->T_table->resizeColumnsToContents();
              ui->info_2->clear();
              ui->info_2->insert("T["+QString::number(i+1)+"]["
                                 +QString::number(j+1)+"]-->"+QString::number(C[i][j]));
              out << "T["+QString::number(i+1)+"]["
                     +QString::number(j+1)+"]-->"+QString::number(C[i][j]); // Записываем в файл
              out<<"\r\n";
                  timer.setInterval(t*500); //2 sec
                  connect (&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
                  timer.start();
                  loop.exec();

              if (C[i][j] == -1)
              {
                 H[i][ j] = 0;// { нет дуги из i в j }
                 item = new QStandardItem(QString::number(H[i][j]));
                 model_H->setItem(i, j, item);
                 ui->H_table->setModel(model_H);
                 ui->H_table->resizeRowsToContents();
                 ui->H_table->resizeColumnsToContents();
                 ui->info_2->clear();
                 ui->info_2->insert("H["+QString::number(i+1)+"]["
                                    +QString::number(j+1)+"]-->"+QString::number(0));
                 out << "H["+QString::number(i+1)+"]["
                        +QString::number(j+1)+"]-->"+QString::number(0); // Записываем в файл
                 out<<"\r\n";
                 timer.setInterval(t*500); //2 sec
                 connect (&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
                 timer.start();
                 loop.exec();
              }
              model_C->item(i, j)->setBackground(Qt::white);
              ui->C_table->setModel(model_C);
          }



       }



       for(int k=0;k<v;k++)
        {
          for(int i=0;i<v;i++)
          {
            for (int j=0;j<v;j++)
            {
               if ((i!= j) && (T[i][k] != -1)&&  (T[k][j]!= -1))
               {
                   model_T->item(i, j)->setBackground(Qt::red);
                   ui->T_table->setModel(model_T);
                   timer.setInterval(t*500); //2 sec
                   connect (&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
                   timer.start();
                   loop.exec();


                   if(T[i][j]==-1)
                  {
                      ui->info_2->clear();
                      ui->info_2->insert("T["+QString::number(i+1)+"]["
                                         +QString::number(j+1)+"]="+QString::number(-1));

                      timer.setInterval(t*2000); //2 sec
                      connect (&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
                      timer.start();
                      loop.exec();


                      ui->info_2->clear();
                      ui->info_2->insert("T["+QString::number(i+1)+"]["
                                         +QString::number(j+1)+"]= T["+QString::number(i+1)+"]["
                                         +QString::number(k+1)+"]+ T["+QString::number(k+1)+"]["
                                         +QString::number(j+1)+"]");
                      T[i][j]=T[i][k]+T[k][j];
                      out << "T["+QString::number(i+1)+"]["
                             +QString::number(j+1)+"]= T["+QString::number(i+1)+"]["
                             +QString::number(k+1)+"]+ T["+QString::number(k+1)+"]["
                             +QString::number(j+1)+"]"; // Записываем в файл
                      out<<"\r\n";
                      model_T->item(i, k)->setBackground(Qt::green);
                      model_T->item(k, j)->setBackground(Qt::green);
                      item = new QStandardItem(QString::number(T[i][j]));
                      model_T->setItem(i, j, item);
                      model_T->item(i, j)->setBackground(Qt::blue);
                      ui->T_table->setModel(model_T);
                      timer.setInterval(t*2000); //1 sec
                      connect (&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
                      timer.start();
                      loop.exec();



                      ui->info_2->clear();
                      ui->info_2->insert("H["+QString::number(i+1)+"]["
                                         +QString::number(j+1)+"]="+ QString::number(k+1));
                      H[i][j] = k+1;
                      out << "H["+QString::number(i+1)+"]["
                             +QString::number(j+1)+"]="+ QString::number(k+1); // Записываем в файл
                      out<<"\r\n";
                      item = new QStandardItem(QString::number(H[i][j]));
                      model_H->setItem(i, j, item);
                      model_H->item(i, j)->setBackground(Qt::blue);
                      ui->H_table->setModel(model_H);
                      timer.setInterval(t*2000); //1 sec
                      connect (&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
                      timer.start();
                      loop.exec();


                      //чистим таблички
                      model_T->item(i, j)->setBackground(Qt::white);
                      model_T->item(i, k)->setBackground(Qt::white);
                      model_T->item(k, j)->setBackground(Qt::white);
                      ui->T_table->setModel(model_T);
                      model_H->item(i, j)->setBackground(Qt::white);
                      ui->H_table->setModel(model_H);

                  }
                  else
                   {
                      if(T[i][j]> T[i][k] + T[k][j])
                      {
                          ui->info_2->clear();
                          ui->info_2->insert("T["+QString::number(i+1)+"]["
                                             +QString::number(j+1)+"]>T["+QString::number(i+1)+"]["
                                             +QString::number(k+1)+"]+ T["+QString::number(k+1)+"]["
                                             +QString::number(j+1)+"]");
                          out << "T["+QString::number(i+1)+"]["
                                 +QString::number(j+1)+"]>T["+QString::number(i+1)+"]["
                                 +QString::number(k+1)+"]+ T["+QString::number(k+1)+"]["
                                 +QString::number(j+1)+"]"; // Записываем в файл
                          out<<"\r\n";
                          model_T->item(i, j)->setBackground(Qt::blue);
                          model_T->item(i, k)->setBackground(Qt::green);
                          model_T->item(k, j)->setBackground(Qt::green);
                          ui->T_table->setModel(model_T);

                          timer.setInterval(t*2000); //1 sec
                          connect (&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
                          timer.start();
                          loop.exec();


                          ui->info_2->clear();
                          ui->info_2->insert("T["+QString::number(i+1)+"]["
                                             +QString::number(j+1)+"]= T["+QString::number(i+1)+"]["
                                             +QString::number(k+1)+"]+ T["+QString::number(k+1)+"]["
                                             +QString::number(j+1)+"]");
                          T[i][j]=T[i][k]+T[k][j];
                          out << "T["+QString::number(i+1)+"]["
                                 +QString::number(j+1)+"]= T["+QString::number(i+1)+"]["
                                 +QString::number(k+1)+"]+ T["+QString::number(k+1)+"]["
                                 +QString::number(j+1)+"]";
                          out<<"\r\n";
                          item = new QStandardItem(QString::number(T[i][j]));
                          model_T->setItem(i, j, item);
                          model_T->item(i, j)->setBackground(Qt::blue);
                          model_T->item(i, k)->setBackground(Qt::green);
                          model_T->item(k, j)->setBackground(Qt::green);
                          ui->T_table->setModel(model_T);
                          timer.setInterval(t*2000); //2 sec
                          connect (&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
                          timer.start();
                          loop.exec();



                          ui->info_2->clear();
                          ui->info_2->insert("H["+QString::number(i+1)+"]["
                                             +QString::number(j+1)+"]="+ QString::number(k+1));
                          H[i][j] = k+1;
                          out << "H["+QString::number(i+1)+"]["
                                 +QString::number(j+1)+"]="+ QString::number(k+1); // Записываем в файл
                          out<<"\r\n";
                          item = new QStandardItem(QString::number(H[i][j]));
                          model_H->setItem(i, j, item);
                          model_H->item(i, j)->setBackground(Qt::blue);
                          ui->H_table->setModel(model_H);
                          timer.setInterval(t*1000); //1 sec
                          connect (&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
                          timer.start();
                          loop.exec();


                          /*чистим таблички*/
                          model_T->item(i, j)->setBackground(Qt::white);
                          model_T->item(i, k)->setBackground(Qt::white);
                          model_T->item(k, j)->setBackground(Qt::white);
                          ui->T_table->setModel(model_T);
                          model_H->item(i, j)->setBackground(Qt::white);
                          ui->H_table->setModel(model_H);
                          T[i][j]=T[i][k] + T[k][j];
                          H[i][j] = k+1; //{ запомнить новый путь }
                      }
                   }
                   model_T->item(i, j)->setBackground(Qt::white);
                   ui->T_table->setModel(model_T);
                }
             }
          }
        }

       for(int i=0;i<v;i++)
           for(int j=0;j<v;j++)
           {
               if(H[i][j]==0&&T[i][j]>0)
                   H[i][j]=i+1;
               /*else if(H[i][j]=0&&T[i][j]==0)
                   H[i][j]=-1;*/
               item = new QStandardItem(QString::number(H[i][j]));
               model_H->setItem(i, j, item);
               ui->H_table->setModel(model_H);
           }
       file_text.close(); // Закрываем file



            model_T->item(from-1, to-1)->setBackground(Qt::blue);
            ui->T_table->setModel(model_T);
            timer.setInterval(t*1000); //1 sec
            connect (&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
            timer.start();
            loop.exec();

            ui->long_2->insert(QString::number(T[from-1][to-1]));
            timer.setInterval(t*1000); //1 sec
            connect (&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
            timer.start();
            loop.exec();


            //вызываем функцию поиска пути
            koll=0;
            way=new int [v];
            way[0]=from;
            func_way(from,to);


            for(int i=0;i<koll;i++)
            {
                ui->way->insertPlainText(QString::number(way[i])+"-->");
                timer.setInterval(t*500); //0.5 sec
                connect (&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
                timer.start();
                loop.exec();
            }
            ui->way->insertPlainText(QString::number(way[koll]));
           update();
    }
    else
    {
        ui->way->insertPlainText("FROM=TO");
    }


}

void MainWindow::on_clear_clicked()
{
    ui->long_2->clear();
    ui->info_2->clear();
    ui->way->clear();
    ui->comboBox_FROM->clear();
    ui->comboBox_TO->clear();
    model_C->clear();
    model_T->clear();
    model_H->clear();
    koll=0;
    v=0;
    update();
}

//Выход
void MainWindow::on_exit_clicked()
{
    exit(1);
}

//АЛГОРИТМ ФЛОЙДА

/*
for(int i=0;i<v;i++)
{
   for(int j=0;j<v;j++)
   {
        if (C[i][j] == -1)
       {
          H[i][ j] = 0;// { нет дуги из i в j }

       }
   }
}



 for(int k=0;k<v;k++)
 {
   for(int i=0;i<v;i++)
   {
     for (int j=0;j<v;j++)
     {
        if ((i!= j) && (T[i][k] != -1)&&  (T[k][j]!= -1))
        {
            if(T[i][j]==-1)
           {
               T[i][j]=T[i][k]+T[k][j];

               H[i][j] = k+1;



           }
           else
            {
               if(T[i][j]> T[i][k] + T[k][j])
               {
                   T[i][j]=T[i][k]+T[k][j];
                   H[i][j] = k+1;
                   T[i][j]=T[i][k] + T[k][j];
                   H[i][j] = k+1; //{ запомнить новый путь }
               }
            }

         }
      }
   }
 }

for(int i=0;i<v;i++)
    for(int j=0;j<v;j++)
    {
        if(H[i][j]==0&&T[i][j]>0)
            H[i][j]=i+1;

    }

 */
