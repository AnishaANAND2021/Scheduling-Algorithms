#include <bits/stdc++.h>
using namespace std;

vector<pair<float, int>> SJF(map<int, vector<float>> processes, map<int, float> &waiting_time)
{
    vector<pair<float, int>> gantt_chart;          // {turnAroundTime,processNo.}
    map<float, vector<int>> AT_P;                  // arrivalTime,processes(in case of many process)
    map<pair<float, int>, pair<int, float>> TET_P; //{remainingBurstTime,priority},{process,arrivalTime}
    float cur_time = 0;                            // currentTime
    for (auto it : processes)
    {
        AT_P[it.second[0]].push_back(it.first); // storing all the processes w.r.t arrivalTime
    }
    while (!AT_P.empty() || !TET_P.empty())
    {
        if (TET_P.empty()) // if there is nothing to be executed
        {
            auto it = AT_P.begin();   // first element of ATP
            cur_time = it->first;     // current time is the arrival time of the process
            for (auto i : it->second) // pushing all process of same time
                TET_P[{min(processes[i][2], processes[i][1]), processes[i][4]}] = {i, cur_time};
            AT_P.erase(it->first); // erasing that process
        }
        auto i = *TET_P.begin(); // first element

        TET_P.erase(i.first);

        float bt = i.first.first; // remainingBurstTime
        auto itt = AT_P.begin();  // first element of the arrival time
        waiting_time[i.second.first] += (cur_time - i.second.second);

        if (AT_P.size() && (cur_time + bt) > itt->first)
        {
            float remtime = itt->first - cur_time;   // time remaining
            bt -= remtime;                           // remaining burstTime
            processes[i.second.first][1] -= remtime; // reducing it from TotalExecutingTime
            TET_P[{bt, processes[i.second.first][4]}] = {i.second.first, itt->first};
            for (auto j : itt->second) // storing all processes in the queue
                TET_P[{min(processes[j][2], processes[j][1]), processes[j][4]}] = {j, itt->first};
            AT_P.erase(itt->first);
            cur_time = itt->first;
        }
        else
        {
            cur_time += bt;                     // now it will run till the burst time ends
            processes[i.second.first][1] -= bt; // remaining total execution time
            if (processes[i.second.first][1] > 0)
            {
                AT_P[cur_time + processes[i.second.first][3]].push_back(i.second.first);
            }
            else
            {
                gantt_chart.push_back({cur_time, i.second.first}); // if the process completes
            }
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
    // reading from the file
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
    vector<pair<float, int>> gantt_chart = SJF(processes, waiting_time);
    print(gantt_chart, processes, waiting_time);
    return 0;
}