#include "functions.h"

int task01(student *a, int n) {
    if (n <= 1)
        return n;
    int l = 0;
    int replace_flag = 0;
    for (int i = 1; i < n; i++) {
        if (replace_flag == 1) {
            if (a[i] < a[i - 1]) {
                replace_flag = 1;
            } else {
                l++;
                if (l != i)
                    a[l] = (student &&) a[i];
                replace_flag = 0;
            }
        } else {
            if (a[i] < a[l]) {
                replace_flag = 1;
            } else {
                l++;
                if (l != i)
                    a[l] = (student &&) a[i];
            }
        }
    }
    return l + 1;
}

int task02(student *a, int n) {
    if (n <= 2)
        return n;
    int l = 1;
    int replace_flag = 0;
    for (int i = 2; i < n; i++) {
        if (replace_flag == 1) {
            if (a[i] > a[i - 1] && a[i] > a[i - 2]) {
                replace_flag = 1;
            } else {
                l++;
                if (l != i)
                    a[l] = (student &&) a[i];
                replace_flag = 0;
            }
        } else {
            if (a[i] > a[l] && a[i] > a[i - 2]) {
                replace_flag = 1;
            } else {
                l++;
                if (l != i)
                    a[l] = (student &&) a[i];
            }
        }
    }
    return l + 1;
}

int task03(student *a, int n) {
    if (n <= 2)
        return n;
    int l = 0;
    int replace_flag = 0;
    for (int i = 1; i < n - 1; i++) {
        if (replace_flag == 1) {
            if (a[i] < a[i - 1] && a[i] < a[i + 1]) {
                replace_flag = 1;
            } else {
                l++;
                if (l != i)
                    a[l] = (student &&) a[i];
                replace_flag = 0;
            }
        } else {
            if (a[i] < a[l] && a[i] < a[i + 1]) {
                replace_flag = 1;
            } else {
                l++;
                if (l != i)
                    a[l] = (student &&) a[i];
            }
        }
    }
    l++;
    if (l != n - 1)
        a[l] = (student &&) a[n - 1];
    return l + 1;
}

int task04(student *a, int n) {
    if (n <= 4)
        return n;
    int l = 0;
    a[l] = (student &&) a[0];
    l++;
    a[l] = (student &&) a[1];
    l++;
    int k = l - 1;
    int flag = 0;
    student el;
    for (int i = 2; i < n - 2; i++) {
        if (flag == 0) {
            if (a[i] > a[k] && a[i] > a[k - 1] && a[i] > a[i + 1] && a[i] > a[i + 2]) {
                flag = 1;
            } else {
                a[l] = (student &&) a[i];
                l++;
                k = l - 1;
            }
        } else if (flag == 1) {
            if (a[i] > a[i - 1] && a[i] > a[k] && a[i] > a[i + 1] && a[i] > a[i + 2]) {
                flag = 2;
            } else {
                int pos = l;
                el = (student &&) a[i - 1];
                a[pos] = (student &&) a[i];
                l = pos + 1;
                k = l - 1;
                flag = 3;
            }
        } else if (flag == 2) {
            if (!(a[i] > a[i - 1] && a[i] > a[i - 2] && a[i] > a[i + 1] && a[i] > a[i + 2])) {
                a[l] = (student &&) a[i];
                l++;
                k = l - 1;
                flag = 0;
            }
        } else {
            if (a[i] > el && a[i] > a[k] && a[i] > a[i + 1] && a[i] > a[i + 2]) {
                flag = 1;
            } else {
                a[l] = (student &&) a[i];
                l++;
                k = l - 1;
                flag = 0;
            }
        }
    }
    a[l] = (student &&) a[n - 2];
    l++;
    a[l] = (student &&) a[n - 1];
    l++;
    return l;
}


int task05(student *a, int n) {
    if (n <= 1)
        return n;
    int l = 0;
    int i = 0;
    while (i < n) {
        if (i + 1 < n && a[i] > a[i + 1]) {
            int j = i + 1;
            while (j + 1 < n && a[j] > a[j + 1])
                j++;
            i = j + 1;
        } else {
            a[l] = (student &&) a[i];
            l++;
            i++;
        }
    }
    return l;
}

int task06(student *a, int n, const student &x) {
    int l = 0;
    for (int i = 0; i < n; i++) {
        if (!(a[i] < x)) {
            a[l] = (student &&) a[i];
            l++;
        }
    }
    return l;
}

int task07(student *a, int n, const student &x) {
    if (n == 0)
        return 0;
    int l = 0;
    int i = 0;
    while (i < n) {
        if (a[i] < x) {
            a[l] = (student &&) a[i];
            l++;
            i++;
            while (i < n && a[i] < x)
                i++;
        } else {
            a[l] = (student &&) a[i];
            l++;
            i++;
        }
    }
    return l;
}

int task08(student *a, int n, const student &x) {
    if (n == 0)
        return 0;
    int l = 0;
    int i = 0;
    while (i < n) {
        if (a[i] > x) {
            int start = i;
            int j = i;
            while (j + 1 < n && a[j + 1] > x)
                j++;
            if (start == j) {
                a[l] = (student &&) a[start];
                l++;
            } else {
                a[l] = (student &&) a[start];
                l++;
                a[l] = (student &&) a[j];
                l++;
            }
            i = j + 1;
        } else {
            a[l] = (student &&) a[i];
            l++;
            i++;
        }
    }
    return l;
}

int task09(student *a, int n, const student &x) {
    if (n < 2)
        return n;
    int l = -1;
    int flag = 0;
    if (!(a[1] < x))
        l = 0;
    for (int i = 1; i < n - 1; i++) {
        if (flag == 1) {
            if (a[l] < x || a[i + 1] < x) {
                flag = 0;
            } else {
                l++;
                if (l != i)
                    a[l] = (student &&) a[i];
            }
        } else if (!(a[i - 1] < x || a[i + 1] < x)) {
            l++;
            if (l != i)
                a[l] = (student &&) a[i];
            flag = 1;
        }
    }
    if (flag == 1) {
        if (!(a[l] < x)) {
            l++;
            if (l != n - 1)
                a[l] = (student &&) a[n - 1];
        }
    } else if (!(a[n - 2] < x)) {
        l++;
        if (l != n - 1)
            a[l] = (student &&) a[n - 1];
    }
    return l + 1;
}

int task10(student *a, int n, const student &x) {
    if (n <= 1)
        return n;
    int l = -1;
    int run_len = 0;
    int run_start = -1;
    if (a[0] < x) {
        run_len = 1;
        run_start = 0;
    } else {
        l = 0;
    }
    for (int i = 1; i < n; ++i) {
        if (!(a[i] < x)) {
            if (run_len == 1) {
                ++l;
                if (l != run_start)
                    a[l] = (student &&) a[run_start];
            }
            ++l;
            if (l != i)
                a[l] = (student &&) a[i];
            run_len = 0;
            run_start = -1;
        } else {
            if (run_len >= 1 && a[i] > a[i - 1]) {
                ++run_len;
            } else if (run_len == 1) {
                ++l;
                if (l != run_start)
                    a[l] = (student &&) a[run_start];
                run_start = i;
                run_len = 1;
            } else {
                run_start = i;
                run_len = 1;
            }
        }
    }
    if (run_len == 1) {
        ++l;
        if (l != n - 1)
            a[l] = (student &&) a[n - 1];
    }
    return l + 1;
}
