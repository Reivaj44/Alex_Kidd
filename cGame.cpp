#include "cGame.h"
#include "Globals.h"
#include "cPtero.h"
#include "cSFish.h"
#include "cGhost.h"
#include "cMiniboss.h"


cGame::cGame(void)
{
	jump_key = false;
	punch_key = false;
}

cGame::~cGame(void)
{
}

bool cGame::Init()
{
	bool res=true;
	cam_x = 0.25;
	cam_y = 18.25;

	mov_x = 0;
	mov_y = 0;

	//Graphics initialization
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,256,0,192,0,1);
	glMatrixMode(GL_MODELVIEW);
	
	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	//Scene initialization
	res = Data.LoadImage(IMG_TILES,"Pantalla01.png",GL_RGBA);
	if(!res) return false;
	res = Scene.LoadLevel(3);
	if(!res) return false;

	//Player initialization
	res = Data.LoadImage(IMG_PLAYER,"Alex.png",GL_RGBA);
	if(!res) return false;
	Player.SetWidthHeight(32,32);
	Player.SetTile(3,113); //init position
	Player.SetState(STATE_LOOKRIGHT);

	res = Data.LoadImageA(IMG_ENEMY, "Monsters.png", GL_RGBA);
	if(!res) return false;
	
	cPtero* Ptero = new cPtero();
	Ptero->SetWidthHeight(32,32);
	Ptero->SetTile(10,110);

	cSFish* SFish = new cSFish();
	SFish->SetWidthHeight(32,32);
	SFish->SetTile(10,108);
	
	cGhost* Ghost = new cGhost();
	Ghost->SetWidthHeight(32,32);
	Ghost->SetTile(10,111);

	cMiniboss* Miniboss = new cMiniboss();
	Miniboss->SetWidthHeight(32,32);
	Miniboss->SetTile(10,113);
	
	res = Data.LoadImageA(IMG_BLOCKS, "blocks.png", GL_RGBA);
	if(!res) return false;

	cBlock* Block1 = new cBlock();
	Block1->SetWidthHeight(16,16);
	Block1->SetTile(5,113);

	cBlock* Block2 = new cBlock();
	Block2->SetWidthHeight(16,16);
	Block2->SetTile(5,114);
	Block2->SetState(2);


	//monsters.push_back(Ptero);
	//monsters.push_back(SFish);
	//monsters.push_back(Ghost);
	//monsters.push_back(Miniboss);

	blocks.push_back(Block1);
	blocks.push_back(Block2);

	return res;
}

bool cGame::Loop()
{
	int t1 = glutGet(GLUT_ELAPSED_TIME);
	int t2;
	bool res=true;

	res = Process();
	if(res) Render();
	do {
		t2 = glutGet(GLUT_ELAPSED_TIME);
	} while(t2-t1<17);
	return res;
}

void cGame::Finalize()
{
}

//Input
void cGame::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	keys[key] = press;
	if(key=='x' && !press) jump_key=false;
	if(key=='c' && !press) punch_key=false;
}

void cGame::ReadMouse(int button, int state, int x, int y)
{
}

//Process
bool cGame::Process()
{
	bool res=true;
	
	//Process Input
	if(keys[27])	res=false;
	
	bool keypressed = false;

	if(keys[GLUT_KEY_DOWN]) 
	{		
		Player.Crouch(Scene.GetMap());					
		keypressed=true;
	}

	if(keys['x'] && !jump_key) 
	{
		Player.Jump(Scene.GetMap()); 
		jump_key=true;		
		keypressed=true;
	}

	if(keys['c'] && !punch_key)
	{
		Player.Punch(Scene.GetMap());
		punch_key=true;
		keypressed=true;
	}

	if(keys[GLUT_KEY_LEFT]) 
	{		
		Player.MoveLeft(Scene.GetMap(), blocks);	
		keypressed=true;
	}

	else if(keys[GLUT_KEY_RIGHT]) 
	{	
		Player.MoveRight(Scene.GetMap(), blocks);	
		keypressed=true;
	}

	if(!keypressed) Player.Stop();
	
	
	//Game Logic
	for(unsigned int i = 0; i < monsters.size(); i++)
			monsters[i]->Logic(Scene.GetMap(), Player, blocks);
	Player.Logic(Scene.GetMap(),monsters, blocks);

	return res;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	int play_x, play_y;
	Player.GetPosition(play_x, play_y);
	float yyy = 1808-(play_y + mov_y);
	//if ((1808-(play_y + mov_y) > 2 * 192 / 3) && (cam_y+mov_y > 0.4)) mov_y -= CAM_STEP;
	if (play_y - mov_y > 2 * 192 / 3) mov_y += CAM_STEP;
	else if ((play_x - mov_x > 2 * 256 / 3) && (cam_x+mov_x < 4.24)) mov_x += CAM_STEP;
	/*if (yy < 0.995 && cam_y > 0.4) {
		init_relat_y = relat_y;
		cam_y -= CAM_STEP;
	}
	else if (relat_x < 4.24) cam_x += CAM_STEP;*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glTranslatef(-cam_x-mov_x,-cam_y+mov_y,0.0);
	glOrtho(0,256,0,192,0,1);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	Scene.Draw(Data.GetID(IMG_TILES));
	for(unsigned int i = 0; i < monsters.size(); i++) 
		if(!monsters[i]->isDead()) monsters[i]->Draw(Data.GetID(IMG_ENEMY));
	for(unsigned int i = 0; i < blocks.size(); i++) 
		if(!blocks[i]->isDestroyed()) blocks[i]->Draw(Data.GetID(IMG_BLOCKS));
	Player.Draw(Data.GetID(IMG_PLAYER));

	glutSwapBuffers();
}
