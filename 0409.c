#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_CHILD 37
#define MAXPN 10 //提交时改大
//struct

char asc[] = {'0','1','2','3','4','5','6','7','8','9','_',
                'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
int hash[128];

typedef struct keepword {
    int isEnd; 
    struct keepword* next[MAX_CHILD];  
}keepword;


typedef struct content
{//link
    char *line;
    struct content* next;
}content;

typedef struct stream{//link
    char *stream;
    struct stream* next;
}stream;

typedef struct function{
    char name[50];
    content* pos;
    stream fstream;
}function;

typedef struct program
{
    int id;
    content *head_c,*ptr_c,*last_c;
    function pFunction[50];
    int pFnum;
    stream* head_s,*ptr_s,*last_s;
}Program;


//fucntion
keepword *new_word();
void insert_word(keepword *t, char *str,int len);
void dfs(keepword *p);

void read_code();
// void idf_pFunction(int i);
char* gnrt_stream(content*);

//element
keepword *root = NULL;
char t[1024];
int numOfPrograms = 0;
Program *programs[MAXPN],*temp_P;

int main(){
    //预处理keepword
    FILE *kw,*fc;
    kw = fopen("keepwords.txt","r");
    memset(hash,-1,sizeof(hash));
    for(int i=0;i<MAX_CHILD;i++){
        hash[asc[i]] = i;
    }
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
            temp_P->pFnum = 0;
            memset(temp_P->pFunction,'\0',sizeof(temp_P->pFunction));//?????
            continue;
        }
        read_code();
    }
    // *** debug input ***
    // FILE *out;
    // out = fopen("temp_out.txt","w");
    // for(int i=0;i<numOfPrograms;i++){
    //     fprintf(out,"%d\n",programs[i]->id);
    //     programs[i]->ptr_c = programs[i]->head_c;
    //     while(programs[i]->ptr_c != NULL){
    //         // fprintf(out,"%s",programs[i]->ptr_c->line);
    //         int k=0;
    //         while(programs[i]->ptr_c->line[k]){
    //             fprintf(out,"%d ",programs[i]->ptr_c->line[k++]);
    //         }
    //         fprintf(out,"\n");
    //         programs[i]->ptr_c = programs[i]->ptr_c->next;
    //     }
    // }
    //
    for(int i=0;i<numOfPrograms;i++){
        // printf("%d\n",programs[i]->id);
        // idf_pFunction(i);
        temp_P = programs[i];
        temp_P->ptr_c = temp_P->head_c;
        while(temp_P->ptr_c){
            if(isalpha(temp_P->ptr_c->line[0])){
                int j=0;
                char tn[51];
                while(isalpha(temp_P->ptr_c->line[j])){
                    tn[j] = temp_P->ptr_c->line[j];
                    j++;
                }
                tn[j] = '\0';
                //
                puts(tn);
                //
                for(int i=0;i<temp_P->pFnum;i++){
                    if(strcmp(tn,temp_P->pFunction[i].name)){
                        //生成流
                        temp_P->pFunction[i].fstream.stream = gnrt_stream(temp_P->pFunction[i].pos);
                        printf("%s\n",temp_P->pFunction[i].fstream.stream);
                    }
                }
            }
        }
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
    int i=0;
    for(int i=0;i<MAX_CHILD;i++) t->next[i] = NULL;
    t->isEnd = 0;
    return t;
}

void insert_word(keepword *t, char *str,int len){
    for(int i=0;i<len;i++){
        if(t->next[hash[str[i]]] == NULL){
           keepword *p = new_word();
           t->next[hash[str[i]]] = p;
           for(int i=0;i<=9;i++) t->next[hash[asc[i]]] = p;
        }
        t = t->next[hash[str[i]]];
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
    if(isalpha(t[0])){
        int j=0;
        while(isalpha(t[j])){
            //录入函数名字
            temp_P->pFunction[temp_P->pFnum].name[j] = t[j];
            j++;
        }
        temp_P->pFunction[temp_P->pFnum].pos = temp_P->ptr_c->next;
        temp_P->pFnum++;
    }
}

// void idf_pFunction(int i){
//     // int Fnum = 0;

//     temp_P = programs[i];
//     temp_P->ptr_c = temp_P->head_c;
//     // memset(temp_P->pFunction,'\0',sizeof(temp_P->pFunction));
//     content* pMain = NULL;
//     while(temp_P->ptr_c != NULL){
//         if(isalpha(temp_P->ptr_c->line[0])){
//             // int j = 0;
//             // while(isalpha(temp_P->ptr_c->line[j])){
//             //     //录入函数名
//             //     temp_P->pFunction[Fnum].name[j] = temp_P->ptr_c->line[j];
//             //     j++;
//             // }
//             if(strcmp(temp_P->pFunction[Fnum].name,"main") != 0){
//                 //生成函数流
//                 temp_P->pFunction[Fnum].fstream.stream = gnrt_stream(temp_P->ptr_c->next->next);
//             }
//             else pMain = temp_P->ptr_c;
//             //
//             printf("%s\n",temp_P->pFunction[Fnum].fstream.stream);
//             //
//             Fnum++;
//         }
//         temp_P->ptr_c = temp_P->ptr_c->next;
//     }
//     temp_P->pFnum = Fnum;
//     //最后再处理main
//     // gnrt_main_stream(pMain);
// }

char* gnrt_stream(content* ptr){
    // ptr = ptr->next;
    int sumBig = 1, numOfsc = 1;//{
    char c;
    char* temp_s = (char*)malloc(sizeof(char)*2048);
    temp_s[0] = '{';
    while(sumBig){
        int i=0;
        while(ptr->line[i] != '\n'){
            c = ptr->line[i];
            // putchar(c);
            //通过识别大括号个数以确定该函数是否已经结束
            if(c == '{') sumBig++;
            else if(c == '}') sumBig--;
            //删除空白符
            else if(c == ' ' || c == '\r' || c == '\t' || isupper(c)){
                i++;
                continue;
            }
            else if(isalpha(c)||c == '_'){
                int ancn = numOfsc;
                keepword* pTrie = root;
                int sign = 0;
                while( hash[ptr->line[i]] >= 0 && pTrie->next[hash[ptr->line[i]]] != NULL){
                    sign = 1;
                    temp_s[numOfsc++] = ptr->line[i];
                    pTrie = pTrie->next[hash[ptr->line[i]]];
                    i++;
                }
                if(pTrie->isEnd == 0){
                    numOfsc = ancn;
                    if(!sign) i++;
                }
                continue;
            }
            temp_s[numOfsc++] = ptr->line[i++];
        }
        ptr = ptr->next;
    }
    temp_s[numOfsc] = '\0';
    return temp_s;
}