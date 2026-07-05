#include "functions.h"

int count_decreases(data *a, int n) {
    if (n <= 1)
        return 0;
    int cnt = 0;
    for (int i = 1; i < n; ++i) {
        if (a[i].get_m() < a[i - 1].get_m())
            ++cnt;
    }
    return cnt;
}


int binary_search(data *a, int n, const data &x) {
    int left = 0, right = n;
    while (left != right) {
        int mid = (left + right) / 2;
        if (a[mid] < x)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

void merge_arrays(data *a, data *b, data *c, int n, int m) {
    int i = 0, j = 0, k = 0;
    while (i < n && j < m) {
        if (a[i] < b[j]) {
            c[k++] = a[i++];
        } else {
            c[k++] = b[j++];
        }
    }
    while (i < n)
        c[k++] = a[i++];
    while (j < m)
        c[k++] = b[j++];
}

int dividing_array(data *a, int n, int m) {
    int i = 0, j = n - 1;
    data pivot = a[m];

    while (i <= j) {
        while (i < n && a[i] < pivot)
            i++;
        while (j >= 0 && pivot < a[j])
            j--;

        if (i <= j) {
            if (i == j) {
                return i;
            }
            data tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;

            if (i == m)
                m = j;
            else if (j == m)
                m = i;

            i++;
            j--;
        }
    }
    return m;
}

void bubble_sort(data *a, int n) {
    int i = 0, j = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (a[j] > a[j + 1]) {
                data tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }
}

void find_min_sort(data *a, int n) {
    int i, k, ind_min;
    data a_min;
    data tmp;
    for (k = n - 1; k > 1; k--) {
        a_min = a[0];
        ind_min = 0;
        for (i = 1; i <= k; i++) {
            if (a_min < a[i]) {
                a_min = a[i];
                ind_min = i;
            }
        }
        if (ind_min != k) {
            tmp = a[k];
            a[k] = a[ind_min];
            a[ind_min] = tmp;
        }
    }
}

void linear_insert_sort(data *a, int n) {
    int i, j;
    data ins_el;
    for (i = 1; i < n; i++) {
        ins_el = a[i];
        j = i - 1;
        while (j >= 0 && a[j] > ins_el) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = ins_el;
    }
}

void binary_insert_sort(data *a, int n) {
    int i, j, ind;
    data ins_el;
    for (i = 1; i < n; i++) {
        j = i - 1;
        ins_el = a[i];
        ind = binary_search(a, i, ins_el);

        while (j >= ind) {
            a[j + 1] = a[j];
            j--;
        }
        a[ind] = ins_el;
    }
}

void merge_sort(data *a, data *b, int n) {
    data *c = a;
    data *a_original = a;
    int part_len, i;
    for (part_len = 1; part_len < n; part_len *= 2) {
        int last_len = n % (2 * part_len);
        for (i = 0; i <= n - 2 * part_len; i += 2 * part_len) {
            merge_arrays(&a[i], &a[i + part_len], &b[i], part_len, part_len);
        }
        if (last_len > part_len) {
            merge_arrays(&a[i], &a[i + part_len], &b[i], part_len, n % part_len);
        } else if (last_len > 0) {
            for (; i < n; i++) {
                b[i] = a[i];
            }
        }
        c = a;
        a = b;
        b = c;
    }
    if (a != a_original) {
        for (i = 0; i < n; i++) {
            a_original[i] = a[i];
        }
    }
}

void quick_sort(data *a, int n) {
    if (n <= 1) return;

    while (n > 1) {
        int pivot_index = n / 2;
        int ind = dividing_array(a, n, pivot_index);

        if (ind <= 0) ind = 1;
        if (ind >= n) ind = n - 1;

        if (ind < n - ind) {
            quick_sort(a, ind);
            a = a + ind;
            n -= ind;
        } else {
            quick_sort(a + ind, n - ind);
            n = ind;
        }
    }
}


void tournament_sort(data *a, int n) {
    int k, parent_index, left_index, right_index, index, largest, j;
    data mid, tmp;

    for (k = 1; k < n; k++) {
        index = k;
        mid = a[k];
        while (index > 0) {
            parent_index = (index - 1) / 2;
            if (!(a[parent_index] < mid))
                break;
            a[index] = a[parent_index];
            index = parent_index;
        }
        a[index] = mid;
    }

    for (k = n - 1; k > 0; k--) {
        tmp = a[k];
        a[k] = a[0];
        a[0] = tmp;

        j = 0;
        while (true) {
            left_index = 2 * j + 1;
            right_index = 2 * j + 2;
            if (left_index >= k)
                break;

            if (right_index < k && a[left_index] < a[right_index]) {
                largest = right_index;
            } else {
                largest = left_index;
            }

            if (!(a[j] < a[largest]))
                break;

            tmp = a[j];
            a[j] = a[largest];
            a[largest] = tmp;

            j = largest;
        }
    }
}
