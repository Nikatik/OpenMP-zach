#include <iostream>
#include <time.h>
#include <omp.h>
#include <chrono>
#define SIZE 30000
#define SIZELINE 30000
#define RAND 5

using namespace std;
void print(int**);
void print(int);

void print(int** a)
{
    for(int i = 0; i < SIZE; i++)
    {
        for(int l = 0; l < SIZELINE; l++)
        {
            if(a[i][l] < 10)
                cout << "0";
            cout << a[i][l] << " ";
        }
        cout << endl;
    }
}
void print(int a)
{
    cout << endl << a / 1000 << ".";
    if((a%1000)<10) cout<<"00";
    if((a%1000)>10&&(a%1000)<100) cout<<"0";
    cout<<(a%1000)<<" s"<<endl;
}
int main()
{
    cout << "Hello world!" << endl;

    int** a = new int* [SIZE * sizeof(int)];
    srand( static_cast<unsigned int>(time(nullptr)));
    auto begin = std::chrono::steady_clock::now();

#pragma omp parallel  sections  shared(a)  //параллельные секции, выполняются для всех потоков
	{
		#pragma omp    section  //потоки разделяются, эту секцию выполняет один поток
		for(int i = 0; i < SIZE / 2; i++)
		{
			a[i] = new int [SIZELINE * sizeof(int)];
			for(int l = 0; l < SIZELINE; l++)
                a[i][l] = rand() % (RAND-1) + 1;
		}
		#pragma omp    section  //потоки разделяются, эту секцию выполняет один поток

        for(int i = SIZE / 2; i < SIZE; i++)
		{
			a[i] = new int [SIZELINE * sizeof(int)];
			for(int l = 0; l < SIZELINE; l++)
                a[i][l] = rand() % (RAND-1) + 1;
		}
	}
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    print(elapsed_ms.count());
    int** c = new int*[SIZE * sizeof(int*)];
    for(int i=0;i<SIZE;i++)
    {
        c[i]=new int[SIZELINE * sizeof(int)];
        for(int l=0;l<SIZELINE;l++)
            c[i][l]=a[i][l];
    }
    //print(a);
    cout << "\n";
    begin = std::chrono::steady_clock::now();
    int max = 0, max1 = SIZE / 2;
    #pragma omp parallel  sections shared(a, max, max1)  //параллельные секции, выполняются для всех потоков
	{
		#pragma omp    section  //потоки разделяются, эту секцию выполняет один поток
		for(int i = 1; i < SIZE / 2; i++)
		{
			if(a[i][0] > a[max][0])
                max = i;
            if(a[i][0] == a[max][0])
                for(int l = 0; l < SIZELINE; l++)
                    if(a[i][l] > a[max][l])
                    {
                        max = i;
                        break;
                    }
		}
		#pragma omp    section  //потоки разделяются, эту секцию выполняет один поток

        for(int i = SIZE / 2 + 1; i < SIZE; i++)
		{
			if(a[i][0] > a[max1][0])
                max1 = i;
            if(a[i][0] == a[max1][0])
                for(int l = 0; l < SIZELINE; l++)
                    if(a[i][l] > a[max1][l])
                    {
                        max1 = i;
                        break;
                    }
		}
	}

	if(a[max1][0] > a[max][0])
                max = max1;
            if(a[max1][0] == a[max][0])
                for(int l = 0; l < SIZELINE; l++)
                    if(a[max1][l] > a[max][l])
                    {
                        max = max1;
                        break;
                    }
    end = std::chrono::steady_clock::now();
    elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    print(elapsed_ms.count());

    int *tmp = a[SIZE - 1];
    a[SIZE - 1] = a[max];
    a[max] = tmp;

    cout << "Num " << max << endl;
    //print(a);
    cout << "\n\n\n";
    begin = std::chrono::steady_clock::now();
    int** b = new int* [SIZE * sizeof(int)];
    for(int i = 0; i < SIZE; i++)
    {
        b[i] = new int [SIZELINE * sizeof(int)];
        for(int l = 0; l < SIZELINE; l++)
            b[i][l] = rand() % (RAND-1) + 1;
    }
    end = std::chrono::steady_clock::now();
    elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    print(elapsed_ms.count());
    //print(b);
    cout << "\n";
    begin = std::chrono::steady_clock::now();
    max1 = 0;
    for(int i = 1; i < SIZE; i++)
    {
		if(c[i][0] > c[max1][0])
            max1 = i;
        if(c[i][0] == c[max1][0])
            for(int l = 0; l < SIZELINE; l++)
                if(c[i][l] > c[max1][l])
                {
                    max1 = i;
                    break;
                }
    }
    end = std::chrono::steady_clock::now();
    elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    print(elapsed_ms.count());
    tmp = c[SIZE - 1];
    c[SIZE - 1] = c[max1];
    c[max1] = tmp;
    cout << "Num " << max1 << endl;
    //print(c);
    return 0;
}
