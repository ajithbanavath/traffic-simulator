#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <vector>

class vehicle{
	public :
		std::string Vehicle_Type;
		int Vehicle_Length;
		int Vehicle_Width;
		int Vehicle_MaxSpeed;
		int Vehicle_Acceleration;
		std::string Vehicle_Color;
		int x_p;
		int y_p;
		int time;
		
};

class road{
	public :
		int Road_Id;
		int Road_Length;
		int Road_Width;
		int Road_Signal;		
};

class signal{
	public :
		int Signal_Time;
		std::string Signal_Color;
};

std::vector <signal> siglist;

std::vector <vehicle> vehlist;

std::vector<std::string> splitStrings(std::string str, char dl) { 
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

std::string ret_sig(int x,std::string a){
	for(int t=0;t<siglist.size();t++){
		if(x == siglist[t].Signal_Time) return siglist[t].Signal_Color;
	}
	return a;
}

bool p_occp(int x, int y){
	for(int h=0;h<vehlist.size();h++){
		int a = vehlist[h].Vehicle_Length;
		int k = 0;
		while(k < a){
			if(vehlist[h].x_p-k == x && vehlist[h].y_p == y) return true;
			else k++;
		}
	}
	return false;
}

int n_veh(int y){
	int a = 0;
	for(int h=0;h<vehlist.size();h++){
		if(vehlist[h].y_p == y) a++;
	}
	return a;
}

bool l_occp(int y){
	for(int h=0;h<vehlist.size();h++){
		if(vehlist[h].y_p == y) return true;
	}
	return false;
}

int change_y(int Road_Width, vehicle a){
	if(a.y_p==1){
		if(p_occp(a.x_p,2) == false) return 2;
		else return 1;
	}
	else if(a.y_p = Road_Width){
		if(p_occp(a.x_p,Road_Width-1) == false) return Road_Width-1;
		else return Road_Width;
	}
	else{
		if(p_occp(a.x_p,a.y_p-1) == false){
			if(p_occp(a.x_p,a.y_p+1) == false){
				int q = n_veh(a.y_p-1);
				int w = n_veh(a.y_p+1);
				if(q<w) return (a.y_p-1);
				else return (a.y_p+1);
			}
			else return (a.y_p-1);
		}
		else{
			if(p_occp(a.x_p,a.y_p+1) == false) return (a.y_p+1);
			else return a.y_p;
		} 
	}
}


void gotoxy(int x, int y){
	printf("%c[%d;%df",0x1B,y,x);
}

int main(){
	int i=0;
	std::ifstream cFile ("config.ini");
	vehicle veh[100];
	std::string sig = "GREEN";
	road rd;
	if(cFile.is_open()){
		std::string line;
		while(getline(cFile, line)){
			if(line[0] == '#' || line.empty()) continue;	
			char dl = ' ';
			std::vector<std::string> res = splitStrings(line, dl);
			//std::cout << res[0] << std::endl;
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
				veh[i].Vehicle_Acceleration = stoi(res[1], nullptr, 10);
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
				for(int t=1;t<30;t++){
					std::system("clear");
					sig = ret_sig(t,sig);
					gotoxy(rd.Road_Signal,6);
					std::cout << t ;
					/*for(int k=1;k<1000;k++){
						gotoxy(k,2);
						std::cout<<"-";
					}*/
					gotoxy(20,20);
					std::cout << sig;
					gotoxy(rd.Road_Signal,4);
					std::cout << "-";					
					for(int j=0;j<vehlist.size();j++){
						int y = 1;
						if(t > vehlist[j].time){
							int a = vehlist[j].Vehicle_Acceleration;
							std::string s = vehlist[j].Vehicle_Type;
							if(a*(t-vehlist[j].time) < vehlist[j].Vehicle_MaxSpeed){
								int x = (0.5)*a*(t-vehlist[j].time)*(t-vehlist[j].time);
								int z = vehlist[j].x_p;
								int p = vehlist[j].y_p;
								while(z < x){
									if(p_occp(z+1,p) == false) z++;
									else break;
								}
								if(z<x){
									y = change_y(rd.Road_Width,vehlist[j]);
								}
								else y = p;
								if(sig.compare("RED") == 0 && vehlist[j].x_p <= rd.Road_Signal){
									if(z > rd.Road_Signal){
										z = rd.Road_Signal;
										vehlist[j].time = t;
									}
								}
								vehlist[j].x_p = z;
								vehlist[j].y_p = y;
								gotoxy(z,y);
								std::cout << s.at(0);
							}
							else{
								int x = vehlist[j].Vehicle_MaxSpeed;
								int z = vehlist[j].x_p;
								int p = vehlist[j].y_p;
								while(z < x + vehlist[j].x_p){
									if(p_occp(z+1,p) == false) z++;
									else break;
								}
								if(z<x){
									y = change_y(2,vehlist[j]);
								}
								else y = p;
								if(sig.compare("RED") == 0 && vehlist[j].x_p <= rd.Road_Signal){
									if(z > rd.Road_Signal){
										z = rd.Road_Signal;
										vehlist[j].time = t;
									}
								}
								gotoxy(z,y);
								vehlist[j].x_p = z;
								vehlist[j].y_p = y;
								std::cout << s.at(0);
							}					
						}
					}
					fflush(stdout);
					sleep(2);
				}
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
				a.x_p = 1;
				a.y_p = 1;
				a.Vehicle_Color = res[1];
				a.time = stoi(res[2], nullptr, 10);
				vehlist.push_back(a);
				//std::cout << a.Vehicle_Type << std::endl;				
			}
		}			
	}
	else{
		std::cerr << "Couldn't open config file for reading.\n";
	}
}
