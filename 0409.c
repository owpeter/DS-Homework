#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_CHILD 28
#define MAXPN 10 //提交时改大
//struct
typedef struct keepword {
    int isEnd; 
    struct keepword* next[MAX_CHILD];  
}keepword;
keepword *root = NULL;

typedef struct content
{//link
    char *line;
    struct content* next;
}content;

typedef struct stream{//link
    char *stream;
    struct stream* next;
}stream;

typedef struct program
{
    int id;
    content *head_c,*ptr_c,*last_c;
    char pFunction[50][51];
    int pFnum;
    stream* head_s,*ptr_s,*last_s;
}Program;


//fucntion
keepword *new_word();
void insert_word(keepword *t, char *str,int len);
void dfs(keepword *p);

void read_code();
void idf_pFunction(int i);
void gnrt_stream(int i);

//array
char t[1024];
int numOfPrograms = 0;
Program *programs[MAXPN],*temp_P;

int main(){
    //预处理keepword
    FILE *kw,*fc;
    kw = fopen("keepwords.txt","r");
    root = new_word();

    int cnt=1;
    while(fscanf(kw,"%s",t)!=EOF){
        if(t[0] == '\n') continue;
        int len = strlen(t);
        insert_word(root,t,len);
    }
    // dfs(root);
    fc = fopen("tc.txt","r");
    // programs = (Program*)malloc(sizeof(Program));
    int id;
    while (fgets(t,1024,fc)!=NULL)
    {
        if(t[0] == '\n') continue;
        if(isdigit(t[0])){
            sscanf(t,"%d",&id);
            temp_P = (Program*)malloc(sizeof(Program));
            programs[numOfPrograms++] = temp_P;
            temp_P->id = id;
            temp_P->head_c = NULL;
            temp_P->head_s = NULL;
            continue;
        }
        read_code();
    }
    //*** debug input ***
    // FILE *out;
    // out = fopen("temp_out.txt","w");
    // for(int i=0;i<numOfPrograms;i++){
    //     fprintf(out,"%d\n",programs[i]->id);
    //     programs[i]->ptr_c = programs[i]->head_c;
    //     while(programs[i]->ptr_c != NULL){
    //         fprintf(out,"%s",programs[i]->ptr_c->line);
    //         programs[i]->ptr_c = programs[i]->ptr_c->next;
    //     }
    // }
    for(int i=0;i<numOfPrograms;i++){
        idf_pFunction(i);
    }
    // FILE *out;
    // out = fopen("temp_out.txt","w");
    // for(int i=0;i<numOfPrograms;i++){
    //     fprintf(out,"id:%d\n",programs[i]->id);
    //     for(int j=0;j<programs[i]->pFnum;j++){
    //         fprintf(out,"%s\n",programs[i]->pFunction[j]);
    //     }
    // }

}

keepword *new_word(){
    keepword *t = (keepword*)malloc(sizeof(keepword));
    for(int i=0;i<MAX_CHILD;i++) t->next[i] = NULL;
    t->isEnd = 0;
    return t;
}

void insert_word(keepword *t, char *str,int len){
    for(int i=0;i<len;i++){
        if(t->next[str[i]-'_'] == NULL){
           keepword *p = new_word();
           t->next[str[i]-'_'] = p; 
        }
        t = t->next[str[i]-'_'];
    }
    t->isEnd = 1;
}

void dfs(keepword *p){
    //遍历输出
    if(p->isEnd == 1){
        putchar('\n');
        return;
    }
    for(int i=0;i<MAX_CHILD;i++){
        if(p->next[i] == NULL){
            continue;
        }
        else{
            printf("%c",i+'_');
            dfs(p->next[i]);
        }
    }
}

void read_code(){
    if(temp_P->head_c == NULL){
        temp_P->ptr_c = (content*)malloc(sizeof(content));
        temp_P->ptr_c->line = (char*)malloc(sizeof(char)*(strlen(t)+1));
        strcpy(temp_P->ptr_c->line, t);
        temp_P->head_c = temp_P->ptr_c;
        temp_P->last_c = temp_P->head_c;
        temp_P->last_c->next = NULL;
    }else{
        temp_P->ptr_c = (content*)malloc(sizeof(content));
        temp_P->ptr_c->line = (char*)malloc(sizeof(char)*(strlen(t)+1));
        strcpy(temp_P->ptr_c->line, t);
        temp_P->last_c->next = temp_P->ptr_c;
        temp_P->last_c = temp_P->ptr_c;
        temp_P->last_c->next = NULL;
    }
}

void idf_pFunction(int i){
    int Fnum = 0;

    temp_P = programs[i];
    temp_P->ptr_c = temp_P->head_c;
    memset(temp_P->pFunction,'\0',sizeof(temp_P->pFunction));
    while(temp_P->ptr_c != NULL){
        if(isalpha(temp_P->ptr_c->line[0])){
            int j = 0;
            while(isalpha(temp_P->ptr_c->line[j])){
                temp_P->pFunction[Fnum][j] = temp_P->ptr_c->line[j];
                j++;
            }
            // debug pFunction
            // int k=0;
            // while(temp_P->pFunction[Fnum][k]) printf("%c",temp_P->pFunction[Fnum][k++]);
            // putchar('\n');
            // //
            Fnum++;
        }
        temp_P->ptr_c = temp_P->ptr_c->next;
    }
    temp_P->pFnum = Fnum;
}

void gnrt_stream(int i){
    //改上面的idf函数，在获取函数名的同时产生流
    //如果是main函数，先跳过，如果不是，则记录函数名，同时产生该函数的流
    //最后处理主函数
}