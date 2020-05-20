#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <mutex>

using std::cout;
using std::endl;
using std::vector;
using std::thread;

std::mutex MyMutex;

void FindMax(vector<uint> &Arr, size_t Count, uint &Max) {
    Max = 0;
    for (size_t i = 0; i < Count; ++i) {
        if (Arr[i] > Max) {
            Max = Arr[i];
        }
    }
    std::lock_guard<std::mutex> lg(MyMutex);
    cout << "ID thread = " << std::this_thread::get_id() << "\t\tMax = " << Max << "\n";
}

int main() {
    size_t n = 99999;
    uint MaxCount = 10000000;
    vector<uint> arr(n);
    srand(static_cast<uint>(time(NULL)));
    for (int i = 0; i < n; ++i) {
        uint first = static_cast<uint>(std::rand());
        uint second = static_cast<uint>(std::rand());
        arr[i] = first * second % (MaxCount + 1);
    }

    cout << "Size of vector = " << n << ".\n";
    cout << "Numbers in vector from 0 to " << MaxCount << ".\n";

    std::thread *myThreads[3];
    vector<vector<uint>> Parts(3);
    vector<uint> Result(3);
    int j = 0;
    for (auto begin = arr.begin(), end = begin + n / 3; begin != arr.end(); begin = end, end += n / 3) {
        vector<uint> ArrPart(n / 3);
        std::copy(begin, end, ArrPart.begin());
        Parts[j] = ArrPart;
        myThreads[j] = new thread(FindMax, std::ref(Parts[j]), n / 3, std::ref(Result[j]));
        ++j;
    }

    for (size_t i = 0; i < 3; ++i) {
        myThreads[i]->join();
        delete myThreads[i];
    }

    for (int i = 0; i < 3; ++i) {
        cout << "result[" << i << "] = " << Result[i] << endl;
    }
    uint Max = 0;
    thread t(FindMax, std::ref(Result), 3, std::ref(Max));
    t.join();
    cout << "Max = " << Max << "\n";
    return 0;
}
