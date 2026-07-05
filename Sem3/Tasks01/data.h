#ifndef DATA_H
#define DATA_H

#include <stdio.h>

#ifndef M
#error "M must be defined at compile time. Example: g++ -DM=10"
#endif

class data {
private:
    int m;
    double a[M];
    static int p;

public:
    data();
    ~data() = default;

    int get_m() const { return m; }
    int get_M() const { return (m < M ? m : M); }
    const double *get_a() const { return a; }

    static int get_p() { return p; }
    static void set_p(int q) { p = q; }

    void set_m(int new_m) { m = new_m; }

    double &operator[](int index) { return a[index]; }
    const double &operator[](int index) const { return a[index]; }

    data(const data &x) = default;
    data &operator=(const data &x) = default;

    bool operator<(const data &x) const { return m < x.m; }
    bool operator>(const data &x) const { return m > x.m; }
    bool operator<=(const data &x) const { return m <= x.m; }
    bool operator>=(const data &x) const { return m >= x.m; }

    void print(FILE *fp) const;
    int read(FILE *fp);
    void init(int s, int n, int i);
};

#endif // DATA_H
