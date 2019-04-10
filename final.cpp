#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <vector>
#include <GL/glut.h>

void timer(int);		// Function for redisplay


class vehicle{					// vehicle class
	public :
		std::string Vehicle_Type;
		int Vehicle_Length;
		int Vehicle_Width;
		int Vehicle_MaxSpeed;
		float Vehicle_Acceleration;
		std::string Vehicle_Color;
		float x_p;
		float y_p;
		float time;
		float q;
		
};

std::string sig = "RED";


class road{					//road class
	public :
		int Road_Id;
		int Road_Length;
		int Road_Width;
		int Road_Signal;		
};

class signal{				//signal class
	public :
		float Signal_Time;
		std::string Signal_Color;
};

road rd;
std::vector <signal> siglist;	//signal list
std::vector <vehicle> vehlist;	//vehicle list

std::string ret_sig(int x,std::string a){			// returns signal at given time t
	for(int t=0;t<siglist.size();t++){
		if(x == siglist[t].Signal_Time) return siglist[t].Signal_Color;
	}
	return a;
}

void initOpenGl(){
    glClearColor(0.5,0.5,0.5,1); //Background Color
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,2000,0,1000);
    glMatrixMode(GL_MODELVIEW);
}


std::vector<std::string> splitStrings(std::string str, char dl) { 		//splits the string into substrings
    std::string word = ""; 
    int num = 0; 
    str = str + dl; 
    int l = str.size();
    std::vector<std::string> substr_list; 
    for (int i = 0; i < l; i++) { 
	if (str[i] != dl) word = word + str[i]; 
        else { 
            if ((int)word.size() != 0) 
                substr_list.push_back(word); 
  	        word = ""; 
        } 
    } 
    return substr_list; 
} 


bool p_occp(float x, float y){			//to know if a vehicle occupies a point
	for(int j=0;j<vehlist.size();j++){
		if(x >= (vehlist[j].x_p-vehlist[j].Vehicle_Length) && x < vehlist[j].x_p){
			if( y >= vehlist[j].y_p && y < (vehlist[j].y_p+vehlist[j].Vehicle_Width)){
				return true;
			}
		}
	}
	if(y > (rd.Road_Width/5)+5) return true;
	else if(y < -5) return true;
	else return false;
}

vehicle ret_veh(float x,float y){		//returns a vehicle  at given point
	vehicle c;
	c.Vehicle_Length = 0;
	c.Vehicle_Width = 0;
	for(int j=0;j<vehlist.size();j++){
		if(x >= (vehlist[j].x_p-vehlist[j].Vehicle_Length) && x <= vehlist[j].x_p){
			if(y >= vehlist[j].y_p && y <= (vehlist[j].y_p+vehlist[j].Vehicle_Width)){
				return vehlist[j];
			}
		}
	}
	return c;
}

float change_y(vehicle a,float x){		// change y position of vehicle
	int RW = rd.Road_Width*20;
	float q = 0.5;
	if(a.Vehicle_Type.compare("bike")==0){q = 0;}
	vehicle z = ret_veh(x,a.y_p);
	if(a.y_p == 400){
		if(p_occp(a.x_p,400+a.Vehicle_Width+z.Vehicle_Width)==false){
			if(400+a.Vehicle_Width+z.Vehicle_Width <= 400+RW)return 400+q+z.Vehicle_Width;
			else return a.y_p;
		}
		else return 400;
	}
	else if(a.y_p == 400+RW-a.Vehicle_Width){
		if(p_occp(a.x_p,z.y_p-a.Vehicle_Width-1) == false){
			if(z.y_p-a.Vehicle_Width >= 400)return z.y_p-a.Vehicle_Width-q;
			else return a.y_p;
		}
		else return 400+RW-a.Vehicle_Width;
	}
	else{
		if(p_occp(a.x_p,z.y_p-a.Vehicle_Width) == false){
			if(p_occp(a.x_p,z.y_p+z.Vehicle_Width+a.Vehicle_Width) == false){
				if(z.y_p+z.Vehicle_Width+a.Vehicle_Width <= 400+RW)return z.y_p+z.Vehicle_Width+q;
				else if(z.y_p-a.Vehicle_Width >= 400) return z.y_p-a.Vehicle_Width-q;
				else return a.y_p;
			}
			else{ 
				if(z.y_p-a.Vehicle_Width >= 400) return z.y_p-a.Vehicle_Width-q;
				else return a.y_p;
			}
		}
		else{
			if(p_occp(a.x_p,z.y_p+z.Vehicle_Width+a.Vehicle_Width) == false){
				if(z.y_p+z.Vehicle_Width+a.Vehicle_Width < 400+RW+5) return(z.y_p+z.Vehicle_Width+q);
				else return a.y_p;
			}
			else return a.y_p;
		}
	}
}

void veh(float x, float y, int length, int width, std::string color){		//a opengl function to display vehicles
	if(color.compare("RED") == 0)glColor3f(1,0,0);
	else if(color.compare("GREEN") == 0)glColor3f(0,1,0);
	else if(color.compare("BLUE") == 0)glColor3f(0,0,1);
	glBegin(GL_POLYGON);
	x = x * 10;
	y = y * 10;
	length = length * 10;
	width = width * 10;
	//glTranslatef(x,y,0);
	glVertex2f(x,400+y);
	glVertex2f(x,400+y+width);
	glVertex2f(x-length,400+y+width);
	glVertex2f(x-length,400+y);
	glEnd();
}

float t=0;
std::string a;


void display(){								// opengl display function
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0,1,1);					
	glBegin(GL_LINES);
		glVertex2f(0,400);
		glVertex2f(2000,400);			
		glVertex2f(2000,400+rd.Road_Width*20);
		glVertex2f(0,400+rd.Road_Width*20);					
	glEnd();
	if(sig.compare("RED")==0){glColor3f(1,0,0);}
	else glColor3f(0,1,0);
	glLineWidth(2);
	glBegin(GL_LINES);
		glVertex2f(rd.Road_Signal,400+rd.Road_Width*20);
		glVertex2f(rd.Road_Signal,400);
	glEnd();
	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
		glVertex2f(400,200);
		glVertex2f(400,350);
		glVertex2f(500,350);
		glVertex2f(500,200);
	glEnd();
	if(sig.compare("RED") == 0){
		glColor3f(1,0,0);
	}
	else glColor3f(1,1,1);
	glBegin(GL_POLYGON);
		glVertex2f(425,212.5);
		glVertex2f(425,262.5);
		glVertex2f(475,262.5);
		glVertex2f(475,212.5);
	glEnd();
	if(sig.compare("GREEN") ==0){
		glColor3f(0,1.0,0.0);
	}
	else glColor3f(1,1,1);
	glBegin(GL_POLYGON);
		glVertex2f(425,287.5);
		glVertex2f(425,337.5);
		glVertex2f(475,337.5);
		glVertex2f(475,287.5);
	glEnd();

	for(int j=0;j<vehlist.size();j++){
		float y = 0;;
		if(t >= vehlist[j].time){
			float a = vehlist[j].Vehicle_Acceleration;
			std::string s = vehlist[j].Vehicle_Type;
			if(a*(t-vehlist[j].time) < vehlist[j].Vehicle_MaxSpeed){
				float x = (0.5)*a*(t-vehlist[j].time)*(t-vehlist[j].time);
				float z = vehlist[j].x_p;
				float p = vehlist[j].y_p;
				int h=1;
				if(vehlist[j].Vehicle_Type.compare("bike") ==0){h = 0;}
				while(z < x+vehlist[j].q){
					if(p_occp(z+h,p) == false) z=z+1;
					else break;
				}
				if(z<x+vehlist[j].q){
					y = change_y(vehlist[j],z+2);
				}
				else y = p;
				if(sig.compare("RED") == 0 && vehlist[j].x_p*10 <= rd.Road_Signal){
					if(z*10 >= rd.Road_Signal){
						z = rd.Road_Signal/10;
						vehlist[j].time = t;
						vehlist[j].q = vehlist[j].x_p;
					}
				}
				vehlist[j].x_p = z;
				vehlist[j].y_p = y;
				veh(z,y,vehlist[j].Vehicle_Length,vehlist[j].Vehicle_Width,vehlist[j].Vehicle_Color);
			}
			else{
				float x = vehlist[j].Vehicle_MaxSpeed;
				float z = vehlist[j].x_p;
				float p = vehlist[j].y_p;
				int h=1;
				if(vehlist[j].Vehicle_Type.compare("bike") ==0){h = 0;}
				while(z < x + vehlist[j].x_p){
					if(p_occp(z+h,p) == false) z=z+1;
					else break;
				}
				vehlist[j].x_p = z;
				if(z<x){
					y = change_y(vehlist[j],z+2);
				}
				else y = p;
				if(sig.compare("RED") == 0 && vehlist[j].x_p*10 <= rd.Road_Signal){
					if(z*10 >= rd.Road_Signal){
						z = rd.Road_Signal/10;
						vehlist[j].time = t;
					}
				}
				vehlist[j].x_p = z;
				vehlist[j].y_p = y;
				veh(z,y,vehlist[j].Vehicle_Length,vehlist[j].Vehicle_Width,vehlist[j].Vehicle_Color);	
			}					
		}
	}
	glutSwapBuffers();
}

int main(int argc, char** argv){					// main function
	int i=0;
	std::ifstream cFile ("config.ini");
	vehicle veh[100];
	if(cFile.is_open()){
		std::string line;
		while(getline(cFile, line)){
			if(line[0] == '#' || line.empty()) continue;	
			char dl = ' ';
			std::vector<std::string> res = splitStrings(line, dl);
			if(res[0].compare("vehicle") == 0){
				i++;
			}
			else if(res[0].compare("Vehicle_Type") == 0){
				veh[i].Vehicle_Type = res[1];
			}
			else if(res[0].compare("Vehicle_Length") == 0){
				veh[i].Vehicle_Length = stoi(res[1], nullptr, 10);
			}
			else if(res[0].compare("Vehicle_Width") == 0){
				veh[i].Vehicle_Width = stoi(res[1], nullptr, 10);
			}
			else if(res[0].compare("Vehicle_MaxSpeed") == 0){
				veh[i].Vehicle_MaxSpeed = stoi(res[1], nullptr, 10);
			}
			else if(res[0].compare("Vehicle_Acceleration") == 0){
				veh[i].Vehicle_Acceleration = stof(res[1], nullptr);
			}
			else if(res[0].compare("Road_Id") == 0){
				rd.Road_Id = stoi(res[1], nullptr, 10);
			}
			else if(res[0].compare("Road_Length") == 0){
				rd.Road_Length = stoi(res[1], nullptr, 10);
			}
			else if(res[0].compare("Road_Width") == 0){
				rd.Road_Width = stoi(res[1], nullptr, 10);
			}
			else if(res[0].compare("Road_Signal") == 0){
				rd.Road_Signal = stoi(res[1], nullptr, 10);
			}
			else if(res[0].compare("Signal") == 0){
				signal b;
				b.Signal_Color = res[1];
				b.Signal_Time = stoi(res[2], nullptr, 10);
				siglist.push_back(b);
			}
			else if(res[0].compare("start_sim") == 0){
				glutInit(&argc,argv);
		   	 	glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE);
	                	glutInitWindowSize(2000,1000);
				glutInitWindowPosition(0,0);
				glutCreateWindow("traffic simulation");
				initOpenGl();
				glutDisplayFunc(display);
				glutIdleFunc(display);
				glutTimerFunc(0,timer,0);
				glutMainLoop();
			}
			else{
				int z = 1;
				while(res[0].compare(veh[z].Vehicle_Type) != 0){
					if(z < i) z++;
					else std::cout << "Error";
				}
				vehicle a;
				a.Vehicle_Type = veh[z].Vehicle_Type;
				a.Vehicle_Length = veh[z].Vehicle_Length;
				a.Vehicle_Width = veh[z].Vehicle_Width;
				a.Vehicle_MaxSpeed = veh[z].Vehicle_MaxSpeed;
				a.Vehicle_Acceleration = veh[z].Vehicle_Acceleration;
				a.x_p = 0;
				a.y_p = 0;
				a.q=0;
				a.Vehicle_Color = res[1];
				a.time = stoi(res[2], nullptr, 10);
				vehlist.push_back(a);			
			}
		}			
	}
	else{
		std::cerr << "Couldn't open config file for reading.\n";
	}
}

void timer(int){
	t = t + 0.03125;
	sig = ret_sig(t,a);
	a = sig;
	glutTimerFunc(1000/50,timer,0);
	glutPostRedisplay();

}
