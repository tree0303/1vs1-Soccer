#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <random>

#define LEGWIDTH 0.8//���̑���
#define LEGHEIGTHUP 2//���̕G��̒���
#define LEGHEIGHTDOWN 1.7//���̕G���̒���
#define ARMWIDTH 0.7//�r�̑���
#define ARMHEIGHTUP 1.6//��r����
#define ARMHEIGHTDOWN 1.4//�O�r����

#define imageWidth		256
#define imageHeight		256
#define INITIROTEV 0.05	//��]����
#define	INITIV 0.1	//�ړ�����
#define VARIATION1 1.4; //�֐ߑ��x���Ђ�
#define VARIATION2 -1.4;//�֐ߑ��x�E�Ђ�
#define VARIATION3 -1.4;//�֐ߑ��x���Ђ�
#define VARIATION4 1.4;//�֐ߑ��x�E�Ђ�
#define BODYSIZE 0.8//�̂̑傫��

std::mt19937 engine;
std::uniform_real_distribution<> dist2(-10, 10);//�����_���ϐ�2
std::uniform_real_distribution<> dist(0, 10);//�����_���ϐ�

//�e�N�X�`��������
unsigned char texImage[imageHeight][imageWidth][4];
GLuint texname[2];//�e�N�X�`���̐�

char text[50];//�`��e�L�X�g

int width = 1000, height = 600;//�E�B���h�E�T�C�Y

int mouseflag = 0;//�}�E�X�t���O(���N���b�N)

double fieldx = 48;//�t�B�[���hx��
double fieldz = 28;//�t�B�[���hz��
int f1_flag = 0;//���_��ς�����		F1�L�[
int viewpointflag = 0;//���_	0:���Ձ@1:��l��

double ballv = 0;//�{�[���̑���
double ballvx = 0;//�{�[��x���̌���
double ballvy = 0;
double ballvz = 0;//�{�[��z���̌���
double ballx = 0;//�{�[��x�����W
double bally = 2;//�{�[��y�����W
double ballz = 0;//�{�[��z�����W
double balldownv = 0.8;//�{�[��������
double ballarg = 0;//�{�[����]�p
double ballargx = 0;//�{�[����x�]��
double ballargz = 0;//�{�[����z�]��
double ballcos = 0;//�{�[��x����]
double ballsin = 0;//�{�[��z����]
double tx = 0;//�i�ޔ䗦
double tz = 0;//�i�ޔ䗦

double goalx = 50;//�S�[�����W

int point1 = 0;//�v���C���[�_��
int point2 = 0;//CMP�_��

float light0_posi[] = { 100.0, 100.0, 100.0, 1.0 };//�����̈ʒu

GLfloat whiten[] = { 1,1,1,1 };//�n�ʂ̐F�i�ꉞ�j
GLfloat reds_ambient[] = { 0,0,0,1 };//�ގ��̊��E��
GLfloat reds_diffuse[] = { 1,0,0,1 };//�ގ��̊g�U�E��
GLfloat blues_ambient[] = { 0,0,0,1 };//�ގ��̊��E��
GLfloat blues_diffuse[] = { 0,0,1,1 };//�ގ��̊g�U�E��
GLfloat yellows_ambient[] = { 0,0,0,1 };//�ގ��̊��E��
GLfloat yellows_diffuse[] = { 1,1,0,1 };//�ގ��̊g�U�E��

//�v���C���[�N���X
class People
{
public:
	double body;		//���֐�
	double rilegup;		//�E���t�����֐�
	double rilegmidle;	//�E�G�֐�
	double rilegdown;	//�E����֐�
	double lelegup;		//�����t�����֐�
	double lelegmidle;	//���G�֐�
	double lelegdown;	//������֐�
	double riarmup;		//�E���֐�
	double riarmdown;	//�E�I�֐�
	double learmup;		//�����֐�
	double learmdown;	//���I�֐�
	double head;		//��֐�
	double all;//�̂̌���
	double vbody;//�̂̌����̋Ȃ���p�x
	double xposi;//�ړ��x�N�g��
	double yposi;//�ړ��x�N�g��
	double zposi;//�ړ��x�N�g��
	double xlength;//x���ړ�����
	double zlength;//z���ړ�����
	double xcood;//x�����ړ�����
	double zcood;//z�����ړ�����
	int xflag;//�ړ�������	�ړ������v�Z�p
	int zflag;//�ړ�������	�ړ������v�Z�p
	double vx;//�����ړ��x�N�g��
	double vy;	//�����ړ��x�N�g��	�W�����v�p
	double vz;//�����ړ��x�N�g��

	double v;//�ړ����x
	double jump;//�W�����v�̍���
	int jumpflag;//�W�����v���Ă��邩	�X�y�[�X�L�[
	int k;//���x�W��					���L�[
	int rightflag;//�E�ɋȂ����Ă��邩	�E���L�[
	int leftflag;//���ɋȂ����Ă��邩	�����L�[
	int bflag;//�����V���Ă��邩		���L�[
	int tflag;//�~�܂��Ă��邩			���L�[
	int viewpointflag;//���_	0:���Ձ@1:��l��

	double variation1; //�֐ߑ��x���Ђ�
	double variation2;//�֐ߑ��x�E�Ђ�
	double variation3;//�֐ߑ��x���Ђ�
	double variation4;//�֐ߑ��x�E�Ђ�
	double variation5;//�̂̏㉺��
	double ybody;//�̂̏㉺

	double cross;//�E������
	double arg;//��]�p�x
	double bpx;//�ڕW�����x�N�g��x
	double bpz;//�ڕW�����x�N�g��z
	double l;//�x�N�g���̑傫��
	double bpxl;//�x�N�g�����K��x
	double bpzl;//�x�N�g�����K��z

	
	void setallies();//�v���C���[������
	void setenemy();//CMP������
	void drow();//�v���C���[�\��
	void drowcmp();//CMP�\��
	
	void bdown();//b����
	void tdown();//t����
	void fdown();//f����
	void spacedown();//space����
	void leftdown();//left����
	void rightdown();//right����

	void bup();//b����
	void tup();//t����
	void fup();//f����
	void leftup();//left����
	void rightup();//right����

	void bow();//�����V
	void rightturn();//�E��]
	void leftturn();//����]

	void movecmp(double fieldx, double fieldz, double goalx, double ballx, double ballz);//CMP����
	void moveplayer(double fieldx, double fieldz);//�v���C���[����
	void jointmove();//�֐ߓ���
	void jumping();//�W�����v

	bool btop(double ballx, double bally, double ballz);//�{�[���Ƃ̓����蔻��


};

void People::movecmp(double fieldx, double fieldz, double goalx, double ballx, double ballz)
{
	double goalz = 1;
	
	if (btop(ballx, 0, ballz)) {//�{�[���ɐG��Ă�����
		bpx = (-goalx - xposi);//x�x�N�g��
		bpz = (-goalz - zposi);//z�x�N�g��
		l = sqrt(pow(bpx, 2) + pow(bpz, 2));//�x�N�g���̑傫��
		bpxl = bpx / l;//x�x�N�g�����K��
		bpzl = bpz / l;//z�x�N�g�����K��
		arg = acos((vx * bpxl + vz * bpzl) / (sqrt(pow(vx, 2) + pow(vz, 2)) * sqrt(pow(bpxl, 2) + pow(bpzl, 2)))) + 0.000001;//�Ȃ��p
		cross = vx * -goalz - vz * -goalx;//�E������
		
		if (bpxl != vx || bpzl != vz) {
			if (cross > 0) {
				all -= arg / 10;
				vx = cos(all);
				vz = bpzl;
			}
			else {
				all += arg / 10;
				vx = cos(all);
				vz = bpzl;
			}
		}
	}
	else {//�{�[���ɐG��Ă��Ȃ�������
		bpx = (ballx+0.5 - xposi);//x�x�N�g��
		bpz = (ballz+0.5 - zposi);//z�x�N�g��
		l = sqrt(pow(bpx, 2) + pow(bpz, 2));//�x�N�g���̑傫��
		bpxl = bpx / l;//x�x�N�g�����K��
		bpzl = bpz / l;//z�x�N�g�����K��
		arg = acos((vx * bpxl + vz * bpzl) / (sqrt(pow(vx, 2) + pow(vz, 2)) * sqrt(pow(bpxl, 2) + pow(bpzl, 2)))) + 0.000001;//�Ȃ��p
		cross = vx * bpzl - vz * bpzl;//�E������
		if (bpxl != vx || bpzl != vz) {
			if (cross > 0) {
				all -= arg / 10;
				vx = cos(all);
				vz = -sin(all);
			}
			else {
				all += arg / 10;
				vx = cos(all);
				vz = -sin(all);
			}
		}
		
	}
	all = fmod(all, 360);
	//�ړ������̃x�N�g���ύX
	xlength = vx * v * k;
	zlength = vz * v * k;

	//�ړ������v�Z���ړ��ł��邩
	//x�������ɂɈړ��ł��邩
	if (xcood >= fieldx) {
		xflag = 0;
		if (xlength > 0) xlength = 0;
	}
	else if (xcood <= -fieldx) {
		xflag = 0;
		if (xlength < 0) xlength = 0;
	}
	else {
		xflag = 1;
	}
	//z�������Ɉړ��ł��邩
	if (zcood >= fieldz) {
		zflag = 0;
		if (zlength > 0) zlength = 0;
	}
	else if (zcood <= -fieldz) {
		zflag = 0;
		if (zlength < 0) zlength = 0;
	}
	else {
		zflag = 1;
	}
	xcood += xlength;//x���̈ړ�����(���W)
	zcood += zlength;//z���̈ړ�����(���W)

	//�ړ�
	xposi += xlength * xflag;
	zposi += zlength * zflag;
}

//CMP������
void People::setenemy()
{
	body = 0;		 //���֐�
	rilegup = 0;	 //�E���t�����֐�
	rilegmidle = 0;	 //�E�G�֐�
	rilegdown = 0;	 //�E����֐�
	lelegup = 0;	 //�����t�����֐�
	lelegmidle = 0;	 //���G�֐�
	lelegdown = 0;	 //������֐�
	riarmup = 0;	 //�E���֐�
	riarmdown = 0;	 //�E�I�֐�
	learmup = 0;	 //�����֐�
	learmdown = 0;	 //���I�֐�
	head = 0;		 //��֐�

	all = 180;//�̂̌���
	vbody = INITIROTEV;//�̂̌����̋Ȃ���p���x
	xposi = 15;//�ړ��x�N�g��
	yposi = 0;//�ړ��x�N�g��
	zposi = dist2(engine);//�ړ��x�N�g��
	xlength = 0;//x���ړ�����
	zlength = 0;//z���ړ�����
	xcood = 15;//x�����ړ�����
	zcood = zposi;//z�����ړ�����
	xflag = 1;//�ړ�������	�ړ������v�Z�p
	zflag = 1;//�ړ�������	�ړ������v�Z�p
	vx = -1;//�����ړ��x�N�g��
	vy;	//�����ړ��x�N�g��	�W�����v�p
	vz = 0;//�����ړ��x�N�g��


	v = INITIV;//�ړ����x
	jump = 3;//�W�����v�̍���
	jumpflag = 0;//�W�����v���Ă��邩	�X�y�[�X�L�[
	k = 1;//���x�W��					���L�[
	rightflag = 0;//�E�ɋȂ����Ă��邩	�E���L�[
	leftflag = 0;//���ɋȂ����Ă��邩	�����L�[
	bflag = 0;//�����V���Ă��邩		���L�[
	tflag = 0;//�~�܂��Ă��邩			���L�[
	viewpointflag = 0;//���_	0:���Ձ@1:��l��

	variation1 = VARIATION1; //�֐ߑ��x���Ђ�
	variation2 = VARIATION2;//�֐ߑ��x�E�Ђ�
	variation3 = VARIATION3;//�֐ߑ��x���Ђ�
	variation4 = VARIATION4;//�֐ߑ��x�E�Ђ�
	variation5 = 0.005;//�̂̏㉺��
	ybody = 0;//�̂̏㉺

	cross = 0;//�O��
	arg = 0;//�Ȃ��p
	bpx = 0;//x�x�N�g��
	bpz = 0;//z�x�N�g��
	l = 0;//�x�N�g���̑傫��

}

//�v���C���[������
void People::setallies()
{
	body = 0;		 //���֐�
	rilegup = 0;	 //�E���t�����֐�
	rilegmidle = 0;	 //�E�G�֐�
	rilegdown = 0;	 //�E����֐�
	lelegup = 0;	 //�����t�����֐�
	lelegmidle = 0;	 //���G�֐�
	lelegdown = 0;	 //������֐�
	riarmup = 0;	 //�E���֐�
	riarmdown = 0;	 //�E�I�֐�
	learmup = 0;	 //�����֐�
	learmdown = 0;	 //���I�֐�
	head = 0;		 //��֐�

	all = 0;//�̂̌���
	vbody = INITIROTEV;//�̂̌����̋Ȃ���p�x
	xposi = -15;//�ړ��x�N�g��
	yposi = 0;//�ړ��x�N�g��
	zposi = dist2(engine);//�ړ��x�N�g��
	xlength = 0;//x���ړ�����
	zlength = 0;//z���ړ�����
	xcood = -15;//x�����ړ�����
	zcood = zposi;//z�����ړ�����
	xflag = 1;//�ړ�������	�ړ������v�Z�p
	zflag = 1;//�ړ�������	�ړ������v�Z�p
	vx = 1;//�����ړ��x�N�g��
	vy;	//�����ړ��x�N�g��	�W�����v�p
	vz = 0;//�����ړ��x�N�g��

	v = INITIV;//�ړ����x
	jump = 3;//�W�����v�̍���
	jumpflag = 0;//�W�����v���Ă��邩	�X�y�[�X�L�[
	k = 1;//���x�W��					���L�[
	rightflag = 0;//�E�ɋȂ����Ă��邩	�E���L�[
	leftflag = 0;//���ɋȂ����Ă��邩	�����L�[
	bflag = 0;//�����V���Ă��邩		���L�[
	tflag = 0;//�~�܂��Ă��邩			���L�[
	viewpointflag = 0;//���_	0:���Ձ@1:��l��

	variation1 = VARIATION1; //�֐ߑ��x���Ђ�
	variation2 = VARIATION2;//�֐ߑ��x�E�Ђ�
	variation3 = VARIATION3;//�֐ߑ��x���Ђ�
	variation4 = VARIATION4;//�֐ߑ��x�E�Ђ�
	variation5 = 0.005;//�̂̏㉺��
	ybody = 0;//�̂̏㉺
}

//�v���C���[�\��
void People::drow()
{
	//�ړ�
	glTranslated(xposi, yposi, zposi);
	//�̂̌���
	glRotated(all * 57, 0, 1, 0);
	//�̂̑傫��
	glScaled(BODYSIZE, BODYSIZE, BODYSIZE);
	//�㔼�g
	glPushMatrix();
	{
		//��
		glTranslated(0, 8 + ybody, 0);
		glRotated(body, 0, 0, 1);
		glTranslated(0, 3, 0);
		glPushMatrix();
		glScaled(1.5, 3, 2);
		glutSolidCube(2);
		glPopMatrix();//��

		//��
		glTranslated(0, 4, 0);
		glRotated(head, 0, 0, 1);
		glPushMatrix();
		glScaled(1, 1, 1);
		glutSolidCube(2);
		glPopMatrix();//��

		//�r
		glPushMatrix();
		{
			//���r
			glPushMatrix();
			{
				//��r
				glTranslated(0, -1.5, -2.7);
				glRotated(riarmup, 0, 0, 1);
				glTranslated(0, -1.3, 0);
				glPushMatrix();
				glRotated(5, 1, 0, 0);
				glScaled(ARMWIDTH, ARMHEIGHTUP, ARMWIDTH);
				glutSolidCube(2);
				glPopMatrix();//��r
				//�O�r
				glTranslated(0, -1.9, 0);
				glRotated(riarmdown, 0, 0, 1);
				glTranslated(0, -1.1, -0.2);
				glPushMatrix();
				glScaled(ARMWIDTH, ARMHEIGHTDOWN, ARMWIDTH);
				glutSolidCube(2);
				glPopMatrix();//�O�r
			}
			glPopMatrix();//���r

			//�E�r
			glPushMatrix();
			{
				//��r
				glTranslated(0, -1.5, 2.7);
				glRotated(learmup, 0, 0, 1);
				glTranslated(0, -1.3, 0);
				glPushMatrix();
				glRotated(-5, 1, 0, 0);
				glScaled(ARMWIDTH, ARMHEIGHTUP, ARMWIDTH);
				glutSolidCube(2);
				glPopMatrix();//��r
				//�O�r
				glTranslated(0, -1.9, 0);
				glRotated(learmdown, 0, 0, 1);
				glTranslated(0, -1.1, 0.2);
				glPushMatrix();
				glScaled(ARMWIDTH, ARMHEIGHTDOWN, ARMWIDTH);
				glutSolidCube(2);
				glPopMatrix();//�O�r
			}
			glPopMatrix();//�E�r
		}
		glPopMatrix();//�r
	}
	glPopMatrix();//�㔼�g
	//�����g
	glPushMatrix();
	{
		//����
		glPushMatrix();
		{
			//��
			glTranslated(0, 7.6, -1);
			glRotated(rilegup, 0, 0, 1);
			glTranslated(0, -1.4, 0);
			glPushMatrix();
			glScaled(LEGWIDTH, LEGHEIGTHUP, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//��
			//��
			glTranslated(0, -2.0, 0);
			glRotated(rilegmidle, 0, 0, 1);
			glTranslated(0, -1.7, 0);
			glPushMatrix();
			glScaled(LEGWIDTH, LEGHEIGHTDOWN, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//��
			//��
			glPushMatrix();
			glTranslated(-0.7, -2, 0);
			glRotated(rilegdown, 0, 0, 1);
			glTranslated(1, 0, 0);
			glScaled(1.2, 0.3, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//��
		}
		glPopMatrix();//����

		//�E��
		glPushMatrix();
		{
			//��
			glTranslated(0, 7.6, 1);
			glRotated(lelegup, 0, 0, 1);
			glTranslated(0, -1.4, 0);
			glPushMatrix();
			glScaled(LEGWIDTH, LEGHEIGTHUP, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//��
			//��
			glTranslated(0, -2.0, 0);
			glRotated(lelegmidle, 0, 0, 1);
			glTranslated(0, -1.7, 0);
			glPushMatrix();
			glScaled(LEGWIDTH, LEGHEIGHTDOWN, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//��
			//��
			glPushMatrix();
			glTranslated(-0.7, -2, 0);
			glRotated(lelegdown, 0, 0, 1);
			glTranslated(1, 0, 0);
			glScaled(1.2, 0.3, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//��
		}
		glPopMatrix();//�E��
	}
	glPopMatrix();//�����g

}

//CMP�\��
void People::drowcmp()
{
	
	//�ړ�
	glTranslated(xposi, yposi, zposi);
	//�̂̌���
	glRotated(all * 57, 0, 1, 0);
	//�̂̑傫��
	glScaled(BODYSIZE, BODYSIZE, BODYSIZE);
	//�㔼�g
	glPushMatrix();
	{
		//��
		glTranslated(0, 8 + ybody, 0);
		glTranslated(0, 3, 0);
		glPushMatrix();
		glScaled(1.5, 3, 2);
		glutSolidCube(2);
		glPopMatrix();//��

		//��
		glTranslated(0, 4, 0);
		glRotated(head, 0, 0, 1);
		glPushMatrix();
		glScaled(1, 1, 1);
		glutSolidCube(2);
		glPopMatrix();//��

		//�r
		glPushMatrix();
		{
			//���r
			glPushMatrix();
			{
				//��r
				glTranslated(0, -1.5, -2.7);
				glRotated(riarmup, 0, 0, 1);
				glTranslated(0, -1.3, 0);
				glPushMatrix();
				glRotated(5, 1, 0, 0);
				glScaled(ARMWIDTH, ARMHEIGHTUP, ARMWIDTH);
				glutSolidCube(2);
				glPopMatrix();//��r
				//�O�r
				glTranslated(0, -1.9, 0);
				glRotated(riarmdown, 0, 0, 1);
				glTranslated(0, -1.1, -0.2);
				glPushMatrix();
				glScaled(ARMWIDTH, ARMHEIGHTDOWN, ARMWIDTH);
				glutSolidCube(2);
				glPopMatrix();//�O�r
			}
			glPopMatrix();//���r

			//�E�r
			glPushMatrix();
			{
				//��r
				glTranslated(0, -1.5, 2.7);
				glRotated(learmup, 0, 0, 1);
				glTranslated(0, -1.3, 0);
				glPushMatrix();
				glRotated(-5, 1, 0, 0);
				glScaled(ARMWIDTH, ARMHEIGHTUP, ARMWIDTH);
				glutSolidCube(2);
				glPopMatrix();//��r
				//�O�r
				glTranslated(0, -1.9, 0);
				glRotated(learmdown, 0, 0, 1);
				glTranslated(0, -1.1, 0.2);
				glPushMatrix();
				glScaled(ARMWIDTH, ARMHEIGHTDOWN, ARMWIDTH);
				glutSolidCube(2);
				glPopMatrix();//�O�r
			}
			glPopMatrix();//�E�r
		}
		glPopMatrix();//�r
	}
	glPopMatrix();//�㔼�g
	//�����g
	glPushMatrix();
	{
		//����
		glPushMatrix();
		{
			//��
			glTranslated(0, 7.6, -1);
			glRotated(rilegup, 0, 0, 1);
			glTranslated(0, -1.4, 0);
			glPushMatrix();
			glScaled(LEGWIDTH, LEGHEIGTHUP, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//��
			//��
			glTranslated(0, -2.0, 0);
			glRotated(rilegmidle, 0, 0, 1);
			glTranslated(0, -1.7, 0);
			glPushMatrix();
			glScaled(LEGWIDTH, LEGHEIGHTDOWN, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//��
			//��
			glPushMatrix();
			glTranslated(-0.7, -2, 0);
			glRotated(rilegdown, 0, 0, 1);
			glTranslated(1, 0, 0);
			glScaled(1.2, 0.3, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//��
		}
		glPopMatrix();//����

		//�E��
		glPushMatrix();
		{
			//��
			glTranslated(0, 7.6, 1);
			glRotated(lelegup, 0, 0, 1);
			glTranslated(0, -1.4, 0);
			glPushMatrix();
			glScaled(LEGWIDTH, LEGHEIGTHUP, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//��
			//��
			glTranslated(0, -2.0, 0);
			glRotated(lelegmidle, 0, 0, 1);
			glTranslated(0, -1.7, 0);
			glPushMatrix();
			glScaled(LEGWIDTH, LEGHEIGHTDOWN, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//��
			//��
			glPushMatrix();
			glTranslated(-0.7, -2, 0);
			glRotated(lelegdown, 0, 0, 1);
			glTranslated(1, 0, 0);
			glScaled(1.2, 0.3, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//��
		}
		glPopMatrix();//�E��
	}
	glPopMatrix();//�����g
}

//b����
void People::bdown()
{
	if (jumpflag == 0 && tflag == 0) {
		bflag = 2;
		variation1 = 0;
		variation2 = 0;
		variation3 = 0;
		variation4 = 0;
		variation5 = 0;
		ybody = 0;
		v = 0;
		if (body > -40) {
			body = fmod(body - 2, 360) * 1.5;
		}
	}
}

//t����
void People::tdown()
{
	if (bflag == 0) {
		if (tflag == 0)tflag = 1;
		if (tflag == 1) {//��~
			variation1 = 0;
			variation2 = 0;
			variation3 = 0;
			variation4 = 0;
			variation5 = 0;
			ybody = 0;
			v = 0;
			vbody = 0;
		}
		else if (tflag == 2) {//�ĊJ
			tflag = 3;
			variation1 = VARIATION1;
			variation2 = VARIATION2;
			variation3 = VARIATION3;
			variation4 = VARIATION4;
			variation5 = 0.005;
			vbody = INITIROTEV;
			v = INITIV;
		}
	}
}

//f����
void People::fdown()
{
	k = 2;
}

//space����
void People::spacedown()
{
	if (jumpflag != 1)
	{
		if (bflag == 0) {
			jumpflag = 1;
			vy = 0.3;
		}
	}
}

//left����
void People::leftdown()
{
	if (tflag == 0) leftflag = 1;
}

//right����
void People::rightdown()
{
	if (tflag == 0) rightflag = 1;
}

//b����
void People::bup()
{
	if (tflag == 0) {
		bflag = 1;
	}
}

//t����
void People::tup()
{
	if (tflag == 1) {
		tflag = 2;
	}
	else if (tflag == 3) {
		tflag = 0;
	}
}

//f����
void People::fup()
{
	k = 1;
}

//left����
void People::leftup()
{
	leftflag = 0;
}

//right����
void People::rightup()
{
	rightflag = 0;
}

//�����V
void People::bow()
{
	if (bflag == 1) {
		body = fmod(body + 2, 360);
		if (body > 0) {
			body = 0;
			variation1 = VARIATION1;
			variation2 = VARIATION2;
			variation3 = VARIATION3;
			variation4 = VARIATION4;
			variation5 = 0.005;
			v = INITIV;
			bflag = 0;
		}
	}
}

//�E��]
void People::rightturn()
{
	if (rightflag == 1)
	{
		all = all - vbody;
		vx = cos(all);
		vz = -sin(all);
	}
}

//����]
void People::leftturn()
{
	if (leftflag == 1)
	{
		all = all + vbody;
		vx = cos(all);
		vz = -sin(all);
	}
}

//�v���C���[����(����)
void People::moveplayer(double fieldx, double fieldz)
{
	//�ړ������̃x�N�g���ύX
	xlength = vx * v * k;
	zlength = vz * v * k;

	//�ړ������v�Z���ړ��ł��邩
	//x�������ɂɈړ��ł��邩
	if (xcood >= fieldx) {
		xflag = 0;
		if (xlength > 0) xlength = 0;
	}
	else if (xcood <= -fieldx) {
		xflag = 0;
		if (xlength < 0) xlength = 0;
	}
	else {
		xflag = 1;
	}
	//z�������Ɉړ��ł��邩
	if (zcood >= fieldz) {
		zflag = 0;
		if (zlength > 0) zlength = 0;
	}
	else if (zcood <= -fieldz) {
		zflag = 0;
		if (zlength < 0) zlength = 0;
	}
	else {
		zflag = 1;
	}
	xcood += xlength;//x���̈ړ�����(���W)
	zcood += zlength;//z���̈ړ�����(���W)

	//�ړ�
	xposi += xlength * xflag;
	zposi += zlength * zflag;

}

//�֐ߓ���
void People::jointmove()
{
	//�����֐�
	{
		variation1 = fabs(rilegup) >= 40 ? variation1 * -1 : variation1;
		//�t����
		rilegup = fmod(rilegup + variation1 * k, 360);
		//�G
		rilegmidle = fmod(rilegmidle - variation1, 360);
		rilegmidle = rilegup <= 5 ? -10 : rilegmidle;
		//����
		rilegdown = fmod(rilegdown - variation1 / 2, 360);
		rilegdown = rilegup >= -15 ? 0 : rilegdown;
		//���Z�b�g
		if (variation1 == 0)
		{
			rilegup = 0;
			rilegmidle = 0;
			rilegdown = 0;
		}
	}
	//�E���֐�
	{
		variation2 = fabs(lelegup) >= 40 ? variation2 * -1 : variation2;
		//�t����
		lelegup = fmod(lelegup + variation2 * k, 360);
		//�G
		lelegmidle = fmod(lelegmidle - variation2, 360);
		lelegmidle = lelegup <= 5 ? -10 : lelegmidle;
		//����
		lelegdown = fmod(lelegdown - variation2 / 2, 360);
		lelegdown = lelegup >= -15 ? 0 : lelegdown;
		//���Z�b�g
		if (variation2 == 0)
		{
			lelegup = 0;
			lelegmidle = 0;
			lelegdown = 0;
		}
	}
	//���r�֐�
	{
		variation3 = fabs(riarmup) >= 20 ? variation3 * -1 : variation3;
		//��
		riarmup = fmod(riarmup + variation3 / 2 * k, 360);
		//�I
		riarmdown = fmod(riarmdown + variation3 * 0.8, 360);
		riarmdown = riarmup <= 15 ? 10 : riarmdown;
		//���Z�b�g
		if (variation3 == 0)
		{
			riarmup = 0;
			riarmdown = 0;
		}
	}
	//�E�r�֐�
	{
		variation4 = fabs(learmup) >= 20 ? variation4 * -1 : variation4;
		//��
		learmup = fmod(learmup + variation4 / 2 * k, 360);
		//�I
		learmdown = fmod(learmdown + variation4 * 0.8, 360);
		learmdown = learmup <= 15 ? 10 : learmdown;
		//���Z�b�g
		if (variation4 == 0)
		{
			learmup = 0;
			learmdown = 0;
		}
	}
	//�̂̏㉺�^��
	if (jumpflag == 0)
	{
		if (fabs(rilegup) == 0 || fabs(rilegup) >= 40) {
			variation5 *= -1;
		}
		ybody += variation5;
	}
}

//�W�����v
void People::jumping()
{
	if (jumpflag == 1)
	{
		yposi += vy;
		vy -= 9.8 * 0.001;//�d��
		//�֐�
		rilegup = 50;
		rilegmidle = -100;
		rilegdown = 0;
		lelegup = 50;
		lelegmidle = -100;
		lelegdown = 0;
		riarmup = 60;
		riarmdown = 110;
		learmup = 60;
		learmdown = 110;
		//�̂̏㉺�^����~
		variation5 = 0;

		if (yposi <= -1.0)//�W�����v�I���i���n�j
		{
			yposi = 0;//���Z�b�g
			jumpflag = 0;
			if (tflag == 0) variation5 = 0.005;//�̂̏㉺�^��
			ybody = 0;
			//�֐߃��Z�b�g
			rilegup = 0;
			rilegmidle = 0;
			rilegdown = 0;
			lelegup = 0;
			lelegmidle = 0;
			lelegdown = 0;
			riarmup = 0;
			riarmdown = 0;
			learmup = 0;
			learmdown = 0;
		}
	}
}

//�{�[���Ƃ̓����蔻��
bool People::btop(double ballx, double bally, double ballz)
{
	if ((pow(ballx - xcood, 2) + pow(bally - ybody, 2) + pow(ballz - zcood, 2)) < 13)
	{
		return TRUE;
	}
	else {
		return FALSE;
	}
}
People player1, player2;

//setup
void setup()
{
	player1.setallies();
	player2.setenemy();
}

//�}�E�X�N���b�N
void myMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseflag = 1;
	}
	else 
	{
		mouseflag = 0;
	}
	glutPostRedisplay();
}

//�L�[����
void myKeyboard(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
	switch (key)
	{
	case 'b'://�����V����
		player1.bdown();
		break;
	case 't'://��~�F�ĊJ
		player1.tdown();
		break;
	case 'f'://����
		player1.fdown();
		break;
	case ' '://�W�����v
		player1.spacedown();
		player2.spacedown();
		break;

	default:
		break;
	}
	glutPostRedisplay();//�R�[���o�b�N
}

//�L�[����
void myKeyboardup(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'b'://�̂����̎p���ɖ߂�
		player1.bup();
		break;
	case 't'://t�������o�O�h�~
		player1.tup();
		break;
	case 'f'://���Ƃ̑���
		player1.fup();
		break;
	default:
		break;
	}
	glutPostRedisplay();//�R�[���o�b�N
}

//����L�[����
void mySpecialKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT: // ���L�[	�ړ������ύX
		player1.leftdown();
		break;
	case GLUT_KEY_RIGHT: // �E�L�[�@�ړ������ύX
		player1.rightdown();
		break;
	case GLUT_KEY_F1://F1�L�[�@���_�ύX
		if (f1_flag == 0) {
			f1_flag = 1;
			viewpointflag = viewpointflag == 1 ? 0 : 1;
		}
		break;
	default:
		break;
	}

	glutPostRedisplay();//�R�[���o�b�N
}

//����L�[����
void mySpecialKeyup(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:// ���L�[	�����ϊ��I��
		player1.leftup();
		break;
	case GLUT_KEY_RIGHT:// �E�L�[	�����ϊ��I��
		player1.rightup();
		break;
	case GLUT_KEY_F1://F1�L�[�@�������h�~
		f1_flag = 0;
		break;
	default:
		break;
	}
}

//���Z�b�g
void reset()
{
	ballv = 0;//�{�[���̑���
	ballvx = 0;//�{�[��x���̌���
	ballvy = 0;
	ballvz = 0;//�{�[��z���̌���
	ballx = 0;//�{�[��x�����W
	bally = 2;//�{�[��y�����W
	ballz = 0;//�{�[��z�����W
	balldownv = 0.8;//�{�[��������
	ballarg = 0;//�{�[����]�p
	ballargx = 0;//�{�[����x�]��
	ballargz = 0;//�{�[����z�]��
	ballcos = 0;
	ballsin = 0;
	tx = 0;
	tz = 0;

	goalx = 50;

	point1 = 0;
	point2 = 0;

	player1.setallies();
	player2.setenemy();
}

//�n��
void ground(int x, int y, int z){
	glNormal3d(0, 1, 0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-x, 0, -z);
	glTexCoord2f(0.0, 1.0); glVertex3f(-x, 0, z);
	glTexCoord2f(1.0, 1.0); glVertex3f(x, 0, z);
	glTexCoord2f(1.0, 0.0); glVertex3f(x, 0, -z);
	glEnd();
}

//�^�C�}
void myTime(int value)
{
	if (value == 1)
	{
		glutTimerFunc(10, myTime, 1);

		//�����V��߂�
		player1.bow();

		//�ړ������E�̂̌���
		//�E��]
		player1.rightturn();
		//����]
		player1.leftturn();

		//�ړ�
		player1.moveplayer(fieldx, fieldz);
		player2.movecmp(fieldx, fieldz, goalx, ballx, ballz);

		//�֐�
		player2.jointmove();
		player1.jointmove();
		
		//�W�����v��
		player1.jumping();
		
		//�{�[��
		if (player1.btop(ballx, bally, ballz) && player2.btop(ballx, bally, ballz)) {
			
			ballx = dist(engine);
			ballz = dist(engine);
		}
		else if(player1.btop(ballx, bally, ballz))
		{
			ballx = player1.xposi + player1.vx * 3;
			ballz = player1.zposi + player1.vz * 3;
			ballvx = player1.xlength;//�{�[���̏���x
			ballvz = player1.zlength;//�{�[���̏���z
			ballcos = player1.vx;
			ballsin = player1.vz;

			if (mouseflag == 1)
			{
				ballvx *= 5;
				ballvz *= 5;
				ballvy = 0.4;
			}
			ballv = sqrt(pow(ballvx, 2) + pow(ballvz, 2));
		}else if (player2.btop(ballx, bally, ballz))
		{
			ballx = player2.xposi + player2.vx * 3;
			ballz = player2.zposi + player2.vz * 3;
			ballvx = player2.xlength;//�{�[���̏���x
			ballvz = player2.zlength;//�{�[���̏���z
			ballcos = player2.vx;
			ballsin = player2.vz;

			if (sqrt(pow(player2.xposi + goalx,2))< 20) {
				ballvx *= 5;
				ballvz *= 5;
				ballvy = 0.4;
			}
			ballv = sqrt(pow(ballvx, 2) + pow(ballvz, 2));
		}
		else {
			if (ballv > 0) {
				ballv -= balldownv;
				if (ballv < 0) ballv = 0;
			}

			tx = fabs(ballcos / (ballcos + ballsin)) * 0.01;
			tz = fabs(ballsin / (ballcos + ballsin)) * 0.01;

			if (ballvx > 0) {
				ballvx -= balldownv * tx;
				if (ballvx <= 0) ballvx = 0;
			}
			else if (ballvx < 0) {
				ballvx += balldownv * tx;
				if (ballvx >= 0)ballvx = 0;
			}
						

			if (ballvz > 0) {
				ballvz -= balldownv * tz;
				if (ballvz <= 0) ballvz = 0;
			}
			else if (ballvz < 0) {
				ballvz += balldownv * tz;
				if (ballvz >= 0)ballvz = 0;
			}
		}
		if (ballvy != 0) {
			ballvy -= 9.8 * 0.001;
			if (bally < 2) {
				ballvy *= 0.5;
				if (ballvy < 0.05) {
					ballvy = 0;
				}
			}
		}

		ballx += ballvx;//�{�[��
		bally += ballvy;
		ballz += ballvz;
		ballarg = ballv*10;
		if (ballx != 0) ballargx = 1; else ballargx = 0;
		if (ballz != 0) ballargz = 1; else ballargz = 0;

		if (ballz > fieldz || ballz < -fieldz) {
			ballvz *= -1;
			if (ballvz == 0) ballvz = 1;
		}
		if (ballx > fieldx || ballx < -fieldx) {
			if (ballz > 7 || ballz < -7) {
				ballvx *= -1;
				if (ballvx == 0) ballvx = 1;
			}
			if (ballz < 7 && ballz > -7) {
				if (ballx > 0)point1 += 1;
				if (ballx < 0)point2 += 1;
				ballx = 0;
				ballz = 0;
				ballvx = 0;
				ballvz = 0;

				if (point1 == 10 || point2 == 10) {
					reset();
				}
			}
		}

	}
	glutPostRedisplay();//�R�[���o�b�N
}

//�f�B�X�v���C
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//�S��
	glPushMatrix();
	{
		//���_�E����	0:���Ձ@1:��l��
		if (viewpointflag == 0) gluLookAt(player1.xposi, 60, 45 + player1.zposi, player1.xposi, 0, player1.zposi, 0, 1, 0);
		//if (viewpointflag == 0) gluLookAt(player2.xposi, 60, 45 + player2.zposi, player2.xposi, 0, player2.zposi, 0, 1, 0);
		if (viewpointflag == 1) {
			glTranslated(0, -7*0.9 , 0);
			glTranslated(0, 8.5, 4);
			gluLookAt(player1.xposi, 17 + player1.yposi, player1.zposi, player1.xposi + player1.vx, 16.5 + player1.yposi + player1.ybody / 4, player1.zposi + player1.vz, 0, 1, 0);
		}

		//�����ݒu
		glLightfv(GL_LIGHT0, GL_POSITION, light0_posi);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, whiten);

		//�{�[��
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		{
			glTranslated(ballx, bally, ballz);
			glBindTexture(GL_TEXTURE_2D, texname[0]);
			glPushMatrix();
			{
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
				glutSolidSphere(2, 20, 20);
				glDisable(GL_TEXTURE_GEN_S);
				glDisable(GL_TEXTURE_GEN_T);
			}
			glPopMatrix();
		}
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

		//�n�ʕ`��
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		{
			glBindTexture(GL_TEXTURE_2D, texname[1]);
			ground(57,0,37);
			glFlush();
		}
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

		//���{�b�g2//�F�i�j���{�b�g
		glMaterialfv(GL_FRONT, GL_AMBIENT, blues_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, blues_diffuse);
		glPushMatrix();
		player2.drowcmp();
		glPopMatrix();//���{�b�g2

		//���{�b�g1//�F�i�ԁj���{�b�g
		glMaterialfv(GL_FRONT, GL_AMBIENT, reds_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, reds_diffuse);
		glPushMatrix();
		player1.drow();
		glPopMatrix();//���{�b�g1

		//�t�B�[���h�p
		glPushMatrix();
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT, yellows_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, yellows_diffuse);
			//1
			glPushMatrix();
			{
				glTranslatef(50, 5, 30);
				glScalef(0.5, 10, 0.5);
				glutSolidCube(1);
			}
			glPopMatrix();//1
			//2
			glPushMatrix();
			{
				glTranslatef(50, 5, -30);
				glScalef(0.5, 10, 0.5);
				glutSolidCube(1);
			}
			glPopMatrix();//2
			//3
			glPushMatrix();
			{
				glTranslatef(-50, 5, -30);
				glScalef(0.5, 10, 0.5);
				glutSolidCube(1);
			}
			glPopMatrix();//3
			//4
			glPushMatrix();
			{
				glTranslatef(-50, 5, 30);
				glScalef(0.5, 10, 0.5);
				glutSolidCube(1);
			}
			glPopMatrix();//4
		}
		glPopMatrix();
		//�S�[��1
		glPushMatrix();
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT, blues_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, blues_diffuse);
			//�|�[��1
			glPushMatrix();
			{
				glTranslatef(50, 5, 7);
				glScalef(0.5, 10, 0.5);
				glutSolidCube(1);
			}
			glPopMatrix();//�|�[��1
			//�|�[��2
			glPushMatrix();
			{
				glTranslatef(50, 5, -7);
				glScalef(0.5, 10, 0.5);
				glutSolidCube(1);
			}
			glPopMatrix();//�|�[��2
			//�|�[��3
			glPushMatrix();
			{
				glTranslatef(50, 10, 0);
				glScalef(0.5, 0.5, 14.5);
				glutSolidCube(1);
			}
			glPopMatrix();//�|�[��3
		}
		glPopMatrix();//�S�[��1
		//�S�[��2
		glPushMatrix();
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT, reds_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, reds_diffuse);
			//�|�[��1
			glPushMatrix();
			{
				glTranslatef(-50, 5, 7);
				glScalef(0.5, 10, 0.5);
				glutSolidCube(1);
			}
			glPopMatrix();//�|�[��1
			//�|�[��2
			glPushMatrix();
			{
				glTranslatef(-50, 5, -7);
				glScalef(0.5, 10, 0.5);
				glutSolidCube(1);
			}
			glPopMatrix();//�|�[��2
			//�|�[��3
			glPushMatrix();
			{
				glTranslatef(-50, 10, 0);
				glScalef(0.5, 0.5, 14.5);
				glutSolidCube(1);
			}
			glPopMatrix();//�|�[��3
		}
		glPopMatrix();//�S�[��2

		//�����`��
		glPushMatrix();
		glColor3d(0, 0, 0);
		glRasterPos3d(-10, 0, -30);
		sprintf(text, "red:%d : blue:%d", point1,point2);
		for (int i = 0; i < strlen(text); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
		}
		glPopMatrix();
	}
	glPopMatrix();//�S��

	glutSwapBuffers();
}

//���j���[���e
void getvalue(int value) {
	switch (value)
	{
	case 1:
		reset();
		break;
	default:
		break;
	}
}

//���j���[
void mySetMenu()
{
	glutCreateMenu(getvalue);
	glutAddMenuEntry("restart", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

//ppm�ǂݍ���
void setupTextures(GLuint texID, const char* filename)
{
	FILE* fp;
	int  ch, i;

	if (fopen_s(&fp, filename, "r") != 0) {
		fprintf(stderr, "Cannot open ppm file %s\n", filename);
		exit(1);
	}
	
	for (i = 0; i < 4; i++) {
		while (1) {
			if ((ch = fgetc(fp)) != '#') break;
			fgets((char*)texImage, 1024, fp);
			while (isspace(ch)) ch = fgetc(fp);
		}
		while (!isspace(ch)) ch = fgetc(fp);
		if (i < 3) {
			while (isspace(ch)) ch = fgetc(fp);
		}
	}
	fread(texImage, 1, imageWidth * imageHeight * 3, fp);
	fclose(fp);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0,
		GL_RGB, GL_UNSIGNED_BYTE, texImage);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);


}

//���T�C�Y
void myReshape(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)width / (double)height, 0.1, 120.0/*�`�拗��*/);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}
//�����ݒ�
void myInit(char* report2)
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(report2);
	glClearColor(1, 1, 1, 0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);
	glutKeyboardUpFunc(myKeyboardup);
	glutSpecialFunc(mySpecialKey);
	glutSpecialUpFunc(mySpecialKeyup);
	glutTimerFunc(10, myTime, 1);
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);

	mySetMenu();
	glGenTextures(2, texname);
	setupTextures(texname[0], "aaa1.ppm");
	setupTextures(texname[1], "ddd.ppm");
	setup();
	glutMainLoop();
	return 0;
}
