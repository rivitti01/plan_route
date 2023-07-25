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





//#############################################################
//-------------------------PROTOTIPI DI FUNZIONI-----
void analyzeMessage(char *message);
p_car new_car(int fuel);
p_car insert_car(p_car cars, int fuel);
p_station new_station(int km);
p_station insert_station(p_station stations, int km, int* cars);
p_station find_station(p_station stations, int km);
p_car find_car(p_car cars, int fuel);
p_car delete_car(p_car cars, int fuel);
p_station delete_station(p_station stations, int km);
void add_station_command(char* message);




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
    if (cars == NULL){
        return new_car(fuel);
    }
    if (cars->fuel == fuel){
        cars->available = cars->available + 1;
        return cars;
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
p_station insert_station(p_station stations, int km, int* cars){
    if (stations == NULL){
        return new_station(km);
    }
    if (km < stations->km){
        stations->left = insert_station(stations->left, km, cars);
        stations->left->parent = stations;
    }else{
        stations->right = insert_station(stations->right, km,cars);
        stations->right->parent = stations;
    }
    return stations;
}
p_station find_station(p_station stations, int km){
    if (stations->km == km){
        return stations;
    }
    p_station tmp = stations;
    do {
        if (tmp->km > km){
            tmp = tmp->left;
        }else{
            tmp = tmp->right;
        }
    } while (tmp->km != km);
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
    if (tmp->available > 1){
        tmp->available = tmp->available - 1;
        return cars;
    } else{

    }
}
p_station delete_station(p_station stations, int km){

}

void add_station_command(char* message){
    const char* prefix = "aggiungi-stazione ";
    char* param = strstr(message, prefix);
    if (param != NULL){
        param += strlen(prefix);
    }
    char* end_ptr;
    char *km_str = malloc(2*sizeof (char));
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

    int* cars_array = malloc(strlen(cars_str)*sizeof (int));

    int cars_number = (int)strlen(cars_str);

    if (cars_number % 2 == 1){
        cars_number = (cars_number+1)/2;
    } else{
        cars_number = cars_number/2;
    }


    int index = 0;
    for (int i = 0; i < cars_number && macchine[index] != '\n'; ++i) {
        char automobile = macchine[index];
        index++;
        if (automobile == ' '){
            i--;
            continue;
        } else{
            cars_array[i] = (int) automobile - '0';
        }
    }
    stations = insert_station(stations,km,cars_array);
}
