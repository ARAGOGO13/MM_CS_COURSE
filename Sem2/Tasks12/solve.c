#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "solve.h"

int task01 (node * head)
{
    int cnt = 1;
    char *max_str = 0;
    node *curr = 0;

    if (head == 0) { return 0; }

    max_str = head->string;
    curr = head->next;

    while (curr != 0) 
        {
            int cmp = strcmp(curr->string, max_str);
            if (cmp > 0) 
                {
                    max_str = curr->string;
                    cnt = 1;
                }
            else if (cmp == 0) cnt ++;
            curr = curr->next;
        }

    return cnt;
}

int task02 (node * head)
{
    int cnt = 0;
    node *prev = 0;
    node *curr = 0;

    if (head == 0 || head->next == 0) { return 0; }

    prev = head;
    curr = head->next;

    while (curr != 0) 
        {
            int cmp = strcmp(curr->string, prev->string);
            if (cmp > 0) cnt ++;
            prev = curr;
            curr = curr->next;
        }

    return cnt;
}

int task03 (node * head)
{
    int cnt = 0;
    node *prev = 0;
    node *curr = 0;
    node *next = 0;

    if (head == 0) return 0;
    if (head->next == 0) return 0;

    prev = head;
    curr = head->next;
    next = curr->next;

    while (next != 0) 
        {
            int is_loc_max = 1;
            if (strcmp(curr->string, prev->string) < 0) is_loc_max = 0;
            if (strcmp(curr->string, next->string) < 0) is_loc_max = 0;
            if (is_loc_max) cnt++;

            prev = curr;
            curr = next;
            next = next ? next->next : 0;
        }

    return cnt;
}

int task04 (node * head)
{
    int mx_dist = 0;
    int last_mx_ind = -1;
    int curr_ind = 1;

    node *prev = 0;
    node *curr = 0;
    node *next = 0;

    if (head == 0 || head->next == 0 || head->next->next == 0) { return 0; }

    prev = head;
    curr = head->next;
    next = curr->next;

    while (next != 0)
        {
            int is_mx = 1;
            if (prev != 0 && strcmp(curr->string, prev->string) < 0) is_mx = 0;
            if (next != 0 && strcmp(curr->string, next->string) < 0) is_mx = 0;

            if (is_mx)
                {
                    if (last_mx_ind != -1) 
                        {
                            int dist = curr_ind - last_mx_ind - 1;
                            if (dist > mx_dist) mx_dist = dist;
                        }
                    last_mx_ind = curr_ind;
                }

            prev = curr;
            curr = next;
            next = next->next;
            curr_ind++;
        }

    return mx_dist;
}

int task05 (node * head)
{
    int mx_dist = 0;
    int cnt = 1;
    node *prev = 0;
    node *curr = 0;

    if (head == 0) return 0;

    prev = head;
    curr = head->next;

    while (curr != 0)
        {
            if (strcmp(prev->string, curr->string) < 0) 
                {
                    cnt ++;
                    if (cnt > mx_dist) mx_dist = cnt;
                }
            else cnt = 1;
            prev = curr;
            curr = curr->next;
        }

    return mx_dist;
}

int task06 (node * head)
{
    int cnt = 0;

    node *prev = 0;
    node *curr = 0;

    if (head == 0 || head->next == 0) return 0;
    
    prev = head;
    curr = head->next;

    while (curr != 0)
        {
            if (strcmp(prev->string, curr->string) == 0) 
                {
                    cnt ++;
                    while (curr != 0 && strcmp(prev->string, curr->string) == 0)
                        {
                            prev = curr;
                            curr = curr->next;
                        }
                    if (curr == 0) break;
                }
            prev = curr;
            curr = curr->next;
        }

    return cnt;
}

int task07 (node * head)
{
    int dist = 0;
    int mx_dist = 0;
    int flag = 0;

    node *prev = 0;
    node *curr = 0;

    if (head == 0 || head->next == 0) return 0;
    
    prev = head;
    curr = head->next;


    while (curr != 0 && curr->next != 0)
        {
            if (strcmp(prev->string, curr->string) == 0) 
                {
                    if (dist > mx_dist) mx_dist = dist;
                    flag = 1;
                    dist = 0;
                    while (curr != 0 && strcmp(prev->string, curr->string) == 0)
                        {
                            prev = curr;
                            curr = curr->next;
                        }
                    if (curr == 0) break;
                }
            else
                {   
                    if (flag) dist++;
                    prev = curr;
                    curr = curr->next;
                }
        }

    return flag ? mx_dist : 0;
}

node * task08 (node * head)
{
    node *prev = 0;
    node *curr = 0;

    if (head == 0 || head->next == 0) return head;

    while (head != 0 && head->next != 0 && strcmp(head->string, head->next->string) >= 0) 
    {
        node* temp = head;
        head = head->next;
        free(temp->string);
        free(temp);
    }

    prev = head;
    curr = head->next;


    while (curr != 0 && curr->next != 0) 
        {
            if (strcmp(curr->string, curr->next->string) >= 0) 
                {
                    node* temp = curr;
                    prev->next = curr->next;
                    curr = curr->next;
                    free(temp->string);
                    free(temp);
                } 
            else 
                {
                    prev = curr;
                    curr = curr->next;
                }
        }

    return head;
}

node * task09 (node * head)
{
    node* prev;
    node* curr;
    node* next;
    node* next2;
    int cmp1, cmp2;

    if (head == 0 || head->next == 0 || head->next->next == 0) return head;

    prev = head;
    curr = head;
    next = curr->next;
    next2 = next->next;

    cmp1 = strcmp(curr->string, next->string);
    while (next2 != 0)
        {
            cmp2 = strcmp(next2->string, next->string);
            if (cmp1 > 0 || cmp2 > 0)
                {
                    prev = next;
                }
            else
                {
                    prev->next = next2;
                    free(next->string);
                    free(next);
                }
            cmp1 = cmp2 > 0 ? -1 : 1;
            if (cmp2 == 0) cmp1 = 0;
            curr = prev;
            next = next2;
            next2 = next->next;
        }

    return head;
}

node * task10 (node * head)
{
    node head0;

    node *prev = &head0;
    node *curr = head;

    node *start = 0;
    node *end_next = curr->next;
    node *tmp = start;

    head0.next = head;

    while (curr && curr->next) 
        {
            if (strcmp(curr->string, curr->next->string) >= 0) 
                {
                    start = curr;
                    while (curr->next && strcmp(curr->string, curr->next->string) >= 0) 
                        {
                            curr = curr->next;
                        }
                    end_next = curr->next;
                    tmp = start;
                    while (tmp != end_next) 
                        {
                            node *to_free = tmp;
                            tmp = tmp->next;
                            free(to_free->string);
                            free(to_free);
                        }

                    prev->next = end_next;
                    curr = end_next;
                }
            else 
                {
                    prev = curr;
                    curr = curr->next;
                }
        }

    return head0.next;
}


