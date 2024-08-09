#include<bits/stdc++.h>
#include<graphics.h>
#include<queue>
#include<vector>
#include<map>
#include<cstdlib>
#include<string>
#include<conio.h>
#include<cmath>
#include<ctime>
#include<time.h>
#define G 0.1
using namespace std;

const int Bloom_node_amount=25;
const int Pursuit_node_amount=17;
const int Physics_node_amount=8;
//Vector and Calculation
struct vec{
	double x,y;
};

vec vecPlus(vec a,vec b){
	return (vec){a.x+b.x,a.y+b.y};
}
vec vecMinus(vec a,vec b){
	return (vec){a.x-b.x,a.y-b.y};
}
vec vecQproduct(vec a,vec b){
	return (vec){a.x*b.x,a.y*b.y};
}
double vecDproduct(vec a,vec b){
	return a.x*b.x+a.y*b.y;
}
double vecnorm(vec a){
	return vecDproduct(a,a);
} 


//particle attribute
struct Style{
	string s;
	int size;
};
struct particle{
	vec pos,v,ov;
	double lightness;
	Style particle_style;
	int color,life;
};
//Physics engine
struct GP{
	int x,y,r;
};
vector<GP> gpoint;
void Collision_rebound(particle &p);
void Gravitational_point();
vec Gravity(particle p);
void Physics_engine();
char col='n',gp='n';

//Menu
struct Menu{
	string type,title,text[15],inp;
	int up_step,son_node[11],father_node,text_amount;
	char cho;
}Bloom_node[Bloom_node_amount+5],Pursuit_node[Pursuit_node_amount+5],Physics_node[Physics_node_amount+5]; 

void menu_iteration(Menu node[],int x);
void text_input(vector<string> &v,string s);
void array_input(vector<int> &v,string s);
void Menu_init(Menu node[],const int N,string s[],int up[],string t[],string son[]);

//Main
void Help();
void Bloom();
void Pursuit();

//Graphics
void Display();
void Empty();

//Bloom
void Bloom_menu_init();
void Bloom_par_init();
void Bloom_Motion();
void Bloom_input();
double Bloom_double[7]={0,2,0.98,0.98,1,7,7};
int Bloom_int[9]={0,50,2,10,1,150,1,1,1};
char Bloom_char[3]={' ','n','n'};
string particle_sizes;

//Pursuit
void Pursuit_menu_init();
void Pursuit_par_init();
void Pursuit_Motion();
void Pursuit_input();

int last_x,last_y,current_x,current_y;
int Pursuit_int[6]={0,1,300,1,1,2};
double Pursuit_double[5]={0,2,1,1,0.001},sense;
char Pursuit_char[5]={' ','n','y'},Pursuit_style;

//Auxiliary function
int rd_assignment();
void SetColor(UINT uFore,UINT uBack);
void output(string s);
vector<double> number_transformer(string s);
double angle_calculation(int sx,int sy,int ex,int ey);
int distance_calculation(int x1,int y1,int x2,int y2);

int Width,Height,cursor_x,cursor_y,born_x,born_y;
double t=0,t_r,interval,t_l;

queue<particle> Draw_par;
queue<particle> volume;
ExMessage Msg;
COORD coord;

double route_length,x_rst,y_rst,launch_rate,velocity_x,velocity_y,g0=0;
int particle_amount,color_amount,color_range,color_ac,particle_lifetime,mode,particle_size[20],total_size;
char add_white='n',mass_option='n',cursor_tracking='y';
bool click;

int main()
{
	Width=GetSystemMetrics(SM_CXSCREEN),Height=GetSystemMetrics(SM_CYSCREEN);
	Width-=100;
	Height-=100;
	initgraph(Width,Height,EX_SHOWCONSOLE);
	srand(time(NULL));
    while(1){
    	system("cls");
    	SetColor(9,0);
    	output("particle Displayer\n\n");
    	SetColor(8,0);
		printf("Main Menu\n\n");
		SetColor(15,0);
		printf("1.Bloom\n2.Pursuit\n");
		SetColor(14,0);
		printf("H.Help\n");
		printf("Q.Quit\n\n");
		SetColor(11,0);
		output("Press the buttons on the keyboard to enter the options(example:press 'H' to enter 'Help')");
		char key=getch();
		switch(key){
			case 'h':{
				Help();
				break;
			}
			case '1':{
				Bloom_menu_init();
				Bloom();
				break;
			}
			case '2':{
				Pursuit_menu_init(); 
				Pursuit();
				break;
			}
			case 'q':{	
				SetColor(15,0);
				return 0;
			}
		}	
	}
	return 0;
}

//Physics engine
void Collision_rebound(particle &p){
	double vx=p.v.x,vy=p.v.y,px=p.pos.x,py=p.pos.y;
	px+=vx,py+=vy;
	bool f1,f2,f3,f4;
	for(int i=1;i<=2;i++){
		f1=f2=f3=f4=true;
		if(px>=Width&&vx!=0){
			py=py+vy*(Width-p.pos.x)/vx;
			px=p.pos.x;
			vx=-vx;
			f1=false;
		}
		if(px<=0&&vx!=0){
			py=py+vy*(p.pos.x)/vx;
			px=p.pos.x;
			vx=-vx;
			f2=false;
		}
		if(py>=Height&&vy!=0){
			px=px+vx*(Height-p.pos.y)/vy;
			py=p.pos.y;
			vy=-vy;
			f3=false;
		}
		if(py<=0&&vy!=0){
			px=px+vx*(p.pos.y)/vy;
			py=p.pos.y;
			vy=-vy;
			f4=false;
		}
		if(f1&&f2&&f3&&f4)break;
//		cout<<"yes"<<endl;
	}
	p.v.x=vx;
	p.v.y=vy;
	p.pos.x=px;
	p.pos.y=py;
}

void Gravitational_point(){
	SetColor(15,0);
	system("cls");
	printf("left/right click the mouse to set/delete gravitational point\npress 'w'/'s' to increase/reduce the radius of gravitational point");
	t=clock();
	cleardevice();
	setfillcolor(RGB(255,255,255));
	int x,y,r=6;
	for(int i=0;i<gpoint.size();i++){
		solidcircle(gpoint[i].x,gpoint[i].y,gpoint[i].r);
	}
	while(1){
		char key;
		if(_kbhit()){
			key=getch();
			switch(key){
				case 'w':{
					r++;
					break;
				}
				case 's':{
					if(r==1)break;
					r--;
					break;
				}
				case 'q':{
					cleardevice();
					return;
				}
			}
			system("cls");
			printf("left/right click the mouse to set/delete gravitational point\npress 'w'/'s' to increase/reduce the radius of gravitational point\ncurrent radius:%d",r);
		}
		t_r=clock();
		interval=(double)(t_r-t)/CLOCKS_PER_SEC;
		peekmessage(&Msg,EX_MOUSE);
		if(Msg.message==WM_LBUTTONDOWN&&interval>=0.1){
			t=t_r;
			x=Msg.x;
			y=Msg.y;
			solidcircle(x,y,r);
			gpoint.push_back((GP){x,y,r}); 
		}
		if(Msg.message==WM_RBUTTONDOWN&&interval>=0.1){
			t=t_r;
			x=Msg.x;
			y=Msg.y;
			int l=gpoint.size();
			for(int i=0;i<l;i++){
				if(distance_calculation(x,y,gpoint[i].x,gpoint[i].y)<=gpoint[i].r*gpoint[i].r){
					cleardevice();
					gpoint.erase(gpoint.begin()+i);
					for(int j=0;j<gpoint.size();j++){
						solidcircle(gpoint[j].x,gpoint[j].y,gpoint[j].r);
					}
					break;
				}
			}
		}
		flushmessage();
	}
}

vec Gravity(particle p){
	vec a={0,0};
	double Pi=3.1415926;
	int l=gpoint.size(),x,y,d;
	for(int i=0;i<l;i++){
		x=gpoint[i].x;
		y=gpoint[i].y;
		d=distance_calculation(x,y,p.pos.x,p.pos.y);
		double g;
		if(d>=gpoint[i].r*gpoint[i].r){
			g=G*pow(gpoint[i].r,3)/d;
		}else{
			g=G*sqrt(d);
		}
		double ang=angle_calculation(p.pos.x,p.pos.y,x,y);
		a.x+=g*cos(ang*Pi/180);
		a.y+=g*sin(ang*Pi/180);
	}
	return a;
}

void Physics_engine(){	
	/*
	2.gravitational acceleration
1	3.collision rebound
		5.enabled
		6.disabled
	4.gravitational point
	    7.enabled
	    8.disabled
	*/
	string Physics_text[Physics_node_amount+1]={
	"",
	"1.gravitational acceleration/2.collision rebound/3.gravitational point",
	"Input the gravitational acceleration(recommended range is 0~0.20)",
	"1.enabled/2.disabled",
	"1.enabled/2.disabled",
	"",
	"",
	"",
	""};
	int Physics_up_step[Physics_node_amount+1]={0,1,1,1,1,2,2,2,2};
	string Physics_type[Physics_node_amount+1]={"","o","i","o","o","o","o","f","o"};
	string Physics_son_node[Physics_node_amount+1]={	
	"",
	"2/3/4",
	"",
	"5/6",
	"7/8",
	"",
	"",
	"",
	""};
	Menu_init(Physics_node,Physics_node_amount,Physics_text,Physics_up_step,Physics_type,Physics_son_node);
	menu_iteration(Physics_node,1);
	if(Physics_node[3].cho=='1'){
		col='y';
	}else{
		col='n';
	}
	if(Physics_node[4].cho=='1'){
		gp='y';
	}else{
		gp='n';
	}
	if(Physics_node[2].inp!="")g0=number_transformer(Physics_node[2].inp)[0];
}

//Menu
void menu_iteration(Menu node[],int x){
	int n;
	if(x==0)return;
	system("cls");
	for(int i=1;i<=node[x].text_amount;i++){
		cout<<node[x].text[i]<<endl;
	}
	if(node[x].son_node[0]==0){
		string s;
		if(node[x].type=="i"){
			cin>>s;
			if(s!="q")node[x].inp=s;
		}
		if(node[x].type=="f"){
			if(x==7)Gravitational_point();
		}
		n=node[x].up_step;
		for(int i=1;i<=n;i++){
			x=node[x].father_node; 
		}
		menu_iteration(node,x);
		return;
	}
	while(1){
		node[x].cho=getch();
		if(node[x].cho=='q'){
			n=node[x].up_step;
			for(int i=1;i<=n;i++){
				x=node[x].father_node; 
			}
			menu_iteration(node,x);
			return;
		}else if(node[x].cho-'1'<0||node[x].cho-'0'>node[x].son_node[0]){
			continue;
		}else{
			x=node[x].son_node[node[x].cho-'0'];
			menu_iteration(node,x);
			return;
		}
	}
}

void text_input(vector<string> &v,string s){
	s+="/";
	int p=0;
	for(int i=0;i<s.size();i++){
		if(s[i]=='/'){
			v.push_back(s.substr(p,i-p));
			p=i+1;
		} 
	}
	if(v.size()==2&&v[1]=="")v.pop_back();
}

void array_input(vector<int> &v,string s){
	s+="/";
	int p=0,num;
	for(int i=0;i<s.size();i++){
		if(s[i]=='/'){
			num=atof(s.substr(p,i-p).c_str());
			v.push_back(num);
			p=i+1;
		}
	}
	if(v.size()==2&&v[1]==0)v.pop_back();
}

void Menu_init(Menu node[],const int N,string s[],int up[],string t[],string son[]){
	//text initialization 
	vector<string> txt[N+1];
	for(int i=1;i<=N;i++){
		txt[i].push_back("");
		text_input(txt[i],s[i]);
	}
	for(int i=1;i<=N;i++){
		for(int j=1;j<txt[i].size();j++){
			node[i].text[j]=txt[i][j];
		}
		node[i].text_amount=txt[i].size()-1;
	}
	//up_step initialization
	for(int i=1;i<=N;i++){
		node[i].up_step=up[i];
	}
	//type initialization
	for(int i=1;i<=N;i++){
		node[i].type=t[i];
	}
	//tree initialization
    vector<int> point[N+1];
	for(int i=1;i<=N;i++){
		point[i].push_back(0);
		array_input(point[i],son[i]);
	}
	for(int i=1;i<=N;i++){
		for(int j=1;j<point[i].size();j++){
			node[i].son_node[j]=point[i][j];
		} 
		node[i].son_node[0]=point[i].size()-1;
	}
	for(int i=1;i<=N;i++){
		for(int j=1;j<=node[i].son_node[0];j++){
			node[node[i].son_node[j]].father_node=i;
		}
	}
	node[1].father_node=0;
}

//Main
void Help(){	
	system("cls");
	SetColor(8,0);	
	printf("Main Menu->Help\n\n");
	SetColor(15,0);
	output("This is a program that you can appreciate various particles\n");
	Sleep(500);
	output("¡¤By setting values to achieve the desired particle effect\n");
	Sleep(500);
	output("¡¤Press 'c' to clear all the particles on the screen at any time\n");
	output("Have fun exploring!\n");
	SetColor(14,0); 
	output("Press any button to quit current page");
	char ct=getch();
	return; 
}

void Bloom(){
	t=clock();
	system("cls");
	SetColor(8,0);	
	printf("Main Menu->Bloom\n\n");
	SetColor(14,0);
	printf("S.Settings\nP.Physics engine\nQ.quit the program");
	while(1){
//		system("cls");
//		SetColor(8,0);	
//		printf("Main Menu->Bloom\n\n");
//		SetColor(14,0);
//		printf("S.Settings\nEsc.quit the program");
//		printf("\n\ncurrent particle amount:%d\n",Draw_par.size());
		char key=' ';
		if(_kbhit()){
			key=getch();
		}
		switch(key){
			case 's':{
				menu_iteration(Bloom_node,1);
				Bloom_input(); 
				Empty();
				system("cls");
				SetColor(8,0);	
				printf("Main Menu->Bloom\n\n");
				SetColor(14,0);
				printf("S.Settings\nP.Physics engine\nQ.quit the program");
				break;
			}
			case 'p':{
				Physics_engine();
				system("cls");
				SetColor(8,0);
				printf("Main Menu->Bloom\n\n");
				SetColor(14,0);
				printf("S.Settings\nP.Physics engine\nQ.quit the program");
				break;
			}
			case 'c':{
				Empty();
				cleardevice();
				break;
			}
			case 'q':{
				return;
			}
		}
		t_r=clock();
		interval=(double)(t_r-t)/CLOCKS_PER_SEC;
		if(mode==1){
			peekmessage(&Msg,EX_MOUSE);
			if(Msg.message==WM_LBUTTONDOWN&&interval>=0.1){
				born_x=Msg.x;
				born_y=Msg.y;
//				cout<<Msg.x<<' '<<Msg.y<<endl;
				t=t_r; 
				Bloom_par_init();
			}
			flushmessage();
			Bloom_Motion();
			Display();
		}
	    if(mode==2){
	    	if(interval>=launch_rate){
	    		born_x=rand()%(Width-200)+100;
	    		born_y=rand()%(Height-200)+100;
	    		t=t_r; 
				Bloom_par_init();
			}
			Bloom_Motion();
			Display();
		}
	}
	return;
}

void Pursuit(){
	t=clock();
	t_l=clock();
	system("cls");
	SetColor(8,0);	
	printf("Main Menu->Pursuit\n\n");
	SetColor(14,0);
	printf("S.Settings\nP.Physics engine\nQ.quit the program");
	peekmessage(&Msg,EX_MOUSE);
	last_x=Msg.x,last_y=Msg.y;
	while(1){
		click=false;
		char key=' ';
		if(_kbhit()){
			key=getch();
		}
		switch(key){
			case 's':{
				menu_iteration(Pursuit_node,1);
				Pursuit_input(); 
				Empty();
				system("cls");
				SetColor(8,0);
				printf("Main Menu->Pursuit\n\n");
				SetColor(14,0);
				printf("S.Settings\nP.Physics engine\nQ.quit the program");
				break;
			}
			case 'p':{
				Physics_engine();
				system("cls");
				SetColor(8,0);
				printf("Main Menu->Pursuit\n\n");
				SetColor(14,0);
				printf("S.Settings\nP.Physics engine\nQ.quit the program");
				break;
			}
			case 'c':{
				Empty();
				cleardevice();
				break;
			}
			case 'q':{
				return;
			}
		}
		if(mode==1){
			t_r=clock();
			interval=(double)(t_r-t)/CLOCKS_PER_SEC;
			peekmessage(&Msg,EX_MOUSE);
			if(interval>=sense){
				if(Msg.message==WM_LBUTTONDOWN&&t_r-t_l>=0.02){
					click=true;
					t_l=t_r;
				}
				current_x=Msg.x,current_y=Msg.y;
				Pursuit_par_init();
				last_x=current_x,last_y=current_y;
				t=t_r;
			}
			flushmessage();
			Pursuit_Motion();
			Display();
		}
	} 
}

//Graphics
void Display(){
	if(Draw_par.empty()){
		cleardevice();
		return;
	}
	int length=Draw_par.size();
	cleardevice();
	BeginBatchDraw();
	particle p;
	for(int i=0;i<length;i++){
	    p=Draw_par.front();
	    if(p.color==114514)setfillcolor(HSLtoRGB(p.color,0,p.lightness));
	    else setfillcolor(HSLtoRGB(p.color,1,p.lightness));
	    if(p.particle_style.s=="p"){
	    	solidcircle(p.pos.x,p.pos.y,p.particle_style.size);
		}
	    Draw_par.pop();
	    p.lightness/=route_length;
	    Draw_par.push(p);
	}
	EndBatchDraw();
	while(!Draw_par.empty()&&Draw_par.front().lightness<0.02){
		Draw_par.pop();
	}
	Sleep(1);
	return;
}

void Empty(){
	while(!Draw_par.empty()){
		Draw_par.pop();
	}
	while(!volume.empty()){
		volume.pop();
	}
}

//Bloom
void Bloom_menu_init(){
	route_length=Bloom_double[1];
	x_rst=Bloom_double[2],y_rst=Bloom_double[3];
	launch_rate=Bloom_double[4];
	velocity_x=Bloom_double[5],velocity_y=Bloom_double[6];
	particle_amount=Bloom_int[1];
	color_amount=Bloom_int[2];
	color_range=Bloom_int[3];
	color_ac=Bloom_int[4];
	particle_lifetime=Bloom_int[5];
	mode=Bloom_int[6];
	particle_size[1]=Bloom_int[7];
	total_size=Bloom_int[8];
	add_white=Bloom_char[1];
	mass_option=Bloom_char[2];
	string Bloom_text[Bloom_node_amount+1]={
	"",
	"1.route length/2.particle amount/3.physical parameter settings/4.color/5.particle lifetime/6.Add white particle/7.particle size/8.mode",
	"Input the route length(recommended range is 1.5~2.0,high value lead to shorter route length)",
	"Input the particle amount(recommended range is 30~100,too many particles may cause lagging)",
	"1.motion resistance/2.particle initial speed/3.Add particle mass",
	"1.Color types(single,double etc.,decide to how many colors there are in one blooming)/2.Color range(decide how many colors there are in total)",
	"Input the particle lifetime(decide how long will the particles exist,recommended range is 100~200)", 
	"Add white?(white particles will appear,input y,n,y=yes,n=no)",
	"Input the size of particle(if you want two or more sizes,seperate the size with ','(example:1,2),recommended sizes are 1,2,3)",
	"1.Manual mode(Click to launch particles with the mouse)/2.Automatic mode",
	"Input the resistance(both x,y direction,seperated with space,decide the rate of slowing down,recommended range is 0.90~1.00)",
	"Input the particle speed(both x,y direction,seperated with space,recommended range is 5~10",
	"Add particle mass?(Larger particles will have greater resistance,input y,n,y=yes,n=no)",
	"1.Single Color/2.Double Color/3.Input the amount",
	"1.6colors/2.10colors/3.20colors/4.36colors",
	"",
	"Input the launch rate(per second):",
	"",
	"1.Adjacent color(ÁÚ½üÉ«)/2.Complementary color(»¥²¹É«)",
	"Input the amount:",
	"",
	"",
	"",
	"",
	"",
	""};
	int Bloom_up_step[Bloom_node_amount+1]={0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3};
	string Bloom_type[Bloom_node_amount+1]={"","o","i","i","o","o","i","i","i","o","i","i","i","o","o","o","i","o","o","i","o","o","o","o","o","o"};
	string Bloom_son_node[Bloom_node_amount+1]={
	"",
	"2/3/4/5/6/7/8/9",
	"","",
	"10/11/12",
	"13/14",
	"","","",
	"15/16",
	"","","",
	"17/18/19",
	"20/21/22/23",
	"","","",
	"24/25",
	};
	Menu_init(Bloom_node,Bloom_node_amount,Bloom_text,Bloom_up_step,Bloom_type,Bloom_son_node);
}

void Bloom_par_init(){
	int vdir;
	double ang,sep;
	ang=(rand()%color_range)*360/color_range;
	for(int i=1;i<=particle_amount;i++){
		particle p;
		p.particle_style={"p",1};
		p.color=0;
		p.pos=(vec){born_x*1.0,born_y*1.0};
		vdir=rd_assignment();
		p.v.x=(rand()%((int)velocity_x*10)+1)/10.0*vdir;
		vdir=rd_assignment();
		p.v.y=(rand()%((int)velocity_y*10)+1)/10.0*vdir;
		p.lightness=0.5;
		p.life=particle_lifetime; 
		p.particle_style.size=particle_size[rand()%total_size+1];
		if(color_amount==1){
			p.color=ang;
		}
		if(color_amount==2){
			if(color_ac==1){
				if(rd_assignment()==1)p.color=ang;
				else p.color=ang+60;
			}
			if(color_ac==2){
				if(rd_assignment()==1)p.color=ang;
				else p.color=ang+180;
			}
		}
		if(color_amount>=3){
			if(color_amount%2==1){
				sep=45.0/(color_amount/2);
				p.color=ang+rand()%(color_amount/2+1)*rd_assignment()*sep;
			}
			if(color_amount%2==0){
				sep=90/(color_amount-1);
				p.color=ang-45+(rand()%color_amount)*sep;
			}
		}
		if(add_white=='y'&&i%6==0){
			p.color=114514;
			p.lightness=1;
		}
		volume.push(p);
	}	
    sep=60/color_amount;
	return;
}

void Bloom_Motion(){
	particle p;
	int length=volume.size();
	for(int i=1;i<=length;i++){
		p=volume.front();
		volume.pop();
		if(mass_option=='y'){
			p.v=vecQproduct(p.v,(vec){x_rst-(p.particle_style.size-1)*0.015,y_rst-(p.particle_style.size-1)*0.015});
		}else{
			p.v=vecQproduct(p.v,(vec){x_rst,y_rst});
		}
		p.v.y+=g0;
		if(gp=='y')p.v=vecPlus(p.v,Gravity(p));
	   	if(col=='y')Collision_rebound(p);
		else p.pos=vecPlus(p.pos,p.v);
		if(col=='n'&&gp=='n'&&g0==0&&(p.pos.x>Width||p.pos.x<0||p.pos.y>Height||p.pos.y<0)){
			continue;
		}
	    if(p.life<=20&&p.lightness<=0.5)p.lightness/=1.2;
	    p.life--;
		Draw_par.push(p);
		volume.push(p);
	}
	while(!volume.empty()&&volume.front().life==0)volume.pop();
	return;
}

void Bloom_input(){
	if(Bloom_node[2].inp!="")route_length=number_transformer(Bloom_node[2].inp)[0];
	if(Bloom_node[3].inp!="")particle_amount=(int)number_transformer(Bloom_node[3].inp)[0];
	if(Bloom_node[10].inp!=""){
		x_rst=number_transformer(Bloom_node[10].inp)[0];
		y_rst=number_transformer(Bloom_node[10].inp)[1];
//		for(int i=0;i<number_transformer(Bloom_node[10].inp).size();i++)cout<<number_transformer(Bloom_node[10].inp)[i]<<' ';
//		_getch(); 
	}
	if(Bloom_node[11].inp!=""){
		velocity_x=number_transformer(Bloom_node[11].inp)[0];
		velocity_y=number_transformer(Bloom_node[11].inp)[1];
	}
	if(Bloom_node[12].inp!="")mass_option=Bloom_node[12].inp[0];
	if(Bloom_node[6].inp!="")particle_lifetime=(int)number_transformer(Bloom_node[6].inp)[0];
	if(Bloom_node[7].inp!="")add_white=Bloom_node[7].inp[0]; 
	if(Bloom_node[8].inp!=""){
		vector<double> v=number_transformer(Bloom_node[8].inp);
		total_size=v.size();
		for(int i=1;i<=total_size;i++){
			particle_size[i]=(int)v[i-1]-1;
		}
	}
	switch(Bloom_node[13].cho){
		case '1':{
			color_amount=1;
			break;
		}
		case '2':{
			color_amount=2;
			if(Bloom_node[18].cho=='1')color_ac=1;
			else color_ac=2;
			break;
		}
		case '3':{
			if(Bloom_node[19].inp!="")color_amount=(int)number_transformer(Bloom_node[19].inp)[0];
			break;
		}
	}
	switch(Bloom_node[14].cho){
		case '1':{
			color_range=6;
			break;
		}
		case '2':{
			color_range=10;
			break;
		}
		case '3':{
			color_range=20;
			break;
		}
		case '4':{
			color_range=36;
			break;
		}
	}
	switch(Bloom_node[9].cho){
		case '1':{
			mode=1;
			break;
		}
		case '2':{
			mode=2;
			if(Bloom_node[16].inp!="")launch_rate=number_transformer(Bloom_node[16].inp)[0];
			break;
		}
	}
}

//Pursuit
void Pursuit_menu_init(){
	particle_amount=Pursuit_int[1];
	particle_lifetime=Pursuit_int[2];
	total_size=Pursuit_int[3];
	mode=Pursuit_int[4];
	particle_size[1]=Pursuit_int[5];
	add_white=Pursuit_char[1];
	cursor_tracking=Pursuit_char[2];
	route_length=Pursuit_double[1];
	x_rst=Pursuit_double[2];
	y_rst=Pursuit_double[3];
	sense=Pursuit_double[4];
	string Pursuit_text[Pursuit_node_amount+1]={
	"",
	"1.route length/2.particle amount/3.physical parameter settings/4.sensitivity/5.particle lifetime/6.particle size/7.mode/8.cursor tracking",
	"Input the route length(recommended range is 1.5~2.0,high value lead to shorter route length)",
	"Input the particle amount(recommended range is 1~5,too many particles may cause lagging)",
	"1.motion resistance/2.Add particle mass",
	"1.low/2.medium/3.high",
	"Input the particle lifetime(decide how long will the particles exist,recommended range is 100~500)",
	"Input the size of particle(if you want two or more sizes,seperate the size with ','(example:1,2),recommended sizes are 1,2,3)",
	"1.Manual mode(Click to launch particles with the mouse)/2.Automatic mode",
	"Enable cursor tracking?(particles will be launched with the move of cursor,y or n)",
	"Input the resistance(both x,y direction,seperated with space,decide the rate of slowing down,recommended range is 0.90~1.00)",
	"Add particle mass?(Larger particles will have greater resistance,y or n)",
	"",
	"",
	"",
	"",
	"1.heart",
	""};
	int Pursuit_up_step[Pursuit_node_amount+1]={0,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,3};
	string Pursuit_type[Pursuit_node_amount+1]={"","o","i","i","o","o","i","i","o","i","i","i","o","o","o","o","o","o"};
	string Pursuit_son_node[Pursuit_node_amount+1]={
	"",
	"2/3/4/5/6/7/8/9",
	"",
	"",
	"10/11",
	"12/13/14",
	"",
	"",
	"15/16",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"17",
	""};
	Menu_init(Pursuit_node,Pursuit_node_amount,Pursuit_text,Pursuit_up_step,Pursuit_type,Pursuit_son_node);
//	cout<<Pursuit_node[3].up_step;
//	_getch();
	/*
	2.route length
	3.particle amount
	4.physical parameter settings
		10.motion resistance
		11.Add particle mass
	5.sensitivity
1		12.low
		13.medium
		14.high
	6.particle lifetime
	7.particle size
	8.mode
		15.Manual mode
		16.Automatic mode
			17.heart
	9.cursor trakcing
	*/
}

void Pursuit_par_init(){
	double k;
	int vdir;
	if(current_x==last_x&&current_y==last_y&&!click)return;
	for(int i=1;i<=particle_amount;i++){
		particle p;
		if(click){
			p.color=114514;
			p.lightness=1;
			vdir=rd_assignment();
			p.v.x=rand()%10/10.0*vdir;
			vdir=rd_assignment();
			p.v.y=rand()%10/10.0*vdir;
			p.particle_style={"p",particle_size[rand()%total_size+1]};
			p.pos=(vec){current_x*1.0,current_y*1.0};
			p.life=particle_lifetime; 
			volume.push(p);
			continue;
		}
		if(cursor_tracking=='y'){
			p.particle_style={"p",particle_size[rand()%total_size+1]};
			p.pos=(vec){current_x*1.0,current_y*1.0};
			k=(rand()%15+9)/10.0;
			p.v.x=(current_x-last_x)*0.2*k;
			k=(rand()%15+9)/10.0;
			p.v.y=(current_y-last_y)*0.2*k;
			if(p.v.x==0&&p.v.y!=0){
				vdir=rd_assignment();
				p.v.x=rand()%20/100.0*vdir;
			}
			if(p.v.y==0&&p.v.x!=0){
				vdir=rd_assignment();
				p.v.y=rand()%20/100.0*vdir;
			}
			p.lightness=0.5;
			p.life=particle_lifetime; 
			p.color=angle_calculation(last_x,last_y,current_x,current_y);
			volume.push(p);
		}
	}
}

void Pursuit_Motion(){
	particle p;
	int length=volume.size();
	for(int i=1;i<=length;i++){
		p=volume.front();
		volume.pop();
		if(mass_option=='y'){
			p.v=vecQproduct(p.v,(vec){x_rst-(p.particle_style.size-1)*0.015,y_rst-(p.particle_style.size-1)*0.015});
		}else{
			p.v=vecQproduct(p.v,(vec){x_rst,y_rst});
		}
		p.v.y+=g0;
		if(gp=='y')p.v=vecPlus(p.v,Gravity(p));
	   	if(col=='y')Collision_rebound(p);
		else p.pos=vecPlus(p.pos,p.v);
		if(col=='n'&&gp=='n'&&g0==0&&(p.pos.x>Width||p.pos.x<0||p.pos.y>Height||p.pos.y<0)){
			continue;
		}
	    if(p.life<=20&&p.lightness<=0.5)p.lightness/=1.2;
	    p.life--;
		Draw_par.push(p);
		volume.push(p);
	}
	while(!volume.empty()&&volume.front().life==0)volume.pop();
	return;
}

void Pursuit_input(){
	if(Pursuit_node[2].inp!="")route_length=number_transformer(Pursuit_node[2].inp)[0];
	if(Pursuit_node[3].inp!="")particle_amount=(int)number_transformer(Pursuit_node[3].inp)[0];
	if(Pursuit_node[6].inp!="")particle_lifetime=(int)number_transformer(Pursuit_node[6].inp)[0];
	if(Pursuit_node[7].inp!=""){
		vector<double> v=number_transformer(Pursuit_node[7].inp);
		total_size=v.size();
		for(int i=1;i<=total_size;i++){
			particle_size[i]=(int)v[i-1]-1;
		}
	}
	if(Pursuit_node[9].inp!="")cursor_tracking=Pursuit_node[9].inp[0];
	if(Pursuit_node[10].inp!=""){
		x_rst=number_transformer(Pursuit_node[10].inp)[0];
		y_rst=number_transformer(Pursuit_node[10].inp)[1];
	}
	if(Pursuit_node[11].inp!="")mass_option=Pursuit_node[11].inp[0];
	switch(Pursuit_node[5].cho){
		case '1':{
			sense=0.001;
			break;
		}
		case '2':{
			sense=0.01;
			break;
		}
		case '3':{
			sense=0.1;
			break;
		}
	}
	switch(Pursuit_node[8].cho){
		case '1':{
			mode=1;
			break;
		}
		case '2':{
			mode=2;
			if(Pursuit_node[16].cho!=' ')Pursuit_style=Pursuit_node[16].cho;
			break;
		}
	}
}
//Auxiliary function
int rd_assignment(){
	if(rand()%2==0)return -1;
	return 1;
}

void SetColor(UINT uFore,UINT uBack){
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle,uFore+uBack*0x10);
	/*
	0 = black       8 = gray
	1 = blue        9 = light blue
	2 = green       10 = light green
	3 = pale green  11 = light plae green
	4 = red         12= light red
	5 = purple      13 = light purple
	6 = yellow      14 = light yellow
	7 = white       15 = bright white 
	*/
} 

void output(string s){
	for(int i=0;i<s.size();i++){
		printf("%c",s[i]);
		Sleep(10);
	}
}

vector<double> number_transformer(string s){
	vector<double> v;
	int p=0;
	s+=",";
	for(int i=0;i<s.size();i++){
		if(s[i]==','){
			string num=s.substr(p,i-p);
			double n=atof(num.c_str());
			p=i+1;
			v.push_back(n); 	
		}
	}
	return v;
}

double angle_calculation(int sx,int sy,int ex,int ey){
	int dx=ex-sx;
	int dy=ey-sy;
	double Pi=3.1415926;
	if(dx==0&&dy>0)return 90.0;
	if(dx==0&&dy<0)return -90.0;
	if(dy==0&&dx>0)return 0;
	if(dy==0&&dx<0)return 180.0;
	double ang=dy*1.0/dx;
	if(dx>0)return atan(ang)*180/Pi;
	if(dx<0)return atan(ang)*180/Pi+180;
}

int distance_calculation(int x1,int y1,int x2,int y2){
	return (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
}
