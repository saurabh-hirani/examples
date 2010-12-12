#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <limits.h>
//#include <time.h>

#define MAXEMP 5
#define MAXSAL 5000


struct struct_emp {
    char* name;
    int base_salary;
    int commission;

};
typedef struct struct_emp emp;

struct struct_store_sal_emp_ds {
    emp* curr_emp_ptr;
    int salary;
};
typedef struct struct_store_sal_emp_ds store_sal_emp_ds;

//int seed_rand() {
//    time_t seconds;
//    time(&seconds);
//    srand((unsigned int)seconds);
//}

int get_total_salary(emp* emp_ptr) {
    return(emp_ptr->base_salary + emp_ptr->commission);
}

int salary_comparator(const void* emp1, const void* emp2) {
    store_sal_emp_ds* emp1_ptr = *(store_sal_emp_ds* const*) emp1;
    store_sal_emp_ds* emp2_ptr = *(store_sal_emp_ds* const*) emp2;
    return (emp1_ptr->salary - emp2_ptr->salary);
}

void build_emp_list(emp* emp_ptr_arr[], int n_emp) {
    int count = 0;
    int max_base_salary = MAXSAL;
    char* base_emp_name = "emp-";
    while (count < n_emp) {
        char* numstr = (char*)(malloc(sizeof(char)) + 10);
        sprintf(numstr, "%d", count);

        char* name = (char*) malloc(strlen(base_emp_name) + strlen(numstr) + 1);
        strcat(name, base_emp_name);
        strcat(name, numstr);
        int base_salary = rand() % max_base_salary;
        emp_ptr_arr[count] = (emp*) malloc(sizeof(emp*));
        emp_ptr_arr[count]->name = name;
        emp_ptr_arr[count]->base_salary = base_salary;
        emp_ptr_arr[count]->commission = count + 10;
        count++;
    }
}

void print_emp_list(emp* emp_ptr_arr[], int n_emp) {
    int count = 0;
    while(count < n_emp) {
        printf("COUNT: %d NAME: %s BASE_SAL: %d TOTAL_SAL: %d\n",
               count,
               emp_ptr_arr[count]->name,
               emp_ptr_arr[count]->base_salary,
               emp_ptr_arr[count]->base_salary +
               emp_ptr_arr[count]->commission);
        count++;
    }
}

void build_store_sal_emp_ds(emp* emp_ptr_arr[], store_sal_emp_ds* store_sal_emp_ds_ptr_arr[], int n_emp) {
    int count = 0;
    while (count < n_emp) {
        emp* emp_ptr = (emp*) (malloc(sizeof(emp)));
        emp_ptr = emp_ptr_arr[count];
        store_sal_emp_ds_ptr_arr[count] = (store_sal_emp_ds*) malloc(sizeof(store_sal_emp_ds*));
        store_sal_emp_ds_ptr_arr[count]->salary = get_total_salary(emp_ptr);
        store_sal_emp_ds_ptr_arr[count]->curr_emp_ptr = emp_ptr;
        count++;
    }
}

void build_emp_list_frm_store_sal_emp_ds(store_sal_emp_ds* store_sal_emp_ds_ptr_arr[], emp* sorted_emp_ptr_arr[], int n_emp) {
    int count = 0;
    while (count < n_emp) {
        sorted_emp_ptr_arr[count] = store_sal_emp_ds_ptr_arr[count]->curr_emp_ptr;
        count++;
    }
}

int main() {
    emp* emp_ptr_arr[MAXEMP];
    build_emp_list(emp_ptr_arr, MAXEMP);
    print_emp_list(emp_ptr_arr, MAXEMP);
    /* 
      map { [$_, $_->get_salary] } 
      @employees
    */
    store_sal_emp_ds* store_sal_emp_ds_ptr_arr[MAXEMP];
    build_store_sal_emp_ds(emp_ptr_arr, store_sal_emp_ds_ptr_arr, MAXEMP);
    /* 
      sort { $a->[1] &br;=> $b->[1] } 
      map { [$_, $_->get_salary] } 
      @employees
    */
    qsort(store_sal_emp_ds_ptr_arr, MAXEMP, sizeof(store_sal_emp_ds*), salary_comparator);
    /*
      @sorted_emp = map { $_->[0] } 
                    sort { $a->[1] &br;=> $b->[1] } 
                    map { [$_, $_->get_salary] } 
                    @employees
    */
    emp* sorted_emp_ptr_arr[MAXEMP];
    build_emp_list_frm_store_sal_emp_ds(store_sal_emp_ds_ptr_arr, sorted_emp_ptr_arr, MAXEMP);
    printf("----------------------------\n");
    print_emp_list(sorted_emp_ptr_arr, MAXEMP);
    return(0);
}
