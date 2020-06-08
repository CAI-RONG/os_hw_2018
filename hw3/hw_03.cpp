
#include <iostream>
#include <fstream>
#include <string.h>
#include <algorithm>
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct rms_process
{
    int pid;
    int arrival_time;
    int CPU_burst;
    int deadline;
    int period;
    bool arrive;
    bool operator < (const rms_process& right) const
    {
        return period< right.period;
    }
};
vector<rms_process> p1;

struct edf_process
{
    int pid;
    int arrival_time;
    int CPU_burst;
    int deadline;
    int period;
    bool arrive;
    bool operator < (const edf_process & right) const
    {
        return deadline < right.deadline;
    }
};
vector<edf_process> p2;

void RMS(int start, int end, int number)
{
    //  Analyzing Priority
    sort(p1.begin(), p1.end());
    int current_process = -1;//initialize
    bool miss = false;
    int process_end = 0;
    
    for (int time = start; time <= end; ++time)
    {
        sort(p1.begin(), p1.end());
        for (int i = 0; i<number; i++) // process had arrived
        {
            if (p1[i].arrival_time == time)
            {
                cout << time << " p" << p1[i].pid << " arrives\n";
                p1[i].arrive = true;
            }
        }
        
        for (int i = 0; i<number; i++)
        {
            if (p1[i].arrive && p1[i].arrival_time <= time)
            {
                if (p1[i].arrival_time == time)
                {
                    if (current_process != -1) {
                        cout << time << " p" << p1[current_process].pid << " ends\n";
                        p1[current_process].arrival_time += p1[current_process].period;
                        process_end = time;
                        
                        p1[current_process].arrive = false;
                    }
                    cout << time << " p" << p1[i].pid << " starts\n"; // "0 p1 starts"
                }
                current_process = i;
                break;
            }
        }
        
        if (current_process >= 0)
        {
            if (time == process_end + p1[current_process].CPU_burst)
            {
                cout << time << " p" << p1[current_process].pid << " ends\n";//3 p1 ends
                process_end = time;
                p1[current_process].arrival_time += p1[current_process].period;
                p1[current_process].deadline = p1[current_process].arrival_time + p1[current_process].period;
                p1[current_process].arrive = false;
                
                for (int i = 0; i < number; i++)
                {
                    if (p1[i].deadline >= time && p1[i].arrive && p1[i].arrival_time <= time)
                    {
                        cout << time << " p" << p1[i].pid << " starts\n"; // "0 p1 starts"
                        current_process = i;
                        break;
                    }
                    
                }
            }
        }
        
        for (int i = 0; i<number; i++)
        {
            if (p1[i].deadline < time)
            {
                cout << time << " p" << i + 1 << " misses the deadline\n";//16 p3 misses the deadline
                miss = true;
            }
        }
        if (miss)
            break;
    }
}

void EDF(int start, int end, int number)
{
    //  Analyzing Priority
   sort(p2.begin(), p2.end());
    
    int current_process = -1;//initialize
    bool miss = false;
    int process_end = 0;

    for (int time = start; time <= end; ++time)
    {
        for (int i = 0; i<number; i++) // process had arrived
        {
            if (p2[i].arrival_time == time)
            {
                cout << time << " p" << p2[i].pid << " arrives\n";
                p2[i].arrive = true;
            }
        }
        
        for (int i = 0; i<number; i++)
        {
            if (p2[i].arrive && p2[i].arrival_time <= time)
            {
                if (p2[i].arrival_time == time)
                {
                    if (current_process != -1) {
                        cout << time << " p" << p2[current_process].pid << " ends\n";
                        p2[current_process].arrival_time += p2[current_process].period;
                        p2[current_process].deadline = p2[current_process].arrival_time + p2[current_process].period;
                        process_end = time;
                        p2[current_process].arrive = false;
                        sort(p2.begin(), p2.end());
                    }
                    cout << time << " p" << p2[i].pid << " starts\n"; // "0 p1 starts"
                }
                current_process = i;
                break;
            }
        }
        
        if (current_process >= 0)
        {
            if (time == process_end + p2[current_process].CPU_burst)
            {
                cout << time << " p" << p2[current_process].pid << " ends\n";//3 p1 ends
                process_end = time;
                p2[current_process].arrival_time += p2[current_process].period;
                p2[current_process].deadline = p2[current_process].arrival_time + p2[current_process].period;
                p2[current_process].arrive = false;
                sort(p2.begin(), p2.end());
                
                for (int i = 0; i < number; i++)
                {
                    if (p2[i].deadline >= time && p2[i].arrive && p2[i].arrival_time <= time)
                    {
                        cout << time << " p" << p2[i].pid << " starts\n"; // "0 p1 starts"
                        current_process = i;
                        break;
                    }
                }
            }
        }
        for (int i = 0; i<number; i++) // process had arrived
        {
            if (p2[i].deadline < time)
            {
                cout << time << " p" << i + 1 << " misses the deadline\n";//16 p3 misses the deadline
                miss = true;
            }
        }
        if (miss)
            break;
        
       // sort(p2.begin(), p2.end());
    }
}

int main(int argc, const char * argv[])
{
    /*--read file--*/
    string line;
    ifstream fin("1062-prog3-data.txt");
    if (!fin) {
        cout << "讀檔失敗" << endl;
    }
    
    int startTime, endTime;
    getline(fin, line); //# Simulation output time: start end
    fin >> startTime;
    fin >> endTime;
    //cout << "startTime: " << startTime <<endl;
    //cout << "endTime: " << endTime<<endl;
    getline(fin, line); // 0
    getline(fin, line); // # Process info: pid, r, C, D, T
    
    
    string schedule;
    cin >> schedule;
    
    if (schedule == "rms")
    {
        rms_process temp;
        while (fin >> temp.pid) {
            fin >> temp.arrival_time >> temp.CPU_burst >> temp.deadline >> temp.period;
            p1.push_back(temp);
        }
        fin.close();
        RMS(startTime, endTime, p1.size());
    }
    else if (schedule == "edf")
    {
        edf_process temp;
        while (fin >> temp.pid) {
            fin >> temp.arrival_time >> temp.CPU_burst >> temp.deadline >> temp.period;
            p2.push_back(temp);
        }
        fin.close();
        EDF(startTime, endTime, p2.size());
    }
    else
    {
        srand(time(NULL));
        int r=rand();
        if(r%2==0)
        {
            rms_process temp;
            while (fin >> temp.pid) {
                fin >> temp.arrival_time >> temp.CPU_burst >> temp.deadline >> temp.period;
                p1.push_back(temp);
            }
            fin.close();
            RMS(startTime, endTime, p1.size());
        }
        else
        {
            edf_process temp;
            while (fin >> temp.pid) {
                fin >> temp.arrival_time >> temp.CPU_burst >> temp.deadline >> temp.period;
                p2.push_back(temp);
            }
            fin.close();
            EDF(startTime, endTime, p2.size());
   
        }
    }
    return 0;
}


