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
	lifes = 3;
	money = 0;
	score = 0;
	ring = 0;
}

cGame::~cGame(void)
{
}

bool cGame::Init()
{
	bool res=true;

	//Graphics initialization
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(cam_x, cam_x+256, cam_y, cam_y+192);
	glMatrixMode(GL_MODELVIEW);
	
	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	//Scene initialization
	res = Data.LoadImage(IMG_TILES,"Pantalla01.png",GL_RGBA);
	if(!res) return false;
	res = Scene.LoadLevel(3);
	if(!res) return false;

	cam_x = 0; cam_y = 1920; //s'ha de llegir de loadlevel

	//Player initialization
	res = Data.LoadImage(IMG_PLAYER,"Alex.png",GL_RGBA);
	if(!res) return false;
	Player.SetTile(3,113); //init position
	Player.SetState(STATE_LOOKRIGHT);
	Player.Swim();

	res = Data.LoadImageA(IMG_ENEMY, "Monsters.png", GL_RGBA);
	if(!res) return false;
	
	res = Data.LoadImageA(IMG_BLOCKS, "blocks.png", GL_RGBA);
	if(!res) return false;

	//Creem monstres i blocks de prova
	cPtero* Ptero = new cPtero();
	Ptero->SetTile(8,113);

	cSFish* SFish = new cSFish();
	SFish->SetTile(10,108);
	
	cGhost* Ghost = new cGhost();
	Ghost->SetTile(10,111);

	cMiniboss* Miniboss = new cMiniboss();
	Miniboss->SetTile(10,107);

	cBlock* Block1 = new cBlock();
	Block1->SetTile(5,113);

	cBlock* Block2 = new cBlock();
	Block2->SetTile(10,113);
	Block2->SetState(R_GREEN);

	cBlock* Box1 = new cBlock();
	Box1->SetTile(1,113);
	Box1->SetState(STAR);
	Box1->SetTreasure(BMON);

	cBlock* Box2 = new cBlock();
	Box2->SetTile(6,112);
	Box2->SetState(SKULL_P);

	cBlock* Box3 = new cBlock();
	Box3->SetTile(0,115);
	Box3->SetState(SKULL);

	monsters.push_back(Ptero);
	//monsters.push_back(SFish);
	//monsters.push_back(Ghost);
	//monsters.push_back(Miniboss);

	blocks.push_back(Block1);
	blocks.push_back(Block2);
	blocks.push_back(Box1);
	blocks.push_back(Box2);
	blocks.push_back(Box3);

	PlaySound(TEXT("Sounds/03-Main_Theme.wav"), NULL, SND_ASYNC | SND_LOOP);

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
	if(key=='c' && !press) jump_key=false;
	if(key=='x' && !press) punch_key=false;
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
		Player.Crouch(Scene.GetMap(),blocks);					
		keypressed=true;
	}

	if(keys['c'] && !jump_key) 
	{
		Player.Jump(Scene.GetMap()); 
		jump_key=true;		
		keypressed=true;
	}

	if(keys['x'] && !punch_key)
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
		if(monsters[i]->Appears(cam_x, cam_y)) monsters[i]->Logic(Scene.GetMap(), Player, blocks);
	Player.Logic(Scene.GetMap(),monsters, blocks);
	for(unsigned int i = 0; i < blocks.size(); i++)
		if(blocks[i]->Appears(cam_x, cam_y)) blocks[i]->Logic(Player,money,ring,lifes,monsters);

	return res;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	int play_x, play_y;
	Player.GetPosition(play_x, play_y);
	
	if( (play_x - cam_x) > (CAM_WIDTH / 2) ) cam_x = play_x - CAM_WIDTH / 2;
	if( (play_y - cam_y) < (CAM_HEIGHT / 2) ) cam_y = play_y - CAM_HEIGHT / 2;
	
	int level_width = 256; //momentani, ha de llegir de rectangle del nivell
	int level_height = 120 * 16; //momentani, ha de llegir de rectangle del nivell
	cam_x = max(0, cam_x);
	cam_y = max(0, cam_y);
	cam_x = min(cam_x, level_width - CAM_WIDTH);
	cam_y = min(cam_y, level_height - CAM_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluOrtho2D(cam_x, cam_x + CAM_WIDTH, cam_y, cam_y + CAM_HEIGHT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	Scene.Draw(Data.GetID(IMG_TILES));
	for(unsigned int i = 0; i < blocks.size(); i++) 
		if(blocks[i]->Appears(cam_x, cam_y)) blocks[i]->Draw(Data.GetID(IMG_BLOCKS));
	for(unsigned int i = 0; i < monsters.size(); i++) 
		if(monsters[i]->Appears(cam_x, cam_y)) monsters[i]->Draw(Data.GetID(IMG_ENEMY));
	Player.Draw(Data.GetID(IMG_PLAYER));

	glutSwapBuffers();
}
