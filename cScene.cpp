#include "cScene.h"
#include "Globals.h"
#include "cPtero.h"
#include "cSFish.h"
#include "cGhost.h"
#include "cFrog.h"
#include "cMiniboss.h"
#include "cScorpion.h"

cScene::cScene(void)
{
}

cScene::~cScene(void)
{
}

bool cScene::LoadLevel(int level)
{
	bool res;
	FILE *fd;
	char file[16];
	int i,j,k,px,py,num_quads,tile_num;
	char tile;
	float coordx_tile, coordy_tile;

	res=true;

	if(level<10) sprintf(file,"%s0%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);
	else		 sprintf(file,"%s%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);

	fd=fopen(file,"r");
	if(fd==NULL) return false;

	rects.clear();
	isWater.clear();
	player_position.clear();
	monsters.clear();
	blocks.clear();

	int num_monsters;
	fscanf(fd, "%d", &num_monsters);
	for(int i=0; i<num_monsters; i++)
	{
		int t_monster;
		int x_monster;
		int y_monster;
		cMonster* monster;
		fscanf(fd, "%d", &t_monster);
		fscanf(fd, "%d", &x_monster);
		fscanf(fd, "%d", &y_monster);
		switch(t_monster) {
			case 0: monster = new cMonster(); break;
			case 1: monster = new cPtero(); break;
			case 2: monster = new cSFish(); break;
			case 3: monster = new cMiniboss(); break;
			case 4: monster = new cScorpion(); break;
			case 5: monster = new cFrog(); break;
		}
		monster->SetTile(x_monster,y_monster);
		monsters.push_back(monster);
	}

	int num_blocks;
	fscanf(fd, "%d", &num_blocks);
	for(int i=0; i<num_blocks; i++)
	{
		int t_block;
		int t_tres;
		int x_block;
		int y_block;
		fscanf(fd, "%d", &t_block);
		fscanf(fd, "%d", &t_tres);
		fscanf(fd, "%d", &x_block);
		fscanf(fd, "%d", &y_block);
		cBlock* block = new cBlock();
		block->SetState(t_block);
		block->SetTreasure(t_tres);
		block->SetTile(x_block,y_block);
		blocks.push_back(block);
	}

	
	id_DL=glGenLists(1);
	glNewList(id_DL,GL_COMPILE);
		glPushMatrix();
		glBegin(GL_QUADS);

			fscanf(fd,"%d",&scene_width);
			fscanf(fd,"%d",&scene_height);

			map = new int[scene_width * scene_height];

			fscanf(fd,"%d",&num_quads);
			int water;
			
			for(k=0;k<num_quads;k++) {
				float f;
				int p;
				cRect* rec = new cRect();
				fscanf(fd,"%f",&f);
				rec->top = f*16+15;
				fscanf(fd,"%f",&f);
				rec->bottom = f*16;
				fscanf(fd,"%f",&f);
				rec->left = f*16;
				fscanf(fd,"%f",&f);
				rec->right = f*16+15;
				rects.push_back(rec);
				
				fscanf(fd,"%d",&water);
				isWater.push_back(water);

				fscanf(fd,"%d",&p);
				player_position.push_back(p); //x position
				fscanf(fd,"%d",&p);
				player_position.push_back(p); //y position
			}
	
			for(j=scene_height-1;j>=0;j--)
			{
				px = 0;
				py = j * TILE_SIZE;

				for(i=0;i<scene_width;i++)
				{
					tile_num;
					fscanf(fd,"%d",&tile_num);
					if(tile_num == 0)
						map[(j*scene_width)+i] = tile_num;
					else {
						tile_num--;
						if (tile_num >= 0) {
							//Tiles = 1,2,3,...
							map[(j*scene_width)+i] = tile_num;

							coordx_tile = (float)((tile_num%15)*(16.0/240.0));
							coordy_tile = (float)((tile_num/15)*(16.0/320.0));

							glTexCoord2f(coordx_tile             ,coordy_tile+(16.0/320.0));	glVertex2i(px           ,py           );
							glTexCoord2f(coordx_tile+(16.0/240.0),coordy_tile+(16.0/320.0));	glVertex2i(px+TILE_SIZE, py           );
							glTexCoord2f(coordx_tile+(16.0/240.0),coordy_tile       );			glVertex2i(px+TILE_SIZE, py+TILE_SIZE);
							glTexCoord2f(coordx_tile             ,coordy_tile       );			glVertex2i(px           ,py+TILE_SIZE);

						
						}
					}
					px+=TILE_SIZE;
					
				}
				fscanf(fd,"%c",&tile); //pass enter
			}

		glEnd();
		glPopMatrix();
	glEndList();

	bg_DL=glGenLists(1);
	glNewList(bg_DL,GL_COMPILE);
		glPushMatrix();
		glBegin(GL_QUADS);
			for(j=scene_height-1;j>=0;j--)
			{
				px = 0;
				py = j * TILE_SIZE;

				for(i=0;i<scene_width;i++)
				{
					tile_num;
					fscanf(fd,"%d",&tile_num);
					tile_num--;
					if (tile_num >= 0) {
						coordx_tile = (float)((tile_num%15)*(16.0/240.0));
						coordy_tile = (float)((tile_num/15)*(16.0/320.0));

						glTexCoord2f(coordx_tile             ,coordy_tile+(16.0/320.0));	glVertex2i(px           ,py           );
						glTexCoord2f(coordx_tile+(16.0/240.0),coordy_tile+(16.0/320.0));	glVertex2i(px+TILE_SIZE, py           );
						glTexCoord2f(coordx_tile+(16.0/240.0),coordy_tile       );			glVertex2i(px+TILE_SIZE, py+TILE_SIZE);
						glTexCoord2f(coordx_tile             ,coordy_tile       );			glVertex2i(px           ,py+TILE_SIZE);

						
					}
					px+=TILE_SIZE;
					
				}
				fscanf(fd,"%c",&tile); //pass enter
			}
		glEnd();
	glEndList();

	fclose(fd);

	return res;
}

void cScene::Draw(int tex_id)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glCallList(bg_DL);
	glCallList(id_DL);
	glDisable(GL_TEXTURE_2D);
}
int* cScene::GetMap()
{
	return map;
}

int cScene::GetNumRects() {
	return rects.size();
}

cRect* cScene::GetRectangles(int i) {
	return rects[i];
}

int cScene::GetIsWater(int i) {
	return isWater[i];
}

void cScene::GetPlayerInitPosition(int& x, int& y, int num_rect) {
	x = player_position[2*num_rect];
	y = player_position[2*num_rect+1];
}

int cScene::GetWidth() {
	return scene_width;
}

int cScene::GetHeight() {
	return scene_height;
}