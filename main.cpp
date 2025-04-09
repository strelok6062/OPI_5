#include <iostream>
#include <fstream>
#include <map>
#include <random>
#include <ctime>
#include <cmath>

const int MAX_HEIGHT = 20;

void generate_rand(int min, int max, int count, std::map<int, int>& result) {
    for (int i = 0; i < count; i++) {
        int num = std::rand() % (max - min + 1) + min;
        result[num]++;
    }
}

void generate_uniform(int min, int max, int count, std::map<int, int>& result, std::mt19937& mt) {
    std::uniform_int_distribution<int> dist(min, max);
    for (int i = 0; i < count; i++) {
        int num = dist(mt);
        result[num]++;
    }
}

void generate_normal(int min, int max, int count, std::map<int, int>& result, std::mt19937& mt) {
    float mean = (min + max) / 2.0f;
    float stddev = (max - min) / 6.0f;
    std::normal_distribution<float> dist(mean, stddev);

    for (int i = 0; i < count; i++) {
        int num = std::round(dist(mt));
        if (num >= min && num <= max) {
            result[num]++;
        }
    }
}

void ensure_all_keys(std::map<int, int>& data, int min, int max) {
    for (int i = min; i <= max; i++) {
        if (data.find(i) == data.end()) {
            data[i] = 0;
        }
    }
}

void draw_graph(std::map<int, int>& data, std::ofstream& out, const std::string& title) {
    out << "\n[" << title << "]\n";

    int max_val = 0;
    for (auto pair : data) {
        if (pair.second > max_val) {
            max_val = pair.second;
        }
    }

    std::string lines[MAX_HEIGHT];
    int index = 0;

    for (auto pair : data) {
        int height = (int)((float)pair.second / max_val * MAX_HEIGHT);
        for (int i = 0; i < MAX_HEIGHT; i++) {
            if (MAX_HEIGHT - i <= height) {
                lines[i] += "#";
            }
            else {
                lines[i] += " ";
            }
        }
        index++;
    }

    for (int i = 0; i < MAX_HEIGHT; i++) {
        out << "|" << lines[i] << std::endl;
    }

    out << "+";
    for (int i = 0; i < data.size(); i++) {
        out << "-";
    }
    out << "-> x\n";

    out << " ";
    for (auto pair : data) {
        out << pair.first % 10;
    }
    out << "\n";
}
void print_counts(const std::map<int, int>& data, const std::string& title, std::ostream& out) {
    out << "\n[" << title << " -reckoning ]\n";
    for (auto& pair : data) {
        out << pair.first << ": " << pair.second << "\n";
    }
}
int main() {
    std::srand(std::time(0));
    std::random_device rd;
    std::mt19937 mt(rd());
    int min;
    int max;
    int count;
    std::cout << "min range : ";
    std::cin >> min;
    std::cout << "max range : ";
    std::cin >> max;
    std::cout << "size range : ";
    std::cin >> count;
    std::map<int, int> rand_data;
    std::map<int, int> uniform_data;
    std::map<int, int> normal_data;
    generate_rand(min, max, count, rand_data);
    generate_uniform(min, max, count, uniform_data, mt);
    generate_normal(min, max, count, normal_data, mt);
    ensure_all_keys(rand_data, min, max);
    ensure_all_keys(uniform_data, min, max);
    ensure_all_keys(normal_data, min, max);

    std::ofstream file("graphs.txt");
    draw_graph(rand_data, file, "std::rand");
    draw_graph(uniform_data, file, "std::uniform_int_distribution");
    draw_graph(normal_data, file, "std::normal_distribution");
    print_counts(rand_data, "std::rand", file);
    print_counts(uniform_data, "std::uniform_int_distribution", file);
    print_counts(normal_data, "std::normal_distribution", file);
    file.close();
    print_counts(rand_data, "std::rand", std::cout);
    print_counts(uniform_data, "std::uniform_int_distribution", std::cout);
    print_counts(normal_data, "std::normal_distribution", std::cout);

    std::cout << "save in graphs.txt" << std::endl;

    return 0;
}