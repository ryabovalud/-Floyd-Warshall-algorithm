#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent)
{

}

Widget::~Widget()
{
    delete ui;
}

int **I,//������� ������� �������������
    **Sm,//������� ������� ���������
    **T,//������� ���� �����
    **H,//������� ����� �����
    *e;//�������������� ������

int N=0,M=0;//���������� ������ � ����
const int n=20;//������������ ���������� ���������� � �����


//������ ����
void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);


  QPainter painter(this);
  painter.begin(this);
  painter.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::FlatCap));

  float x[N],y[N];//���������� ����� - ������ i-�� �������
  float alpha = 2*M_PI/N;
  int x0 = 919/4, y0 = 614/4, R=100,//������ �����
                                        r= 10;

     for(int i = 1; i<=N ; i++) //������ N ������ �������
     {
          x[i]= x0+R*cos(i*alpha);

          y[i]= y0+R*sin(i*alpha);
       QPoint center(x[i],y[i]);
        painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));
         painter.drawEllipse(center,r,r);
         painter.drawText(x[i]-r/2,y[i]+r/2,QString::number(i));
     }

//������ ����
     int from=0,to=0;
     for (int j =1;j<=M;j++)//�������
     {
         from=0;
         for (int i=1;i<=N;i++)//������
         {
            if(I[i-1][j-1]==1)
            {
                if(!from) from=i;
                else to=i;
            }
         }

         //������ �����
         painter.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::FlatCap));
         float x_1,x_2,y_1,y_2;
         x_1= x[from]+r*cos(to*alpha);y_1= y[from]+r*sin(to*alpha);
         x_2= x[to]+r*cos(from*alpha);y_2= y[to]+r*sin(from*alpha);
           painter.drawLine(x_1, y_1, x_2, y_2);
      }



}
