#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <map>
#include <algorithm>
#include <cctype>
#include <tuple>
#include <cassert>

struct Coord {
    long long x, y, z;

    bool operator<(const Coord& c) const {
        return std::tie(x, y, z) < std::tie(c.x, c.y, c.z);
    }
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

using Particles = std::list<Particle>;

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

void collide(Particles& particles) {
    using Collided = std::list<Particles::iterator>;
    std::map<Coord, Collided> seen;

    for (auto it = begin(particles); it != end(particles); ++it) {
        seen[it->p].push_back(it);
    }

    for (const auto& s: seen) {
        if (s.second.size() > 1) {
            for (auto collided: s.second) {
                particles.erase(collided);
            }
        }
    }
}

int closest(const Particles& particles) {
    return distance(begin(particles), min_element(begin(particles), end(particles)));
}

int num_survives(Particles particles) {
    int minIter = 0;
    int min = 0;
    for (int iter = 0; minIter + 1000 > iter; ++iter) {
        for (auto& p: particles) {
            step(p);
        }

        collide(particles);

        int newmin = closest(particles);
        if (newmin != min) {
            minIter = iter;
            min = newmin;
        }
    }
    return particles.size();
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    Particle p0 = read_particle("p=<-6,0,0>, v=< 3,0,0>, a=< 0,0,0>");
    Particle p1 = read_particle("p=<-4,0,0>, v=< 2,0,0>, a=< 0,0,0>");
    Particle p2 = read_particle("p=<-2,0,0>, v=< 1,0,0>, a=< 0,0,0>");
    Particle p3 = read_particle("p=< 3,0,0>, v=<-1,0,0>, a=< 0,0,0>");

    Particles particles {p0, p1, p2, p3};
    ASSERT_EQ(num_survives(particles), 1);
}

void run() {
    Particles particles;
    std::string line;
    while (getline(std::cin, line)) {
        particles.push_back(read_particle(line));
    }
    std::cout << num_survives(particles) << "\n";
}

int main() {
    test();
    run();
}

