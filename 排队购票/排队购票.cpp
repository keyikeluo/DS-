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
//生成一个在 1 到 1000 之间的随机整数
int generateRandomOrder()
{
    static random_device rd;            // 创建一个硬件随机数生成器，用于生成种子
    static mt19937 generator(rd());     // 生成一个伪随机数生成器
    uniform_int_distribution<int> distribution(1, 1000);  // 建一个均匀分布的整数分布器，范围是1到1000
    return distribution(generator);          // 调用该分布器，从而产生一个在1到1000之间的随机整数。
} 

// 输出原始购票者的信息
void printOriginalOrder(const deque<Customer>& originalQueue)
{
    cout << "排序前的购票者信息：" << endl;
    for (const Customer& customer : originalQueue)//foreach 循环
    {
        cout << "姓名：" << customer.name;
        cout << "，随机编号：" << customer.randomOrder;
        cout << "，票种类：" << customer.ticketType << endl;
    }
}
//输出排序后的购票者的信息
void printSortedOrder(const deque<Customer>& sortedQueue)
{
    cout << "排序后的购票者信息：" << endl;
    for (const Customer& customer : sortedQueue)
    {
        cout << "姓名：" << customer.name;
        cout << "，随机编号：" << customer.randomOrder;
        cout << "，票种类：" << customer.ticketType << endl;
    }
}

void sellTickets(deque<Customer>& ticketQueue, deque<Customer>& failedCustomers,int& totalTickets, int numWindows, int& totalNormalTickets, int& totalPremiumTickets)
{
    ofstream outFile("C:\\Users\\Lenovo\\Desktop\\customer_info.txt", ios::app);  // 打开文件用于追加写入

    while (!ticketQueue.empty() && totalTickets > 0)//empty() 函数是容器类的一个成员函用于检查容器是否为空。
    {
        int windowIndex = rand() % numWindows;  // 随机分配窗口,使用 `rand()` 函数来生成伪随机数
        int instruction = rand() % 2;           // 售票窗口空闲时随机发出0或1指令

        if (instruction == 0)  // 指令为0时，最小编号者到窗口购票
        {
            Customer currentCustomer = ticketQueue.front();//front（）类模板的一个成员函数，用于返回队列的第一个元素（即队首元素）的引用。
            ticketQueue.pop_front();

            cout << "窗口" << windowIndex + 1 << "发出指令0，" << currentCustomer.name << "到窗口购票：";

            if (currentCustomer.ticketType == "普通票")
            {
                if (totalNormalTickets == 0)
                {
                    cout << "普通票已售空，后面的人不能再买到普通票了。" << endl;
                }
                else
                {
                    totalNormalTickets--;
                    currentCustomer.statue = 1;
                }
            }
            else
            {
                if (totalPremiumTickets == 0)
                {
                    cout << "高级票已售空，后面的人不能再买到高级票了。" << endl;
                }
                else
                {
                    totalPremiumTickets--;
                    currentCustomer.statue = 1;
                }
            }

            if (totalTickets == 0)
            {
                cout << "票已售空，后面的人不能再买到票了。" << endl;
                break;
            }
            if (currentCustomer.statue == 1)
            {
                totalTickets--;
                cout << currentCustomer.name << "  排序随机数：" << currentCustomer.randomOrder << "购买票种类：" << currentCustomer.ticketType << "）购票成功！" << endl;
            }
            else {
                failedCustomers.push_back(currentCustomer);  // 购票失败，将信息添加到failedCustomers中
            }
            //然后将购票者信息写入文件
            outFile << currentCustomer.name << " " << currentCustomer.randomOrder << " " << currentCustomer.ticketType << " " << currentCustomer.statue << endl;
        }
        else
        {
            Customer currentCustomer = ticketQueue.back();//返回队列的最后一个元素（即队尾元素）的引用
            ticketQueue.pop_back();

            cout << "窗口" << windowIndex + 1 << "发出指令1，" << currentCustomer.name << "到窗口购票：";

            if (currentCustomer.ticketType == "普通票")
            {
                if (totalNormalTickets == 0)
                {
                    cout << "普通票已售空，后面的人不能再买到普通票了。" << endl;
                }
                else
                {
                    currentCustomer.statue = 1;
                    totalNormalTickets--;
                }
            }
            else
            {
                if (totalPremiumTickets == 0)
                {
                    cout << "高级票已售空，后面的人不能再买到高级票了。" << endl;
                }
                else
                {
                    currentCustomer.statue = 1;
                    totalPremiumTickets--;
                }
            }

            if (totalTickets == 0)
            {
                cout << "票已售空，后面的人不能再买到票了。" << endl;
                break;
            }
            if (currentCustomer.statue == 1)
            {
                totalTickets--;
                cout << currentCustomer.name << " (排序随机数：" << currentCustomer.randomOrder << "购买票种类：" << currentCustomer.ticketType << "）购票成功！" << endl;
            }

            outFile << currentCustomer.name << " " << currentCustomer.randomOrder << " " << currentCustomer.ticketType << " " << currentCustomer.statue << endl;
        }
    }

    outFile.close();  // 关闭文件
}
//文件中查找购票者的信息
void searchCustomerFromFile(const string& fileName, const string& targetName)
{
    ifstream inFile(fileName);//创建了一个输入文件流对象 `inFile`，yu `fileName` 相关联，以便后续的文件读取操作。

    if (!inFile.is_open())//于检查与输入文件流对象 inFile 关联的文件是否成功打开。
    {
        cout << "无法打开文件 " << fileName << "。" << endl;
        return;
    }

    bool found = false;
    string line;
    Customer lastMatchingCustomer;//因为后面修改客户信息我们增加了客户的信息，所以查找的时候我们需要插找最后一个
    // 逐行读取文件内容
    while (getline(inFile, line))
    {
        // 检查是否成功从文件中读取一行
        if (!line.empty())
        {
            // 找到空格的位置，将信息分割
            size_t pos1 = line.find(' ');//一种数据类型，通常用于表示对象的大小或索引
            size_t pos2 = line.find(' ', pos1 + 1);
            size_t pos3 = line.find(' ', pos2 + 1);

            // 检查是否找到了三个空格，表示文件格式正确
            if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos)//string::npos` 表示在字符串中未找到指定字符或子字符串的特殊值
            {
                // 提取购票者信息的各个部分
                string name = line.substr(0, pos1);
                int randomOrder = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
                string ticketType = line.substr(pos2 + 1, pos3 - pos2 - 1);
                int statue = stoi(line.substr(pos3 + 1));

                // 检查姓名是否匹配目标姓名
                if (name == targetName)
                {
                    found = true;
                    // 将匹配到的购票者信息存储在 lastMatchingCustomer 中
                    lastMatchingCustomer = { name, randomOrder, ticketType, statue };
                }
            }
        }
    }

    inFile.close();

    if (found)
    {
        cout << "姓名：" << lastMatchingCustomer.name;
        cout << "，随机编号：" << lastMatchingCustomer.randomOrder;
        cout << "，票种类：" << lastMatchingCustomer.ticketType;
        cout << "，购票状态：" << (lastMatchingCustomer.statue == 1 ? "成功" : "失败") << endl;
    }
    else
    {
        cout << "未找到姓名为 " << targetName << " 的购票者信息。" << endl;
    }
}


void inputCustomerInfo(deque<Customer>& originalQueue, deque<Customer>& ticketQueue, int totalCustomers, int totalNormalTickets, int totalPremiumTickets)
{
    for (int i = 0; i < totalCustomers; ++i)
    {
        string customerName;
        cout << "请输入第" << i + 1 << "位购票者的姓名：";
        cin >> customerName;

        int randomOrder = generateRandomOrder();//调用generateRandomOrder函数生成随机数

        cout << "请输入 " << customerName << " 要购买的票的种类（0 表示普通票，1 表示高级票）：";
        int ticketChoice;
        cin >> ticketChoice;
        if (ticketChoice != 0 && ticketChoice != 1)
        {
            cout << "您的输入有误，请你重新输入要购买的票的种类（0 表示普通票，1 表示高级票）：";
            int a;
            cin >> a;
            ticketChoice = a;
        }

        string ticketType = (ticketChoice == 0) ? "普通票" : "高级票";

        originalQueue.push_back({ customerName, randomOrder, ticketType });//新的元素添加到队列的末尾
        ticketQueue.push_back({ customerName, randomOrder, ticketType });
    }
}
void inputTicketInfo(int& totalCustomers, int& totalNormalTickets, int& totalPremiumTickets, int& numWindows)
{
    cout << "请输入购票者总数：";
    cin >> totalCustomers;

    cout << "请输入总普通票数：";
    cin >> totalNormalTickets;

    cout << "请输入总高级票数：";
    cin >> totalPremiumTickets;

    cout << "请输入窗口数量：";
    cin >> numWindows;
}
void askRetryPurchase(deque<Customer>& failedCustomers, deque<Customer>& ticketQueue, int& totalNormalTickets, int& totalPremiumTickets)
{
    ofstream outFile("C:\\Users\\Lenovo\\Desktop\\customer_info.txt", ios::app);

    for (Customer& failedCustomer : failedCustomers)
    {
        cout << failedCustomer.name << "购票失败，是否要重新购买？（y/n）: ";
        char choice;
        cin >> choice;

        if (choice == 'y' || choice == 'Y')
        {
            cout << "请选择要购买的票的种类（0 表示普通票，1 表示高级票）： ";
            int ticketChoice;
            cin >> ticketChoice;

            if (ticketChoice != 0 && ticketChoice != 1)
            {
                cout << "您的输入有误，请重新输入要购买的票的种类（0 表示普通票，1 表示高级票）：";
                int a;
                cin >> a;
                ticketChoice = a;
            }

            if (ticketChoice == 0 && totalNormalTickets > 0)
            {
                failedCustomer.ticketType = "普通票";
                failedCustomer.statue = 1;
                totalNormalTickets--;

                cout << failedCustomer.name << "购票成功！购票种类：普通票" << endl;
                ticketQueue.push_back(failedCustomer);  // 将购票成功的顾客信息添加到队列中
            }
            else if (ticketChoice == 1 && totalPremiumTickets > 0)
            {
                failedCustomer.ticketType = "高级票";
                failedCustomer.statue = 1;
                totalPremiumTickets--;

                cout << failedCustomer.name << "购票成功！购票种类：高级票" << endl;
                ticketQueue.push_back(failedCustomer);  // 将购票成功的顾客信息添加到队列中
            }
            else
            {
                cout << "对不起，该票种已售空，无法购买。" << endl;
            }


        }

        outFile << failedCustomer.name << " " << failedCustomer.randomOrder << " " << failedCustomer.ticketType << " " << failedCustomer.statue << endl;
        failedCustomers.pop_back();
    }

    outFile.close();
}

void refundTicket(int& totalNormalTickets, int& totalPremiumTickets) 
{
    // 退票功能
    cout << "请输入要退票的购票者姓名：";
    string refundName;
    cin >> refundName;

    // 从文件中读取购票者信息
    ifstream inFile("C:\\Users\\Lenovo\\Desktop\\customer_info.txt");

    if (!inFile.is_open())
    {
        cout << "无法打开文件 customer_info.txt。" << endl;
        //break;
    }

    deque<Customer> customerList;

    string line;
    while (getline(inFile, line))
    {
        // 解析文件中的购票者信息
        size_t pos1 = line.find(' ');
        size_t pos2 = line.find(' ', pos1 + 1);
        size_t pos3 = line.find(' ', pos2 + 1);

        if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos)
        {
            string name = line.substr(0, pos1);
            int randomOrder = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
            string ticketType = line.substr(pos2 + 1, pos3 - pos2 - 1);
            int statue = stoi(line.substr(pos3 + 1));

            customerList.push_back({ name, randomOrder, ticketType, statue });
        }
    }

    inFile.close();

    // 在购票者列表中查找要退票的购票者
    //库函数 find_if，它在指定范围内查找第一个满足指定条件的元素。auto的目的是编译器自动推断返回类型
    auto refundCustomer = find_if(customerList.begin(), customerList.end(), [refundName](const Customer& c)
        {
            return c.name == refundName && c.statue == 1;  // 只能退成功的票
        });

    if (refundCustomer != customerList.end())//果等于，表示在 customerList 中没有找到满足条件的元素，即没有找到名为 refundName 且购票状态为成功的顾客。
        //如果不等于，说明找到了匹配的元素，可以进行后续的操作。
    {
        // 退票成功，增加相应的票数和状态更新
        if (refundCustomer->ticketType == "普通票")
        {
            totalNormalTickets++;
        }
        else
        {
            totalPremiumTickets++;
        }
        refundCustomer->statue = 0;  // 退票状态设置为失败

        cout << refundName << " 退票成功！" << endl;

        // 更新文件中的购票者信息
        ofstream outFile("C:\\Users\\Lenovo\\Desktop\\customer_info.txt");
        for (const Customer& customer : customerList)
        {
            outFile << customer.name << " " << customer.randomOrder << " " << customer.ticketType << " " << customer.statue << endl;
        }
        outFile.close();
    }
    else
    {
        cout << "未找到姓名为 " << refundName << " 的购票者信息或该购票者购票状态为失败，退票失败。" << endl;
    }
}

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