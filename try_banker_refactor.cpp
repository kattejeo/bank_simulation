#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <ctime>
#include <math.h>
using namespace std;

class Time
{
private:
    int houre, minute;

public:
    Time()
    {
        houre = minute = 0;
    }

    Time(int hour_e, int minut_e)
    {
        houre = hour_e;
        minute = minut_e;
    }

    void setTime(int hour_e, int minut_e)
    {
        if (hour_e < 0 || hour_e > 23)
        {
            throw "Houre must be between 0 - 23";
        }

        if (minut_e < 0 || minut_e > 59)
        {
            throw "Minute must be between 0- 59";
        }
        houre = hour_e;
        minute = minut_e;
    }

    int getHoure()
    {
        return houre;
    }

    int getMinute()
    {
        return minute;
    }

    Time operator-(Time &other_time)
    {
        Time time;
        time.houre = this->houre - other_time.houre;
        time.minute = this->minute - other_time.minute;
        return time;
    }

    static int duration(Time &time)
    {
        return time.houre * 60 + time.minute;
    }

    static void toString(Time &time)
    {
        int duration = Time::duration(time);
        int h_oure = duration / 60;
        int m_inute = duration % 60;
        cout << h_oure << "hr " << m_inute << "min." << endl;
    }

    static string toString(int value)
    {
        return to_string(value / 60) + "hr " + to_string(value% 60) + "min.";
    }
};

class Randomize
{
private:
    int seed;

    int nextSeed(int value)
    {
        return int(pow(value, 2) / 100) % 10000;
    }

public:
    Randomize()
    {
        seed = 0;
    }

    Randomize(int see_d)
    {
        seed = see_d;
    }

    void setSeed(int see_d)
    {
        if (see_d < 1000 || see_d > 9999)
        {
            throw "Seed must be between 1000-9999";
        }
        seed = see_d;
    }

    int getSeed()
    {
        return seed;
    }

    auto randomize()
    {
        struct randomize
        {
            int arrivalTime;
            int serviceType;
        };
        seed = nextSeed(seed);
        // seed = 1000 + rand() % 8999;

        return randomize{1 + int(float(seed) / 1000), 1 + int(float(seed) / 4000)};
    }
};

class Node
{
public:
    int arrivalTime, waitingTime, served,
        workLoad, serviceType, serviceTime,
        idelTime;
    Node *next;

    Node()
    {
        arrivalTime = waitingTime = served =
            workLoad = serviceType = serviceTime =
                idelTime = 0;
        next = NULL;
    }
};

class CustomerList : public Randomize
{
public:
    Time stime, start, end;

    Node *head;

    CustomerList()
    {
        head = NULL;
    }

    CustomerList(Node *nod_e)
    {
        head = nod_e;
    }

    bool isEmpty()
    {
        if (head)
        {
            return false;
        }

        return true;
    }

    void init()
    {

        int hour, minute, seed;
        bool exit = true;
        do
        {
            try
            {
                cout << "start Houre : ";
                cin >> hour;
                cout << "start Minute: ";
                cin >> minute;

                start.setTime(hour, minute);
                exit = false;
            }
            catch (const char *msg)
            {
                cout << "Exception: " << msg << endl
                     << endl;
            }
        } while (exit);

        exit = true;
        do
        {
            try
            {
                cout << "end Houre : ";
                cin >> hour;
                cout << "end Minute: ";
                cin >> minute;

                end.setTime(hour, minute);
                exit = false;
            }
            catch (const char *msg)
            {
                cout << "Exception: " << msg << endl
                     << endl;
            }
        } while (exit);

        exit = true;
        do
        {
            try
            {
                cout << "Enter the seed: ";
                cin >> seed;

                setSeed(seed);
                startSimulation();
                buildList();
                exit = false;
            }
            catch (const char *msg)
            {
                cout << "Exception: " << msg << endl
                     << endl;
            }
        } while (exit);
    }

    void startSimulation()
    {
        cout << "*Simulation started*" << endl;
    }

    void buildList()
    {
        stime = end - start;
        for (int i = 0; i < Time::duration(stime); i++)
        {
            pushCustomer();
        }

    }
    void _buildList(){
        
    }

    void pushCustomer()
    {
        int waitingTime = 0;
        auto rrand = randomize();
        Node *currentCustomer = new Node();

        currentCustomer->arrivalTime = rrand.arrivalTime;
        currentCustomer->serviceType = rrand.serviceType;
        switch (rrand.serviceType)
        {
        case 1:
            currentCustomer->workLoad = 3;
            break;
        case 2:
            currentCustomer->workLoad = 5;
            break;
        default:
            currentCustomer->workLoad = 8;
            break;
        }

        if (head == NULL)
        {
            currentCustomer->waitingTime = 0;
            currentCustomer->idelTime = currentCustomer->arrivalTime;
            currentCustomer->serviceTime = currentCustomer->workLoad;
            head = currentCustomer;
        }
        else
        {

            Node *previusCustomer = head;
            while (previusCustomer->next)
            {
                previusCustomer = previusCustomer->next;
            }

            previusCustomer->next = currentCustomer;
            waitingTime = previusCustomer->serviceTime - currentCustomer->arrivalTime;
            if (waitingTime < 0)
            {
                currentCustomer->idelTime = abs(waitingTime);
                currentCustomer->waitingTime = 0;
            }
            else
            {
                currentCustomer->idelTime = 0;
                currentCustomer->waitingTime = waitingTime;
            }

            currentCustomer->serviceTime = currentCustomer->waitingTime + currentCustomer->workLoad;
        }
    }

    void pushCustomerR()
    {
        auto rrand = randomize();
        Node *node = new Node();

        node->arrivalTime = rrand.arrivalTime;
        node->serviceType = rrand.serviceType;
        switch (rrand.serviceType)
        {
        case 1:
            node->workLoad = 5;
            break;
        case 2:
            node->workLoad = 8;
            break;
        default:
            node->workLoad = 10;
            break;
        }

        if (head == NULL)
        {
            node->waitingTime = 0;
            node->idelTime = node->arrivalTime;
            node->serviceTime = node->workLoad;
            head = node;
        }
        else
        {
            int waitingTime = 0;
            Node *temp = head;
            while (temp->next)
            {
                temp = temp->next;
            }

            temp->next = node;
            waitingTime = temp->serviceTime - node->arrivalTime;
            if (waitingTime < 0)
            {
                node->waitingTime = 0;
                node->idelTime = abs(waitingTime);
            }
            else
            {
                node->waitingTime = waitingTime;
                node->idelTime = 0;
            }

            node->serviceTime = node->workLoad + node->waitingTime;
        }
    }

    auto report()
    {
        struct reportStruct
        {
            int totalWaitingTime = 0;
            int totalServiceTime = 0;
            int totalIdleTime = 0;
            struct serviceType{
                int typeone = 0;
                int typetwo = 0;
                int typethree = 0;
            }service;
            int customerAtGivenTime = 0;
            int customerJointAtIdleTime = 0;
            int maximumWaitingTime = 0;
            int averageWaitingTime = 0;
            int averageServiceTime = 0;
            int totalUsers = 0;
        } report;

        Node *ptr = head;
        int duration, temp = 0;
        duration = Time::duration(stime);
        while (ptr)
        {
            report.totalWaitingTime += ptr->waitingTime;
            report.totalServiceTime += ptr->serviceTime;
            report.totalIdleTime += ptr->idelTime;
            if(report.maximumWaitingTime < ptr->waitingTime){
                report.maximumWaitingTime = ptr->waitingTime;
            }
            switch(ptr->serviceType){
                case 1:
                    report.service.typeone ++;
                    break;
                case 2:
                    report.service.typetwo ++;
                    break;
                case 3:
                    report.service.typethree ++;
                    break;
                default:
                    break;
            }
            temp += ptr->serviceTime;
            if(temp <= duration){
                report.customerAtGivenTime ++;
            }

            if(ptr->idelTime == 0){
                report.customerJointAtIdleTime ++;
            }

            report.totalUsers++;
            ptr = ptr->next;
        }

        report.averageWaitingTime = report.totalWaitingTime / report.totalUsers;
        report.averageServiceTime = report.totalServiceTime / report.totalUsers;

        return reportStruct{
            report.totalWaitingTime,
            report.totalServiceTime,
            report.totalIdleTime,
            report.service,
            report.customerAtGivenTime,
            report.customerJointAtIdleTime,
            report.maximumWaitingTime,
            report.averageWaitingTime,
            report.averageServiceTime,
            report.totalUsers};
    }

    void detailReport()
    {
        auto result = report();
        cout << endl << "Report" << endl
             << "Total waiting time  : " << result.totalWaitingTime << endl
             << "Total service time  : " << Time::toString(result.totalServiceTime) << endl
             << "Total Idle time     : " << Time::toString(result.totalIdleTime) << endl
             << "Service Type:(typeone, typetwo, typethree): (" 
                    << result.service.typeone << ", " 
                    << result.service.typetwo << ", " 
                    << result.service.typethree << ")" << endl 
             << "*Customer at a given time: " << result.customerAtGivenTime << " customers." << endl
             << "*Customer join at idle time: "<< (result.customerJointAtIdleTime) << " customers." << endl
             << "Maximum waiting time: " << Time::toString(result.maximumWaitingTime) << endl 
             << "Average waiting time: " << Time::toString(result.averageWaitingTime) << endl
             << "Average service time: " << Time::toString(result.averageServiceTime) << endl
             << "Total number of user: " << result.totalUsers << endl
             << endl;
    }

    void fileReport(){
        time_t now = time(0);
        auto result = report();
        string filename = to_string(now) + ".txt";
        cout << "Filename: " << filename << endl << endl;
        ofstream out;
        out.open(filename);

        if (isEmpty()){
            out << endl
                 << "List is empty" << endl
                 << endl;
        }else{
            Node *ptr = head;
            int user = 1;
            out << setw(5) << "User"
                << setw(15) << "Arrival Time"
                << setw(15) << "Service Time"
                << setw(15) << "Service Type"
                << setw(15) << "Work Load"
                << setw(15) << "Waiting Time"
                << setw(15) << "Idle Time" << endl;
            while (ptr)
            {
                out << setw(5) << user
                    << setw(15) << ptr->arrivalTime
                    << setw(15) << ptr->serviceTime
                    << setw(15) << ptr->serviceType
                    << setw(15) << ptr->workLoad
                    << setw(15) << ptr->waitingTime
                    << setw(15) << ptr->idelTime << endl;

                ptr = ptr->next;
                user++;
            }

        out << endl << "Final report" << endl 
            << "Total waiting time  : " << result.totalWaitingTime << endl
            << "Total service time  : " << Time::toString(result.totalServiceTime) << endl
            << "Total Idle time     : " << Time::toString(result.totalIdleTime) << endl
            << "Service Type:(typeone, typetwo, typethree): (" 
                    << result.service.typeone << ", " 
                    << result.service.typetwo << ", " 
                    << result.service.typethree << ")" << endl 
            << "*Customer at a given time: " << result.customerAtGivenTime << " customers." << endl
            << "*Customer join at idle time: "<< (result.customerJointAtIdleTime) << " customers." << endl
            << "Maximum waiting time: " << Time::toString(result.maximumWaitingTime) << endl 
            << "Average waiting time: " << Time::toString(result.averageWaitingTime) << endl
            << "Average service time: " << Time::toString(result.averageServiceTime) << endl
            << "Total number of user: " << result.totalUsers << endl
            << endl;
        }
        
        out.close();
    }
    void display()
    {
        if (isEmpty()){
            cout << endl
                 << "List is empty" << endl
                 << endl;
        }else{
            Node *ptr = head;
            int user = 1;
            cout << setw(5) << "User"
                 << setw(15) << "Arrival Time"
                 << setw(15) << "Service Time"
                 << setw(15) << "Service Type"
                 << setw(15) << "Work Load"
                 << setw(15) << "Waiting Time"
                 << setw(15) << "Idle Time" << endl;
            while (ptr)
            {
                cout << setw(5) << user
                     << setw(15) << ptr->arrivalTime
                     << setw(15) << ptr->serviceTime
                     << setw(15) << ptr->serviceType
                     << setw(15) << ptr->workLoad
                     << setw(15) << ptr->waitingTime
                     << setw(15) << ptr->idelTime << endl;

                ptr = ptr->next;
                user++;
            }
        }
    }
};

int main()
{

    CustomerList cl;
    string respond;
    cl.init();
    cl.display();
    cl.detailReport();
    cout << "Do yout want the report in file[Y|y]: ";
    cin >> respond;

    if(tolower(respond[0]) == 'y'){
        cout << endl << "Report file created!" << endl << endl;
        cl.fileReport();
    }

    return 0;
}
