#include <iostream>
#include<stdlib.h>
#include<vector>
#include<unordered_map>
#include<memory>
#include<algorithm>
using namespace std;
class A
{
public:
    void display()
    {
        cout<<"fuck you"<<endl;
    }
};
int main()
{
    
    //shared_ptr<A> a=make_shared<A>();
    //a->display();
   // sb.display();
    int sb=100;
    vector<int> arr;
    arr.push_back(sb);
    cout<<&sb<<endl;
    cout<<&arr[0]<<endl;
    return 0;
}

