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
        if (fgets(input, 1000, stdin)){
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
    while (tmp != NULL){
        if (tmp->km == km){
            return tmp;
        }
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
        if (tmp->parent != NULL && tmp->left == NULL && tmp->right == NULL){
            if (tmp->parent->left == tmp){
                tmp->parent->left = NULL;
            }else{
                tmp->parent->right = NULL;
            }
            in_order_free_cars_iterative(tmp->cars);
            free(tmp);
            printf("demolita\n");
            return highway;
        }else if (tmp->parent != NULL && tmp->left == NULL){
            if (tmp->parent->left == tmp){
                tmp->parent->left = tmp->right;
                tmp->right->parent = tmp->parent;
            }else{
                tmp->parent->right = tmp->right;
                tmp->right->parent = tmp->parent;
            }
            in_order_free_cars_iterative(tmp->cars);
            free(tmp);
            printf("demolita\n");
            return highway;
        }else if (tmp->parent != NULL && tmp->right == NULL){
            if (tmp->parent->left == tmp){
                tmp->parent->left = tmp->left;
                tmp->left->parent = tmp->parent;
            }else{
                tmp->parent->right = tmp->left;
                tmp->left->parent = tmp->parent;
            }
            in_order_free_cars_iterative(tmp->cars);
            free(tmp);
            printf("demolita\n");
            return highway;
        }else{
            if (tmp->parent == NULL && tmp->right == NULL && tmp->left == NULL ){
                in_order_free_cars_iterative(tmp->cars);
                free(tmp);
                printf("demolita\n");
                return NULL;
            }
            if (tmp->parent == NULL && tmp ->right == NULL && tmp->left != NULL){
                tmp->left->parent = NULL;
                stations = tmp->left;
                in_order_free_cars_iterative(tmp->cars);
                free(tmp);
                printf("demolita\n");
                return stations;
            }
            if (tmp->parent == NULL && tmp ->right != NULL && tmp->left == NULL){
                tmp->right->parent = NULL;
                stations = tmp->right;
                in_order_free_cars_iterative(tmp->cars);
                free(tmp);
                printf("demolita\n");
                return stations;
            }
            //se è un nodo NON foglia
            p_station min = tmp->right;
            //cerco il minimo
            while (min->left != NULL){
                min = min->left;
            }
            if (tmp->parent == NULL){
                //se sono la radice
                min->left = tmp->left;
                tmp->left->parent = min;
                stations = tmp->right;
                tmp->right->parent = NULL;
            }else if (tmp->parent->right == tmp){
                //se sono il figlio destro del padre
                min->left = tmp->left;
                tmp->left->parent = min;
                tmp->parent->right = tmp->right;
                tmp->right->parent = tmp->parent;
            }else{
                //se sono il figlio sinistro del padre
                min->left = tmp->left;
                tmp->left->parent = min;
                tmp->parent->left = tmp->right;
                tmp->right->parent = tmp->parent;
            }
            in_order_free_cars_iterative(tmp->cars);
            free(tmp);
            printf("demolita\n");
            return stations;
        }
    }
}

void add_station_command(char* message){
    const char* prefix = "aggiungi-stazione ";
    char* param = strstr(message, prefix);
    if (param != NULL){
        param += strlen(prefix);
    }


    char* end_ptr;
    const char delimiter[] = " ";
    int index = 0;

    //recupero il chilometro della strada
    char* token = strtok(param, delimiter);
    int km = (int) strtol(token, &end_ptr, 10);
    //sposto la stringa e prendo il prossimo token
    token = strtok(NULL, delimiter);

    //creo un array di macchine
    int* cars_array = malloc(1*sizeof (int));
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
            if (tmp != NULL && tmp->left == tmp2){
                tmp->left = NULL;
            }else if (tmp != NULL && tmp->right == tmp2){
                tmp->right = NULL;
            }
            free(tmp2);
        }
    }
}
