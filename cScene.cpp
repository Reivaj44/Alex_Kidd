#include "cScene.h"
#include "Globals.h"

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
	int i,j,px,py, tile_num;
	char tile;
	float coordx_tile, coordy_tile;

	res=true;

	if(level<10) sprintf(file,"%s0%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);
	else		 sprintf(file,"%s%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);

	fd=fopen(file,"r");
	if(fd==NULL) return false;

	id_DL=glGenLists(1);
	glNewList(id_DL,GL_COMPILE);
		glPushMatrix();
		glTranslatef(0.0,9.0,0.0);
		glBegin(GL_QUADS);
	
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px=SCENE_Xo;
				py=SCENE_Yo+(j*TILE_SIZE);

				for(i=0;i<SCENE_WIDTH;i++)
				{
					tile_num;
					fscanf(fd,"%d",&tile_num);
					tile_num--;
					if (tile_num >= 0) {
						//Tiles = 1,2,3,...
						map[(j*SCENE_WIDTH)+i] = tile_num;

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
		glPopMatrix();
	glEndList();

	fclose(fd);

	return res;
}

void cScene::Draw(int tex_id)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glCallList(id_DL);
	glDisable(GL_TEXTURE_2D);
}
int* cScene::GetMap()
{
	return map;
}