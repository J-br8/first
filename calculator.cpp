#include<iostream>
#include<bits/stdc++.h>
#include<math.h> 
#include <sstream>
#include <cstdlib>
#include<windows.h>
#define EPS 1e-9
using namespace std;
//全局变量声明 —————————————————————— 
string c;

//栈处理器(多功能栈）———————————————————
typedef union {
	double num;
	char op;
}Elem;
typedef struct Node{//节点声明 
	Elem data;
	struct Node* next;
}Node;
typedef struct{//标记栈顶 
	Node* top;
}TopStack;
bool IsEmpty(TopStack *s){//测试非空 
	return s->top==NULL;
}
bool Push(TopStack *s,Elem val){//进栈 
	Node *newNode =(Node*)malloc(sizeof(Node));
	if(!newNode) return false;
	newNode->data=val;
	newNode->next=s->top;
	s->top=newNode;
	return true;
}  
bool Pop(TopStack *s,Elem *val){//出栈 
	if(IsEmpty(s)) return false;
	Node *p=s->top;
	*val=p->data;
	s->top=s->top->next;
	free(p);
	return true;
} 
bool GetTop(TopStack *s,Elem *val){//取栈顶 
	if(IsEmpty(s)) return false;
	*val=s->top->data;
	return true;
}
double InterPreter(string c);
string MyFfileR(string s,vector<string> v);
//二元优先级判断器—————————————————————————— 
int GetPriority(char s){//一元运算符优先级最高 
	if(s=='u'||s=='v') return 5;
	if(s=='^') return 4;
	if(s=='*'||s=='/'||s=='%') return 3;
	if(s=='+'||s=='-') return 2;
	if(s=='(') return 1;
	return 0;
} 
//取参函数——————————————————————————————
string Get(string s,int *idx){
	int cnt=1;
	int pos=0;
	string record;
	(*idx)++;
	while(cnt>0&&(*idx)<s.size()){
		if(s[*idx]=='(') cnt++;
		if(s[*idx]==')') cnt--;
		if(cnt>0) record+=s[*idx];
		(*idx)++;
	}
	return record;
}
//函数处理器（函数直接计算成值再入数字栈）——————————————————————————————
string trimZero(string s)
{
    while (s.back() == '0') s.pop_back();
    if (s.back() == '.') s.pop_back();
    return s;
}
double Func(string s,string record){
	int test=0;
	if(s=="sin"||s=="cos"||s=="tan"||s=="sqrt"||
	s=="abs"||s=="log"||s=="log10") test=1;
	if(record.find(",",0)==string::npos&&test){
	
	double h=InterPreter(record);
	if(s=="sin") return sin(h);
    if(s=="cos") return cos(h);
    if(s=="tan") return tan(h);
    if(s=="sqrt") return sqrt(h);
    if(s=="abs") return abs(h);
    if(s=="log") return log(h);
    if(s=="log10") return log10(h);
}else{ //自定义函数部分 
	int cnt=0;//括号计数器，排除括号里的逗号 
	string u;
	vector<string> v;
	for(int i=0;i<record.size();i++){
		if(record[i]=='(') cnt++;
		if(record[i]==')') cnt--; 
		if(record[i]==','&&cnt<=0){
			 
			double h=InterPreter(u);
			string o=to_string(h);o=trimZero(o); 
			v.push_back(o);
			u="";continue;
		}
		u+=record[i];
	}
	
	double h=InterPreter(u);
			string o=to_string(h);o=trimZero(o); 
			v.push_back(o);
	string z=MyFfileR(s,v);
	if(z=="EOF") return 0;
	 
	return InterPreter(z);
	
	
	
}   
} 
//判断浮点数非零
int isZero(double num) {
    return fabs(num) < EPS;
} 
//判断为整数
int isInteger(double num) {
    return fabs(num - floor(num + 0.5)) < EPS;
}
//操作器(处理数字的合并)
void f(TopStack* s,char c){
	if(IsEmpty(s)) return;
	if(c=='u'||c=='v'){
		Elem val;
		Pop(s,&val);
		if(c=='u') Push(s,val);//一元+ 
		if(c=='v'){//一元- 
			val.num=-val.num;
			Push(s,val);
		}
	}else{
		Elem val1,val2,val3;
		Pop(s,&val1);
		if(IsEmpty(s)) return;//防止不够两个数 
		Pop(s,&val2);
		if(c=='+'){val3.num=val1.num+val2.num;Push(s,val3);}
		if(c=='-'){val3.num=val2.num-val1.num;Push(s,val3);}
		if(c=='*'){val3.num=val1.num*val2.num;Push(s,val3);}
		if(c=='/'){if(isZero(val1.num)) return;val3.num=val2.num/val1.num;Push(s,val3);}
		if(c=='^'){val3.num=pow(val2.num,val1.num);Push(s,val3);}
		if(c=='%'){val3.num=fmodf(val2.num,val1.num);Push(s,val3);}
		
		
	}
} 
//运算器 (符号栈非空时)(只处理符号运算)(+ - * / ^ % () ) 
void Option(TopStack* s,TopStack* ch,char k){
	if(k=='('){
		Elem val;
		val.op=k;
		Push(ch,val);
		return;
	}
	if(k==')'){
		Elem val;
		GetTop(ch,&val);
		while(val.op!='('){
			Pop(ch,&val);
			f(s,val.op);
			GetTop(ch,&val);
		}
		Pop(ch,&val);//丢掉'(' 
		return; 
	}
	
while(!IsEmpty(ch)){
	Elem val;
    GetTop(ch,&val);
    char a=val.op;
    int m=GetPriority(k),n=GetPriority(a);
    if(m<=n){
        Pop(ch,&val);
        f(s,val.op);
    }else{
        break;
    }
}
	Elem val;
	val.op=k;
	Push(ch,val);
} 
int isRight(char s){
	if(s=='+'||s=='-'||s=='*'||s=='/'||
		s=='^'||s=='%'||s=='u'||s=='v'||
		s=='('||s==')') return 1;
		return 0;
}
//判断是否为一元操作符
int isUnary(string s,int idx){
	if(idx==0) return 1;
	char prev=s[idx-1];
	if(prev=='(') return 1;
	if (prev == '+' || prev == '-' ||
        prev == '*' || prev == '/' ||
        prev == '%' || prev == '^')
        return 1;
    if ((prev >= 'a' && prev <= 'z'))
        return 1;
    return 0;
} 
//解释器(给表达式得到运算结果，需要调用运算器）—————————————————————————————— 
double InterPreter(string c){
	
	c.erase(remove(c.begin(), c.end(), ' '), c.end());//去空格 
	TopStack *s=(TopStack*)malloc(sizeof(TopStack));
	TopStack *ch=(TopStack*)malloc(sizeof(TopStack));
	s->top=NULL;ch->top=NULL;
	for(int i=0;i<c.size();i++){
		if((c[i]>='0'&&c[i]<='9')||(c[i]=='.')){//处理数字栈 
			string u;
			while((c[i]>='0'&&c[i]<='9')||(c[i]=='.')){
				u+=c[i];i++;
			}
			double n=stod(u);
			Elem val;
			val.num=n;
			Push(s,val);
		}
		if(c[i]>='a'&&c[i]<='z'){//处理函数 (结果入数字栈） 
			string k;
			while(c[i]!='('){
				k+=c[i];
				i++;
			}
			string record=Get(c,&i);
			
			Elem val;
			val.num=Func(k,record);
			
			Push(s,val);
			}
		if(c[i]=='+'||c[i]=='-'){
			if(isUnary(c,i)){
				if(c[i]=='+') c[i]='u';
				else c[i]='v';
			}
		}
		//处理一元运算符 
		if(isRight(c[i]))
		 	Option(s,ch,c[i]);
		 
		}
		
		 
		while(!IsEmpty(ch)){
			Elem val;
			Pop(ch,&val);
			f(s,val.op);
		}
		Elem val;
		
		GetTop(s,&val);
		
		return val.num;
	}
//内存管理器————————————————————
int MyHfile(string s,double val){//历史记录管理 
	
		ofstream file("history.txt",ios::app);
		if(!file.is_open()){
			cout<<"历史记录无法打开"<<endl;
			return 1; 
		}
		file<<s<<"="<<val<<endl;
		file.close();
		return 0;
	
	
	
} 
void MyFfileW(string c,char k){//自定义函数写入 
	c.erase(remove(c.begin(), c.end(), ' '), c.end());
	
	if(k=='w'){//写入 
	string t;int q=0;
	while(c[q]!='(') {
		t+=c[q];q++;
	}
		MyFfileW(t,'c');
		int i=0;
		vector<string> a; 
		string fname,option,xname;
			while(c[i]!='('){
				fname+=c[i];
				i++;
			}
			
			i++;
			while(c[i]!=')'){
				if(c[i]==','){
					a.push_back(xname);
					xname="";
					i++;
				}
				xname+=c[i];
				i++;
			}
			
			a.push_back(xname);
			i++;
			if(c[i]!='=') return;
			
			i++;
			while(i<c.size()){
				option+=c[i];
				i++; 
			}
			
			ofstream file;
			file.open("option.txt",ios::app);
			if(!file.is_open()){
				return;
			}
			file<<fname<<"|";
			for(int i=0;i<a.size();i++){
				file<<a[i]<<",";
			}
			file<<"|"<<option<<endl;
			file.close();
	}
	if(k=='c'){//删除特定函数
		 ifstream inFile("option.txt");
		 ofstream tempFile("temp.txt");
		 string line;
		 while (getline(inFile,line)){
		 	stringstream ss(line);
		 	string part;
		 	getline(ss,part,'|');
		 	if(part!=c){
		 		tempFile<<line<<endl;
			 }
		 }
		 inFile.close();
		 tempFile.close();
		 remove("option.txt");
		 rename("temp.txt","option.txt");
	}
	
}
string MyFfileR(string s,vector<string> v){//读取并替换变量，返回可计算字符串 
	ifstream file("option.txt");
	string line,option;
	string no="EOF";
	vector<string> g;
	while(getline(file,line)){
		stringstream ss(line);
		string part;
		
		getline(ss,part,'|');
		if(part!=s) continue;
		while(getline(ss,part,',')){
			if(part[0]=='|'){
				part.erase(0,1);
				option=part;
			}
			g.push_back(part);
		}
		g.pop_back();
		break;
	}
	file.close();
	
	if(option=="") return no;
	if(g.size()!=v.size()) return no;
	for(int i=0;i<g.size();i++){
		size_t pos=0;
		while((pos=option.find(g[i],pos))!=string::npos){
			option.replace(pos,g[i].size(),v[i]);
			pos+=v[i].length();
		}
	}
	return option;
}
void Gotoxy(int x, int y)
{
    COORD pos;       
    pos.X = x;      
    pos.Y = y;       
    SetConsoleCursorPosition(       
        GetStdHandle(STD_OUTPUT_HANDLE),  
        pos                           
    );
}
void GetConsoleWindowSize(int &cols, int &lines)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;

    // 拿到控制台信息
    GetConsoleScreenBufferInfo(hOut, &info);

    // 计算窗口可见区域 列、行
    cols  = info.srWindow.Right - info.srWindow.Left + 1;
    lines = info.srWindow.Bottom - info.srWindow.Top + 1;
}
void printmain(int w){

    system("cls");
    Gotoxy(w/3,5);
   	cout<<"计算器Pro"; 
    Gotoxy(w/3,7);
    cout<<"功能列表";
	 Gotoxy(w/3,9);
    cout<<"1.计算";
    Gotoxy(w/3,11);
    cout<<"2.函数自定义";
    Gotoxy(w/3,13);
    cout<<"3.历史记录查询";
    Gotoxy(w/3,15);
    cout<<"选择:";

}
void print1(int w){
	while(1){
	
	system("cls");
	Gotoxy(w/5,9);
	cout<<"<返回菜单请输入@，调用函数请以字母接括号f(x)>"; 
	Gotoxy(w/4,11);
	cout<<"————————自定义函数库————————"; 
	ifstream file("option.txt");
	if(!file.is_open()){
		return;
	}
	string line;int i=12;
	while(getline(file,line)){
		Gotoxy(w/3,i++);
		cout<<line;
	}
	file.close();
	Gotoxy(w/3,5);
	cout<<"——<表达式计算>——";
	Gotoxy(w/5,7);
	cout<<"请输入表达式："; 
	string u;
	getline(cin,u);
	if(u=="@") return;
	system("cls");
	Gotoxy(w/4,7);
	double v=InterPreter(u);
	cout<<u<<"= "<<v;
	MyHfile(u,v);
	Gotoxy(w/4,9);
	cout<<"输入任意数继续......"<<endl;
	getline(cin,u);
}
}
void print3(int w){
	while(1){
	
	system("cls");
	Gotoxy(w/3,5);
	cout<<"——历史计算记录——";
	ifstream file("history.txt",ios::in);
		if(!file.is_open()){
			return ; 
		}
		string line;int i=7;
		while(getline(file,line)){
			Gotoxy(w/4,i++);
			cout<<line<<endl;
		}
		file.close();
	Gotoxy(w/4,i+1);
	cout<<"输入字符c清空记录，其它按键退出"<<endl;
	string q;
	getline(cin,q);
	if(q=="c"){
		ofstream file("history.txt", ios::trunc); 
    	file.close();
	}else{
		return;
	}
}
}

void print2(int w){
	while(1){
	
	system("cls");
	
	Gotoxy(w/4,5);
	cout<<"————————自定义函数库————————"; 
	ifstream file("option.txt");
	if(!file.is_open()){
		return;
	}
	string line;int i=6;
	while(getline(file,line)){
		Gotoxy(w/3,i++);
		cout<<line;
	}
	file.close();
	Gotoxy(w/4,i+1);cout<<"添加函数输入1，删除函数输入2，退出输入@"; 
	Gotoxy(w/4,i+3);cout<<"选择：";string p;getline(cin,p);
	if(p=="1"){
		system("cls");
		Gotoxy(w/4,9);cout<<"输入字母带括号的合法函数，如f(x)=x，退出输入@";
		Gotoxy(w/4,7);cout<<"请输入函数： ";string p;getline(cin,p);
		if(p=="@") return;
		MyFfileW(p,'w');
	}
	if(p=="2"){
		system("cls");
		Gotoxy(w/4,9);cout<<"输入函数名删除函数，如f，退出输入@";
		Gotoxy(w/4,7);cout<<"请输入函数名： ";string p;getline(cin,p);
		if(p=="@") return;
		MyFfileW(p,'c');
	}
	if(p=="@") return;
}
}

//输入输出处理
 
int main(){
	int w, h;
    GetConsoleWindowSize(w, h);
    while(1){
    	printmain(w);
    	string k;
    	getline(cin,k);
    	if(k=="1") print1(w);
    	if(k=="2") print2(w);
    	if(k=="3") print3(w);
    	if(k=="@") break;
	}
    
	return 0;
}
