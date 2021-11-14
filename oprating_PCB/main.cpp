#include <iostream>
using namespace std;

#define MAXQSIZE 100
#define MAXINT 100;

typedef struct PCB {
    string name;
    PCB *next;
    int time;
    int prior;   //作出一个约定：当prior等于-1的时候表明此进程已经运行结束，当prior等于-2时表明此队列块失效
    string status; //进程状态只有"E"和"R"两种状态
}PCB,*qPCB;


//----队列的定义及操作--------
typedef struct {
    PCB *base;							//初始化的动态分配存储空间
    int front;								//头指针，若队列不空，指向队头元素
    int rear;								//尾指针，若队列不空，指向队尾元素的下一个位置
}sqQueue;
/////////////////////////////////////////////////////////////////////////////////////////
void InitQueue(sqQueue &Q){
    //构造一个空队列Q
    Q.front = Q.rear = 0;
    Q.base = new PCB[MAXQSIZE];
}//InitQueue

void EnQueue(sqQueue &Q, PCB e){
    //插入元素e为Q的新的队尾元素
    Q.base[Q.rear] = e;
    Q.rear = (Q.rear + 1)%MAXQSIZE;
}//EnQueue

bool QueueEmpty(sqQueue Q){
    //判断是否为空队
    if (Q.rear == Q.front) {
        return true;
    } else {
        return false;
    }
}//QueueEmpty

void DeQueue(sqQueue &Q, PCB &u){
    //队头元素出队并置为u
    u = Q.base[Q.front];
    //以下补充一条语句
    Q.front = (Q.front + 1)%MAXQSIZE;

    if (u.time - 1 > 0) {
        EnQueue(Q,u);
    }
}//DeQueue

void initQueue(sqQueue &Q); //对10个PCB进行初始化
void display(sqQueue q); //展示所有进程的状态
void onControl(sqQueue &Q); //将队列中的一个进程调度到处理器中运行
int scan(sqQueue Q);//扫描整个队列，将优先级最高的一个返回
void place(sqQueue &Q,int position);// 如果时间片没有用完，将此进程放到队尾
bool check(sqQueue Q);//检查队列里面是否还有没执行完的进程
void displaySlected(sqQueue Q,int position); //将选中的一个进程进行单独展示
int main() {

    cout<<"姓名：徐睿航 学号：202013160210 实验：处理机调度"<<endl;
    sqQueue Q;
    initQueue(Q);
    cout<<"------最开始初始化状态------"<<endl;
        display(Q);
    int count = 1;
    while (check(Q)) {
        cout<<"执行第"<<count<<"个时间片"<<endl;
        onControl(Q);
        count++;
    }

    return 0;
}

//对10个PCB进行初始化，并对整个队列进行初始化
void initQueue(sqQueue &Q) {

    Q.base = new PCB[MAXQSIZE];
    Q.front = Q.rear = 0;
    //首先将前十个进程放到队列里面
    for (int i = 0; i < 5; ++i) {
         string count = to_string(i + 1);
         string name = "P";
         name.append(count);
         Q.base[i].next = NULL;
         Q.base[i].prior = 10 - i;
         Q.base[i].time = 2;
         Q.base[i].status = "R";
         Q.base[i].name = name;
         Q.rear++;
    }

    for (int i = 5; i < 10; ++i) {
        string count = to_string(i + 1);
        string name = "P";
        name.append(count);
        Q.base[i].next = NULL;
        Q.base[i].prior = 10 - i;
        Q.base[i].time = 1;
        Q.base[i].status = "R";
        Q.base[i].name = name;
        Q.rear++;
    }

    for (int i = 10; i < MAXQSIZE; ++i) {
         Q.base[i].next = NULL;// 后面的进程，只需要初始化指针
    }

    cout<<"---------初始化完成----------"<<endl;

}

//扫描整个队列，将目前就绪进程中，优先级最高的一个返回
int scan(sqQueue Q) {
    int prior = -1;
    int max = -1;
    int positon = -1;
    for (int i = 0; i < MAXQSIZE; ++i) {
        if (Q.base[i].prior != -1 && Q.base[i].prior > max && Q.base[i].prior) {
            max = Q.base[i].prior;
            positon = i;
        }
    }
    return positon;
}

//整个队列的状态
void display(sqQueue q) {
    int n = 1;
    for (int i = 0; i < MAXQSIZE; ++i) {
        if (q.base[i].prior != -2 && q.base[i].prior) {
            cout<<"第"<<n<<"个进程状态-------------"<<endl;
            cout<<"该进程的名称是： "<<q.base[i].name<<endl;
            cout<<"该进程的状态为： "<<q.base[i].status<<endl;
            cout<<"该进程的目前的优先级为： "<<q.base[i].prior<<endl;
            cout<<"该进程还剩余的时间片为： "<<q.base[i].time<<endl;
            n++;
        }
    }
}
//将队列中的一个进程调度到处理器中运行
void onControl(sqQueue &Q) {
    int selected = scan(Q);
    cout<<"------队列目前的状态是-------"<<endl;
    display(Q);
    cout<<"------将要放上去运行的是:"<<endl;
    displaySlected(Q,selected);

    if (Q.base[selected].time - 1 > 0) {
        Q.base[Q.rear + 1].time = Q.base[selected].time -= 1;//将时间减1
        Q.base[Q.rear + 1].prior = Q.base[selected].prior -= 1;//将优先级减1
        Q.base[Q.rear + 1].status = Q.base[selected].status;
        Q.base[Q.rear + 1].name = Q.base[selected].name;
        Q.base[Q.rear + 1].next = Q.base[selected].next;
        Q.base[selected].prior = -2;
    } else {
        Q.base[selected].prior = -1;//表明此进程已经运行结束
        Q.base[selected].time = 0;//时间片自然为0
        Q.base[selected].status = "E";
    }
    cout<<"------一个时间片后的队列状态是-------"<<endl;
    display(Q);
}

//将选中的一个进程进行单独展示
void displaySlected(sqQueue Q ,int position) {
    int i = position;

    cout<<"该进程的名称是： "<<Q.base[i].name<<endl;
    cout<<"该进程的状态为： "<<Q.base[i].status<<endl;
    cout<<"该进程的目前的优先级为： "<<Q.base[i].prior<<endl;
    cout<<"该进程还剩余的时间片为： "<<Q.base[i].time<<endl;
}

//检查队列里面是否还有没执行完的进程
bool check(sqQueue Q) {
    for (int i = 0; i < MAXQSIZE; ++i) {
        if (Q.base[i].prior > 0) {
            return true;
        }
    }

    return false;
}



