#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <queue>
using namespace std;

vector<string> reserved_word;//������
vector<string> grammar; //���������ķ�
vector<string> code; //����Դ����
vector<char> character={'a','b','c','d','e','f',
                        'g','h','i','j','k','l',
                        'm','n','o','p','q','r',
                        's','t','u','v','w','x',
                        'y','z','+','-','*','/',
                        '.','!','#','&','(',')',
                        '_','=','[',']','{','}',
                        ';','<','>','0','1','2',
                        '3','4','5','6','7','8','9'}; //���������
//״̬ת��
struct state_change
{
    char state_now ;
    char state_next ;
    char input ;
};
//��������DFA 1
struct state_set_change
{
    set<char> state_now ;
    set<char> state_next ;
    char input ;
};
//��������DFA 2
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


//�����ķ���NFA



void set_resreved_word(){ //���ñ�����
    
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


void PrintVector(vector<string> str){ //��ӡ�ַ�������
    for(int i=0;i<str.size();i++)
        cout<<str[i]<<endl;
}

void getFileToVector(ifstream &fin ,vector<string> &str ){ //���ļ���ȡ����Ӧ������
    while (!fin.eof())
        {
            char temp_string[100];
            while(fin.getline(temp_string,100)){
                str.push_back(temp_string);
            }
        }
    
}

void GetNfa(vector<string> str,NFA &n){//�����ķ�->NFA
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


void printNfa(NFA nfa){//��ӡ  ���NFA 
    cout<<"NFA�ĳ�ʼ״̬Ϊ: "<<nfa.start_state<<endl;
    cout<<"NFA�Ľ���״̬Ϊ: ";
    for(int i= 0 ; i< nfa.end_state.size();i++){
        cout<<nfa.end_state[i]<<" ";
    }
    cout<<endl<<"NFA�м�״̬��ϵΪ: "<<endl<<"��ǰ״̬   ����    ��һ״̬"<<endl;
    for(int i= 0 ; i< nfa.m.size();i++){
        
        cout<<"   "<<nfa.m[i].state_now<<"        "<<nfa.m[i].input<<"        "<<nfa.m[i].state_next<<endl;
    }
}


void GetDfa(NFA n,DFA &d){//NFA->DFA
    queue<set<char>> q;  //
    vector<set<char>> v; //DFA�Ľڵ�
    vector<change_name> new_name; //DFA�Ľڵ����  ˳���໥��Ӧ
    vector<state_set_change> s; //����֮��Ĺ�ϵ
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
            //����֮��Ĺ�ϵ
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
    }//��ӡDFA�Ľڵ���
    
    //for(int i=0;i<v.size();i++){    for(set<char>::iterator it=v[i].begin();it!=v[i].end();it++){cout << *it << endl;    //ͨ��������������� }}
    //����DFA  
    //�ڵ�����
    
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
    //��ӡ ���
    //cout<<new_name.size();
    //for(int i =0 ;i<new_name.size();i++){ cout<<new_name[i].name<<endl;}
    //for(int i=0;i<new_name.size();i++){for(set<char>::iterator it=new_name[i].old_name.begin();it!=new_name[i].old_name.end();it++){cout << *it << endl; }}   
    //for(int i=0;i<s.size();i++){    for(set<char>::iterator it=s[i].state_now.begin();it!=s[i].state_now.end();it++){cout << *it << endl;    }    }
    

    // DFA ��̬�ڵ�
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
    
    // DFA ״̬��ϵ
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

void printDfa(DFA d){//��ӡDFA
    cout<<"DFA�ĳ�ʼ״̬Ϊ: "<<d.start_state<<endl;
    cout<<"DFA�Ľ���״̬Ϊ: ";
    for(int i= 0;i<d.end_state.size();i++){
        cout<<d.end_state[i];
    }
    cout << endl << "DFA�м�״̬��ϵΪ: " << endl << "��ǰ״̬   ����    ��һ״̬" << endl;
    for(int i= 0 ; i< d.m.size();i++){
        
        cout<<"   "<<d.m[i].state_now<<"        "<<d.m[i].input<<"        "<<d.m[i].state_next<<endl;
    }
}



void getToken(DFA d,vector<string> v,vector<TOKEN> &t){//��DFA��ȡTOKEN
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
                if(s.state_now==d.m[j].state_now&&s.input==d.m[j].input){//�õ���һ״̬
                    s.state_next=d.m[j].state_next;
                    mark1=1;
                }
            }
            if(!mark1){
             //   cout<<"error"<<endl;
            }
            for(int z=0;z<d.end_state.size();z++){ //�ж��Ƿ�Ϊ��̬
                if(s.state_next==d.end_state[z]){
                    mark=1;
                }
            }
            if(mark){//����̬ //t.push_back(temp_t);
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
                        if(s_next.state_now==d.m[j].state_now&&s_next.input==d.m[j].input){//���Ƿ��ܵõ���һ���ַ���һ״̬
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
            else{//������̬
                p++;
                s_next.input=strline[p];
                s_next.state_now=s.state_next;
                if(strline[p]=='\0'){
                    cout<<"error"<<endl;
                }
                else{
                    int mark2=0;
                    for(int j=0;j<d.m.size();j++){
                        if(s_next.state_now==d.m[j].state_now&&s_next.input==d.m[j].input){//���Ƿ��ܵõ���һ���ַ���һ״̬
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
        //A ����ؼ��ֺͱ�ʶ�� 
        //B��������� 
        //C���� ��� 
        //D�������� F����С�� G������ H��I�����ѧ���� ͳ��Ϊ����
        
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
void printToken(vector<TOKEN> t){//��ӡToken
    cout<<"Token ������-----------------------------------------------"<<endl;
    cout<<endl<<"��ǰ����   ����           Token"<<endl;
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
    //�ķ��ĵ�
    ifstream fin_grammar("grammar.txt");

    if(!fin_grammar){
        cout<<"�ķ��ļ���ʧ��!"<<endl;
    }
    else{
        cout<<"�ķ��ļ��򿪳ɹ�!"<<endl;
        getFileToVector(fin_grammar,grammar); //��ȡ�ķ��ļ�
    }
    
    cout<<"---------------------������ķ��ĵ�����---------------------"<<endl;
    PrintVector(grammar);
    cout<<"-----------------------------------------------------------"<<endl;
    //cout<<grammar.size();
    
    
    //�������ķ���ȡNFA
    GetNfa(grammar,nfa);
    

    //��ӡ  ���NFA out<<endl;
    //��NFA��ȡDFA
    //cout<<nfa.m.size();
    //printNfa(nfa);c
    GetDfa(nfa,dfa);
    
    //��ӡ ���DFA��̬
    
    //for(int i= 0;i<dfa.end_state.size();i++)cout<<dfa.end_state[i]<<endl;
    //��ӡ  ���DFA 
    //printDfa(dfa);cout<<endl;
        

    fin_grammar.close();
    //�����ĵ�
    ifstream fin_code("code.txt");

    if(!fin_code){
        cout<<"�����ļ���ʧ��!"<<endl;
    }
    else{
        cout<<"�����ļ��򿪳ɹ�!"<<endl;
        getFileToVector(fin_code,code);
    }

    //��ӡ
    
    

    
    cout<<"---------------------����Ĵ����ĵ�����---------------------"<<endl;
    PrintVector(code);
    cout<<"-----------------------------------------------------------"<<endl;
    
    //���Token
    getToken(dfa,code,token);
    //��ӡ
    printToken(token);
    fin_code.close();
    //���token.txt�ļ�
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