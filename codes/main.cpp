#include"../include/draw.h"

using namespace std;

float vrec[]={
	-1.0, 1.0, 0.0, 0.0, 1.0,
	-1.0, 1.0, 0.0, 0.0, 0.0,
	-1.0, 1.0, 0.0, 1.0, 0.0,
	-1.0, 1.0, 0.0, 1.0, 1.0
};
unsigned int rec[]={
	0, 1, 2,
	0, 3, 2
};
int pom[8]={-1, 0, 1, 0, 0, -1, 0, 1};
bool buttons[20], questshow, controlsshow;
const int part=10, MAPSIZE=1005;
double camx=-1.0, camy=1.0, xchar, ychar, charmove=1.0/part, charspace=1.19, enemyspace=1.39, checkpointspace=1.0, boxspace=0.99, holespace=1.02, endx, endy, endspace=1.5;
int mapn, mapm, gamex, gamey, posx=0, posy=0, enemies=0, checkpoints=0, activecheckpoint, boxes=0, holes=0, holesfilled=0, questnr=3, activedoorsx, activedoorsy, mainmenushow=1, pauseshow=0;
unsigned int shaderProgram, shaderProgramNoTex;
char gamemap[MAPSIZE][MAPSIZE];
double enemy[1005][2];
int enemydir[1005];
double checkpoint[1005][2];
double box[1005][2];
double hole[1005][2];
double holefilled[1005][2];
bool visited[MAPSIZE][MAPSIZE];
int quest[105][8];

double camxc=-1.0, camyc=1.0, xcharc, ycharc;
int gamexc, gameyc, posxc=0, posyc=0, boxesc=0, holesc=0, holesfilledc=0, questnrc=0;
char gamemapc[1005][1005];
double enemyc[1005][2];
int enemydirc[1005];
double boxc[1005][2];
double holec[1005][2];
double holefilledc[1005][2];

double abs(double a);
void processInput(GLFWwindow *window, double MOVE, unsigned int VAO, unsigned int VBO);
void print_field(char z, float x, float y, int a, int b, unsigned int VAO, unsigned int VBO);
void print_object(char z, float x, float y, float posx, float posy, unsigned int VAO, unsigned int VBO, double space);
void cam_up();
void cam_down();
void cam_right();
void cam_left();
bool check_colision(float x, float y, float space, int dir);
bool processEnemies(unsigned int VAO, unsigned int VBO);
bool visible(double x, double y, double space);
bool dynamic_colision(double a, double b, double c, double x, double y, double z);
void back_to_checkpoint(unsigned int VAO, unsigned int VBO, GLFWwindow *window);
void set_checkpoint();
void return_to_checkpoint();
void processCheckpoints(unsigned int VAO, unsigned int VBO);
bool box_colision(int a, double MOVE);
void processBoxes(unsigned int VAO, unsigned int VBO);
bool box_can_go(int a, double x, double y, double space);
void draw_map(unsigned int VAO, unsigned int VBO);
void processHoles(unsigned int VAO, unsigned int VBO);
void processDoors(unsigned int VAO, unsigned int VBO, GLFWwindow *window);
bool go_quest(unsigned int VAO, unsigned int VBO, double size);
bool check_quest();
void destroy_doors(int a, int b);
void swap_quest(int a);
void set_quest();
bool button_machine(GLFWwindow *window, int a);
bool check_doors_near_you();
void processPause(unsigned int VAO, unsigned int VBO, GLFWwindow *window);
void processMainMenu(unsigned int VAO, unsigned int VBO, GLFWwindow *window);
void new_game();
void save_game();
bool load_game();
void processEndGame(unsigned int VAO, unsigned int VBO, GLFWwindow *window);

int main()
{
	GLFWwindow* window=my_window_init();
	shaderProgram=create_shaders(0);
	shaderProgramNoTex=create_shaders(1);
	
	unsigned int VAO_rec, VBO_rec, EBO_rec;
	
	glGenBuffers(1, &EBO_rec);
	glGenVertexArrays(1, &VAO_rec);
	glGenBuffers(1, &VBO_rec);
	
	glBindVertexArray(VAO_rec);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_rec);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rec), rec, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_rec);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vrec), vrec, GL_DYNAMIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	glActiveTexture(GL_TEXTURE0);
	load_texture("../pics/wall.jpg");

	glActiveTexture(GL_TEXTURE1);
	load_texture("../pics/floor.jpg");
	
	glActiveTexture(GL_TEXTURE2);
	load_texture("../pics/stickman.jpg");
	
	glActiveTexture(GL_TEXTURE3);
	load_texture("../pics/enemy.jpg");
	
	stbi_set_flip_vertically_on_load(true); 
	glActiveTexture(GL_TEXTURE4);
	load_texture("../pics/dead.jpg");
	
	glActiveTexture(GL_TEXTURE5);
	load_texture("../pics/checkpoint.jpg");
	
	glActiveTexture(GL_TEXTURE6);
	load_texture("../pics/checkpoint_active.jpg");
	
	glActiveTexture(GL_TEXTURE7);
	load_texture("../pics/box.jpg");
	
	glActiveTexture(GL_TEXTURE8);
	load_texture("../pics/door.jpg");
	
	glActiveTexture(GL_TEXTURE9);
	load_texture("../pics/hole.jpg");
	
	glActiveTexture(GL_TEXTURE10);
	load_texture("../pics/holefill.jpg");
	
	glActiveTexture(GL_TEXTURE11);
	load_texture("../pics/1.jpg");
	
	glActiveTexture(GL_TEXTURE12);
	load_texture("../pics/2.jpg");
	
	glActiveTexture(GL_TEXTURE13);
	load_texture("../pics/3.jpg");
	
	glActiveTexture(GL_TEXTURE14);
	load_texture("../pics/4.jpg");
	
	glActiveTexture(GL_TEXTURE15);
	load_texture("../pics/5.jpg");
	
	glActiveTexture(GL_TEXTURE16);
	load_texture("../pics/6.jpg");
	
	glActiveTexture(GL_TEXTURE17);
	load_texture("../pics/7.jpg");
	
	glActiveTexture(GL_TEXTURE18);
	load_texture("../pics/console.jpg");
	
	glActiveTexture(GL_TEXTURE19);
	load_texture("../pics/pause1.jpg");
	
	glActiveTexture(GL_TEXTURE20);
	load_texture("../pics/pause2.jpg");
	
	glActiveTexture(GL_TEXTURE21);
	load_texture("../pics/pause3.jpg");
	
	glActiveTexture(GL_TEXTURE22);
	load_texture("../pics/castle1.jpg");
	
	glActiveTexture(GL_TEXTURE23);
	load_texture("../pics/castle2.jpg");
	
	glActiveTexture(GL_TEXTURE24);
	load_texture("../pics/castle3.jpg");
	
	glActiveTexture(GL_TEXTURE25);
	load_texture("../pics/castle4.jpg");
	
	glActiveTexture(GL_TEXTURE26);
	load_texture("../pics/controls.jpg");
	
	glActiveTexture(GL_TEXTURE27);
	load_texture("../pics/save.jpg");
		
	glActiveTexture(GL_TEXTURE28);
	load_texture("../pics/nosavedgame.jpg");
	
	glActiveTexture(GL_TEXTURE29);
	load_texture("../pics/endgame.jpg");
	
	glActiveTexture(GL_TEXTURE30);
	load_texture("../pics/thisistheend.jpg");
	
	while(!glfwWindowShouldClose(window))
	{	
		glUseProgram(shaderProgram);
		if(mainmenushow)
		{
			processMainMenu(VAO_rec, VBO_rec, window);
			glfwSwapBuffers(window);
			glfwPollEvents();
			continue;
		}
		if(!pauseshow)
		{
			for(int i=0; i<2*part+1; i++)
			{
				for(int j=0; j<2*part+1; j++)
				{
					print_field(gamemap[gamey+i][gamex+j], camx , camy, i, j, VAO_rec, VBO_rec);
					visited[gamey+i][gamex+j]=true;
				}
			}
			processHoles(VAO_rec, VBO_rec);
		
			processCheckpoints(VAO_rec, VBO_rec);
		
			processBoxes(VAO_rec, VBO_rec);

			if(processEnemies(VAO_rec, VBO_rec))
			{
				back_to_checkpoint(VAO_rec, VBO_rec, window);
			}
		
			processEndGame(VAO_rec, VBO_rec, window);			
			
			print_object('C', camx, camy, xchar, ychar, VAO_rec, VBO_rec, charspace);

			processDoors(VAO_rec, VBO_rec, window);	
			
			processInput(window, charmove, VAO_rec, VBO_rec);
			
			if(button_machine(window, 7))
				pauseshow=1;
		}
		else
		{
			processPause(VAO_rec, VBO_rec, window);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
void processEndGame(unsigned int VAO, unsigned int VBO, GLFWwindow *window)
{
	if(visible(endx, endy, endspace))
	{
		print_object('e', camx, camy, endx, endy, VAO, VBO, endspace);
	}
	if(dynamic_colision(endx, endy, 0, xchar, ychar, charspace))
	{
		vrec[0]=-1.0;
		vrec[1]=1.0;
		vrec[5]=-1.0;
		vrec[6]=-1.0;
		vrec[10]=1.0;
		vrec[11]=-1.0;
		vrec[15]=1.0;
		vrec[16]=1.0;
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vrec), vrec, GL_STATIC_DRAW);
		glBindVertexArray(VAO);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 30);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
	
		double time=glfwGetTime();
		while(glfwGetTime()-time<4){}
		mainmenushow=1;
	}
}
void new_game()
{
	enemies=0, boxes=0, holes=0, holesfilled=0, checkpoints=0, questnr=3;
	for(int i=0; i<MAPSIZE; i++)
	{
		for(int j=0; j<MAPSIZE; j++)
		{
			visited[i][j]=0;
		}
	}
	ifstream map;
	map.open("../pics/map.txt");
	map>>mapm>>mapn>>gamex>>gamey;
	for(int i=0; i<mapn; i++)
	{
		for(int j=0; j<mapm; j++)
		{
			map>>gamemap[i][j];
			if(gamemap[i][j]=='C' || gamemap[i][j]=='c')
			{
				if(gamemap[i][j]=='C')
				{
					xchar=0.5+j, ychar=0.5+i;
					activecheckpoint=checkpoints;
				}
				checkpoint[checkpoints][0]=0.5+j;
				checkpoint[checkpoints][1]=0.5+i;
				checkpoints++;
			}
			if(gamemap[i][j]=='E')
			{
				enemy[enemies][0]=0.5+j;
				enemy[enemies][1]=0.5+i;
				enemydir[enemies]=0;
				enemies++;
			}
			if(gamemap[i][j]=='B')
			{
				box[boxes][0]=0.5+j;
				box[boxes][1]=0.5+i;
				boxes++;
			}
			if(gamemap[i][j]=='H')
			{
				hole[holes][0]=0.5+j;
				hole[holes][1]=0.5+i;
				holes++;
			}
			if(gamemap[i][j]=='h')
			{
				holefilled[holesfilled][0]=0.5+j;
				holefilled[holesfilled][1]=0.5+i;
				holesfilled++;
			}
			if(gamemap[i][j]=='e')
			{
				endx=0.5+j;
				endy=0.5+i;
			}
		}
	}
	set_quest();
	set_checkpoint();
	map.close();
}
void processMainMenu(unsigned int VAO, unsigned int VBO, GLFWwindow *window)
{
	vrec[0]=-1.0;
	vrec[1]=1.0;
	vrec[5]=-1.0;
	vrec[6]=-1.0;
	vrec[10]=1.0;
	vrec[11]=-1.0;
	vrec[15]=1.0;
	vrec[16]=1.0;
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vrec), vrec, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 21+mainmenushow+2*(int)controlsshow);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	if(button_machine(window, 8))
	{
		mainmenushow--;
		if(!mainmenushow)
			mainmenushow=4;
	}
	if(button_machine(window, 9))
	{
		mainmenushow++;
		if(mainmenushow==5)
			mainmenushow=1;
	}
	if(button_machine(window, 10))
	{
		if(mainmenushow==1)
		{
			mainmenushow=0;
			new_game();
		}
		if(mainmenushow==2)
		{
			if(load_game())
				mainmenushow=0;
			else
			{
				vrec[0]=-0.6;
				vrec[1]=0.3;
				vrec[5]=-0.6;
				vrec[6]=-0.3;
				vrec[10]=0.6;
				vrec[11]=-0.3;
				vrec[15]=0.6;
				vrec[16]=0.3;
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vrec), vrec, GL_STATIC_DRAW);
				glBindVertexArray(VAO);
				glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 28);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glfwSwapBuffers(window);
				double pm=glfwGetTime();
				while(glfwGetTime()-pm<0.8){};
			}
		}
		if(mainmenushow==4)
		{
			glfwSetWindowShouldClose(window, true);
		}
		if(mainmenushow==3)
		{
			controlsshow=1;
		}
	}
	if(controlsshow && button_machine(window, 7))
	{
		controlsshow=0;
	}
}
void processPause(unsigned int VAO, unsigned int VBO, GLFWwindow *window)
{	
	vrec[0]=-1.0;
	vrec[1]=1.0;
	vrec[5]=-1.0;
	vrec[6]=-1.0;
	vrec[10]=1.0;
	vrec[11]=-1.0;
	vrec[15]=1.0;
	vrec[16]=1.0;
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vrec), vrec, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 18+pauseshow);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	if(button_machine(window, 8))
	{
		pauseshow--;
		if(!pauseshow)
			pauseshow=3;
	}
	if(button_machine(window, 9))
	{
		pauseshow++;
		if(pauseshow==4)
			pauseshow=1;
	}
	bool flag=0;
	if(button_machine(window, 10))
	{
		if(pauseshow==1)
		{
			pauseshow=0;
		}
		if(pauseshow==3)
		{
			mainmenushow=1;
			pauseshow=0;
		}
		if(pauseshow==2)
		{
			vrec[0]=-0.6;
			vrec[1]=0.3;
			vrec[5]=-0.6;
			vrec[6]=-0.3;
			vrec[10]=0.6;
			vrec[11]=-0.3;
			vrec[15]=0.6;
			vrec[16]=0.3;
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vrec), vrec, GL_STATIC_DRAW);
			glBindVertexArray(VAO);
			glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 27);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glfwSwapBuffers(window);
			double pm=glfwGetTime();
			while(glfwGetTime()-pm<0.8){}
			save_game();
		}
	}
	if(button_machine(window, 7))
		pauseshow=0;
}
void save_game()
{
	ofstream save;
	save.open("../pics/savedgame.txt");
	save<<mapm<<" "<<mapn<<" "<<gamex<<" "<<gamey<<"\n";
	for(int i=0; i<mapn; i++)
	{
		for(int j=0; j<mapm; j++)
		{
			save<<gamemap[i][j];
		}
		save<<"\n";
	}
	save<<camx<<" "<<camy<<" "<<xchar<<" "<<ychar<<" "<<posx<<" "<<posy<<" "<<endx<<" "<<endy<<"\n";
	save<<enemies<<" "<<checkpoints<<" "<<activecheckpoint<<" "<<boxes<<" "<<holes<<" "<<holesfilled<<" "<<questnr<<"\n";
	for(int i=0; i<enemies; i++)
	{
		save<<enemy[i][0]<<" "<<enemy[i][1]<<" "<<enemydir[i]<<"\n";
	}
	for(int i=0; i<checkpoints; i++)
	{
		save<<checkpoint[i][0]<<" "<<checkpoint[i][1]<<"\n";
	}
	for(int i=0; i<boxes; i++)
	{
		save<<box[i][0]<<" "<<box[i][1]<<"\n";
	}
	for(int i=0; i<holes; i++)
	{
		save<<hole[i][0]<<" "<<hole[i][1]<<"\n";
	}
	for(int i=0; i<holesfilled; i++)
	{
		save<<holefilled[i][0]<<" "<<holefilled[i][1]<<"\n";
	}
	int pom=0, pomi=0, pomj=0;
	while(pom<questnr)
	{
		if(pomj>6-pomi)
		{
			pomi++;
			pomj=0;
		}
		save<<quest[pomi][pomj]<<" ";
		pom++;
		pomj++;
	}
	save<<"\n\n";
	save<<gamexc<<" "<<gameyc<<"\n";
	save<<camxc<<" "<<camyc<<" "<<xcharc<<" "<<ycharc<<" "<<posxc<<" "<<posyc<<"\n";
	save<<boxesc<<" "<<holesc<<" "<<holesfilledc<<" "<<questnrc<<"\n";
	for(int i=0; i<enemies; i++)
	{
		save<<enemyc[i][0]<<" "<<enemyc[i][1]<<" "<<enemydirc[i]<<"\n";
	}
	for(int i=0; i<boxesc; i++)
	{
		save<<boxc[i][0]<<" "<<boxc[i][1]<<"\n";
	}
	for(int i=0; i<holesc; i++)
	{
		save<<holec[i][0]<<" "<<holec[i][1]<<"\n";
	}
	for(int i=0; i<holesfilledc; i++)
	{
		save<<holefilledc[i][0]<<" "<<holefilledc[i][1]<<"\n";
	}
	for(int i=0; i<mapn; i++)
	{
		for(int j=0; j<mapm; j++)
		{
			save<<gamemapc[i][j];
		}
		save<<"\n";
	}
	for(int i=0; i<mapn; i++)
	{
		for(int j=0; j<mapm; j++)
		{
			save<<visited[i][j]<<" ";
		}
		save<<"\n";
	}
	save.close();
}
void destroy_doors(int a, int b)
{
	gamemap[a][b]='f';
	for(int i=0; i<8; i+=2)
	{
		int x=a+pom[i], y=b+pom[i+1];
		if(gamemap[x][y]=='D')
			destroy_doors(x, y);
	}
}
bool load_game()
{
	ifstream save;
	save.open("../pics/savedgame.txt");
	save>>mapm;
	if(mapm==0)
	{
		return 0;
	}
	save>>mapn>>gamex>>gamey;
	for(int i=0; i<mapn; i++)
	{
		for(int j=0; j<mapm; j++)
		{
			save>>gamemap[i][j];
		}
	}
	save>>camx>>camy>>xchar>>ychar>>posx>>posy>>endx>>endy;
	save>>enemies>>checkpoints>>activecheckpoint>>boxes>>holes>>holesfilled>>questnr;
	for(int i=0; i<enemies; i++)
	{
		save>>enemy[i][0]>>enemy[i][1]>>enemydir[i];
	}
	for(int i=0; i<checkpoints; i++)
	{
		save>>checkpoint[i][0]>>checkpoint[i][1];
	}
	for(int i=0; i<boxes; i++)
	{
		save>>box[i][0]>>box[i][1];
	}
	for(int i=0; i<holes; i++)
	{
		save>>hole[i][0]>>hole[i][1];
	}
	for(int i=0; i<holesfilled; i++)
	{
		save>>holefilled[i][0]>>holefilled[i][1];
	}
	int pom=0, pomi=0, pomj=0;
	while(pom<questnr)
	{
		if(pomj>6-pomi)
		{
			pomi++;
			pomj=0;
		}
		save>>quest[pomi][pomj];
		pom++;
		pomj++;
	}
	save>>gamexc>>gameyc;
	save>>camxc>>camyc>>xcharc>>ycharc>>posxc>>posyc;
	save>>boxesc>>holesc>>holesfilledc>>questnrc;
	for(int i=0; i<enemies; i++)
	{
		save>>enemyc[i][0]>>enemyc[i][1]>>enemydirc[i];
	}
	for(int i=0; i<boxesc; i++)
	{
		save>>boxc[i][0]>>boxc[i][1];
	}
	for(int i=0; i<holesc; i++)
	{
		save>>holec[i][0]>>holec[i][1];
	}
	for(int i=0; i<holesfilledc; i++)
	{
		save>>holefilledc[i][0]>>holefilledc[i][1];
	}
	for(int i=0; i<mapn; i++)
	{
		for(int j=0; j<mapm; j++)
		{
			save>>gamemapc[i][j];
		}
	}
	for(int i=0; i<mapn; i++)
	{
		for(int j=0; j<mapm; j++)
		{
			save>>visited[i][j];
		}
	}
	save.close();
	return 1;
}
void swap_quest(int a)
{
	int pom=0, pomi=0, pomj=0;
	while(pom<questnr-1)
	{
		if(pomj>5-pomi)
		{
			pomi++;
			pom++;
			pomj=0;
			continue;
		}
		if(pomj==a)
		{
			int pom=quest[pomi][pomj];
			quest[pomi][pomj]=quest[pomi][pomj+1];
			quest[pomi][pomj+1]=pom;
		}
		pomj++;
		pom++;
	}
}
bool button_machine(GLFWwindow *window, int a)
{
	int key=GLFW_KEY_0+a;
	switch (a){
		case 0: key=GLFW_KEY_I; break;
		case 7: key=GLFW_KEY_ESCAPE; break;
		case 8: key=GLFW_KEY_UP; break;
		case 9: key=GLFW_KEY_DOWN; break;
		case 10: key=GLFW_KEY_ENTER; break;
	}
	if(!buttons[a])
	{
		if(glfwGetKey(window, key) == GLFW_PRESS)
		{
			buttons[a]=1;
		}
		return buttons[a];
	}
	else
	{
		if(glfwGetKey(window, key) == GLFW_RELEASE)
		{
			buttons[a]=0;
		}
		return 0;
	}
}
bool check_doors_near_you()
{
	double DIFF=0.85;
	for(int i=ychar-charspace+DIFF; i<ychar+charspace-DIFF; i++)
	{
		for(int j=xchar-charspace+DIFF; j<xchar+charspace-DIFF; j++)
		{
			for(int k=0; k<8; k+=2)
			{
				if(gamemap[i+pom[k]][j+pom[k+1]]=='D')
				{
					activedoorsx=i+pom[k];
					activedoorsy=j+pom[k+1];
					return 1;
				}
			}
		}
	}
	return 0;
}
void processDoors(unsigned int VAO, unsigned int VBO, GLFWwindow *window)
{		
	if(check_doors_near_you())
	{	
		if(button_machine(window, 0))
		{
			if(questshow)
				questshow=0;
			else
				questshow=1;
		}
		if(questshow)
		{
			double SIZE=0.8;
			vrec[0]=-SIZE;
			vrec[1]=SIZE;
			vrec[5]=-SIZE;
			vrec[6]=-SIZE;
			vrec[10]=SIZE;
			vrec[11]=-SIZE;
			vrec[15]=SIZE;
			vrec[16]=SIZE;
		
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vrec), vrec, GL_STATIC_DRAW);
			glBindVertexArray(VAO);
			glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 18);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			if(go_quest(VAO, VBO, SIZE-0.1))
			{
				destroy_doors(activedoorsx, activedoorsy);
			}
			for(int i=1; i<7; i++)
			{
				if(button_machine(window, i))
				{
					swap_quest(i-1);
				}
			}
		}
	}
	else
	{
		questshow=0;
	}
}
bool check_quest()
{
	int pom=0, pomi=0, pomj=0;
	while(pom<questnr-1)
	{
		if(pomj>5-pomi)
		{
			pomi++;
			pomj=0;
			pom++;
			continue;
		}
		if(quest[pomi][pomj]>quest[pomi][pomj+1])
			return 0;
		pom++;
		pomj++;
	}
	return 1;
}
void set_quest()
{
	if(questnr<27)
		questnr++;
	int pom=0, pomi=0, pomj=0;
	while(pom<questnr)
	{
		if(pomj>6-pomi)
		{
			pomi++;
			pomj=0;
		}
		quest[pomi][pomj]=pomj+1;
		pom++;
		pomj++;
	}
	do
	{
		for(int i=0; i<20; i++)
		{
			int pom=0, pomi=0, pomj=0;
			while(pom<questnr-1)
			{
				if(pomj>5-pomi)
				{
					pomi++;
					pomj=0;
					pom++;
					continue;
				}
				if(((int)(glfwGetTime()*10000))%2)
				{
					int pm=quest[pomi][pomj];
					quest[pomi][pomj]=quest[pomi][pomj+1];
					quest[pomi][pomj+1]=pm;
				}
				pom++;
				pomj++;
			}
		}
	}while(check_quest());
}
bool go_quest(unsigned int VAO, unsigned int VBO, double size)
{
	
	if(check_quest())
	{	
		set_quest();
		return 1;
	}
	double F=0.05, FIELD=2*size/7;
	
	glUseProgram(shaderProgram);
	int pom=0, pomi=0, pomj=0;
	while(pom<questnr)
	{
		if(pomj>6-pomi)
		{
			pomi++;
			pomj=0;
		}
		vrec[0]=-size+((pomj)*FIELD)+F;
		vrec[1]=size-((pomi)*FIELD)-F;
		vrec[5]=-size+((pomj)*FIELD)+F;
		vrec[6]=size-(((pomi)+1)*FIELD)+F;
		vrec[10]=-size+(((pomj)+1)*FIELD)-F;
		vrec[11]=size-(((pomi)+1)*FIELD)+F;
		vrec[15]=-size+(((pomj)+1)*FIELD)-F;
		vrec[16]=size-((pomi)*FIELD)-F;
				
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vrec), vrec, GL_STATIC_DRAW);
		glBindVertexArray(VAO);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 10+quest[pomi][pomj]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		pom++;
		pomj++;
	}
	return 0;
}
void processHoles(unsigned int VAO, unsigned int VBO)
{
	for(int i=0; i<holes; i++)
	{
		if(visible(hole[i][0], hole[i][1], holespace))
		{
			bool flag=true;
			for(int j=0; j<boxes; j++)
			{
				if(dynamic_colision(box[j][0], box[j][1], boxspace, hole[i][0], hole[i][1], 0))
				{
					flag=false;
					holefilled[holesfilled][0]=hole[i][0];
					holefilled[holesfilled][1]=hole[i][1];
					holesfilled++;
					hole[i][0]=hole[holes-1][0];
					hole[i][1]=hole[holes-1][1];
					holes--;
					i--;
					box[j][0]=box[boxes-1][0];
					box[j][1]=box[boxes-1][1];
					boxes--;
					j--;
				}
			}
			if(flag)
				print_object('H', camx, camy, hole[i][0], hole[i][1], VAO, VBO, holespace);
		}
	}
	for(int i=0; i<holesfilled; i++)
	{
		if(visible(holefilled[i][0], holefilled[i][1], holespace))
		{
			print_object('h', camx, camy, holefilled[i][0], holefilled[i][1], VAO, VBO, holespace);
		}
	}
}
void processBoxes(unsigned int VAO, unsigned int VBO)
{
	double MOVE=charmove;
	for(int i=0; i<boxes; i++)
	{
		if(visible(box[i][0], box[i][1], boxspace))
		{
			if(dynamic_colision(box[i][0], box[i][1], boxspace, xchar, ychar, charspace))
			{
				double xdis=abs(box[i][0]-xchar), ydis=abs(box[i][1]-ychar);
				if(xchar<box[i][0] && xdis>0 && ydis/xdis<1)	//box w prawo
				{
					if(check_colision(box[i][0], box[i][1], boxspace, 4) && box_can_go(i, box[i][0]+MOVE, box[i][1], boxspace))
						box[i][0]+=MOVE;
					else
						xchar-=MOVE;
				}
				if(xchar>box[i][0] && xdis>0 && ydis/xdis<1)	//box w lewo
				{
					if(check_colision(box[i][0], box[i][1], boxspace, 3) && box_can_go(i, box[i][0]-MOVE, box[i][1], boxspace))
						box[i][0]-=MOVE;
					else
						xchar+=MOVE;
				}
				if(ychar>box[i][1] && ydis>0 && xdis/ydis<1)	//box w górę
				{
					if(check_colision(box[i][0], box[i][1], boxspace, 1) && box_can_go(i, box[i][0], box[i][1]-MOVE, boxspace))
						box[i][1]-=MOVE;
					else
						ychar+=MOVE;
				}
				if(ychar<box[i][1] && ydis>0 && xdis/ydis<1)	//box w dół
				{
					if(check_colision(box[i][0], box[i][1], boxspace, 2) && box_can_go(i, box[i][0], box[i][1]+MOVE, boxspace))
						box[i][1]+=MOVE;
					else
						ychar-=MOVE;
				}
					
			}
			print_object('B', camx, camy, box[i][0], box[i][1], VAO, VBO, boxspace);
		}
	}	
}
bool box_can_go(int a, double x, double y, double space)
{
	for(int i=0; i<boxes; i++)
	{
		if(a==i)
			continue;
		if(dynamic_colision(x, y, space, box[i][0], box[i][1], boxspace))
			return 0;
	}
	for(int i=0; i<enemies; i++)
	{
		if(dynamic_colision(x, y, space, enemy[i][0], enemy[i][1], enemyspace))
			return 0;
	}
	return 1;
}
void processCheckpoints(unsigned int VAO, unsigned int VBO)
{
	for(int i=0; i<checkpoints; i++)
	{
		if(activecheckpoint==i)
		{
			if(visible(checkpoint[i][0], checkpoint[i][1], checkpointspace))
				print_object('A', camx, camy, checkpoint[i][0], checkpoint[i][1], VAO, VBO, checkpointspace);
			continue;
		}
		if(dynamic_colision(checkpoint[i][0], checkpoint[i][1], 0, xchar, ychar, charspace))
		{
			activecheckpoint=i;
			set_checkpoint();
		}
		if(visible(checkpoint[i][0], checkpoint[i][1], checkpointspace))
			print_object('c', camx, camy, checkpoint[i][0], checkpoint[i][1], VAO, VBO, checkpointspace);
	}
}
void set_checkpoint()
{
	camxc=camx, camyc=camy, xcharc=xchar, ycharc=ychar;
	gamexc=gamex, gameyc=gamey, posxc=posx, posyc=posy;
	boxesc=boxes, holesc=holes, holesfilledc=holesfilled;
	questnrc=questnr;
	for(int i=0; i<mapn; i++)
	{
		for(int j=0; j<mapm; j++)
		{
			gamemapc[i][j]=gamemap[i][j];
		}
	}
	for(int i=0; i<enemies; i++)
	{
		enemyc[i][0]=enemy[i][0];
		enemyc[i][1]=enemy[i][1];
		enemydirc[i]=enemydir[i];
	}
	for(int i=0; i<boxes; i++)
	{
		boxc[i][0]=box[i][0];
		boxc[i][1]=box[i][1];
	}
	for(int i=0; i<holes; i++)
	{
		holec[i][0]=hole[i][0];
		holec[i][1]=hole[i][1];
	}
	for(int i=0; i<holesfilled; i++)
	{
		holefilledc[i][0]=holefilled[i][0];
		holefilledc[i][1]=holefilled[i][1];
	}
}
void back_to_checkpoint(unsigned int VAO, unsigned int VBO, GLFWwindow *window)
{
	vrec[0]=-1;
	vrec[1]=1;
	vrec[5]=-1;
	vrec[6]=-1;
	vrec[10]=1;
	vrec[11]=-1;
	vrec[15]=1;
	vrec[16]=1;
	glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vrec), vrec, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	glfwSwapBuffers(window);
	
	double time=glfwGetTime();
	while(glfwGetTime()-time<2){}
	
	return_to_checkpoint();	
	
}
void return_to_checkpoint()
{
	camx=camxc, camy=camyc, xchar=xcharc, ychar=ycharc;
	gamex=gamexc, gamey=gameyc, posx=posxc, posy=posyc;
	boxes=boxesc, holes=holesc, holesfilled=holesfilledc;
	questnr=questnrc-1;
	set_quest();
	for(int i=0; i<mapn; i++)
	{
		for(int j=0; j<mapm; j++)
		{
			gamemap[i][j]=gamemapc[i][j];
		}
	}
	for(int i=0; i<enemies; i++)
	{
		enemy[i][0]=enemyc[i][0];
		enemy[i][1]=enemyc[i][1];
		enemydir[i]=enemydirc[i];
	}
	for(int i=0; i<boxes; i++)
	{
		box[i][0]=boxc[i][0];
		box[i][1]=boxc[i][1];
	}
	for(int i=0; i<holes; i++)
	{
		hole[i][0]=holec[i][0];
		hole[i][1]=holec[i][1];
	}
	for(int i=0; i<holesfilled; i++)
	{
		holefilled[i][0]=holefilledc[i][0];
		holefilled[i][1]=holefilledc[i][1];
	}
}
void print_field(char z, float x, float y, int a, int b, unsigned int VAO, unsigned int VBO)
{
	float F=1.0/part;
	vrec[0]=x+(F*b);
	vrec[1]=y-(F*a);
	vrec[5]=x+(F*b);
	vrec[6]=y-(F*(a+1));
	vrec[10]=x+(F*(b+1));
	vrec[11]=y-(F*(a+1));
	vrec[15]=x+(F*(b+1));
	vrec[16]=y-(F*a);
	
	if(z=='w')
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
	else
		if(z=='D')
			glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 8);
		else
			glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 1);
		
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vrec), vrec, GL_DYNAMIC_DRAW);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void print_object(char z, float x, float y, float posx, float posy, unsigned int VAO, unsigned int VBO, double space)
{
	float F=space/part, POSX=(posx-gamex)/(part), POSY=(posy-gamey)/(part);
	vrec[0]=x+POSX-F;
	vrec[1]=y-POSY-F;
	vrec[5]=x+POSX-F;
	vrec[6]=y-POSY+F;
	vrec[10]=x+POSX+F;
	vrec[11]=y-POSY+F;
	vrec[15]=x+POSX+F;
	vrec[16]=y-POSY-F;
	if(z=='C')
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 2);
	if(z=='E')
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 3);
	if(z=='c')
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 5);
	if(z=='A')
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 6);
	if(z=='B')
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 7);
	if(z=='H')
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 9);
	if(z=='h')
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 10);
	if(z=='e')
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 29);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vrec), vrec, GL_DYNAMIC_DRAW);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

bool check_colision(float x, float y, float space, int dir)
{
	float F=space, MOVE=(1.5/part);
	if(dir==1 || dir==2)
	{
		double yup=y-F-MOVE, ydown=y+F+MOVE, xleft=x-F, xright=x+F;
		int checked_row;
		if(dir==1)	checked_row=yup;	else	checked_row=ydown;
		for(int i=xleft; i<=xright; i++)
		{
			if(gamemap[checked_row][i]=='w' || gamemap[checked_row][i]=='D')	return 0;
		}
	}
	else
	{
		double xleft=x-F-MOVE, xright=x+F+MOVE, yup=y-F, ydown=y+F;
		int checked_col;
		if(dir==3)	checked_col=xleft;	else	checked_col=xright;
		for(int i=yup; i<=ydown; i++)
		{
			if(gamemap[i][checked_col]=='w' || gamemap[i][checked_col]=='D')	return 0;
		}
	}
	return 1;
}
void draw_map(unsigned int VAO, unsigned int VBO)
{
	int size=max(mapn, mapm);
	double F=2.0/size;
	double mapy=size-mapm, mapx=size-mapn;
	mapy/=2;
	mapx/=2;
	mapy=-1.0+mapy*F;
	mapx=1.0-mapx*F;
	glUseProgram(shaderProgramNoTex);
	
	for(int i=0; i<mapn; i++)
	{
		for(int j=0; j<mapm; j++)
		{
			vrec[0]=mapy+F*j;
			vrec[1]=mapx-F*i;
			vrec[5]=mapy+F*j;
			vrec[6]=mapx-F*(i+1);
			vrec[10]=mapy+F*(j+1);
			vrec[11]=mapx-F*(i+1);
			vrec[15]=mapy+F*(j+1);
			vrec[16]=mapx-F*i;
			
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vrec), vrec, GL_STATIC_DRAW);
			glBindVertexArray(VAO);
			
			int vertexColorLocation = glGetUniformLocation(shaderProgramNoTex, "ourColor");			
			glUniform4f(vertexColorLocation, 0.0, 0.0, 0.0, 1.0);
			if(visited[i][j])
			{
				if(gamemap[i][j]=='w')
					glUniform4f(vertexColorLocation, 0.0, 0.0, 1.0, 1.0);
				else
					if(gamemap[i][j]=='D')
						glUniform4f(vertexColorLocation, 0.8, 0.5, 0.0, 1.0);
					else
						glUniform4f(vertexColorLocation, 0.7, 0.9, 0.0, 1.0);
				if((int)xchar==j && (int)ychar==i)
					glUniform4f(vertexColorLocation, 1.0, 0.0, 0.0, 1.0);
			}
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}
}
void processInput(GLFWwindow *window, double MOVE, unsigned int VAO, unsigned int VBO)
{
	float F=charspace/part, POSX=(xchar-gamex)/(part), POSY=(ychar-gamey)/(part);
	if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		draw_map(VAO, VBO);
	}
	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		bool flag=1;
		if(check_colision(xchar, ychar, charspace, 1))
		{
			for(int i=0; i<holes; i++)
			{
				if(dynamic_colision(xchar, ychar-MOVE, charspace, hole[i][0], hole[i][1], holespace))
					flag=0;
			}
			if(flag)
				ychar-=MOVE;
		}
		if(camy-POSY+F>0.4)
			cam_up();
	}
	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		bool flag=1;
		if(check_colision(xchar, ychar, charspace, 2))
		{
			for(int i=0; i<holes; i++)
			{
				if(dynamic_colision(xchar, ychar+MOVE, charspace, hole[i][0], hole[i][1], holespace))
					flag=0;
			}
			if(flag)
				ychar+=MOVE;
		}
		if(camy-POSY-F<-0.4)
			cam_down();
	}
	if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		bool flag=1;
		if(check_colision(xchar, ychar, charspace, 3))
		{
			for(int i=0; i<holes; i++)
			{
				if(dynamic_colision(xchar-MOVE, ychar, charspace, hole[i][0], hole[i][1], holespace))
					flag=0;
			}
			if(flag)
				xchar-=MOVE;
		}
		if(camx+POSX-F<-0.4)
			cam_left();
	}
	if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		bool flag=1;
		if(check_colision(xchar, ychar, charspace, 4))
		{
			for(int i=0; i<holes; i++)
			{
				if(dynamic_colision(xchar+MOVE, ychar, charspace, hole[i][0], hole[i][1], holespace))
					flag=0;
			}
			if(flag)
				xchar+=MOVE;
		}
		if(camx+POSX+F>0.4)
			cam_right();
	}
}
bool visible(double x, double y, double space)
{
	return !(x+space<gamex || x-space>gamex+2*part+1 || y+space<gamey || y-space>gamey+2*part+1);
}
bool processEnemies(unsigned int VAO, unsigned int VBO)
{
	double MOVE=0.3/part;
	for(int i=0; i<enemies; i++)
	{
		int mvs=0;
		if((!check_colision(enemy[i][0], enemy[i][1], enemyspace, enemydir[i]+1) || box_colision(i, MOVE)))
			enemydir[i]=(glfwGetTime()*10000)+(3*i);
			enemydir[i]%=4;
		while((!check_colision(enemy[i][0], enemy[i][1], enemyspace, enemydir[i]+1) || box_colision(i, MOVE)) && mvs<4)
		{
			enemydir[i]+=1;
			enemydir[i]%=4;
			mvs++;
		}
		if(mvs<4)
			switch (enemydir[i]){
				case 0: enemy[i][1]-=MOVE; break;
				case 1: enemy[i][1]+=MOVE; break;
				case 2: enemy[i][0]-=MOVE; break;
				case 3: enemy[i][0]+=MOVE; break;
		}
		if(visible(enemy[i][0], enemy[i][1], enemyspace))
			print_object('E', camx, camy, enemy[i][0], enemy[i][1], VAO, VBO, enemyspace);
		if(dynamic_colision(enemy[i][0], enemy[i][1], enemyspace, xchar, ychar, charspace))
			return 1;
	}
	return 0;
}
bool box_colision(int a, double MOVE)
{
	for(int i=0; i<boxes; i++)
	{
		switch (enemydir[a]){
			case 0: if(dynamic_colision(enemy[a][0], enemy[a][1]-MOVE, enemyspace, box[i][0], box[i][1], boxspace)) return 1; break;
			case 1: if(dynamic_colision(enemy[a][0], enemy[a][1]+MOVE, enemyspace, box[i][0], box[i][1], boxspace)) return 1; break;
			case 2: if(dynamic_colision(enemy[a][0]-MOVE, enemy[a][1], enemyspace, box[i][0], box[i][1], boxspace)) return 1; break;
			case 3: if(dynamic_colision(enemy[a][0]+MOVE, enemy[a][1], enemyspace, box[i][0], box[i][1], boxspace)) return 1; break;
		}
	}
	for(int i=0; i<enemies; i++)
	{
		if(i==a)
			continue;
		switch (enemydir[a]){
			case 0: if(dynamic_colision(enemy[a][0], enemy[a][1]-MOVE, enemyspace, enemy[i][0], enemy[i][1], enemyspace)) return 1; break;
			case 1: if(dynamic_colision(enemy[a][0], enemy[a][1]+MOVE, enemyspace, enemy[i][0], enemy[i][1], enemyspace)) return 1; break;
			case 2: if(dynamic_colision(enemy[a][0]-MOVE, enemy[a][1], enemyspace, enemy[i][0], enemy[i][1], enemyspace)) return 1; break;
			case 3: if(dynamic_colision(enemy[a][0]+MOVE, enemy[a][1], enemyspace, enemy[i][0], enemy[i][1], enemyspace)) return 1; break;
		}
	}
	for(int i=0; i<holes; i++)
	{
		switch (enemydir[a]){
			case 0: if(dynamic_colision(enemy[a][0], enemy[a][1]-MOVE, enemyspace, hole[i][0], hole[i][1], holespace)) return 1; break;
			case 1: if(dynamic_colision(enemy[a][0], enemy[a][1]+MOVE, enemyspace, hole[i][0], hole[i][1], holespace)) return 1; break;
			case 2: if(dynamic_colision(enemy[a][0]-MOVE, enemy[a][1], enemyspace, hole[i][0], hole[i][1], holespace)) return 1; break;
			case 3: if(dynamic_colision(enemy[a][0]+MOVE, enemy[a][1], enemyspace, hole[i][0], hole[i][1], holespace)) return 1; break;
		}
	}
	return 0;
}
bool dynamic_colision(double a, double b, double c, double x, double y, double z)
{
	return !(x+z<a-c || x-z>a+c || y+z<b-c || y-z>b+c);
}
void cam_up()
{
	float F=1.0/part;
	if(gamey>0 || posy>0)
	{
		posy--;
		if(posy<0)
		{
			gamey--;
			posy=part-1;
		}
		camy=1.0+((float)posy*(F*F));
	}
}
void cam_down()
{
	float F=1.0/part;
	if(gamey<mapn-2*part)
	{
		posy++;
		if(posy==part)
		{
			gamey++;
			posy=0;
		}
		camy=1.0+((float)posy*(F*F));
	}
}
void cam_right()
{
	float F=1.0/part;
	if(gamex<mapm-2*part)
	{
		posx++;
		if(posx==part)
		{
			gamex++;
			posx=0;
		}
		camx=-1.0-((float)posx*(F*F));
	}
}
void cam_left()
{
	float F=1.0/part;
	if(gamex>0)
	{
		posx--;
		if(posx<0)
		{
			gamex--;
			posx=part-1;
		}
		camx=-1.0-((float)posx*(F*F));
	}
}
double abs(double a)
{
	if(a<0)
		return -a;
	return a;
}
