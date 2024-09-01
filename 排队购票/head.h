#include <iostream>
#include <fstream>  // 添加文件操作头文件
#include <deque>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>
#include <random>
#include <cstdlib>
using namespace std;

struct Customer
{
    string name;       // 购票人的姓名
    int randomOrder;   // 生成的随机数
    string ticketType;  // 票的种类
    int statue = 0;     // 购票成功与否的状态显示
};

int generateRandomOrder();//生成一个在 1 到 1000 之间的随机整数
//shuru输入顾客信息
void inputCustomerInfo(deque<Customer>& originalQueue, deque<Customer>& ticketQueue, int totalCustomers, int totalNormalTickets, int totalPremiumTickets);
//输入票的信息
void inputTicketInfo(int& totalCustomers, int& totalNormalTickets, int& totalPremiumTickets, int& numWindows);
void printOriginalOrder(const deque<Customer>& originalQueue);//输出队列的原始信息（未排序的
void printSortedOrder(const deque<Customer>& sortedQueue);//输出排序后的购票者的信息
//购票函数
void sellTickets(deque<Customer>& ticketQueue, deque<Customer>& failedCustomers, int& totalTickets, int numWindows, int& totalNormalTickets, int& totalPremiumTickets);
void searchCustomerFromFile(const string& fileName, const string& targetName);//文件中查找购票者的信息
//改票
void askRetryPurchase(deque<Customer>& failedCustomers, deque<Customer>& ticketQueue, int& totalNormalTickets, int& totalPremiumTickets);
//退票
void refundTicket(int& totalNormalTickets, int& totalPremiumTickets);

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    int totalCustomers;
    int totalNormalTickets;
    int totalPremiumTickets;
    int numWindows;
    int j;
    deque<Customer> ticketQueue;  // 存储了 Customer 结构体类型的双端队列。
    deque<Customer> originalQueue;
    deque<Customer>failedCustomers;

    while (1)
    {
        printf("1.请输入决赛门票信息和购票人的信息\n");
        printf("2.输出未排序时购票人的信息and输出排完序时购票人的信息\n");
        printf("3.购票并输出购票信息\n");
        printf("4.显示剩余门票\n");
        printf("5.查找客户信息\n");
        printf("6.退票\n");
        printf("7.改票\n");
        printf("8.结束\n");
        printf("请输入选择:");
        scanf_s("%d", &j);
        switch (j)
        {


        case 1:
        {
            inputTicketInfo(totalCustomers, totalNormalTickets, totalPremiumTickets, numWindows);
            inputCustomerInfo(originalQueue, ticketQueue, totalCustomers, totalNormalTickets, totalPremiumTickets);
            system("pause");
            system("cls");
            break;
        }

        case 2:
        {
            printOriginalOrder(originalQueue);
            sort(ticketQueue.begin(), ticketQueue.end(), [](const Customer& a, const Customer& b)
                {
                    return a.randomOrder < b.randomOrder;
                });

            printSortedOrder(ticketQueue);
            break;
        }
        case 3:
        {
            sellTickets(ticketQueue, failedCustomers, totalCustomers, numWindows, totalNormalTickets, totalPremiumTickets);
            break;
        }
        case 4:
        {
            cout << "\n普通票剩余数量: " << totalNormalTickets << endl;
            cout << "高级票剩余数量: " << totalPremiumTickets << endl;
            break;
        }
        case 5:
        {
            string searchName;
            cout << "请输入要查找的购票者姓名：";
            cin >> searchName;
            searchCustomerFromFile("C:\\Users\\Lenovo\\Desktop\\customer_info.txt", searchName);
            break;

        }


        case 6:
        {
            refundTicket(totalNormalTickets, totalPremiumTickets);
            break;
        }

        case 7:
        {

            sort(failedCustomers.begin(), failedCustomers.end(), [](const Customer& c, const Customer& d)
                {
                    return c.randomOrder < d.randomOrder;
                });

            printSortedOrder(failedCustomers);
            askRetryPurchase(failedCustomers,ticketQueue, totalNormalTickets, totalPremiumTickets);
            break;


        }

        case 8:
        {
            exit(OVERFLOW);
            break;
        }

        }
    }

    return 0;
}