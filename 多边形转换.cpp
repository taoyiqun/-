#include<Windows.h>
#include<GL\glut.h>
#include<cmath>
//����
class Point
{
public:
	float x, y;
	void SetXY(float x, float y) {
		this->x = x;
		this->y = y;
	}
};
//�߽ṹ��
typedef struct Edge{
    float y_max,x;
    float dx;
    Edge *next;
}Edge;
static int Pointsum = 0;//����εĵ���
static float eps=1e-10;//�����ڳ�ʼ������߱�ʱfloat��int�ıȽ�
static Point points[100];//����ζ���

//��ʼ������

void Init(void) {
	glClearColor(1,1,1,0);
    glColor3f(0,0,1); //�趨��ͼ��ɫ
    glPointSize(5.0); //�趨��Ĵ�С
	glMatrixMode(GL_PROJECTION); // �趨���ʵľ���
    glLoadIdentity();
    gluOrtho2D(0.0,600.0,0.0,480.0); //ƽ��ͶӰ���ĸ������ֱ���x,y��Χ
	glEnable(GL_LINE_SMOOTH);

}

//���Ƶ�
void SetPoint(Point p) {
	glBegin(GL_POINTS);
    glVertex2f(p.x, p.y);
    glEnd();
    glFlush();
}

//������
void SetLine(Point p1, Point p2) {
    glBegin(GL_LINES);
    glVertex2f(p1.x,p1.y);
    glVertex2f(p2.x, p2.y);
    glEnd();
    glFlush();
}

//display����
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

// �������亯��
void Filling(void) {
    int MaxY = 0;
	int i;
	for (i = 0; i < Pointsum; i++){
        if (points[i].y > MaxY){
              MaxY = points[i].y;
        }
	}       //ѡ�����Ķ�������Ӧ��yֵ
	Edge *pAET = new Edge;//��߱�
	pAET->next = NULL;
	Edge *pEdge[1024];//����߱�
	for (i = 0; i <= MaxY; i++){
		pEdge[i] = new Edge;
		pEdge[i]->next = NULL;
	}//��ʼ�����Ա߱�
	//һ�����ǰ��ĵ��γ�һ���߶�,ͬʱ������һ�����γ��߶�
	for (i = 0; i < MaxY; i++){
	    for (int j = 0; j < Pointsum; j++){
			if (fabs(points[j].y - i)<=eps){
			    if (points[(j - 1 + Pointsum) % Pointsum].y > points[j].y){
					Edge *p = new Edge;
					p->x = points[j].x;
					p->y_max = points[(j - 1 + Pointsum) % Pointsum].y;
					p->dx = (points[(j - 1 + Pointsum) % Pointsum].x - points[j].x) / (points[(j - 1 + Pointsum) % Pointsum].y - points[j].y);
					p->next = pEdge[i]->next;
					pEdge[i]->next = p;
				}
				if (points[(j + 1 + Pointsum) % Pointsum].y > points[j].y){
				    Edge *p = new Edge;
                    p->x = points[j].x;
                    p->y_max = points[(j + 1 + Pointsum) % Pointsum].y;
                    p->dx = (points[(j + 1 + Pointsum) % Pointsum].x - points[j].x) / (points[(j + 1 + Pointsum) % Pointsum].y - points[j].y);
                    p->next = pEdge[i]->next;
                    pEdge[i]->next = p;
				}
			}
		}
	}

	//���±߱�Edge[i]�еı߽ڵ��ò������򷨲���AET��ʹ֮����x���������˳������
	for (i = 0; i <= MaxY; i++)
    {
		Edge *p = pAET->next;
		while (p){
			p->x = p->x + p->dx;
			p = p->next;
		}
		Edge *tq = pAET;
		p = pAET->next;
		tq->next = NULL;
		while (p){
			while (tq->next && p->x >= tq->next->x){
			    tq = tq->next;
            }//��������
			Edge *s = p->next;
			p->next = tq->next;
			tq->next = p;
			p = s;
			tq = pAET;
		}

		//����AET������Խ��������(����ҿ����ϵ�����(x,y)
		Edge *q = pAET;
		p = q->next;
		while (p){
			if (p->y_max == i){
				q->next = p->next;
				delete p;
				p = q->next;
			}
			else{
				q = q->next;
				p = q->next;
			}
		}
		p = pEdge[i]->next;
		q = pAET;
		while (p)
		{
			while (q->next && p->x >= q->next->x){
                q = q->next;
			}
			Edge *s = p->next;
			p->next = q->next;
			q->next = p;
			p = s;
			q = pAET;
		}
		p = pAET->next;
		glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(0.0,1.0,0.0);
        glBegin(GL_POINTS);
		while (p && p->next){
			for (float j = p->x; j <= p->next->x; j++){
				glVertex2i(j, i);//��д���ص���ɫֵ
			}
			p = p->next->next;
		}
	}
	glEnd();
	glFlush();
}


void MymouseFunction(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON&&state==GLUT_DOWN) {// �������������
        points[Pointsum].SetXY(x,480- y); // ����������������ʱ��
        // ���õ����ɫ�����Ƶ�
        glColor3f(1.0,0.0,0.0);
        SetPoint(points[Pointsum]);
        // �����ߵ���ɫ��������
        glColor3f(1.0,0.0,0.0);
		if (Pointsum > 0) {
			SetLine(points[Pointsum - 1], points[Pointsum]);
		}
		if (Pointsum < 99) {
			Pointsum++;
		}
	}else if (button == GLUT_RIGHT_BUTTON&&state==GLUT_DOWN) {//����Ҽ�����

		  glColor3f(1.0,0.0,0.0);
        SetLine(points[0], points[Pointsum-1]);

		 //���
			Filling();
			Pointsum = 0;
	}
}



int main(int argc, char *argv[]){
    glutInit(&argc, argv); //�̶���ʽ
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);   //����ģʽ
    glutInitWindowSize(600, 480);    //��ʾ��Ĵ�С
    glutInitWindowPosition(100, 100); //ȷ����ʾ�����Ͻǵ�λ��
    glutCreateWindow("17042127 ����Ⱥ");

    Init(); // ��ʼ��
    glutMouseFunc(MymouseFunction); // �������¼�
    glutDisplayFunc(display); // ִ����ʾ
    glutMainLoop(); //����GLUT�¼�����ѭ��
    return 0;
}
