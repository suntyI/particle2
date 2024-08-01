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
using namespace std;

//Vector and Calculation
struct vec{
	double x,y;
};

vec vecPlus(vec a,vec b){
	return (vec){a.x+b.x,a.y+b.y};
}
vec vecQproduct(vec a,vec b){
	return (vec){a.x*b.x,a.y*b.y};
}
int vecDproduct(vec a,vec b){
	return a.x*b.x+a.y*b.y;
}


//Particular attribute
struct Style{
	string s;
	int size;
};
struct particular{
	vec pos,v,ov;
	double lightness;
	Style particular_style;
	int color,life;
};

//Menu
struct Menu{
	string type,title,text[15],inp;
	int up_step,son_node[11],father_node,text_amount;
	char cho;
}Bloom_node[35];
void menu_iteration(Menu node[],int x);
void text_input(vector<string> &v,string s);
void array_input(vector<int> &v,string s);

queue<particular> Draw_par;
queue<particular> volume;
ExMessage Msg;
COORD coord;

int Width,Height,cursor_x,cursor_y,born_x,born_y;
double t=0,t_r,interval;

//Main
void Help();
void Bloom();
void Display();
void Menu_init(Menu node[],const int N,string s[],int up[],string son);


//Auxiliary function
int rd_assignment();
void SetColor(UINT uFore,UINT uBack);
void output(string s);
vector<double> number_transformer(string s);


//Bloom 
void Bloom_menu_init();
void Bloom_par_init();
void Bloom_Motion();
void Bloom_settings();
void Bloom_input();
const int Bloom_node_amount=25;
double route_length=2,x_rst=0.98,y_rst=0.98,launch_rate=1,velocity_x=7,velocity_y=7;
int particular_amount=50,color_amount=2,color_range=10,color_ac=1,particular_lifetime=100,mode=1,particular_size[100]={0,1},total_size=1;
char add_white='n',mass_option='n';
string particular_sizes;

int main()
{
	Width=GetSystemMetrics(SM_CXSCREEN),Height=GetSystemMetrics(SM_CYSCREEN);
	Width-=100;
	Height-=100;
	initgraph(Width,Height,EX_SHOWCONSOLE);
	srand(time(NULL));
	char op;
    while(1){
    	system("cls");
    	SetColor(9,0);
    	output("Particular Displayer\n\n");
    	SetColor(8,0);
		printf("Main Menu\n\n");
		SetColor(15,0);
		printf("1.Bloom\n");
		SetColor(14,0);
		printf("H.Help\n");
		printf("Q.Quit\n\n");
		SetColor(11,0);
		output("Press the buttons on the keyboard to enter the options(example:press 'H' to enter 'Help')");
		op=getch();
		switch(op){
			case 'h':{
				Help();
				break;
			}
			case '1':{
				Bloom_menu_init();
				Bloom();
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
		}
		if(s!="q")node[x].inp=s;
		n=node[x].up_step;
		for(int i=1;i<=n;i++){
			x=node[x].father_node; 
		}
		menu_iteration(Bloom_node,x);
		return;
	}
	while(1){
		node[x].cho=getch();
		if(node[x].cho=='q'){
			n=node[x].up_step;
			for(int i=1;i<=n;i++){
				x=node[x].father_node; 
			}
			menu_iteration(Bloom_node,x);
			return;
		}else if(node[x].cho-'1'<0||node[x].cho-'0'>node[x].son_node[0]){
			continue;
		}else{
			x=node[x].son_node[node[x].cho-'0'];
			menu_iteration(Bloom_node,x);
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

//Main
void Help(){	
	system("cls");
	SetColor(8,0);	
	printf("Main Menu->Help\n\n");
	SetColor(15,0);
	output("This is a program that you can appreciate various particulars\n");
	Sleep(500);
	output("¡¤By setting values to achieve the desired particle effect\n");
	Sleep(500);
	output("Have fun exploring!\n");
	SetColor(14,0); 
	output("Press any button to quit current page");
	char ct=getch();
	return; 
}

void Bloom(){
	system("cls");
	SetColor(8,0);	
	printf("Main Menu->Bloom\n\n");
	SetColor(14,0);
	printf("S.Settings\nEsc.quit the program");
	while(1){
//		system("cls");
//		SetColor(8,0);	
//		printf("Main Menu->Bloom\n\n");
//		SetColor(14,0);
//		printf("S.Settings\nEsc.quit the program");
//		printf("\n\ncurrent particular amount:%d\n",Draw_par.size());
		char key=0;
		if(_kbhit()){
			key=getch();
		}
		switch(key){
			case 's':{
				menu_iteration(Bloom_node,1);
				Bloom_input(); 
				system("cls");
				SetColor(8,0);	
				printf("Main Menu->Bloom\n\n");
				SetColor(14,0);
				printf("S.Settings\nEsc.quit the program");
				break;
			}
			case 27:{
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

void Display(){
	if(Draw_par.empty()){
		cleardevice();
		return;
	}
	int length=Draw_par.size();
	cleardevice();
	BeginBatchDraw();
	particular p;
	for(int i=0;i<length;i++){
	    p=Draw_par.front();
	    if(p.color==114514)setfillcolor(HSLtoRGB(p.color,0,p.lightness));
	    else setfillcolor(HSLtoRGB(p.color,1,p.lightness));
	    if(p.particular_style.s=="p"){
	    	solidcircle(p.pos.x,p.pos.y,p.particular_style.size);
		}
	    Draw_par.pop();
	    p.lightness/=route_length;
	    Draw_par.push(p);
	}
	EndBatchDraw();
	while(!Draw_par.empty()&&Draw_par.front().lightness<0.01){
		Draw_par.pop();
	}
	Sleep(10);
	return;
}

void Menu_init(Menu node[],const int N,string s[],int up[],string t[],string son[]){
	//clean volumn
	while(!Draw_par.empty())Draw_par.pop();
	//text initialization 
	vector<string> txt[N+1];
	for(int i=1;i<=N;i++){
		txt[i].push_back("");
		text_input(txt[i],s[i]);
	}
	for(int i=1;i<=25;i++){
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

//Bloom
void Bloom_menu_init(){
	string Bloom_text[Bloom_node_amount+1]={
	"",
	"1.route length/2.particular amount/3.physical parameter settings/4.color/5.particular lifetime/6.Add white particular/7.particular size/8.Mode",
	"Input the route length(recommended range is 1.5~2.0,high value lead to shorter route length)",
	"Input the particular amount(recommended range is 30~100,too many particulars may cause lagging)",
	"1.motion resistance/2.particular initial speed/3.Add particular mass",
	"1.Color types(single,double etc.,decide to how many colors there are in one blooming)/2.Color range(decide how many colors there are in total)"
	"Input the particular lifetime(decide how long will the particulars exist,recommended range is 80~120)", 
	"Add white?(white particulars will appear,input y,n,y=yes,n=no)",
	"Input the size of particular(if you want two or more sizes,seperate the size with ','(example:1,2),recommended sizes are 1,2,3)",
	"1.Manual mode(Click to launch particulars with the mouse)/2.Automatic mode",
	"Input the resistance(both x,y direction,seperated with space,decide the rate of slowing down,recommended range is 0.90~1.00)",
	"Input the particular speed(both x,y direction,seperated with space,recommended range is 5~10",
	"Add particular mass?(Larger particles will have greater resistance,input y,n,y=yes,n=no)",
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
	for(int i=1;i<=particular_amount;i++){
		particular p;
		p.particular_style={"p",1};
		p.color=0;
		p.pos=(vec){born_x,born_y};
		vdir=rd_assignment();
		p.v.x=(rand()%((int)velocity_x*10)+1)/10.0*vdir;
		vdir=rd_assignment();
		p.v.y=(rand()%((int)velocity_y*10)+1)/10.0*vdir;
		p.lightness=0.5;
		p.life=particular_lifetime; 
		p.particular_style.size=particular_size[rand()%total_size+1];
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
	particular p;
	int length=volume.size();
	for(int i=1;i<=length;i++){
		p=volume.front();
		p.v.y+=0.01;
		if(mass_option=='y'){
			p.v=vecQproduct(p.v,(vec){x_rst-(p.particular_style.size-1)*0.015,y_rst-(p.particular_style.size-1)*0.015});
		}else{
			p.v=vecQproduct(p.v,(vec){x_rst,y_rst});
		}
	    p.pos=vecPlus(p.v,p.pos);
	    if(p.life<=20&&p.lightness<=0.5)p.lightness/=1.2;
	    p.life--;
		Draw_par.push(p);
		volume.pop();
		volume.push(p);
	}
	while(!volume.empty()&&volume.front().life==0)volume.pop();
	return;
}

void Bloom_input(){
	if(Bloom_node[2].inp!="")route_length=number_transformer(Bloom_node[2].inp)[0];
	if(Bloom_node[3].inp!="")particular_amount=(int)number_transformer(Bloom_node[3].inp)[0];
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
	if(Bloom_node[6].inp!="")particular_lifetime=(int)number_transformer(Bloom_node[6].inp)[0];
	if(Bloom_node[7].inp!="")add_white=Bloom_node[7].inp[0]; 
	if(Bloom_node[8].inp!=""){
		vector<double> v=number_transformer(Bloom_node[8].inp);
		total_size=v.size();
		for(int i=1;i<=total_size;i++){
			particular_size[i]=(int)v[i-1]-1;
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

 

