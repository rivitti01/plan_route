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
    int cars_number;
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
void add_car(int km, int fuel);
p_station new_station(int km);
p_station insert_station(p_station highway, int km, int* cars, int cars_number);
p_station insert_station_iterative(p_station highway, int km,bool* new);
p_station find_station_iterative(p_station highway, int km);
p_car find_car(p_car cars, int fuel);
p_car delete_car(p_car cars, int fuel,p_car car_to_remove,bool* removed);
p_station delete_station(p_station highway, int km);
void add_station_command(char* message);
void remove_station_command(char* message);
void add_car_command(char* message);
void remove_car_command(char* message);
void in_order_free_cars_iterative(p_car cars);
p_car remove_node_car_from_station(p_car car,p_car deleted_car);
char my_get_char();


//#############################################################
//-------------------------MAIN-----

int main(void) {
    char input[3000];
    char c;
    bool eof = false;

    while (!eof){
        int index = 0;
        while ((c = my_get_char()) != '\n' && c != '\0'){
            if (c == EOF){
                eof = true;
                break;
            }
            input[index++] = c;
        }
        input[index] = '\0';
        analyzeMessage(input);
        /*if (fgets(input, sizeof(input), stdin)){
            analyzeMessage(input);
        }else{
            break;
        }*/
    }
    return 0;
}
//#############################################################
//-------------------------FUNZIONI-----

void analyzeMessage(char* message){
    if (strncmp(message,"aggiungi-stazione",17) == 0) {
        add_station_command(message);
    }
    if (strncmp(message,"demolisci-stazione",18) == 0){
        remove_station_command(message);
    }
    if (strncmp(message,"aggiungi-auto",13) == 0){
        add_car_command(message);
    }
    if (strncmp(message,"rottama-auto",12) == 0){
        remove_car_command(message);
    }
}

char my_get_char(){
    char c = getchar_unlocked();
    return c;
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
    //insert car iterative and return root cars
    p_car tmp = cars;
    p_car prec = cars;

    while (tmp != NULL){
        if (tmp->fuel == fuel){
            tmp->available++;
            return cars;
        }
        if (fuel < tmp->fuel){
            prec = tmp;
            tmp = tmp->left;
        } else{
            prec = tmp;
            tmp = tmp->right;
        }
    }
    tmp = new_car(fuel);
    last_car_added = tmp;
    if(prec == NULL){
        cars = tmp;
    }else{
        if (fuel < prec->fuel){
            prec->left = tmp;
            tmp->parent = prec;
        }else if (fuel > prec->fuel){
            prec->right = tmp;
            tmp->parent = prec;
        }
    }
    return cars;
}
p_station new_station(int km){
    p_station new = malloc(sizeof(t_station));
    new->km = km;
    new->cars_number = 0;
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
p_station insert_station_iterative(p_station highway, int km, bool* new){
    p_station tmp = highway;
    p_station prec = highway;
    while (tmp != NULL){
        if (tmp->km == km){
            printf("non aggiunta\n");
            *new = false;
            return highway;
        }
        if (km < tmp->km){
            prec = tmp;
            tmp = tmp->left;
        }else{
            prec = tmp;
            tmp = tmp->right;
        }
    }
    tmp = new_station(km);
    if (stations == NULL){
        stations = tmp;
    }
    if (prec != NULL && km < prec->km){
        prec->left = tmp;
        tmp->parent = prec;
    }else if(prec != NULL && km > prec->km){
        prec->right = tmp;
        tmp->parent = prec;
    }
    printf("aggiunta\n");
    *new = true;
    return tmp;
}

p_station find_station_iterative(p_station highway, int km){
    if (highway == NULL){
        return highway;
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
    } while (tmp != NULL && tmp->fuel != fuel);
    return tmp;
}
p_car delete_car(p_car cars, int fuel,p_car car_to_remove,bool* removed){
    p_car tmp;
    if (car_to_remove != NULL){
        tmp = car_to_remove;
    }else{
        tmp = find_car(cars, fuel);
    }
    if (tmp != NULL && tmp->available > 0){
        tmp->available = tmp->available - 1;

        if (tmp->available == 0){
            cars = remove_node_car_from_station(cars, tmp);
        }

        printf("rottamata\n");
        *removed = true;
        return cars;
    } else{
        printf("non rottamata\n");
        *removed = false;
        return NULL;
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


    //recupero il chilometro della strada
    char* token = strtok(param, delimiter);
    int km = (int) strtol(token, &end_ptr, 10);
    //sposto la stringa e prendo il prossimo token
    token = strtok(NULL, delimiter);

    //il token successivo è il numero di macchine
    int cars_number = (int) strtol(token, &end_ptr, 10);
    //sposto la stringa e prendo il prossimo token
    token = strtok(NULL, delimiter);
    //i prossimi token sono le macchine
    bool new = false;
    p_station tmp_station = insert_station_iterative(stations,km,&new);
    if (!new){
        return;
    }
    tmp_station->cars_number = cars_number;
    int index = 0;
    while (token != NULL && index < cars_number) {
        int fuel = (int) strtol(token, &end_ptr, 10);
        tmp_station->cars = insert_car(tmp_station->cars, fuel);

        if (tmp_station->fast_car == NULL) {
            tmp_station->fast_car = tmp_station->cars;
        }
        if (tmp_station->fast_car->fuel < fuel){
            tmp_station->fast_car = last_car_added;
        }

        token = strtok(NULL, delimiter);
        index++;
    }
}
void remove_station_command(char* message){
    const char* prefix = "demolisci-stazione ";
    const char delimiter[] = " ";
    char* param = strstr(message, prefix);
    if (param != NULL){
        param += strlen(prefix);
    }
    //char *km_str = malloc(2*sizeof (char));
    char* end_ptr;
    char* token = strtok(param, delimiter);
    int km = (int) strtol(token, &end_ptr, 10);
    //strncpy(km_str, param, 2);
    stations = delete_station(stations, km);
}

//prende in input il messaggio "aggiungi-auto" seguito dal chilometro e dall'autonomia dell'auto.
//aggiunge l'auto con l'autonomia data al km assegnato
void add_car_command(char* message){
    const char* prefix = "aggiungi-auto";
    const char delimiter[] = " ";
    char* param = strstr(message,prefix);
    if (param != NULL){
        param += strlen(prefix);
    }
    char* end_ptr;
    char* token = strtok(param, delimiter);
    int km = (int) strtol(token, &end_ptr, 10);
    token = strtok(NULL, delimiter);
    int fuel = (int) strtol(token,&end_ptr,10);
    add_car(km,fuel);

}
void remove_car_command(char* message){
    const char* prefix = "rottama-auto";
    const char delimiter[] = " ";
    char* param = strstr(message,prefix);
    if (param != NULL){
        param += strlen(prefix);
    }
    char* end_ptr;
    char* token = strtok(param, delimiter);
    int km = (int) strtol(token, &end_ptr, 10);
    token = strtok(NULL, delimiter);
    int fuel = (int) strtol(token,&end_ptr,10);

    p_station tmp_found_station = find_station_iterative(stations, km);
    if (tmp_found_station == NULL){
        printf("non rottamata\n");
        return;
    }
    p_car car_to_remove = find_car(tmp_found_station->cars,fuel);
    if (car_to_remove == tmp_found_station->fast_car) {
        tmp_found_station->fast_car = car_to_remove->parent;
    }

    bool removed = false;
    tmp_found_station->cars = delete_car(tmp_found_station->cars, fuel,car_to_remove,&removed);
    if (removed){
        tmp_found_station->cars_number--;
    }
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
void add_car(int km, int fuel){
    p_station tmp = find_station_iterative(stations,km);
    if (tmp == NULL){
        printf("non aggiunta\n");
        return;
    }
    if (tmp->cars_number == 512){
        printf("non aggiunta\n");
        return;
    }
    tmp->cars = insert_car(tmp->cars,fuel);
    if (last_car_added != NULL){
        printf("aggiunta\n");
        tmp->cars_number++;
        //controllo se la macchina aggiunta è più veloce di quella più veloce
        if (tmp->fast_car != NULL && tmp->fast_car->fuel < fuel && last_car_added != tmp->fast_car && last_car_added->parent == tmp->fast_car){
            tmp->fast_car = last_car_added;
        }
    }
}
//rimuove il nodo car dall'albero cars e modifica a sua volta l'albero che contiene quel nodo
p_car remove_node_car_from_station(p_car cars,p_car deleted_car){
    p_car tmp = deleted_car;
    if (tmp->parent != NULL && tmp->left == NULL && tmp->right == NULL){
        if (tmp->parent->left == tmp){
            tmp->parent->left = NULL;
        }else{
            tmp->parent->right = NULL;
        }
        free(tmp);
        return cars;
    }else if(tmp->parent != NULL && tmp->left == NULL){
        if (tmp->parent->left == tmp){
            tmp->parent->left = tmp->right;
            tmp->right->parent = tmp->parent;
        }else{
            tmp->parent->right = tmp->right;
            tmp->right->parent = tmp->parent;
        }
        free(tmp);
        return cars;
    }else if(tmp->parent != NULL && tmp->right == NULL){
        if (tmp->parent->left == tmp){
            tmp->parent->left = tmp->left;
            tmp->left->parent = tmp->parent;
        }else{
            tmp->parent->right = tmp->left;
            tmp->left->parent = tmp->parent;
        }
        free(tmp);
        return cars;
    }else{
        if (tmp->parent == NULL && tmp->right == NULL && tmp->left == NULL ){
            tmp->fuel = 0;
            tmp->available = 0;
            free(tmp);
            return NULL;
        }
        if (tmp->parent == NULL && tmp ->right == NULL && tmp->left != NULL){
            tmp->left->parent = NULL;
            cars = tmp->left;
            free(tmp);
            return cars;
        }
        if (tmp->parent == NULL && tmp ->right != NULL && tmp->left == NULL){
            tmp->right->parent = NULL;
            cars = tmp->right;
            free(tmp);
            return cars;
        }
        //se è un nodo NON foglia
        p_car min = tmp->right;
        while (min->left != NULL){
            min = min->left;
        }
        if (tmp->parent == NULL){
            //se sono la radice
            min->left = tmp->left;
            tmp->left->parent = min;
            cars = tmp->right;
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
        free(tmp);
        return cars;
    }
}

