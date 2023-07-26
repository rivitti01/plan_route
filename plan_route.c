#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdbool.h"

//#############################################################
//-------------------------STRUTTURE DATI----------------------
typedef struct s_car{
    int fuel;
    int available;
    struct s_car* left;
    struct s_car* right;
    struct s_car* parent;
}t_car;

typedef t_car* p_car;


typedef struct s_station{
    int km;
    p_car cars;
    p_car fast_car;
    struct s_station* left;
    struct s_station* right;
    struct s_station* parent;
}t_station;

typedef t_station* p_station;

p_station stations = NULL;
p_car last_car_added = NULL;





//#############################################################
//-------------------------PROTOTIPI DI FUNZIONI-----
void analyzeMessage(char *message);
p_car new_car(int fuel);
p_car insert_car(p_car cars, int fuel);
p_station new_station(int km);
p_station insert_station(p_station highway, int km, int* cars, int cars_number);
p_station insert_station_iterative(p_station highway, int km, int* cars);
p_station find_station(p_station highway, int km);
p_station find_station_iterative(p_station highway, int km);
p_car find_car(p_car cars, int fuel);
p_car delete_car(p_car cars, int fuel);
p_station delete_station(p_station highway, int km);
void add_station_command(char* message);
void remove_station_command(char* message);
void in_order_free_cars_iterative(p_car cars);




int main(void) {
    char *input = malloc(100 * sizeof(char));



    while (1){
        if (fgets(input, 100, stdin)){
            analyzeMessage(input);
        }else{
            break;
        }

    }
    return 0;
}

void analyzeMessage(char* message){
    if (strncmp(message,"aggiungi-stazione",17) == 0) {
        add_station_command(message);
    }
    if (strncmp(message,"demolisci-stazione",18) == 0){
        remove_station_command(message);
    }
}


p_car new_car(int fuel){
    p_car new = malloc(sizeof(t_car));
    new->fuel = fuel;
    new->available = 1;
    new->left = NULL;
    new->right = NULL;
    new->parent = NULL;
    return new;
}
p_car insert_car(p_car cars, int fuel){
    if (cars != NULL && cars->fuel == fuel){
        cars->available = cars->available + 1;
        return cars;
    }
    if (cars == NULL){
        last_car_added = new_car(fuel);
        return last_car_added;
    }
    if (fuel < cars->fuel){
        cars->left = insert_car(cars->left, fuel);
        cars->left->parent = cars;
    }else{
        cars->right = insert_car(cars->right, fuel);
        cars->right->parent = cars;
    }

    return cars;
}
p_station new_station(int km){
    p_station new = malloc(sizeof(t_station));
    new->km = km;
    new->cars = NULL;
    new->fast_car = NULL;
    new->left = NULL;
    new->right = NULL;
    new->parent = NULL;
    return new;
}
p_station insert_station(p_station highway, int km, int* cars, int cars_number){
    if (highway == NULL){
        //return new_station(km);
        p_station new_p_station = new_station(km);
        for (int i = 0; i < cars_number; i++) {
            new_p_station->cars = insert_car(new_p_station->cars, cars[i]);
            if (new_p_station->fast_car == NULL) {
                new_p_station->fast_car = new_p_station->cars;
            }
            if (new_p_station->fast_car->fuel < cars[i]){
                new_p_station->fast_car = last_car_added;
            }
        }
        highway = new_p_station;
        printf("aggiunta\n");
        return highway;
    }
    if (highway->km == km){
        printf("non aggiunta\n");
        return highway;
    }
    if (km < highway->km){
        highway->left = insert_station(highway->left, km, cars,cars_number);
        highway->left->parent = highway;
    }else{
        highway->right = insert_station(highway->right, km, cars,cars_number);
        highway->right->parent = highway;
    }
    return highway;
}
p_station insert_station_iterative(p_station highway, int km, int* cars){
    p_station tmp = highway;
    while (tmp != NULL){
        if (tmp->km == km){
            printf("non aggiunta\n");
            return highway;
        }
        if (km < tmp->km){
            tmp = tmp->left;
        }else{
            tmp = tmp->right;
        }
    }
    p_station new_p_station = new_station(km);
    for (int i = 0; i < sizeof(cars); i++) {
        new_p_station->cars = insert_car(new_p_station->cars, cars[i]);
        if (new_p_station->fast_car == NULL)
            new_p_station->fast_car = new_p_station->cars;
        else if (new_p_station->fast_car->fuel < cars[i])
            new_p_station->fast_car = new_p_station->cars;

    }
    printf("aggiunta\n");
    return highway;
}
p_station find_station(p_station highway, int km){
    if (highway->km == km){
        return highway;
    }
    p_station tmp = highway;
    do {
        if (tmp->km > km){
            tmp = tmp->left;
        }else{
            tmp = tmp->right;
        }
    } while (tmp->km != km);
    return tmp;
}
p_station find_station_iterative(p_station highway, int km){
    if (highway == NULL){
        return NULL;
    }

    p_station tmp = highway;
    while (tmp->km != km && tmp != NULL){
        if (tmp->km > km){
            tmp = tmp->left;
        }else{
            tmp = tmp->right;
        }
    }
    return tmp;
}

p_car find_car(p_car cars, int fuel){
    if (cars->fuel == fuel){
        return cars;
    }
    p_car tmp = cars;
    do {
        if (tmp->fuel > fuel){
            tmp = tmp->left;
        }else{
            tmp = tmp->right;
        }
    } while (tmp->fuel != fuel);
    return tmp;
}
p_car delete_car(p_car cars, int fuel){
    p_car tmp = find_car(cars, fuel);
    if (tmp != NULL && tmp->available > 1){
        tmp->available = tmp->available - 1;
        printf("rottamata\n");
        return cars;
    } else{
        printf("non rottamata\n");
        return cars;
    }
}
p_station delete_station(p_station highway, int km){
    p_station tmp = find_station_iterative(highway, km);
    if (tmp == NULL){
        printf("non demolita\n");
        return highway;
    }else{
        if (tmp->left == NULL && tmp->right == NULL){
            if (tmp->parent->left == tmp){
                tmp->parent->left = NULL;
            }else{
                tmp->parent->right = NULL;
            }
            free(tmp);
            printf("demolita\n");
            return highway;
        }else if (tmp->left == NULL){
            if (tmp->parent->left == tmp){
                tmp->parent->left = tmp->right;
            }else{
                tmp->parent->right = tmp->right;
            }
            free(tmp);
            printf("demolita\n");
            return highway;
        }else if (tmp->right == NULL){
            if (tmp->parent->left == tmp){
                tmp->parent->left = tmp->left;
            }else{
                tmp->parent->right = tmp->left;
            }
            free(tmp);
            printf("demolita\n");
            return highway;
        }else{
            p_station min = tmp->right;
            while (min->left != NULL){
                min = min->left;
            }
            tmp->km = min->km;
            tmp->cars = min->cars;
            tmp->fast_car = min->fast_car;
            tmp->right = delete_station(tmp->right, min->km);
            printf("demolita\n");
            return highway;
        }
    }
}

void add_station_command(char* message){
    const char* prefix = "aggiungi-stazione ";
    char* param = strstr(message, prefix);
    if (param != NULL){
        param += strlen(prefix);
    }
    char *km_str = malloc(2*sizeof (char));
    char* end_ptr;
    strncpy(km_str, param, 2);
    int km = (int)strtol(km_str, &end_ptr, 10);
    char* tmp = " ";
    strcat(km_str, tmp);
    char* cars_str = strstr(param,km_str);

    if (cars_str!= NULL){
        cars_str += strlen(km_str);
    }
    char* macchine = malloc(strlen(cars_str)*sizeof (char));
    strcpy(macchine, cars_str);

    int* cars_array = malloc(1*sizeof (int));

    /*int cars_number = (int)strlen(cars_str)-1;

    if (cars_number % 2 == 1){
        cars_number = (cars_number+1)/2;
    } else{
        cars_number = cars_number/2;
    }*/

    const char delimiter[] = " ";
    int index = 0;
    char* token = strtok(macchine, delimiter);
    while (token != NULL) {
        cars_array[index] = (int) strtol(token, &end_ptr, 10);
        token = strtok(NULL, delimiter);
        index++;
        if (token != NULL) {
        cars_array = realloc(cars_array, (index + 1) * sizeof(int));
        }
    }
    int cars_number = index;

    stations = insert_station(stations,km,cars_array,cars_number);
}
void remove_station_command(char* message){
    const char* prefix = "demolisci-stazione ";
    char* param = strstr(message, prefix);
    if (param != NULL){
        param += strlen(prefix);
    }
    char *km_str = malloc(2*sizeof (char));
    char* end_ptr;
    strncpy(km_str, param, 2);
    int km = (int)strtol(km_str, &end_ptr, 10);
    stations = delete_station(stations, km);
}
void in_order_free_cars_iterative(p_car cars){
    p_car tmp = cars;
    while (tmp != NULL){
        if (tmp->left != NULL){
            tmp = tmp->left;
        }else if (tmp->right != NULL){
            tmp = tmp->right;
        }else{
            p_car tmp2 = tmp;
            tmp = tmp->parent;
            if (tmp->left == tmp2){
                tmp->left = NULL;
            }else{
                tmp->right = NULL;
            }
            free(tmp2);
        }
    }
}
