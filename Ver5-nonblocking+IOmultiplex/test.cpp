#include <iostream>
#include<stdlib.h>
using namespace std;

int main()
{
    char s[]="hello wocao";
    char a[1000];
    char b[1000];
    char c[1000];
    sscanf(s,"%[^]%[]%s",a,b,c);
    cout<<a<<endl;
    cout<<b<<endl;
    cout<<c<<endl;
    return 0;
}

