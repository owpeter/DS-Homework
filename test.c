#include <stdio.h>
#include <ctype.h>
int main(){
    FILE *p,*out;
    p = fopen("codes.txt","r");
    out = fopen("out_debug.txt","w");
    int cnt = 0,num = 0;
    char t[1024];
    int sign = 0;
    while(fgets(t,1024,p)!=NULL){
        if(isdigit(t[0])){
            cnt++;
            fprintf(out,"%d: %s\n",num,t);
        }
        if(t[0] == '\n') continue;
        num++;
    }
    printf("%d %d",cnt,num);
}