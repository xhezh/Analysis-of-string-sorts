#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>
#include <filesystem>


using namespace std;

const string CHARACTERS =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!@#%:;^&*()-.";

class StringGenerator {
public:
    StringGenerator() : rng(random_device{}()) {}
    
    vector<string> generateRandom(int count, int min_len = 10, int max_len = 200) {
        vector<string> result;
        result.reserve(count);
        
        uniform_int_distribution<int> len_dist(min_len, max_len);
        uniform_int_distribution<int> char_dist(0, CHARACTERS.size() - 1);
        
        for (int i = 0; i < count; ++i) {
            int length = len_dist(rng);
            string s;
            s.reserve(length);
            
            for (int j = 0; j < length; ++j) {
                s += CHARACTERS[char_dist(rng)];
            }
            
            result.push_back(move(s));
        }
        
        return result;
    }
    
    vector<string> generateReverseSorted(int count, int min_len = 10, int max_len = 200) {
        auto result = generateRandom(count, min_len, max_len);
        sort(result.begin(), result.end(), greater<string>());
        return result;
    }
    
    vector<string> generateAlmostSorted(int count, int min_len = 10, int max_len = 200, int swaps = 10) {
        auto result = generateRandom(count, min_len, max_len);
        sort(result.begin(), result.end());
        
        uniform_int_distribution<int> index_dist(0, count - 1);
        
        for (int i = 0; i < swaps; ++i) {
            int a = index_dist(rng);
            int b = index_dist(rng);
            swap(result[a], result[b]);
        }
        
        return result;
    }
    
    vector<string> generateWithCommonPrefix(int count, int prefix_len, int min_len = 10, int max_len = 200) {
        vector<string> result;
        result.reserve(count);
        
        uniform_int_distribution<int> len_dist(min_len, max_len);
        uniform_int_distribution<int> char_dist(0, CHARACTERS.size() - 1);
        
        // Генерируем общий префикс
        string common_prefix;
        for (int i = 0; i < prefix_len; ++i) {
            common_prefix += CHARACTERS[char_dist(rng)];
        }
        
        for (int i = 0; i < count; ++i) {
            int length = len_dist(rng);
            string s = common_prefix;
            s.reserve(length);
            
            for (int j = prefix_len; j < length; ++j) {
                s += CHARACTERS[char_dist(rng)];
            }
            
            result.push_back(move(s));
        }
        
        return result;
    }

private:
    mt19937 rng;
};

class StringSortTester {
public:
    struct SortResult {
        long long comparisons;
        double time_ms;
    };
    
    static SortResult testStdQuickSort(vector<string> arr) {
        auto start = chrono::high_resolution_clock::now();
        long long comparisons = 0;
        
        auto compare = [&](const string& a, const string& b) {
            comparisons++;
            return a < b;
        };
        
        sort(arr.begin(), arr.end(), compare);
        
        auto end = chrono::high_resolution_clock::now();
        double time = chrono::duration<double, milli>(end - start).count();
        
        return {comparisons, time};
    }
    
    static SortResult testStdMergeSort(vector<string> arr) {
        auto start = chrono::high_resolution_clock::now();
        long long comparisons = 0;
        
        auto compare = [&](const string& a, const string& b) {
            comparisons++;
            return a < b;
        };
        
        stable_sort(arr.begin(), arr.end(), compare);
        
        auto end = chrono::high_resolution_clock::now();
        double time = chrono::duration<double, milli>(end - start).count();
        
        return {comparisons, time};
    }
    
    static SortResult testStringQuickSort(vector<string> arr) {
        auto start = chrono::high_resolution_clock::now();
        long long comparisons = 0;
        
        function<void(int, int, int)> quickSort = [&](int left, int right, int d) {
            if (left >= right) return;
            
            int lt = left;
            int gt = right;
            int i = left + 1;
            
            char pivot = d < arr[left].size() ? arr[left][d] : -1;
            comparisons++;
            
            while (i <= gt) {
                char current = d < arr[i].size() ? arr[i][d] : -1;
                comparisons += 2;
                
                if (current < pivot) {
                    swap(arr[lt], arr[i]);
                    lt++;
                    i++;
                } else if (current > pivot) {
                    swap(arr[i], arr[gt]);
                    gt--;
                } else {
                    i++;
                }
            }
            
            quickSort(left, lt - 1, d);
            if (pivot != -1) {
                quickSort(lt, gt, d + 1);
            }
            quickSort(gt + 1, right, d);
        };
        
        quickSort(0, arr.size() - 1, 0);
        
        auto end = chrono::high_resolution_clock::now();
        double time = chrono::duration<double, milli>(end - start).count();
        
        return {comparisons, time};
    }
    
    static SortResult testStringMergeSort(vector<string> arr) {
        auto start = chrono::high_resolution_clock::now();
        long long comparisons = 0;
        
        function<void(int, int)> mergeSort = [&](int left, int right) {
            if (left >= right) return;
            
            int mid = left + (right - left) / 2;
            mergeSort(left, mid);
            mergeSort(mid + 1, right);
            
            vector<string> temp;
            temp.reserve(right - left + 1);
            
            int i = left, j = mid + 1;
            while (i <= mid && j <= right) {
                size_t k = 0;
                while (k < arr[i].size() && k < arr[j].size() && arr[i][k] == arr[j][k]) {
                    k++;
                    comparisons++;
                }
                
                if (k < arr[i].size() && k < arr[j].size()) {
                    comparisons++;
                    if (arr[i][k] < arr[j][k]) {
                        temp.push_back(arr[i++]);
                    } else {
                        temp.push_back(arr[j++]);
                    }
                } else {
                    comparisons++;
                    if (arr[i].size() < arr[j].size()) {
                        temp.push_back(arr[i++]);
                    } else {
                        temp.push_back(arr[j++]);
                    }
                }
            }
            
            while (i <= mid) temp.push_back(arr[i++]);
            while (j <= right) temp.push_back(arr[j++]);
            
            for (int k = 0; k < temp.size(); k++) {
                arr[left + k] = temp[k];
            }
        };
        
        mergeSort(0, arr.size() - 1);
        
        auto end = chrono::high_resolution_clock::now();
        double time = chrono::duration<double, milli>(end - start).count();
        
        return {comparisons, time};
    }
    
    static SortResult testMsdRadixSort(vector<string> arr) {
        auto start = chrono::high_resolution_clock::now();
        long long comparisons = 0;
        
        const int ALPHABET_SIZE = 256;
        
        function<void(int, int, int)> radixSort = [&](int low, int high, int depth) {
            if (low >= high) return;
            
            vector<vector<string>> buckets(ALPHABET_SIZE + 1);
            
            for (int i = low; i <= high; i++) {
                if (depth < arr[i].size()) {
                    char c = arr[i][depth];
                    buckets[static_cast<unsigned char>(c)].push_back(arr[i]);
                } else {
                    buckets[ALPHABET_SIZE].push_back(arr[i]);
                }
            }
            
            int index = low;
            for (auto& bucket : buckets) {
                for (string& s : bucket) {
                    arr[index++] = s;
                }
            }
            
            int start = low;
            for (int i = 0; i < ALPHABET_SIZE; i++) {
                int bucket_size = buckets[i].size();
                if (bucket_size > 1) {
                    int end = start + bucket_size - 1;
                    radixSort(start, end, depth + 1);
                }
                start += bucket_size;
            }
        };
        
        radixSort(0, arr.size() - 1, 0);
        
        auto end = chrono::high_resolution_clock::now();
        double time = chrono::duration<double, milli>(end - start).count();
        
        return {comparisons, time};
    }
    
    static SortResult testHybridRadixQuickSort(vector<string> arr) {
        auto start = chrono::high_resolution_clock::now();
        long long comparisons = 0;
        
        const int ALPHABET_SIZE = 256;
        const int SWITCH_THRESHOLD = ALPHABET_SIZE;
        
        function<void(int, int, int)> quickSort = [&](int left, int right, int d) {
            if (left >= right) return;
            
            int lt = left;
            int gt = right;
            int i = left + 1;
            
            char pivot = d < arr[left].size() ? arr[left][d] : -1;
            comparisons++;
            
            while (i <= gt) {
                char current = d < arr[i].size() ? arr[i][d] : -1;
                comparisons += 2;
                
                if (current < pivot) {
                    swap(arr[lt], arr[i]);
                    lt++;
                    i++;
                } else if (current > pivot) {
                    swap(arr[i], arr[gt]);
                    gt--;
                } else {
                    i++;
                }
            }
            
            quickSort(left, lt - 1, d);
            if (pivot != -1) {
                quickSort(lt, gt, d + 1);
            }
            quickSort(gt + 1, right, d);
        };
        
        function<void(int, int, int)> radixSort = [&](int low, int high, int depth) {
            if (low >= high) return;
            
            if (high - low + 1 < SWITCH_THRESHOLD) {
                quickSort(low, high, depth);
                return;
            }
            
            vector<vector<string>> buckets(ALPHABET_SIZE + 1);
            
            for (int i = low; i <= high; i++) {
                if (depth < arr[i].size()) {
                    char c = arr[i][depth];
                    buckets[static_cast<unsigned char>(c)].push_back(arr[i]);
                } else {
                    buckets[ALPHABET_SIZE].push_back(arr[i]);
                }
            }
            
            int index = low;
            for (auto& bucket : buckets) {
                for (string& s : bucket) {
                    arr[index++] = s;
                }
            }
            
            int start = low;
            for (int i = 0; i < ALPHABET_SIZE; i++) {
                int bucket_size = buckets[i].size();
                if (bucket_size > 1) {
                    int end = start + bucket_size - 1;
                    radixSort(start, end, depth + 1);
                }
                start += bucket_size;
            }
        };
        
        radixSort(0, arr.size() - 1, 0);
        
        auto end = chrono::high_resolution_clock::now();
        double time = chrono::duration<double, milli>(end - start).count();
        
        return {comparisons, time};
    }
};

void saveToCSV(const string& filename, const vector<vector<string>>& data) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }
    
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i != row.size() - 1) {
                file << ",";
            }
        }
        file << "\n";
    }
    
    file.close();
    cout << "Data saved to " << filename << endl;
}

void runTestsAndSaveResults(const string& testName, const vector<string>& data) {
    vector<vector<string>> results;
    results.push_back({"Size", "StdQuickSort Time", "StdQuickSort Comparisons",
                      "StdMergeSort Time", "StdMergeSort Comparisons",
                      "StringQuickSort Time", "StringQuickSort Comparisons",
                      "StringMergeSort Time", "StringMergeSort Comparisons",
                      "MSDRadixSort Time", "MSDRadixSort Comparisons",
                      "HybridRadixQuick Time", "HybridRadixQuick Comparisons"});
    
    const int MAX_SIZE = 3000;
    const int STEP = 100;
    
    for (int size = 100; size <= min(MAX_SIZE, (int)data.size()); size += STEP) {
        vector<string> testData(data.begin(), data.begin() + size);
        
        auto stdQuick = StringSortTester::testStdQuickSort(testData);
        auto stdMerge = StringSortTester::testStdMergeSort(testData);
        auto strQuick = StringSortTester::testStringQuickSort(testData);
        auto strMerge = StringSortTester::testStringMergeSort(testData);
        auto msdRadix = StringSortTester::testMsdRadixSort(testData);
        auto hybrid = StringSortTester::testHybridRadixQuickSort(testData);
        
        vector<string> row;
        row.push_back(to_string(size));
        row.push_back(to_string(stdQuick.time_ms));
        row.push_back(to_string(stdQuick.comparisons));
        row.push_back(to_string(stdMerge.time_ms));
        row.push_back(to_string(stdMerge.comparisons));
        row.push_back(to_string(strQuick.time_ms));
        row.push_back(to_string(strQuick.comparisons));
        row.push_back(to_string(strMerge.time_ms));
        row.push_back(to_string(strMerge.comparisons));
        row.push_back(to_string(msdRadix.time_ms));
        row.push_back(to_string(msdRadix.comparisons));
        row.push_back(to_string(hybrid.time_ms));
        row.push_back(to_string(hybrid.comparisons));
        
        results.push_back(row);
    }
    
    string filename = "sort_results_" + testName + ".csv";
    saveToCSV(filename, results);
}

int main() {
    StringGenerator generator;
    const int MAX_SIZE = 3000;
    
    // Генерация тестовых данных
    auto randomStrings = generator.generateRandom(MAX_SIZE);
    auto reverseSorted = generator.generateReverseSorted(MAX_SIZE);
    auto almostSorted = generator.generateAlmostSorted(MAX_SIZE);
    auto commonPrefix = generator.generateWithCommonPrefix(MAX_SIZE, 5);
    
    // Запуск тестов и сохранение результатов
    runTestsAndSaveResults("random", randomStrings);
    runTestsAndSaveResults("reverse_sorted", reverseSorted);
    runTestsAndSaveResults("almost_sorted", almostSorted);
    runTestsAndSaveResults("common_prefix", commonPrefix);
    
    cout << "All tests completed. CSV files have been generated." << endl;
    cout << "Current working directory: " << filesystem::current_path() << endl;

    return 0;
}
