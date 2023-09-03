#include <bits/stdc++.h>
using namespace std;

vector<pair<float, int>> FCFS(map<int, vector<float>> processes, map<int, float> &waiting_time)
{
    vector<pair<float, int>> gantt_chart; // {turnAroundTime,processNo.}
    float cur_time = 0;
    map<pair<float, int>, int> pq; //{arrivalTime,priority},ProcessNo.

    float execute; // time at which the process is in runnable stage

    for (auto it : processes)
    {
        pq[{it.second[0], it.second[4]}] = it.first; // pushing in the priority queue
        waiting_time[it.first] = 0;                  // initialising waiting time to zero
    }

    while (!pq.empty())
    {
        auto it = pq.begin();
        execute = (it->first).first;       // it stores arrival time of the process
        int process = it->second;          // it stores process no
        auto i = processes[process];       // information of the process
        pq.erase(pq.begin());              // removing the first process from the queue
        cur_time = max(execute, cur_time); // the time in which process will execute
        waiting_time[process] += (cur_time - execute);
        if (i[1] > i[2]) // total execution time>burst time
        {
            processes[process][1] -= i[2]; // it will execute till it's burst time
            pq[{cur_time + i[2] + i[3], i[4]}] = process;
            cur_time += i[2]; // updating current time
        }
        else // if the process completes
        {
            processes[process][1] = 0;
            cur_time += i[1];
            gantt_chart.push_back({cur_time, process});
        }
    }

    return gantt_chart;
}

void print(vector<pair<float, int>> gantt_chart, map<int, vector<float>> processes, map<int, float> &waiting_time)
{
    float avg = 0, standard_deviation = 0;
    cout << "\nprocesse name "
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
    map<int, float> waiting_time;
    vector<pair<float, int>> gantt_chart = FCFS(processes, waiting_time);
    print(gantt_chart, processes, waiting_time);
    return 0;
}