#include <iostream>
#include <fstream>  // ����ļ�����ͷ�ļ�
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
    string name;       // ��Ʊ�˵�����
    int randomOrder;   // ���ɵ������
    string ticketType;  // Ʊ������
    int statue = 0;     // ��Ʊ�ɹ�����״̬��ʾ
};

int generateRandomOrder();//����һ���� 1 �� 1000 ֮����������
//shuru����˿���Ϣ
void inputCustomerInfo(deque<Customer>& originalQueue, deque<Customer>& ticketQueue, int totalCustomers, int totalNormalTickets, int totalPremiumTickets);
//����Ʊ����Ϣ
void inputTicketInfo(int& totalCustomers, int& totalNormalTickets, int& totalPremiumTickets, int& numWindows);
void printOriginalOrder(const deque<Customer>& originalQueue);//������е�ԭʼ��Ϣ��δ�����
void printSortedOrder(const deque<Customer>& sortedQueue);//��������Ĺ�Ʊ�ߵ���Ϣ
//��Ʊ����
void sellTickets(deque<Customer>& ticketQueue, deque<Customer>& failedCustomers, int& totalTickets, int numWindows, int& totalNormalTickets, int& totalPremiumTickets);
void searchCustomerFromFile(const string& fileName, const string& targetName);//�ļ��в��ҹ�Ʊ�ߵ���Ϣ
//��Ʊ
void askRetryPurchase(deque<Customer>& failedCustomers, deque<Customer>& ticketQueue, int& totalNormalTickets, int& totalPremiumTickets);
//��Ʊ
void refundTicket(int& totalNormalTickets, int& totalPremiumTickets);

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    int totalCustomers;
    int totalNormalTickets;
    int totalPremiumTickets;
    int numWindows;
    int j;
    deque<Customer> ticketQueue;  // �洢�� Customer �ṹ�����͵�˫�˶��С�
    deque<Customer> originalQueue;
    deque<Customer>failedCustomers;

    while (1)
    {
        printf("1.�����������Ʊ��Ϣ�͹�Ʊ�˵���Ϣ\n");
        printf("2.���δ����ʱ��Ʊ�˵���Ϣand���������ʱ��Ʊ�˵���Ϣ\n");
        printf("3.��Ʊ�������Ʊ��Ϣ\n");
        printf("4.��ʾʣ����Ʊ\n");
        printf("5.���ҿͻ���Ϣ\n");
        printf("6.��Ʊ\n");
        printf("7.��Ʊ\n");
        printf("8.����\n");
        printf("������ѡ��:");
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
            cout << "\n��ͨƱʣ������: " << totalNormalTickets << endl;
            cout << "�߼�Ʊʣ������: " << totalPremiumTickets << endl;
            break;
        }
        case 5:
        {
            string searchName;
            cout << "������Ҫ���ҵĹ�Ʊ��������";
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