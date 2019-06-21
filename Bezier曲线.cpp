#include<Windows.h>
#include<GL\glut.h>
#include<cmath>

//��ʼ����ͼ����
void    Init(void);
//���Ƶ�
void    SetPoint(void);
//������
void    SetLine(void);
//��ʾ����
void    display(void);
//���Ʊ��������
void    SetBezier(void);
//�������¼�
void MymouseFunction(int button, int state, int x, int y);



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

static int Pointsum = 0;//���Ƶ�ĸ���

static Point points[100];//��ſ��Ƶ�
static Point BezerPoints[11000];//��ż���������ɱ�������ߵĵ�

//��ʼ������
void Init(void) {
	glClearColor(1,1,1,0);
    glColor3f(0,0,1); //���û�ͼ��ɫ
    glPointSize(5.0); //�趨��Ĵ�С
	glMatrixMode(GL_PROJECTION);
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

// ���Ʊ���������
void SetBezier(void) {
    	int	cnt = 0;
		float xx[100];
		float yy[100];

        for (double t = 0.0; t <= 1.0; t += 0.0001){
			for (int i = 1; i < Pointsum; i++) {
				for (int k = 0; k < Pointsum - i; k++) {
					if (i == 1) {
						xx[k] = points[k].x*(1 - t) + points[k + 1].x*t;
						yy[k] = points[k].y*(1 - t) + points[k + 1].y*t;
					}
					else{
						xx[k] = xx[k]*(1 - t) + xx[k + 1]*t;
						yy[k] = yy[k]*(1 - t) + yy[k + 1]*t;
					}
				}
			}
			BezerPoints[cnt].SetXY(xx[0], yy[0]);
			cnt++;
        }
		glColor3f(0.0, 0.0, 1.0); // �趨���������ߵ���ɫ
		for (int i = 0; i < cnt-1; i++) {
			SetLine(BezerPoints[i],BezerPoints[i+1]);
		}


}


void MymouseFunction(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON&&state==GLUT_DOWN) {// �������������
        points[Pointsum].SetXY(x,480- y); // ���������������
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
	}else if (button == GLUT_RIGHT_BUTTON&&state==GLUT_DOWN) {
		 //���Ʊ���������
			SetBezier();
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
