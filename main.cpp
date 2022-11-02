#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <random>

#define LEGWIDTH 0.8//足の太さ
#define LEGHEIGTHUP 2//足の膝上の長さ
#define LEGHEIGHTDOWN 1.7//足の膝下の長さ
#define ARMWIDTH 0.7//腕の太さ
#define ARMHEIGHTUP 1.6//上腕長さ
#define ARMHEIGHTDOWN 1.4//前腕長さ

#define imageWidth		256
#define imageHeight		256
#define INITIROTEV 0.05	//回転初速
#define	INITIV 0.1	//移動初速
#define VARIATION1 1.4; //関節速度左ひざ
#define VARIATION2 -1.4;//関節速度右ひざ
#define VARIATION3 -1.4;//関節速度左ひじ
#define VARIATION4 1.4;//関節速度右ひじ
#define BODYSIZE 0.8//体の大きさ

std::mt19937 engine;
std::uniform_real_distribution<> dist2(-10, 10);//ランダム変数2
std::uniform_real_distribution<> dist(0, 10);//ランダム変数

//テクスチャを入れる
unsigned char texImage[imageHeight][imageWidth][4];
GLuint texname[2];//テクスチャの数

char text[50];//描画テキスト

int width = 1000, height = 600;//ウィンドウサイズ

int mouseflag = 0;//マウスフラグ(左クリック)

double fieldx = 48;//フィールドx軸
double fieldz = 28;//フィールドz軸
int f1_flag = 0;//視点を変えたか		F1キー
int viewpointflag = 0;//視点	0:俯瞰　1:一人称

double ballv = 0;//ボールの速さ
double ballvx = 0;//ボールx軸の向き
double ballvy = 0;
double ballvz = 0;//ボールz軸の向き
double ballx = 0;//ボールx軸座標
double bally = 2;//ボールy軸座標
double ballz = 0;//ボールz軸座標
double balldownv = 0.8;//ボール減速率
double ballarg = 0;//ボール回転角
double ballargx = 0;//ボール回x転軸
double ballargz = 0;//ボール回z転軸
double ballcos = 0;//ボールx軸回転
double ballsin = 0;//ボールz軸回転
double tx = 0;//進む比率
double tz = 0;//進む比率

double goalx = 50;//ゴール座標

int point1 = 0;//プレイヤー点数
int point2 = 0;//CMP点数

float light0_posi[] = { 100.0, 100.0, 100.0, 1.0 };//光源の位置

GLfloat whiten[] = { 1,1,1,1 };//地面の色（一応）
GLfloat reds_ambient[] = { 0,0,0,1 };//材質の環境・赤
GLfloat reds_diffuse[] = { 1,0,0,1 };//材質の拡散・赤
GLfloat blues_ambient[] = { 0,0,0,1 };//材質の環境・青
GLfloat blues_diffuse[] = { 0,0,1,1 };//材質の拡散・青
GLfloat yellows_ambient[] = { 0,0,0,1 };//材質の環境・黄
GLfloat yellows_diffuse[] = { 1,1,0,1 };//材質の拡散・黄

//プレイヤークラス
class People
{
public:
	double body;		//腰関節
	double rilegup;		//右足付け根関節
	double rilegmidle;	//右膝関節
	double rilegdown;	//右足首関節
	double lelegup;		//左足付け根関節
	double lelegmidle;	//左膝関節
	double lelegdown;	//左足首関節
	double riarmup;		//右肩関節
	double riarmdown;	//右肘関節
	double learmup;		//左肩関節
	double learmdown;	//左肘関節
	double head;		//首関節
	double all;//体の向き
	double vbody;//体の向きの曲がる角度
	double xposi;//移動ベクトル
	double yposi;//移動ベクトル
	double zposi;//移動ベクトル
	double xlength;//x軸移動距離
	double zlength;//z軸移動距離
	double xcood;//x軸総移動距離
	double zcood;//z軸総移動距離
	int xflag;//移動したか	移動距離計算用
	int zflag;//移動したか	移動距離計算用
	double vx;//初期移動ベクトル
	double vy;	//初期移動ベクトル	ジャンプ用
	double vz;//初期移動ベクトル

	double v;//移動速度
	double jump;//ジャンプの高さ
	int jumpflag;//ジャンプしているか	スペースキー
	int k;//速度係数					ｆキー
	int rightflag;//右に曲がっているか	右矢印キー
	int leftflag;//左に曲がっているか	左矢印キー
	int bflag;//お辞儀しているか		ｂキー
	int tflag;//止まっているか			ｔキー
	int viewpointflag;//視点	0:俯瞰　1:一人称

	double variation1; //関節速度左ひざ
	double variation2;//関節速度右ひざ
	double variation3;//関節速度左ひじ
	double variation4;//関節速度右ひじ
	double variation5;//体の上下量
	double ybody;//体の上下

	double cross;//右か左か
	double arg;//回転角度
	double bpx;//目標方向ベクトルx
	double bpz;//目標方向ベクトルz
	double l;//ベクトルの大きさ
	double bpxl;//ベクトル正規化x
	double bpzl;//ベクトル正規化z

	
	void setallies();//プレイヤー初期化
	void setenemy();//CMP初期化
	void drow();//プレイヤー表示
	void drowcmp();//CMP表示
	
	void bdown();//b押下
	void tdown();//t押下
	void fdown();//f押下
	void spacedown();//space押下
	void leftdown();//left押下
	void rightdown();//right押下

	void bup();//b離す
	void tup();//t離す
	void fup();//f離す
	void leftup();//left離す
	void rightup();//right離す

	void bow();//お辞儀
	void rightturn();//右回転
	void leftturn();//左回転

	void movecmp(double fieldx, double fieldz, double goalx, double ballx, double ballz);//CMP動作
	void moveplayer(double fieldx, double fieldz);//プレイヤー動作
	void jointmove();//関節動作
	void jumping();//ジャンプ

	bool btop(double ballx, double bally, double ballz);//ボールとの当たり判定


};

void People::movecmp(double fieldx, double fieldz, double goalx, double ballx, double ballz)
{
	double goalz = 1;
	
	if (btop(ballx, 0, ballz)) {//ボールに触れていたら
		bpx = (-goalx - xposi);//xベクトル
		bpz = (-goalz - zposi);//zベクトル
		l = sqrt(pow(bpx, 2) + pow(bpz, 2));//ベクトルの大きさ
		bpxl = bpx / l;//xベクトル正規化
		bpzl = bpz / l;//zベクトル正規化
		arg = acos((vx * bpxl + vz * bpzl) / (sqrt(pow(vx, 2) + pow(vz, 2)) * sqrt(pow(bpxl, 2) + pow(bpzl, 2)))) + 0.000001;//なす角
		cross = vx * -goalz - vz * -goalx;//右か左か
		
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
	else {//ボールに触れていなかったら
		bpx = (ballx+0.5 - xposi);//xベクトル
		bpz = (ballz+0.5 - zposi);//zベクトル
		l = sqrt(pow(bpx, 2) + pow(bpz, 2));//ベクトルの大きさ
		bpxl = bpx / l;//xベクトル正規化
		bpzl = bpz / l;//zベクトル正規化
		arg = acos((vx * bpxl + vz * bpzl) / (sqrt(pow(vx, 2) + pow(vz, 2)) * sqrt(pow(bpxl, 2) + pow(bpzl, 2)))) + 0.000001;//なす角
		cross = vx * bpzl - vz * bpzl;//右か左か
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
	//移動方向のベクトル変更
	xlength = vx * v * k;
	zlength = vz * v * k;

	//移動距離計算＆移動できるか
	//x軸方向にに移動できるか
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
	//z軸方向に移動できるか
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
	xcood += xlength;//x軸の移動距離(座標)
	zcood += zlength;//z軸の移動距離(座標)

	//移動
	xposi += xlength * xflag;
	zposi += zlength * zflag;
}

//CMP初期化
void People::setenemy()
{
	body = 0;		 //腰関節
	rilegup = 0;	 //右足付け根関節
	rilegmidle = 0;	 //右膝関節
	rilegdown = 0;	 //右足首関節
	lelegup = 0;	 //左足付け根関節
	lelegmidle = 0;	 //左膝関節
	lelegdown = 0;	 //左足首関節
	riarmup = 0;	 //右肩関節
	riarmdown = 0;	 //右肘関節
	learmup = 0;	 //左肩関節
	learmdown = 0;	 //左肘関節
	head = 0;		 //首関節

	all = 180;//体の向き
	vbody = INITIROTEV;//体の向きの曲がる角速度
	xposi = 15;//移動ベクトル
	yposi = 0;//移動ベクトル
	zposi = dist2(engine);//移動ベクトル
	xlength = 0;//x軸移動距離
	zlength = 0;//z軸移動距離
	xcood = 15;//x軸総移動距離
	zcood = zposi;//z軸総移動距離
	xflag = 1;//移動したか	移動距離計算用
	zflag = 1;//移動したか	移動距離計算用
	vx = -1;//初期移動ベクトル
	vy;	//初期移動ベクトル	ジャンプ用
	vz = 0;//初期移動ベクトル


	v = INITIV;//移動速度
	jump = 3;//ジャンプの高さ
	jumpflag = 0;//ジャンプしているか	スペースキー
	k = 1;//速度係数					ｆキー
	rightflag = 0;//右に曲がっているか	右矢印キー
	leftflag = 0;//左に曲がっているか	左矢印キー
	bflag = 0;//お辞儀しているか		ｂキー
	tflag = 0;//止まっているか			ｔキー
	viewpointflag = 0;//視点	0:俯瞰　1:一人称

	variation1 = VARIATION1; //関節速度左ひざ
	variation2 = VARIATION2;//関節速度右ひざ
	variation3 = VARIATION3;//関節速度左ひじ
	variation4 = VARIATION4;//関節速度右ひじ
	variation5 = 0.005;//体の上下量
	ybody = 0;//体の上下

	cross = 0;//外積
	arg = 0;//なす角
	bpx = 0;//xベクトル
	bpz = 0;//zベクトル
	l = 0;//ベクトルの大きさ

}

//プレイヤー初期化
void People::setallies()
{
	body = 0;		 //腰関節
	rilegup = 0;	 //右足付け根関節
	rilegmidle = 0;	 //右膝関節
	rilegdown = 0;	 //右足首関節
	lelegup = 0;	 //左足付け根関節
	lelegmidle = 0;	 //左膝関節
	lelegdown = 0;	 //左足首関節
	riarmup = 0;	 //右肩関節
	riarmdown = 0;	 //右肘関節
	learmup = 0;	 //左肩関節
	learmdown = 0;	 //左肘関節
	head = 0;		 //首関節

	all = 0;//体の向き
	vbody = INITIROTEV;//体の向きの曲がる角度
	xposi = -15;//移動ベクトル
	yposi = 0;//移動ベクトル
	zposi = dist2(engine);//移動ベクトル
	xlength = 0;//x軸移動距離
	zlength = 0;//z軸移動距離
	xcood = -15;//x軸総移動距離
	zcood = zposi;//z軸総移動距離
	xflag = 1;//移動したか	移動距離計算用
	zflag = 1;//移動したか	移動距離計算用
	vx = 1;//初期移動ベクトル
	vy;	//初期移動ベクトル	ジャンプ用
	vz = 0;//初期移動ベクトル

	v = INITIV;//移動速度
	jump = 3;//ジャンプの高さ
	jumpflag = 0;//ジャンプしているか	スペースキー
	k = 1;//速度係数					ｆキー
	rightflag = 0;//右に曲がっているか	右矢印キー
	leftflag = 0;//左に曲がっているか	左矢印キー
	bflag = 0;//お辞儀しているか		ｂキー
	tflag = 0;//止まっているか			ｔキー
	viewpointflag = 0;//視点	0:俯瞰　1:一人称

	variation1 = VARIATION1; //関節速度左ひざ
	variation2 = VARIATION2;//関節速度右ひざ
	variation3 = VARIATION3;//関節速度左ひじ
	variation4 = VARIATION4;//関節速度右ひじ
	variation5 = 0.005;//体の上下量
	ybody = 0;//体の上下
}

//プレイヤー表示
void People::drow()
{
	//移動
	glTranslated(xposi, yposi, zposi);
	//体の向き
	glRotated(all * 57, 0, 1, 0);
	//体の大きさ
	glScaled(BODYSIZE, BODYSIZE, BODYSIZE);
	//上半身
	glPushMatrix();
	{
		//体
		glTranslated(0, 8 + ybody, 0);
		glRotated(body, 0, 0, 1);
		glTranslated(0, 3, 0);
		glPushMatrix();
		glScaled(1.5, 3, 2);
		glutSolidCube(2);
		glPopMatrix();//体

		//頭
		glTranslated(0, 4, 0);
		glRotated(head, 0, 0, 1);
		glPushMatrix();
		glScaled(1, 1, 1);
		glutSolidCube(2);
		glPopMatrix();//頭

		//腕
		glPushMatrix();
		{
			//左腕
			glPushMatrix();
			{
				//上腕
				glTranslated(0, -1.5, -2.7);
				glRotated(riarmup, 0, 0, 1);
				glTranslated(0, -1.3, 0);
				glPushMatrix();
				glRotated(5, 1, 0, 0);
				glScaled(ARMWIDTH, ARMHEIGHTUP, ARMWIDTH);
				glutSolidCube(2);
				glPopMatrix();//上腕
				//前腕
				glTranslated(0, -1.9, 0);
				glRotated(riarmdown, 0, 0, 1);
				glTranslated(0, -1.1, -0.2);
				glPushMatrix();
				glScaled(ARMWIDTH, ARMHEIGHTDOWN, ARMWIDTH);
				glutSolidCube(2);
				glPopMatrix();//前腕
			}
			glPopMatrix();//左腕

			//右腕
			glPushMatrix();
			{
				//上腕
				glTranslated(0, -1.5, 2.7);
				glRotated(learmup, 0, 0, 1);
				glTranslated(0, -1.3, 0);
				glPushMatrix();
				glRotated(-5, 1, 0, 0);
				glScaled(ARMWIDTH, ARMHEIGHTUP, ARMWIDTH);
				glutSolidCube(2);
				glPopMatrix();//上腕
				//前腕
				glTranslated(0, -1.9, 0);
				glRotated(learmdown, 0, 0, 1);
				glTranslated(0, -1.1, 0.2);
				glPushMatrix();
				glScaled(ARMWIDTH, ARMHEIGHTDOWN, ARMWIDTH);
				glutSolidCube(2);
				glPopMatrix();//前腕
			}
			glPopMatrix();//右腕
		}
		glPopMatrix();//腕
	}
	glPopMatrix();//上半身
	//下半身
	glPushMatrix();
	{
		//左足
		glPushMatrix();
		{
			//上
			glTranslated(0, 7.6, -1);
			glRotated(rilegup, 0, 0, 1);
			glTranslated(0, -1.4, 0);
			glPushMatrix();
			glScaled(LEGWIDTH, LEGHEIGTHUP, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//上
			//中
			glTranslated(0, -2.0, 0);
			glRotated(rilegmidle, 0, 0, 1);
			glTranslated(0, -1.7, 0);
			glPushMatrix();
			glScaled(LEGWIDTH, LEGHEIGHTDOWN, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//中
			//下
			glPushMatrix();
			glTranslated(-0.7, -2, 0);
			glRotated(rilegdown, 0, 0, 1);
			glTranslated(1, 0, 0);
			glScaled(1.2, 0.3, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//下
		}
		glPopMatrix();//左足

		//右足
		glPushMatrix();
		{
			//上
			glTranslated(0, 7.6, 1);
			glRotated(lelegup, 0, 0, 1);
			glTranslated(0, -1.4, 0);
			glPushMatrix();
			glScaled(LEGWIDTH, LEGHEIGTHUP, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//上
			//中
			glTranslated(0, -2.0, 0);
			glRotated(lelegmidle, 0, 0, 1);
			glTranslated(0, -1.7, 0);
			glPushMatrix();
			glScaled(LEGWIDTH, LEGHEIGHTDOWN, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//中
			//下
			glPushMatrix();
			glTranslated(-0.7, -2, 0);
			glRotated(lelegdown, 0, 0, 1);
			glTranslated(1, 0, 0);
			glScaled(1.2, 0.3, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//下
		}
		glPopMatrix();//右足
	}
	glPopMatrix();//下半身

}

//CMP表示
void People::drowcmp()
{
	
	//移動
	glTranslated(xposi, yposi, zposi);
	//体の向き
	glRotated(all * 57, 0, 1, 0);
	//体の大きさ
	glScaled(BODYSIZE, BODYSIZE, BODYSIZE);
	//上半身
	glPushMatrix();
	{
		//体
		glTranslated(0, 8 + ybody, 0);
		glTranslated(0, 3, 0);
		glPushMatrix();
		glScaled(1.5, 3, 2);
		glutSolidCube(2);
		glPopMatrix();//体

		//頭
		glTranslated(0, 4, 0);
		glRotated(head, 0, 0, 1);
		glPushMatrix();
		glScaled(1, 1, 1);
		glutSolidCube(2);
		glPopMatrix();//頭

		//腕
		glPushMatrix();
		{
			//左腕
			glPushMatrix();
			{
				//上腕
				glTranslated(0, -1.5, -2.7);
				glRotated(riarmup, 0, 0, 1);
				glTranslated(0, -1.3, 0);
				glPushMatrix();
				glRotated(5, 1, 0, 0);
				glScaled(ARMWIDTH, ARMHEIGHTUP, ARMWIDTH);
				glutSolidCube(2);
				glPopMatrix();//上腕
				//前腕
				glTranslated(0, -1.9, 0);
				glRotated(riarmdown, 0, 0, 1);
				glTranslated(0, -1.1, -0.2);
				glPushMatrix();
				glScaled(ARMWIDTH, ARMHEIGHTDOWN, ARMWIDTH);
				glutSolidCube(2);
				glPopMatrix();//前腕
			}
			glPopMatrix();//左腕

			//右腕
			glPushMatrix();
			{
				//上腕
				glTranslated(0, -1.5, 2.7);
				glRotated(learmup, 0, 0, 1);
				glTranslated(0, -1.3, 0);
				glPushMatrix();
				glRotated(-5, 1, 0, 0);
				glScaled(ARMWIDTH, ARMHEIGHTUP, ARMWIDTH);
				glutSolidCube(2);
				glPopMatrix();//上腕
				//前腕
				glTranslated(0, -1.9, 0);
				glRotated(learmdown, 0, 0, 1);
				glTranslated(0, -1.1, 0.2);
				glPushMatrix();
				glScaled(ARMWIDTH, ARMHEIGHTDOWN, ARMWIDTH);
				glutSolidCube(2);
				glPopMatrix();//前腕
			}
			glPopMatrix();//右腕
		}
		glPopMatrix();//腕
	}
	glPopMatrix();//上半身
	//下半身
	glPushMatrix();
	{
		//左足
		glPushMatrix();
		{
			//上
			glTranslated(0, 7.6, -1);
			glRotated(rilegup, 0, 0, 1);
			glTranslated(0, -1.4, 0);
			glPushMatrix();
			glScaled(LEGWIDTH, LEGHEIGTHUP, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//上
			//中
			glTranslated(0, -2.0, 0);
			glRotated(rilegmidle, 0, 0, 1);
			glTranslated(0, -1.7, 0);
			glPushMatrix();
			glScaled(LEGWIDTH, LEGHEIGHTDOWN, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//中
			//下
			glPushMatrix();
			glTranslated(-0.7, -2, 0);
			glRotated(rilegdown, 0, 0, 1);
			glTranslated(1, 0, 0);
			glScaled(1.2, 0.3, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//下
		}
		glPopMatrix();//左足

		//右足
		glPushMatrix();
		{
			//上
			glTranslated(0, 7.6, 1);
			glRotated(lelegup, 0, 0, 1);
			glTranslated(0, -1.4, 0);
			glPushMatrix();
			glScaled(LEGWIDTH, LEGHEIGTHUP, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//上
			//中
			glTranslated(0, -2.0, 0);
			glRotated(lelegmidle, 0, 0, 1);
			glTranslated(0, -1.7, 0);
			glPushMatrix();
			glScaled(LEGWIDTH, LEGHEIGHTDOWN, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//中
			//下
			glPushMatrix();
			glTranslated(-0.7, -2, 0);
			glRotated(lelegdown, 0, 0, 1);
			glTranslated(1, 0, 0);
			glScaled(1.2, 0.3, LEGWIDTH);
			glutSolidCube(2);
			glPopMatrix();//下
		}
		glPopMatrix();//右足
	}
	glPopMatrix();//下半身
}

//b押下
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

//t押下
void People::tdown()
{
	if (bflag == 0) {
		if (tflag == 0)tflag = 1;
		if (tflag == 1) {//停止
			variation1 = 0;
			variation2 = 0;
			variation3 = 0;
			variation4 = 0;
			variation5 = 0;
			ybody = 0;
			v = 0;
			vbody = 0;
		}
		else if (tflag == 2) {//再開
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

//f押下
void People::fdown()
{
	k = 2;
}

//space押下
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

//left押下
void People::leftdown()
{
	if (tflag == 0) leftflag = 1;
}

//right押下
void People::rightdown()
{
	if (tflag == 0) rightflag = 1;
}

//b押下
void People::bup()
{
	if (tflag == 0) {
		bflag = 1;
	}
}

//t押下
void People::tup()
{
	if (tflag == 1) {
		tflag = 2;
	}
	else if (tflag == 3) {
		tflag = 0;
	}
}

//f押下
void People::fup()
{
	k = 1;
}

//left押下
void People::leftup()
{
	leftflag = 0;
}

//right押下
void People::rightup()
{
	rightflag = 0;
}

//お辞儀
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

//右回転
void People::rightturn()
{
	if (rightflag == 1)
	{
		all = all - vbody;
		vx = cos(all);
		vz = -sin(all);
	}
}

//左回転
void People::leftturn()
{
	if (leftflag == 1)
	{
		all = all + vbody;
		vx = cos(all);
		vz = -sin(all);
	}
}

//プレイヤー動作(自分)
void People::moveplayer(double fieldx, double fieldz)
{
	//移動方向のベクトル変更
	xlength = vx * v * k;
	zlength = vz * v * k;

	//移動距離計算＆移動できるか
	//x軸方向にに移動できるか
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
	//z軸方向に移動できるか
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
	xcood += xlength;//x軸の移動距離(座標)
	zcood += zlength;//z軸の移動距離(座標)

	//移動
	xposi += xlength * xflag;
	zposi += zlength * zflag;

}

//関節動作
void People::jointmove()
{
	//左足関節
	{
		variation1 = fabs(rilegup) >= 40 ? variation1 * -1 : variation1;
		//付け根
		rilegup = fmod(rilegup + variation1 * k, 360);
		//膝
		rilegmidle = fmod(rilegmidle - variation1, 360);
		rilegmidle = rilegup <= 5 ? -10 : rilegmidle;
		//足首
		rilegdown = fmod(rilegdown - variation1 / 2, 360);
		rilegdown = rilegup >= -15 ? 0 : rilegdown;
		//リセット
		if (variation1 == 0)
		{
			rilegup = 0;
			rilegmidle = 0;
			rilegdown = 0;
		}
	}
	//右足関節
	{
		variation2 = fabs(lelegup) >= 40 ? variation2 * -1 : variation2;
		//付け根
		lelegup = fmod(lelegup + variation2 * k, 360);
		//膝
		lelegmidle = fmod(lelegmidle - variation2, 360);
		lelegmidle = lelegup <= 5 ? -10 : lelegmidle;
		//足首
		lelegdown = fmod(lelegdown - variation2 / 2, 360);
		lelegdown = lelegup >= -15 ? 0 : lelegdown;
		//リセット
		if (variation2 == 0)
		{
			lelegup = 0;
			lelegmidle = 0;
			lelegdown = 0;
		}
	}
	//左腕関節
	{
		variation3 = fabs(riarmup) >= 20 ? variation3 * -1 : variation3;
		//肩
		riarmup = fmod(riarmup + variation3 / 2 * k, 360);
		//肘
		riarmdown = fmod(riarmdown + variation3 * 0.8, 360);
		riarmdown = riarmup <= 15 ? 10 : riarmdown;
		//リセット
		if (variation3 == 0)
		{
			riarmup = 0;
			riarmdown = 0;
		}
	}
	//右腕関節
	{
		variation4 = fabs(learmup) >= 20 ? variation4 * -1 : variation4;
		//肩
		learmup = fmod(learmup + variation4 / 2 * k, 360);
		//肘
		learmdown = fmod(learmdown + variation4 * 0.8, 360);
		learmdown = learmup <= 15 ? 10 : learmdown;
		//リセット
		if (variation4 == 0)
		{
			learmup = 0;
			learmdown = 0;
		}
	}
	//体の上下運動
	if (jumpflag == 0)
	{
		if (fabs(rilegup) == 0 || fabs(rilegup) >= 40) {
			variation5 *= -1;
		}
		ybody += variation5;
	}
}

//ジャンプ
void People::jumping()
{
	if (jumpflag == 1)
	{
		yposi += vy;
		vy -= 9.8 * 0.001;//重力
		//関節
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
		//体の上下運動停止
		variation5 = 0;

		if (yposi <= -1.0)//ジャンプ終わる（着地）
		{
			yposi = 0;//リセット
			jumpflag = 0;
			if (tflag == 0) variation5 = 0.005;//体の上下運動
			ybody = 0;
			//関節リセット
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

//ボールとの当たり判定
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

//マウスクリック
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

//キー入力
void myKeyboard(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
	switch (key)
	{
	case 'b'://お辞儀する
		player1.bdown();
		break;
	case 't'://停止：再開
		player1.tdown();
		break;
	case 'f'://加速
		player1.fdown();
		break;
	case ' '://ジャンプ
		player1.spacedown();
		player2.spacedown();
		break;

	default:
		break;
	}
	glutPostRedisplay();//コールバック
}

//キー離す
void myKeyboardup(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'b'://体を元の姿勢に戻す
		player1.bup();
		break;
	case 't'://t長押しバグ防止
		player1.tup();
		break;
	case 'f'://もとの速さ
		player1.fup();
		break;
	default:
		break;
	}
	glutPostRedisplay();//コールバック
}

//特殊キー入力
void mySpecialKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT: // 左キー	移動方向変更
		player1.leftdown();
		break;
	case GLUT_KEY_RIGHT: // 右キー　移動方向変更
		player1.rightdown();
		break;
	case GLUT_KEY_F1://F1キー　視点変更
		if (f1_flag == 0) {
			f1_flag = 1;
			viewpointflag = viewpointflag == 1 ? 0 : 1;
		}
		break;
	default:
		break;
	}

	glutPostRedisplay();//コールバック
}

//特殊キー離す
void mySpecialKeyup(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:// 左キー	方向変換終了
		player1.leftup();
		break;
	case GLUT_KEY_RIGHT:// 右キー	方向変換終了
		player1.rightup();
		break;
	case GLUT_KEY_F1://F1キー　長押し防止
		f1_flag = 0;
		break;
	default:
		break;
	}
}

//リセット
void reset()
{
	ballv = 0;//ボールの速さ
	ballvx = 0;//ボールx軸の向き
	ballvy = 0;
	ballvz = 0;//ボールz軸の向き
	ballx = 0;//ボールx軸座標
	bally = 2;//ボールy軸座標
	ballz = 0;//ボールz軸座標
	balldownv = 0.8;//ボール減速率
	ballarg = 0;//ボール回転角
	ballargx = 0;//ボール回x転軸
	ballargz = 0;//ボール回z転軸
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

//地面
void ground(int x, int y, int z){
	glNormal3d(0, 1, 0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-x, 0, -z);
	glTexCoord2f(0.0, 1.0); glVertex3f(-x, 0, z);
	glTexCoord2f(1.0, 1.0); glVertex3f(x, 0, z);
	glTexCoord2f(1.0, 0.0); glVertex3f(x, 0, -z);
	glEnd();
}

//タイマ
void myTime(int value)
{
	if (value == 1)
	{
		glutTimerFunc(10, myTime, 1);

		//お辞儀を戻す
		player1.bow();

		//移動方向・体の向き
		//右回転
		player1.rightturn();
		//左回転
		player1.leftturn();

		//移動
		player1.moveplayer(fieldx, fieldz);
		player2.movecmp(fieldx, fieldz, goalx, ballx, ballz);

		//関節
		player2.jointmove();
		player1.jointmove();
		
		//ジャンプ中
		player1.jumping();
		
		//ボール
		if (player1.btop(ballx, bally, ballz) && player2.btop(ballx, bally, ballz)) {
			
			ballx = dist(engine);
			ballz = dist(engine);
		}
		else if(player1.btop(ballx, bally, ballz))
		{
			ballx = player1.xposi + player1.vx * 3;
			ballz = player1.zposi + player1.vz * 3;
			ballvx = player1.xlength;//ボールの初速x
			ballvz = player1.zlength;//ボールの初速z
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
			ballvx = player2.xlength;//ボールの初速x
			ballvz = player2.zlength;//ボールの初速z
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

		ballx += ballvx;//ボール
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
	glutPostRedisplay();//コールバック
}

//ディスプレイ
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//全体
	glPushMatrix();
	{
		//視点・視野	0:俯瞰　1:一人称
		if (viewpointflag == 0) gluLookAt(player1.xposi, 60, 45 + player1.zposi, player1.xposi, 0, player1.zposi, 0, 1, 0);
		//if (viewpointflag == 0) gluLookAt(player2.xposi, 60, 45 + player2.zposi, player2.xposi, 0, player2.zposi, 0, 1, 0);
		if (viewpointflag == 1) {
			glTranslated(0, -7*0.9 , 0);
			glTranslated(0, 8.5, 4);
			gluLookAt(player1.xposi, 17 + player1.yposi, player1.zposi, player1.xposi + player1.vx, 16.5 + player1.yposi + player1.ybody / 4, player1.zposi + player1.vz, 0, 1, 0);
		}

		//光源設置
		glLightfv(GL_LIGHT0, GL_POSITION, light0_posi);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, whiten);

		//ボール
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

		//地面描画
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		{
			glBindTexture(GL_TEXTURE_2D, texname[1]);
			ground(57,0,37);
			glFlush();
		}
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

		//ロボット2//色（青）ロボット
		glMaterialfv(GL_FRONT, GL_AMBIENT, blues_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, blues_diffuse);
		glPushMatrix();
		player2.drowcmp();
		glPopMatrix();//ロボット2

		//ロボット1//色（赤）ロボット
		glMaterialfv(GL_FRONT, GL_AMBIENT, reds_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, reds_diffuse);
		glPushMatrix();
		player1.drow();
		glPopMatrix();//ロボット1

		//フィールド角
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
		//ゴール1
		glPushMatrix();
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT, blues_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, blues_diffuse);
			//ポール1
			glPushMatrix();
			{
				glTranslatef(50, 5, 7);
				glScalef(0.5, 10, 0.5);
				glutSolidCube(1);
			}
			glPopMatrix();//ポール1
			//ポール2
			glPushMatrix();
			{
				glTranslatef(50, 5, -7);
				glScalef(0.5, 10, 0.5);
				glutSolidCube(1);
			}
			glPopMatrix();//ポール2
			//ポール3
			glPushMatrix();
			{
				glTranslatef(50, 10, 0);
				glScalef(0.5, 0.5, 14.5);
				glutSolidCube(1);
			}
			glPopMatrix();//ポール3
		}
		glPopMatrix();//ゴール1
		//ゴール2
		glPushMatrix();
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT, reds_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, reds_diffuse);
			//ポール1
			glPushMatrix();
			{
				glTranslatef(-50, 5, 7);
				glScalef(0.5, 10, 0.5);
				glutSolidCube(1);
			}
			glPopMatrix();//ポール1
			//ポール2
			glPushMatrix();
			{
				glTranslatef(-50, 5, -7);
				glScalef(0.5, 10, 0.5);
				glutSolidCube(1);
			}
			glPopMatrix();//ポール2
			//ポール3
			glPushMatrix();
			{
				glTranslatef(-50, 10, 0);
				glScalef(0.5, 0.5, 14.5);
				glutSolidCube(1);
			}
			glPopMatrix();//ポール3
		}
		glPopMatrix();//ゴール2

		//文字描画
		glPushMatrix();
		glColor3d(0, 0, 0);
		glRasterPos3d(-10, 0, -30);
		sprintf(text, "red:%d : blue:%d", point1,point2);
		for (int i = 0; i < strlen(text); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
		}
		glPopMatrix();
	}
	glPopMatrix();//全体

	glutSwapBuffers();
}

//メニュー内容
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

//メニュー
void mySetMenu()
{
	glutCreateMenu(getvalue);
	glutAddMenuEntry("restart", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

//ppm読み込み
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

//リサイズ
void myReshape(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)width / (double)height, 0.1, 120.0/*描画距離*/);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}
//初期設定
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
