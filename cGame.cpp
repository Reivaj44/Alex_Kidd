#include "cGame.h"
#include "Globals.h"
#include "cPtero.h"
#include "cSFish.h"
#include "cGhost.h"
#include "cFrog.h"
#include "cMiniboss.h"

cGame::cGame(void)
{
	jump_key = false;
	punch_key = false;
	up_key = false;
	down_key = false;
	reappears = false;
	delay = 0;
	lifes = 3;
	money = 0;
	score = 0;
	ring = 0;
	rectangle = 0;
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
	gluOrtho2D(cam.left, cam.right, cam.bottom, cam.top);
	glMatrixMode(GL_MODELVIEW);
	
	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	//Scene initialization
	/**/
	//PlaySound(TEXT("Sounds/01-Title_Screen.wav"), NULL, SND_ASYNC); // CACTUS: activar
	res = InitIntro();

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
	if(key==GLUT_KEY_DOWN && !press) down_key = false;
	if(key==GLUT_KEY_UP && !press) up_key = false;
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
			if(keys['x'] && !punch_key) {
				switch (option) {
					case IMG_INSTRUC:
						res = InitInstrucc();
						break;
					case IMG_CREDITS:
						res = InitCredits();
						break;
					default:
						res = InitMap(1);
						//res = InitLevel1();
						break;
				}
				punch_key=true;
			}

			if(keys[GLUT_KEY_UP] && !up_key) 
			{		
				if(option==0)
					option = 2;
				else
					option--;
				up_key = true;
			}
			else if(keys[GLUT_KEY_DOWN] && !down_key) 
			{		
				option = (option + 1)%3;
				down_key = true;
			}

			break;
		case IMG_INSTRUC:
			if(keys['x'] && !punch_key) {
				res = InitIntro();
				punch_key=true;
			}
			break;
		case IMG_CREDITS:
			if(keys['x'] && !punch_key) {
				res = InitIntro();
				punch_key=true;
			}
			break;
		case IMG_MAP:
			if(keys['x'] && !punch_key) {
				res = InitLevel1();
				punch_key=true;
			}
			break;
		default:
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
				Player.MoveLeft(Scene.GetMap(), blocks, GetBorder(Player));	
				keypressed=true;
			}

			else if(keys[GLUT_KEY_RIGHT]) 
			{	
				Player.MoveRight(Scene.GetMap(), blocks, GetBorder(Player));	
				keypressed=true;
			}

			if(!keypressed) Player.Stop();
			
			
			//Game Logic
			if(Player.isDead() && !Player.Appears(cam)) 
			{
				Player.Resurrect(3,113);
				cam.left = Scene.GetRectangles(0)->left; //valor inicial rectangle
				cam.bottom = Scene.GetRectangles(0)->top-CAM_HEIGHT; //valor inicial rectangle
				reappears = true;
				delay = 0;
			}

			if(!reappears && delay%20==0)
			{
				int rect = GetRectanglePlayer(Player);
				if(!Player.isSwimming() && Scene.GetIsWater(rect)==1) {
					Player.Swim();
				}
				for(unsigned int i = 0; i < monsters.size(); i++)
					if(monsters[i]->Appears(cam)) monsters[i]->Logic(Scene.GetMap(), Player, blocks, GetBorder(Player));
				Player.Logic(Scene.GetMap(),monsters, blocks, GetBorder(Player));
				for(unsigned int i = 0; i < blocks.size(); i++)
					if(blocks[i]->Appears(cam)) blocks[i]->Logic(Player,money,ring,lifes,monsters);
			}

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
		case IMG_INTRO:
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(-320,320,-240,240);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,Data.GetID(IMG_INTRO));
			glBegin(GL_QUADS);
				glTexCoord2f(0.0,1.0);	glVertex2i(-320,-240);
				glTexCoord2f(1.0,1.0);	glVertex2i(320,-240);
				glTexCoord2f(1.0,0.0);	glVertex2i(320,240);
				glTexCoord2f(0.0,0.0);	glVertex2i(-320,240);
			glEnd();
			switch (option) {
				case 0: //30,-63
					arr_x = 30;
					arr_y = -63;
					break;
				case 1: //-60,-130
					arr_x = -60;
					arr_y = -130;
					break;
				case 2: //8,-195
					arr_x = 8;
					arr_y = -195;
					break;
			}
			glBindTexture(GL_TEXTURE_2D,Data.GetID(IMG_ARROWS));
			glBegin(GL_QUADS);
				glTexCoord2f(0.0,1.0);	glVertex2i(arr_x-8,arr_y-8);
				glTexCoord2f(0.5,1.0);	glVertex2i(arr_x+8,arr_y-8);
				glTexCoord2f(0.5,0.0);	glVertex2i(arr_x+8,arr_y+8);
				glTexCoord2f(0.0,0.0);	glVertex2i(arr_x-8,arr_y+8);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			break;
		case IMG_INSTRUC:
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(-320,320,-240,240);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,Data.GetID(IMG_INSTRUC));
			glBegin(GL_QUADS);
				glTexCoord2f(0.0,1.0);	glVertex2i(-320,-240);
				glTexCoord2f(1.0,1.0);	glVertex2i(320,-240);
				glTexCoord2f(1.0,0.0);	glVertex2i(320,240);
				glTexCoord2f(0.0,0.0);	glVertex2i(-320,240);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			break;
		case IMG_CREDITS:
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(-320,320,-240,240);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,Data.GetID(IMG_CREDITS));
			glBegin(GL_QUADS);
				glTexCoord2f(0.0,1.0);	glVertex2i(-320,-240);
				glTexCoord2f(1.0,1.0);	glVertex2i(320,-240);
				glTexCoord2f(1.0,0.0);	glVertex2i(320,240);
				glTexCoord2f(0.0,0.0);	glVertex2i(-320,240);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			break;
		case IMG_MAP:
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(-320,320,-240,240);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,Data.GetID(IMG_MAP));
			glBegin(GL_QUADS);
				glTexCoord2f(0.0,1.0);	glVertex2i(-320,-240);
				glTexCoord2f(1.0,1.0);	glVertex2i(320,-240);
				glTexCoord2f(1.0,0.0);	glVertex2i(320,240);
				glTexCoord2f(0.0,0.0);	glVertex2i(-320,240);
			glEnd();
			delay++;
			

			glBindTexture(GL_TEXTURE_2D,Data.GetID(IMG_ARROWS));
			glBegin(GL_QUADS);
				glTexCoord2f(0.5,1.0);	glVertex2i(arr_x-8,arr_y-8);
				glTexCoord2f(1.0,1.0);	glVertex2i(arr_x+8,arr_y-8);
				glTexCoord2f(1.0,0.0);	glVertex2i(arr_x+8,arr_y+8);
				glTexCoord2f(0.5,0.0);	glVertex2i(arr_x-8,arr_y+8);
			glEnd();

			eat_x = 260;
			eat_y = -150;
			glBindTexture(GL_TEXTURE_2D,Data.GetID(IMG_EATING));
			glBegin(GL_QUADS);
				glTexCoord2f(0.0,1.0);	glVertex2i(eat_x-32,eat_y-64);
				glTexCoord2f(0.5,1.0);	glVertex2i(eat_x+32,eat_y-64);
				glTexCoord2f(0.5,0.0);	glVertex2i(eat_x+32,eat_y+64);
				glTexCoord2f(0.0,0.0);	glVertex2i(eat_x-32,eat_y+64);
			glEnd();

			glDisable(GL_TEXTURE_2D);
			break;
		default:
			if(!reappears && delay%20==0)
			{
				int play_x, play_y;
				Player.GetPosition(play_x, play_y);

				if(rectangle<(Scene.GetNumRects()-1) && cBicho::BoxInsideBox( *(Scene.GetRectangles(rectangle+1)), cam ) ) rectangle++;
				
				int level_width = Scene.GetRectangles(rectangle)->right; 
				int level_height = Scene.GetRectangles(rectangle)->top; 
				
				if( (play_x - cam.left) > (CAM_WIDTH / 2) ) cam.left = play_x - CAM_WIDTH / 2;
				if( (play_y - cam.bottom) < (CAM_HEIGHT / 2) ) cam.bottom = play_y - CAM_HEIGHT / 2;
				cam.top = cam.bottom + CAM_HEIGHT;
				cam.right = cam.left + CAM_WIDTH;
				
				cam.bottom = max(0, cam.bottom);
				cam.left = min(cam.left, level_width - CAM_WIDTH);
				cam.top = cam.bottom + CAM_HEIGHT;
				cam.right = cam.left + CAM_WIDTH;

				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
			
				gluOrtho2D(cam.left, cam.right, cam.bottom, cam.top);
				glMatrixMode(GL_MODELVIEW);

				glLoadIdentity();

				Scene.Draw(Data.GetID(IMG_TILES));
				for(unsigned int i = 0; i < blocks.size(); i++) 
					if(blocks[i]->Appears(cam)) blocks[i]->Draw(Data.GetID(IMG_BLOCKS));
				for(unsigned int i = 0; i < monsters.size(); i++) 
					if(monsters[i]->Appears(cam)) monsters[i]->Draw(Data.GetID(IMG_ENEMY));
				Player.Draw(Data.GetID(IMG_PLAYER));
			}
			else 
			{	
				reappears = false;
				delay++;
			}
			break;
	}
	
	glutSwapBuffers();
}

bool cGame::InitIntro() {
	bool res = true;

	stage = IMG_INTRO;
	option = 0;
	res = Data.LoadImage(IMG_INTRO, "start_menu.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_ARROWS, "arrows.png",GL_RGBA);
	if(!res) return false;

	//PlaySound(TEXT("Sounds/01-Title_Screen.wav"), NULL, SND_ASYNC); // CACTUS: activar
	
	return res;
}

bool cGame::InitInstrucc() {
	bool res = true;

	stage = IMG_INSTRUC;
	option = 0;
	res = Data.LoadImage(IMG_INSTRUC, "instructions.png",GL_RGBA);
	if(!res) return false;
	
	return res;
}

bool cGame::InitCredits() {
	bool res = true;

	stage = IMG_CREDITS;
	option = 0;
	res = Data.LoadImage(IMG_CREDITS, "credits.png",GL_RGBA);
	if(!res) return false;
	
	return res;
}

bool cGame::InitMap(int lvl) {
	bool res = true;

	stage = IMG_MAP;
	delay = 0;
	switch (lvl) {
		case 1: //-139,-45
			arr_x = -139;
			arr_y = -45;
			break;
		case 2: //-154,-17
			arr_x = -154;
			arr_y = -17;
			break;
	}
	res = Data.LoadImage(IMG_MAP, "world_map.png",GL_RGBA);
	if(!res) return false;
	res = Data.LoadImage(IMG_EATING, "alex_eating.png",GL_RGBA);
	if(!res) return false;

	//PlaySound(TEXT("Sounds/02-Level_Start.wav"), NULL, SND_ASYNC); // CACTUS: activar
	
	return res;
}

bool cGame::InitLevel1() {
	bool res = true;
	stage = 4;
	res = Data.LoadImage(IMG_TILES,"Pantalla01.png",GL_RGBA);
	if(!res) return false;
	res = Scene.LoadLevel(3); // CACTUS: canviar num
	if(!res) return false;

	cam.left = Scene.GetRectangles(0)->left;
	cam.top = Scene.GetRectangles(0)->top;
	cam.bottom = cam.top - CAM_HEIGHT;
	cam.right = cam.left + CAM_WIDTH;
	
	//Player initialization
	res = Data.LoadImage(IMG_PLAYER,"Alex.png",GL_RGBA);
	if(!res) return false;
	int play_x,play_y;
	Scene.GetPlayerInitPosition(&play_x,&play_y);
	Player.SetTile(play_x,play_y); //init position
	Player.SetState(STATE_LOOKRIGHT);

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

	cFrog* Frog = new cFrog();
	Frog->SetTile(7,110);

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
	monsters.push_back(Frog);
	//monsters.push_back(SFish);
	//monsters.push_back(Ghost);
	//monsters.push_back(Miniboss);

	blocks.push_back(Block1);
	blocks.push_back(Block2);
	blocks.push_back(Box1);
	blocks.push_back(Box2);
	blocks.push_back(Box3);

	//PlaySound(TEXT("Sounds/03-Main_Theme.wav"), NULL, SND_ASYNC | SND_LOOP); // CACTUS: activar

	return res;
}

cRect cGame::GetBorder(const cPlayer &player)
{
	int p = GetRectanglePlayer(player);
	cRect playRect = *(Scene.GetRectangles(p));
	cRect camRect  = *(Scene.GetRectangles(rectangle));
	cRect aux;
	aux.top		= min(camRect.top,		playRect.top,		cam.top);
	aux.bottom	= min(camRect.bottom,	playRect.bottom);
	aux.left	= min(camRect.left,		playRect.left,		cam.left);
	aux.right	= min(camRect.right,	playRect.right);
	return aux;
}

int cGame::GetRectanglePlayer(const cPlayer &player)
{
	unsigned int i = 0; 
	bool rect_found = false;
	while (i < Scene.GetNumRects() && !rect_found) 
	{	
		if(Player.InBox( *(Scene.GetRectangles(i))) ) rect_found = true;
		else i++;
	}
	return i;
}