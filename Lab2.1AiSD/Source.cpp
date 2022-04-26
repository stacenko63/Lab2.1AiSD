#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
using namespace std;

//25 = 2 * 3^2 + 2 * 3^1 + 1 * 3^0  

struct stats {
    size_t comparison_count = 0, copy_count = 0;
};

void input_correctly_number(int& aa)
{
    while (!(cin >> aa) || cin.peek() != '\n')
    {
        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
        cout << "������� �������� ��������. ��������� �������: ";
    }
}

template<typename T = int, typename TCompare = less<T>> 
stats qs(vector<T>& s_arr, int first, int last) 
{
    stats result; 
    int i = first, j = last, x = s_arr[(first + last) / 2];
    do {
        result.comparison_count++;
        while (TCompare()(s_arr[i], x)) {
            i++;
            result.comparison_count++;
        }
        result.comparison_count += 2;
        while (TCompare()(x, s_arr[j])) {
            j--;
            result.comparison_count++;
        }
        result.comparison_count += 3;
        if (i <= j) {
            result.comparison_count++; 
            if (TCompare()(s_arr[j], s_arr[i])) { 
                swap(s_arr[i], s_arr[j]); 
                result.copy_count += 3;
            }
            i++;
            j--;
        }
        result.comparison_count++;
    } while (i <= j); 

    result.comparison_count++;
    if (i < last) {
       auto result_intermediate = qs<T, TCompare>(s_arr, i, last); 
       result.comparison_count += result_intermediate.comparison_count;
       result.copy_count += result_intermediate.copy_count;
    }
    result.comparison_count++;
    if (first < j) {
        auto result_intermediate = qs<T, TCompare>(s_arr, first, j); 
        result.comparison_count += result_intermediate.comparison_count; 
        result.copy_count += result_intermediate.copy_count;
    }
    return result;
}

template<typename T, typename TCompare = less<T>>
stats Merg(vector<T>& arr, int begin, int end)
{
    stats result;
    int i = begin, t = 0, mid = begin + (end - begin) / 2, j = mid + 1;
    vector<T> d(arr.size());
    while (i <= mid && j <= end) {
        result.comparison_count += 3;
        if (TCompare()(arr[i], arr[j]) || equal_to<T>()(arr[i],arr[j])) {
            d[t] = arr[i];
            result.copy_count++;
            i++;
        }
        else {
            d[t] = arr[j];
            result.copy_count++;
            j++;
        }
        t++;
    }
    result.comparison_count += 3;


    while (i <= mid) {
        d[t] = arr[i];
        result.copy_count++;
        i++;
        t++;
    }
    result.comparison_count += 3;
    while (j <= end) {
        d[t] = arr[j];
        result.copy_count++;
        j++;
        t++;
    }
    for (i = 0; i < t; i++) {
        arr[begin + i] = d[i];
        result.copy_count++;
    }
    return result; 
}

template<typename T, typename TCompare = less<T>>
stats MergSort(vector<T>& arr, int first, int last)
{
    stats result;
    result.comparison_count++;
    if (first < last) {
        result.comparison_count++;
        if (last - first == 1) {
            result.comparison_count++;
            if (TCompare()(arr[last], arr[first])) {
                swap(arr[first], arr[last]);
                result.copy_count += 3;
            }
        }
        else {
            auto result_intermediate = MergSort(arr, first, first + (last - first) / 2);
            result.comparison_count += result_intermediate.comparison_count;
            result.copy_count += result_intermediate.copy_count;

            result_intermediate = MergSort(arr, first + (last - first) / 2 + 1, last);
            result.comparison_count += result_intermediate.comparison_count;
            result.copy_count += result_intermediate.copy_count;

            result_intermediate = Merg(arr, first, last);
            result.comparison_count += result_intermediate.comparison_count;
            result.copy_count += result_intermediate.copy_count;
        }
    }
    return result;
}

template<typename T, typename TCompare = less<T>>
stats selection_sort(vector<T>& data) { 
    stats result;
    for (int i = 0; i < data.size() - 1; i++) {
        int min_index = i; //�����������
        for (int j = i + 1; j < data.size(); j++) {
            result.comparison_count++; 
            if (TCompare()(data[j], data[min_index])) { //���������
                min_index = j; //�����������
                result.copy_count++;
            }
        }
        swap(data[i], data[min_index]); 
        result.copy_count += 4; 
    }
    return result;
}

void Limitations(int min, int max, int& value)
{
    while (value < min || value > max)
    {
        cout << "��������� ����� �� ������������� ����������. ��������� ����: ";
        input_correctly_number(value);
    }
}

template<typename T> 
void print(const vector<T>& data) {
    for (auto el : data) cout << el << " ";
    cout << "\n\n";
}

template<typename T>
vector<T> create_arr(int size, int option) {
    vector<T> data;
    for (int i = 0; i < size; i++) {
        if (option == 1) data.push_back(1 + rand() % size);
        else if (option == 2) data.push_back(i);
        else data.push_back(size - i);
    }
    if (size < 10) {
        cout << "�������� ������: ";
        print(data);
    }
    return data; 
}

template<typename T, typename TCompare = less<T>>
pair<stats,vector<T>> sort_process(int size, int option, stats (sort_function)(vector<T>&)) {
    auto data = create_arr<T>(size, option);     
    return pair<stats, vector<T>>(sort_function(data), data);     
}

template<typename T, typename TCompare = less<T>>
pair<stats, vector<T>> sort_process(int size, int option, stats(sort_function)(vector<T>&, int, int)) {
    auto data = create_arr<T>(size, option);
    return pair<stats, vector<T>>(sort_function(data, 0, size-1), data); 
}

template<typename T, typename TCompare = less<T>>
pair<double, double> average_values(int size, int option, stats(sort_function)(vector<T>&)) {
    double sum_comparison_count = 0, sum_copy_count = 0;
    for (int i = 0; i < 100; i++) {
        auto result = sort_process<T, TCompare>(size, 1, sort_function).first;
        sum_comparison_count += result.comparison_count;
        sum_copy_count += result.copy_count;
    }
    return pair<double, double>(sum_comparison_count / 100, sum_copy_count / 100);
}

template<typename T, typename TCompare = less<T>>
pair<double, double> average_values(int size, int option, stats(sort_function)(vector<T>&, int, int)) {
    double sum_comparison_count = 0, sum_copy_count = 0;
    for (int i = 0; i < 100; i++) {
        auto result = sort_process<T, TCompare>(size, 1, sort_function).first;
        sum_comparison_count += result.comparison_count;
        sum_copy_count += result.copy_count;
    }
    return pair<double, double>(sum_comparison_count / 100, sum_copy_count / 100);
}

template<typename T, typename TCompare = less<T>>
void start_work(int size, stats(sort_function)(vector<T>&)) {
    cout << "������ �� " << to_string(size) << " ��������: \n"; 
    auto average_result = average_values<T, TCompare>(size, 1, sort_function);
    cout << "������� ���������� ���������: " << average_result.first << "\n������� ���������� �����������: " <<
        average_result.second << "\n";
    cout << "��� ��������� ���������������� �������: \n"; 
    auto result = sort_process<T, TCompare>(size, 2, sort_function);
    cout << "���������� ���������: " << result.first.comparison_count << "\n���������� ����������� ��������: "
        << result.first.copy_count << "\n"; 
    cout << "��� ������� ���������������� �������: \n";
    result = sort_process<T, TCompare>(size, 3, sort_function);
    cout << "���������� ���������: " << result.first.comparison_count << "\n���������� ����������� ��������: "
        << result.first.copy_count << "\n";
}

template<typename T, typename TCompare = less<T>>
void start_work(int size, stats(sort_function)(vector<T>&, int, int)) {
    cout << "������ �� " << to_string(size) << " ��������: \n";
    auto average_result = average_values<T, TCompare>(size, 1, sort_function);  
    cout << "������� ���������� ���������: " << average_result.first << "\n������� ���������� �����������: " <<
        average_result.second << "\n";
    cout << "��� ��������� ���������������� �������: \n";
    auto result = sort_process<T,TCompare>(size, 2, sort_function);
    cout << "���������� ���������: " << result.first.comparison_count << "\n���������� ����������� ��������: "
        << result.first.copy_count << "\n";
    cout << "��� ������� ���������������� �������: \n";
    result = sort_process<T, TCompare>(size, 3, sort_function);
    cout << "���������� ���������: " << result.first.comparison_count << "\n���������� ����������� ��������: "
        << result.first.copy_count << "\n";
}

vector<int> create_vector_by_user() {
    int size = 0;
    cout << "������� ���������� ��������� � �������: ";
    input_correctly_number(size);
    while (size < 0) {
        cout << "������ ������� �� ����� ������������� ������. ��������� ����: ";
        input_correctly_number(size);
    }
    vector<int> v(size);
    for (int i = 0; i < size; i++) { 
        cout << "������� " << i + 1 << " ������� �������: "; 
        input_correctly_number(v[i]);
    }
    return v;
}

int main() {
    setlocale(LC_ALL, "RUS");
    int menu = 1;
    do
    {
        system("cls");
        stats result;
        vector<int> v = create_vector_by_user(); 
        cout << "�������� ����������: \n";
        cout << "1 - ���������� �������\n";
        cout << "2 - ������� ����������\n";
        cout << "3 - ���������� ������������ ����������� ��������\n";
        cout << "0 - �����";
        cout << "������� ��� �����: ";
        input_correctly_number(menu);
        Limitations(0, 3, menu); 
        auto begin = std::chrono::steady_clock::now();
        result = menu == 1 ? selection_sort(v) : menu == 2 ? qs(v, 0, v.size() - 1) : MergSort(v, 0, v.size() - 1); 
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        cout << "��������������� ������������������: \n";
        print(v);
        cout << "\n���������� �������: " << elapsed_ms.count() << "\n";
        cout << "1 - ��������� ��������� �����\n";
        cout << "0 - ����� �� ���������\n";
        cout << "������� ��� �����: ";
        input_correctly_number(menu);
        Limitations(0, 1, menu);
    } while (menu != 0);

    
    {
        
        /*auto result = sort_process<int, less<int>>(9, 1, selection_sort); 
        cout << "���������� ������� (�������� ������������): " << "\n������ �� 9 ��������\n" ;
        print(result.second); 
        cout << "���������� ���������: " << result.first.comparison_count << "\n���������� ����������� ��������: "
            << result.first.copy_count << "\n";
        for (int i = 1000; i < 11000; i += 1000) start_work<int, less<int>>(i, selection_sort);  */  
    }

    {
        //cout << "####################################################################\n\n";
        /*auto result = sort_process<int, less<int>>(9, 1, qs);
        cout << "������� ���������� (�������� ������������): " << "\n������ �� 9 ��������\n";
        print(result.second); 
        cout << "���������� ���������: " << result.first.comparison_count << "\n���������� ����������� ��������: "
            << result.first.copy_count << "\n";
        for (int i = 1000; i < 11000; i += 1000) start_work<int, less<int>>(i, qs);  */
    }

    { 
        //cout << "####################################################################\n\n";
        /*auto result = sort_process<int, less<int>>(9, 1, MergSort); 
        cout << "���������� ������������ ����������� �������� (�������� ������������) " << "\n������ �� 9 ��������\n";
        print(result.second);
        cout << "���������� ���������: " << result.first.comparison_count << "\n���������� ����������� ��������: "
            << result.first.copy_count << "\n";
        for (int i = 1000; i < 11000; i += 1000) start_work<int, less<int>>(i, MergSort);  */
    }



}