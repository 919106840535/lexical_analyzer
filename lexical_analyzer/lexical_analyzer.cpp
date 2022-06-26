#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <queue>
using namespace std;

vector<string> reserved_word;//保留字
vector<string> grammar; //保存正规文法
vector<string> code; //保存源代码
vector<char> character={'a','b','c','d','e','f',
                        'g','h','i','j','k','l',
                        'm','n','o','p','q','r',
                        's','t','u','v','w','x',
                        'y','z','+','-','*','/',
                        '.','!','#','&','(',')',
                        '_','=','[',']','{','}',
                        ';','<','>','0','1','2',
                        '3','4','5','6','7','8','9'}; //可输入符号
//状态转换
struct state_change
{
    char state_now ;
    char state_next ;
    char input ;
};
//用于整理DFA 1
struct state_set_change
{
    set<char> state_now ;
    set<char> state_next ;
    char input ;
};
//用于整理DFA 2
struct change_name
{
    char name;
    set<char> old_name;
};
//DFA
struct DFA
{
    char start_state;
    vector<char> end_state ;
    vector<state_change> m;

};
//NFA
struct NFA
{
    char start_state ;
    vector<char> end_state ;
    vector<state_change> m;
};

//Token
struct TOKEN
{
    int line;
    string type;
    string value;
};


//正规文法到NFA



void set_resreved_word(){ //设置保留字
    
    reserved_word.push_back("if");
    reserved_word.push_back("else");
    reserved_word.push_back("for");
    reserved_word.push_back("while");
    reserved_word.push_back("include");
    reserved_word.push_back("using");
    reserved_word.push_back("namespace");
    reserved_word.push_back("int");
    reserved_word.push_back("double");
    reserved_word.push_back("float");
    reserved_word.push_back("char");
    reserved_word.push_back("true");
    reserved_word.push_back("false");
    reserved_word.push_back("void");
    reserved_word.push_back("return");
    reserved_word.push_back("plural");
    //......
}


void PrintVector(vector<string> str){ //打印字符串容器
    for(int i=0;i<str.size();i++)
        cout<<str[i]<<endl;
}

void getFileToVector(ifstream &fin ,vector<string> &str ){ //将文件读取到相应的容器
    while (!fin.eof())
        {
            char temp_string[100];
            while(fin.getline(temp_string,100)){
                str.push_back(temp_string);
            }
        }
    
}

void GetNfa(vector<string> str,NFA &n){//正规文法->NFA
    state_change temp_state_change ;
    int i=0;
    while(i<str.size()){        
        
        if(str [i][3]=='$' ){
            n.end_state.push_back(str[i][0]);

        }
        else{
            temp_state_change.state_now=str[i][0];
            temp_state_change.input=str[i][3];
            temp_state_change.state_next=str[i][4];
            n.m.push_back(temp_state_change);
        }
        i++;
    }
}


void printNfa(NFA nfa){//打印  检查NFA 
    cout<<"NFA的初始状态为: "<<nfa.start_state<<endl;
    cout<<"NFA的结束状态为: ";
    for(int i= 0 ; i< nfa.end_state.size();i++){
        cout<<nfa.end_state[i]<<" ";
    }
    cout<<endl<<"NFA中间状态关系为: "<<endl<<"当前状态   输入    下一状态"<<endl;
    for(int i= 0 ; i< nfa.m.size();i++){
        
        cout<<"   "<<nfa.m[i].state_now<<"        "<<nfa.m[i].input<<"        "<<nfa.m[i].state_next<<endl;
    }
}


void GetDfa(NFA n,DFA &d){//NFA->DFA
    queue<set<char>> q;  //
    vector<set<char>> v; //DFA的节点
    vector<change_name> new_name; //DFA的节点改名  顺序相互对应
    vector<state_set_change> s; //集合之间的关系
    set<char> temp_set;
    set<char> I;
    temp_set.insert(n.start_state);
    q.push(temp_set);
    v.push_back(temp_set);
    while(!q.empty()){
        for(int i=0;i<character.size();i++){
            state_set_change temp_s;
            int mark1=0;
            for(int j=0;j<n.m.size();j++){
                if(q.front().count(n.m[j].state_now)){
                    if(n.m[j].input==character[i]){
                        I.insert(n.m[j].state_next);
                        mark1=1;
                    }
                }
            }
            //集合之间的关系
            temp_s.input=character[i];
            temp_s.state_now=q.front();
            temp_s.state_next=I;
            if(mark1){
                s.push_back(temp_s);
            }
            

            int mark=1;
            for(int w=0;w<v.size();w++){
                if(I==v[w]){
                    mark=0;
                }
            }
            if(mark){
                q.push(I);
                v.push_back(I);
            }
            I.clear();
        }
        q.pop();
    }//打印DFA的节点检查
    
    //for(int i=0;i<v.size();i++){    for(set<char>::iterator it=v[i].begin();it!=v[i].end();it++){cout << *it << endl;    //通过迭代器输出对象 }}
    //整理DFA  
    //节点命名
    
    vector<set<char>> temp_ch;
    for(int i = 0; i <v.size();i++){
        change_name temp_name;
        if(v[i].count('S')){
            temp_name.name='S';
            temp_name.old_name=v[i];
            temp_ch.push_back(temp_name.old_name);
        }

        else{
            temp_name.name=64+new_name.size();
            temp_name.old_name=v[i];
            temp_ch.push_back(temp_name.old_name);
        }
        if(!temp_name.old_name.empty())
            new_name.push_back(temp_name);
        
    }
    //打印 检查
    //cout<<new_name.size();
    //for(int i =0 ;i<new_name.size();i++){ cout<<new_name[i].name<<endl;}
    //for(int i=0;i<new_name.size();i++){for(set<char>::iterator it=new_name[i].old_name.begin();it!=new_name[i].old_name.end();it++){cout << *it << endl; }}   
    //for(int i=0;i<s.size();i++){    for(set<char>::iterator it=s[i].state_now.begin();it!=s[i].state_now.end();it++){cout << *it << endl;    }    }
    

    // DFA 终态节点
    for(int i = 0 ;i<new_name.size();i++){
        for(int j = 0;j<n.end_state.size();j++){
            if(new_name[i].old_name.count(n.end_state[j])){
                int mark=1;
                for(int z;z<d.end_state.size();z++){
                    if(d.end_state[z]==new_name[i].name){
                        mark=0;
                    }
                }
                if(mark){
                    d.end_state.push_back(new_name[i].name);
                }
            }
        }
    }
    
    // DFA 状态关系
    for(int i=0;i<s.size();i++){
        state_change temp_s;
        temp_s.input=s[i].input;
        for(int j=0;j<new_name.size();j++){
            if(s[i].state_now==new_name[j].old_name){
                temp_s.state_now=new_name[j].name;
                
            }
            if(s[i].state_next==new_name[j].old_name){
                temp_s.state_next=new_name[j].name;
            }
        }
        d.m.push_back(temp_s);
    }
    //end
}

void printDfa(DFA d){//打印DFA
    cout<<"DFA的初始状态为: "<<d.start_state<<endl;
    cout<<"DFA的结束状态为: ";
    for(int i= 0;i<d.end_state.size();i++){
        cout<<d.end_state[i];
    }
    cout << endl << "DFA中间状态关系为: " << endl << "当前状态   输入    下一状态" << endl;
    for(int i= 0 ; i< d.m.size();i++){
        
        cout<<"   "<<d.m[i].state_now<<"        "<<d.m[i].input<<"        "<<d.m[i].state_next<<endl;
    }
}



void getToken(DFA d,vector<string> v,vector<TOKEN> &t){//由DFA获取TOKEN
    int line=1;
    TOKEN temp_t;
    state_change s;
    state_change s_next;
    for(int i =0;i<v.size();i++){
        string strline;
        strline=v[i];
        int p=0;
        s.state_now='S';
        string temp_str="";
        while(strline[p]!='\0'){
            while(strline[p]==' '){
                p++;
            }
            temp_str=temp_str+strline[p];
            s.input=strline[p];
            int mark=0;
            int mark1=0;
            for(int j=0;j<d.m.size();j++){
                if(s.state_now==d.m[j].state_now&&s.input==d.m[j].input){//得到下一状态
                    s.state_next=d.m[j].state_next;
                    mark1=1;
                }
            }
            if(!mark1){
             //   cout<<"error"<<endl;
            }
            for(int z=0;z<d.end_state.size();z++){ //判断是否为终态
                if(s.state_next==d.end_state[z]){
                    mark=1;
                }
            }
            if(mark){//是终态 //t.push_back(temp_t);
                temp_t.line=line;
                temp_t.type=s.state_next;
                temp_t.value=temp_str;
                p++;
                s_next.input=strline[p];
                s_next.state_now=s.state_next;
                if(strline[p]==' '||strline[p]=='\0'){
                    s.state_now='S';
                    t.push_back(temp_t);
                    temp_str="";
                }
                else{
                    int mark2=0;
                    int mark3=0;
                    for(int j=0;j<d.m.size();j++){
                        if(s_next.state_now==d.m[j].state_now&&s_next.input==d.m[j].input){//看是否能得到下一个字符下一状态
                            s_next.state_next=d.m[j].state_next;
                            mark2=1;
                        }
                    }
                    if(!mark2){
                        s.state_now='S';
                        t.push_back(temp_t);
                        temp_str="";
                    }
                    else{
                        s.state_now=s.state_next;
                    }
                }
            }
            else{//不是终态
                p++;
                s_next.input=strline[p];
                s_next.state_now=s.state_next;
                if(strline[p]=='\0'){
                    cout<<"error"<<endl;
                }
                else{
                    int mark2=0;
                    for(int j=0;j<d.m.size();j++){
                        if(s_next.state_now==d.m[j].state_now&&s_next.input==d.m[j].input){//看是否能得到下一个字符下一状态
                            s_next.state_next=d.m[j].state_next;
                            mark2=1;
                        }
                    }
                    if(!mark2){
                        cout<<"error"<<endl;
                    }
                    else{
                        s.state_now=s.state_next;
                    }
                    
                }
            }
        }
        line++;
    }
    for(int i=0;i<t.size();i++){
        //A 代表关键字和标识符 
        //B代表运算符 
        //C代表 界符 
        //D代表整数 F代表小数 G代表复数 H、I代表科学计数 统称为常数
        
        if(t[i].type=="A"){
            int mark=0;
            for(int j=0;j<reserved_word.size();j++)
            {
                if (t[i].value==reserved_word[j])
                {
                    mark=1;
                    break;
                }
            }
            if(mark){ 
                t[i].type="keyword";
            }
            if(!mark){
                t[i].type="identifier";
            }
        }
        if(t[i].type=="B"){
            t[i].type="operator";
        }
        if(t[i].type=="C"){
            t[i].type="limiter";
        }
        
        if(t[i].type=="D"||t[i].type=="F"||t[i].type=="G"||t[i].type=="H"||t[i].type=="I"){
            t[i].type="const";
        }
        

    }
}
void printToken(vector<TOKEN> t){//打印Token
    cout<<"Token 表如下-----------------------------------------------"<<endl;
    cout<<endl<<"当前行数   类型           Token"<<endl;
    for(int i= 0 ; i< t.size();i++){
        if(t[i].line<10)
            cout<<"    "<<t[i].line;
        else
            cout<<"   "<<t[i].line;
        if(t[i].type=="keyword"||t[i].type=="limiter"){
            cout<<"       "<<t[i].type<<"        "<<t[i].value<<endl;
        }
        if(t[i].type=="identifier"){
            cout<<"       "<<t[i].type<<"     "<<t[i].value<<endl;
        }
        if(t[i].type=="operator"){
            cout<<"       "<<t[i].type<<"       "<<t[i].value<<endl;
        }
        if(t[i].type=="const"){
            cout<<"       "<<t[i].type<<"          "<<t[i].value<<endl;
        }
        
    }
    cout<<"-----------------------------------------------------------"<<endl;
}





int main(){
    NFA nfa;
    DFA dfa;
    vector<TOKEN> token;
    nfa.start_state='S';
    dfa.start_state='S';
    set_resreved_word();
    //文法文档
    ifstream fin_grammar("grammar.txt");

    if(!fin_grammar){
        cout<<"文法文件打开失败!"<<endl;
    }
    else{
        cout<<"文法文件打开成功!"<<endl;
        getFileToVector(fin_grammar,grammar); //获取文法文件
    }
    
    cout<<"---------------------输入的文法文档如下---------------------"<<endl;
    PrintVector(grammar);
    cout<<"-----------------------------------------------------------"<<endl;
    //cout<<grammar.size();
    
    
    //由正规文法获取NFA
    GetNfa(grammar,nfa);
    

    //打印  检查NFA out<<endl;
    //由NFA获取DFA
    //cout<<nfa.m.size();
    //printNfa(nfa);c
    GetDfa(nfa,dfa);
    
    //打印 检查DFA终态
    
    //for(int i= 0;i<dfa.end_state.size();i++)cout<<dfa.end_state[i]<<endl;
    //打印  检查DFA 
    //printDfa(dfa);cout<<endl;
        

    fin_grammar.close();
    //代码文档
    ifstream fin_code("code.txt");

    if(!fin_code){
        cout<<"代码文件打开失败!"<<endl;
    }
    else{
        cout<<"代码文件打开成功!"<<endl;
        getFileToVector(fin_code,code);
    }

    //打印
    
    

    
    cout<<"---------------------输入的代码文档如下---------------------"<<endl;
    PrintVector(code);
    cout<<"-----------------------------------------------------------"<<endl;
    
    //获得Token
    getToken(dfa,code,token);
    //打印
    printToken(token);
    fin_code.close();
    //输出token.txt文件
    fstream f_out;
    f_out.open("./token.txt",fstream::out);
    for(int i=0;i<token.size();i++){
        string temp_str =to_string(token[i].line)+" "+token[i].type+" "+token[i].value+'\n';
        f_out<<temp_str;
    }
    f_out.close();
    system("pause");
    return 0;
}