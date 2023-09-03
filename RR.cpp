#include <bits/stdc++.h>
using namespace std;

float quant; // time slice

vector<pair<float, int>> RR(map<int, vector<float>> processes, map<int, float> &waiting_time)
{
    vector<pair<float, int>> gantt_chart; // {turnAroundTime,processNo.}
    float cur_time = 0;                   // current time
    map<pair<float, int>, int> pq;        //{ArrivalTime,priority},processNo.
    float execute;                        // time at which the process is in runnable stage

    for (auto it : processes)
    {
        processes[it.first].push_back(min(it.second[1], it.second[2])); // pushing the remaining burstTime
        pq[{it.second[0], it.second[4]}] = it.first;                    // pushing in queue
    }

    while (!pq.empty())
    {
        auto it = pq.begin();
        execute = (it->first).first;       // arrivalTime
        int process = it->second;          // processNo.
        auto i = processes[process];       // information of the process
        pq.erase(pq.begin());              // removing the first one
        cur_time = max(execute, cur_time); // time in which process will execute
        waiting_time[process] += cur_time - execute;
        if (i[5] <= quant) // if remainingBurstTime < quant ,(the process executes completely)
        {
            cur_time += i[5]; // it will run till it's remaining burst time
            processes[process][1] -= i[5];
            if (processes[process][1])
            {
                pq[{cur_time + i[3], i[4]}] = process;
                processes[process][5] = min(i[2], processes[process][1]);
            }
            else
            {
                gantt_chart.push_back({cur_time, process});
            }
        }
        else // if it is less than quant , it will execute till quant
        {
            cur_time += quant;              // the process will run till quant
            processes[process][1] -= quant; // removing that time from TET
            pq[{cur_time, i[4]}] = process;
            processes[process][5] -= quant;
        }
    }

    return gantt_chart;
}

void print(vector<pair<float, int>> gantt_chart, map<int, vector<float>> processes, map<int, float> &waiting_time)
{
    float avg = 0, standard_deviation = 0;
    cout << "processe name "
         << " turn around time "
         << " total wait time" << endl;
    // printing the turn around time and total wait time
    for (auto it : gantt_chart)
    {
        int x = it.second;
        cout << "   P" << it.second << "             " << it.first - processes[x][0] << "               " << waiting_time[it.second] << endl;
        avg += (it.first - processes[x][0]); // cout << it.first << ' ' << it.second << endl;
    }
    avg /= gantt_chart.size();
    cout << endl
         << "The average time for processes to complete = " << avg;
    // calculating the standard deviation
    for (auto it : gantt_chart)
    {
        int x = it.second;
        standard_deviation += ((it.first - processes[x][0] - avg) * (it.first - processes[x][0] - avg));
    }
    standard_deviation /= gantt_chart.size();
    cout << "\nThe standard deviation for the average process completion time = " << sqrt(standard_deviation) << endl
         << endl;
}

int main()
{
    ifstream fin;
    string line;
    cout << "Print the file name to be read: ";
    cin >> line;
    fin.open(line);
    map<int, vector<float>> processes;
    float i = 0;
    // reading from input file
    while (fin.peek() != EOF)
    {
        string process;
        float arrival_time, TET, WT, priority, BT;
        fin >> process >> arrival_time >> TET >> BT >> WT >> priority;
        if (process[0] != 'p' && process[0] != 'P')
            break;
        i += (arrival_time);
        vector<float> v = {i, TET, BT, WT, priority};
        processes[process[1] - '0'] = v;
    }
    fin.close();
    cout << "Enter time-slice: ";
    cin >> quant;
    cout << endl;
    map<int, float> waiting_time;
    vector<pair<float, int>> gantt_chart = RR(processes, waiting_time);
    print(gantt_chart, processes, waiting_time);
    return 0;
}