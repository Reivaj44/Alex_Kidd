#include "cGame.h"
#include "Globals.h"
#include "cPtero.h"
#include "cSFish.h"
#include "cGhost.h"
#include "cMiniboss.h"
#include "cBox.h"


cGame::cGame(void)
{
	jump_key = false;
	punch_key = false;
	lifes = 3;
	money = 0;
	score = 0;
}

cGame::~cGame(void)
{
}

bool cGame::Init()
{
	bool res=true;

	stage = 0;

	//Graphics initialization
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(cam_x, cam_x+256, cam_y, cam_y+192);
	glMatrixMode(GL_MODELVIEW);
	
	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	//res = Scene.LoadLevel(3);
	//if(!res) return false;
	//Scene.Draw(Data.GetID(IMG_INTRO));

	//Scene initialization
	/**/
	//PlaySound(TEXT("Sounds/01-Title_Screen.wav"), NULL, SND_ASYNC); // CACTUS: activar
	res = InitIntro(true);

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

	switch (stage) {
		case IMG_INTRO:
			if(keys['x']) {
				switch (option) {
					case IMG_INSTRUC:
						stage = option;

						break;
					case IMG_CREDITS:
						stage = option;

						break;
					default:
						stage = 4;
						res = InitLevel1();
						break;
				}
			}

			/*if(keys[GLUT_KEY_UP]) 
			{		
				option = (option - 1)%3;
			}
			if(keys[GLUT_KEY_DOWN]) 
			{		
				option = (option + 1)%3;
			}*/

			break;
		case IMG_INSTRUC:
			stage++;
			break;
		case IMG_CREDITS:
			stage++;
			break;
		default:
			bool keypressed = false;

			if(keys[GLUT_KEY_DOWN]) 
			{		
				Player.Crouch(Scene.GetMap());					
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
				monsters[i]->Logic(Scene.GetMap(), Player, blocks);
			Player.Logic(Scene.GetMap(),monsters, blocks);
			for(unsigned int i = 0; i < blocks.size(); i++)
				blocks[i]->Logic(Player,money,ring,lifes);

			break;
	}

	return res;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	bool res = true;
	
	switch (stage) {
		case 0:
			res = InitIntro(false);
			break;
		case 1:
			break;
		case 2:
			break;
		default:
			int play_x, play_y;
			Player.GetPosition(play_x, play_y);
	
			if( (play_x - cam_x) > (2 * CAM_WIDTH / 3) ) cam_x = play_x - 2 * CAM_WIDTH / 3;
			if( (play_y - cam_y) < (CAM_HEIGHT / 3) ) cam_y = play_y - CAM_HEIGHT / 3;
	
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
			for(unsigned int i = 0; i < monsters.size(); i++) 
				if(!monsters[i]->isDead()) monsters[i]->Draw(Data.GetID(IMG_ENEMY));
			for(unsigned int i = 0; i < blocks.size(); i++) 
				if(blocks[i]->Appears()) blocks[i]->Draw(Data.GetID(IMG_BLOCKS));
			Player.Draw(Data.GetID(IMG_PLAYER));
			break;
	}
	

	glutSwapBuffers();
}

//Load

bool cGame::InitIntro(bool first) {
	bool res = true;

	if (first) {
		option = 0;
		res = Data.LoadImage(IMG_INTRO, "start.png",GL_RGB);
		if(!res) return false;
	}
	else {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-320,320,-240,240);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,IMG_INTRO);
		glRotatef(180.0,0.0,1.0,0.0);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0,0.0);	glVertex2i(-320,-240);
			glTexCoord2f(1.0,0.0);	glVertex2i(320,-240);
			glTexCoord2f(1.0,1.0);	glVertex2i(320,240);
			glTexCoord2f(0.0,1.0);	glVertex2i(-320,240);
		glEnd();
		glColor3f(1.0,1.0,1.0);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	//PlaySound(TEXT("Sounds/01-Title_Screen.wav"), NULL, SND_ASYNC); // CACTUS: activar

	return res;
}
bool cGame::InitLevel1() {
	bool res = true;
	res = Data.LoadImage(IMG_TILES,"Pantalla01.png",GL_RGBA);
	if(!res) return false;
	res = Scene.LoadLevel(3); // CACTUS: canviar num
	if(!res) return false;

	cam_x = 0; cam_y = 1920; //s'ha de llegir de loadlevel

	//Player initialization
	res = Data.LoadImage(IMG_PLAYER,"Alex.png",GL_RGBA);
	if(!res) return false;
	Player.SetWidthHeight(32,32);
	Player.SetTile(3,113); //init position
	Player.SetState(STATE_LOOKRIGHT);

	res = Data.LoadImageA(IMG_ENEMY, "Monsters.png", GL_RGBA);
	if(!res) return false;
	
	res = Data.LoadImageA(IMG_BLOCKS, "blocks.png", GL_RGBA);
	if(!res) return false;

	//Creem monstres i blocks de prova
	cPtero* Ptero = new cPtero();
	Ptero->SetWidthHeight(32,32);
	Ptero->SetTile(8,113);

	cSFish* SFish = new cSFish();
	SFish->SetWidthHeight(32,32);
	SFish->SetTile(10,108);
	
	cGhost* Ghost = new cGhost();
	Ghost->SetWidthHeight(32,32);
	Ghost->SetTile(10,111);

	cMiniboss* Miniboss = new cMiniboss();
	Miniboss->SetWidthHeight(32,32);
	Miniboss->SetTile(10,113);

	cBlock* Block1 = new cBlock();
	Block1->SetWidthHeight(16,16);
	Block1->SetTile(5,113);

	cBlock* Block2 = new cBlock();
	Block2->SetWidthHeight(16,16);
	Block2->SetTile(10,113);
	Block2->SetState(2);

	cBox* Box1 = new cBox();
	Box1->SetWidthHeight(16,16);
	Box1->SetTile(2,113);
	Box1->SetState(6);
	Box1->SetTreasure(1);

	monsters.push_back(Ptero);
	//monsters.push_back(SFish);
	//monsters.push_back(Ghost);
	//monsters.push_back(Miniboss);

	blocks.push_back(Block1);
	blocks.push_back(Block2);
	blocks.push_back(Box1);

	//PlaySound(TEXT("Sounds/03-Main_Theme.wav"), NULL, SND_ASYNC | SND_LOOP); // CACTUS: activar

	return res;
}