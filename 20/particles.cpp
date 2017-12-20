#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <tuple>
#include <cassert>

struct Coord {
    long long x, y, z;
};

long long distance(const Coord& c) {
    return abs(c.x) + abs(c.y) + abs(c.z);
}

struct Particle {
    Coord p, v, a;

    bool operator<(const Particle& o) const {
        return distance(p) < distance(o.p);
    }
};

Particle read_particle(std::string line) {
    line.erase(remove_if(begin(line), end(line), [](char c) {
            return !isdigit(c) && c != ',' && c != '-';
        }), end(line));
    std::transform(begin(line), end(line), begin(line), [](char c) {
            return c == ',' ? ' ' : c;
        });
    std::istringstream iss(line);
    Particle p;
    iss >> p.p.x >> p.p.y >> p.p.z
        >> p.v.x >> p.v.y >> p.v.z
        >> p.a.x >> p.a.y >> p.a.z;
    return p;
}

void step(Particle& p) {
    p.v.x += p.a.x;
    p.v.y += p.a.y;
    p.v.z += p.a.z;

    p.p.x += p.v.x;
    p.p.y += p.v.y;
    p.p.z += p.v.z;
}

int closest(const std::vector<Particle>& particles) {
    return distance(begin(particles), min_element(begin(particles), end(particles)));
}

int get_closest(std::vector<Particle> particles) {
    int minIter = 0;
    int min = 0;
    for (int iter = 0; minIter + 1000 > iter; ++iter) {
        for (auto& p: particles) {
            step(p);
        }

        int newmin = closest(particles);
        if (newmin != min) {
            minIter = iter;
            min = newmin;
        }
    }
    return min;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    Particle p0 = read_particle("p=< 3,0,0>, v=< 2,0,0>, a=<-1,0,0>");
    Particle p1 = read_particle("p=< 4,0,0>, v=< 0,0,0>, a=<-2,0,0>");
    ASSERT_EQ(p0.p.x, 3);
    ASSERT_EQ(p0.v.x, 2);
    ASSERT_EQ(p0.a.x, -1);

    ASSERT_EQ(distance(p0.p), 3);
    ASSERT_EQ(distance(p1.p), 4);

    step(p0); step(p1);
    ASSERT_EQ(distance(p0.p), 4);
    ASSERT_EQ(distance(p1.p), 2);

    std::vector<Particle> particles {p0, p1};
    ASSERT_EQ(get_closest(particles), 0);
    std::vector<Particle> particles2 {p1, p0};
    ASSERT_EQ(get_closest(particles2), 1);
}

void run() {
    std::vector<Particle> particles;
    std::string line;
    while (getline(std::cin, line)) {
        particles.push_back(read_particle(line));
    }
    std::cout << get_closest(particles) << "\n";
}

int main() {
    test();
    run();
}

