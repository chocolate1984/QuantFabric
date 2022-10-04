#include <stdio.h>
#include "Singleton.hpp"

#include <string>
#include <vector>
#include <string.h>
#include "SnapShotHelper.hpp"

#include <thread>
#include <unistd.h>
#include <sys/time.h>
#include "RingBuffer.hpp"

// ����01 ����ģ��
class Test
{
    // ��Ϊ��Ԫ�Ļ��Ϳ���ֻҪ�������������Ϳ���ʵ�ֵ���ģʽ
    friend Utils::Singleton<Test>;  
public:
    void Print()
    {
        printf("Test::Print\n");
    }

private:
    Test(){}
    Test(const Test& other);
    Test& operator=(const Test& other);
};

void T01forSingleton(){
    Utils::Singleton<Test>::GetInstance()->Print();
}

// ����2 ����ģ��
class Test02
{
public:
    int ID;
    char name[16];
};

int T02forSnapShot()
{
    std::string binPath = "20220311.bin";
    // write
    for (size_t i = 0; i < 100; i++)
    {
        Test02 test;
        test.ID = i;
        sprintf(test.name, "Test%03d", i);
        Utils::SnapShotHelper<Test02>::WriteData(binPath, test);
    }
    // load
    std::vector<Test02> vec;
    Utils::SnapShotHelper<Test02>::LoadSnapShot(binPath, vec);
    for (size_t i = 0; i < vec.size(); i++)
    {
        printf("%s\n", vec.at(i).name);  
    }

    return 0;
}

// 03 RingBufferģ��
class Test03
{
public:
   Test03(int id = 0, int value = 0)
   {
	    this->id = id;
        this->value = value;
	    sprintf(data, "id = %d, value = %d", this->id, this->value);
   }
   void display()
   {
       // IO������ʱ����
 	   // printf("%s\n", data);
        sprintf(data,"hello %s", data);
   }
private:
   int id;
   int value;
   char data[760];
   // ���ݴ�С--��Ŀǰ����������ϵͳ���棬Э��Ĺ涨�������Ϣ��Ĵ�С��768���ֽ� �ٽ����������Σ�����768
};

double getdeltatimeofday(struct timeval *begin, struct timeval *end)
{
    return (end->tv_sec + end->tv_usec * 1.0 / 1000000) -
           (begin->tv_sec + begin->tv_usec * 1.0 / 1000000);
}

// ʹ�ù����ڴ�
#if defined IPC
    // IPC ShareMemory, key=0xFF01
    Utils::RingBuffer<Test03> queue(1 << 10, 0xFF01);
#endif
// ʹ�öѿռ�
#if defined HEAP
    Utils::RingBuffer<Test03> queue(1 << 10);  // 2^10
#endif

#define N (20000000)  // ��2ǧ�������ݽ��д���


// �������̺߳���
void produce()
{
    queue.reset();
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    unsigned int i = 0;
    while(i < N)
    {
        if(queue.push(Test03(i >> 10, i)))
        {
	        i++;
        }
    }
    gettimeofday(&end, NULL);
    double tm = getdeltatimeofday(&begin, &end);
    printf("producer tid=%lu %f MB/s %f msg/s elapsed= %f size= %u\n", pthread_self(), N * sizeof(Test) * 1.0 / (tm * 1024 * 1024), N * 1.0 / tm, tm, i);
}

void consume()
{
    usleep(2000);
    Test03 test;
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    unsigned int i = 0;
    while(i < N)
    {
        if(queue.pop(test))
        {
           test.display();
           i++;
        }
    }
    gettimeofday(&end, NULL);
    double tm = getdeltatimeofday(&begin, &end);
    printf("consumer tid=%lu %f MB/s %f msg/s elapsed= %f size= %u\n", pthread_self(), N * sizeof(Test) * 1.0 / (tm * 1024 * 1024), N * 1.0 / tm, tm, i);
}

// �߳�֮��
// test RingBuffer by Heap Memory in one Process
// g++ --std=c++11 -O2 -DHEAP -DPRODUCER -DCONSUMER main.cpp -o ringbufferHeaptest -pthread
// test RingBuffer by Share Memory in different Process
// g++ --std=c++11 -O2 -DIPC -DPRODUCER -DCONSUMER main.cpp -o ringbufferMmptest -pthread

// result �Ѹ���
// consumer tid=140364480468736 4.250042 MB/s 4456491.659692 msg/s elapsed= 4.487835 size= 20000000
// producer tid=140364488861440 4.247908 MB/s 4454254.752799 msg/s elapsed= 4.490089 size= 20000000

// consumer tid=140356254099200 3.925878 MB/s 4116581.500468 msg/s elapsed= 4.858400 size= 20000000
// producer tid=140356262491904 3.924318 MB/s 4114946.031303 msg/s elapsed= 4.860331 size= 20000000


int main(int argc, char* argv[])
{
    // T01forSingleton();
    // T02forSnapShot();
    
    // T03RingBuffer
    // #if defined PRODUCER
    //     std::thread producer(produce);
    // #endif
    // #if defined CONSUMER
    //     std::thread consumer(consume);
    // #endif
    // #if defined PRODUCER
    //     producer.join();
    // #endif
    // #if defined CONSUMER
    //     consumer.join();
    // #endif
    
    
    return 0;

}

