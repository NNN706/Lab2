#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <string>
#include "../include/block_sort.h"

using namespace std;
using sort::BlockSorter;

static void print_vector(const vector<int>& v) {
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) cout << " ";
        cout << v[i];
    }
    cout << '\n';
}

static vector<int> read_from_file(const string& path) {
    vector<int> v;
    ifstream in(path);
    if (!in) return v;
    int x;
    while (in >> x) v.push_back(x);
    return v;
}

static void write_to_file(const string& path, const vector<int>& v) {
    ofstream out(path);
    for (size_t i = 0; i < v.size(); ++i) {
        out << v[i];
        if (i + 1 < v.size()) out << " ";
    }
}

int main() {
    cout << "Block sort console app\n";
    cout << "Choose input method:\n";
    cout << "1) Keyboard\n2) Random generation\n3) Load from file\n";
    int choice = 0;
    cin >> choice;
    vector<int> data;
    if (choice == 1) {
        cout << "Enter numbers separated by spaces, end with EOF (Ctrl+Z/Ctrl+D):\n";
        int x;
        while (cin >> x) data.push_back(x);
    } else if (choice == 2) {
        int n;
        cout << "Enter number of elements: ";
        cin >> n;
        std::mt19937 rng((unsigned)std::random_device{}());
        std::uniform_int_distribution<int> dist(-1000, 1000);
        data.resize(n);
        for (int i = 0; i < n; ++i) data[i] = dist(rng);
    } else if (choice == 3) {
        cout << "Enter file path: ";
        string path;
        cin >> path;
        data = read_from_file(path);
        if (data.empty()) cout << "No data loaded or file empty.\n";
    } else {
        cout << "Invalid choice\n";
        return 1;
    }

    cout << "Original:\n";
    print_vector(data);

    BlockSorter::sort(data);

    cout << "Sorted:\n";
    print_vector(data);

    cout << "Save to file? (y/n): ";
    char c; cin >> c;
    if (c == 'y' || c == 'Y') {
        cout << "Enter output file path: ";
        string outpath; cin >> outpath;
        write_to_file(outpath, data);
        cout << "Saved.\n";
    }

    return 0;
}
