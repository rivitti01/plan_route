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
    int id;
    p_car cars;
    p_car fast_car;
    struct s_station* left;
    struct s_station* right;
    struct s_station* parent;
}t_station;

typedef t_station* p_station;

p_station stations = NULL;
p_car last_car_added = NULL;
int id = 1;






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
void plan_route_command(char* message);
void in_order_free_cars_iterative(p_car cars);
p_car remove_node_car_from_station(p_car car,p_car deleted_car);
//char my_get_char();
void plan_route_forward(int km1, int km2);
p_station find_min_station(p_station highway, p_station km1_station, int km2);
void plan_route_backward(int km1,int km2);
p_station find_min_station_backward(p_station highway, p_station km1_station, int km2);
void free_all_stations(p_station highway);


//#############################################################
//-------------------------MAIN-----

int main(void) {
    char input[20000];

    while (true){
        if (fgets(input, 20000, stdin)){
            analyzeMessage(input);
        }else{
            break;
        }
    }

    /*char c;
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
    }*/
    free_all_stations(stations);
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
    if(strncmp(message,"pianifica-percorso",18) == 0){
        plan_route_command(message);
    }
}

/*char my_get_char(){
    char c = getchar_unlocked();
    return c;
}*/


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
            last_car_added = tmp;
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
    new->id = 0;
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
    if (cars != NULL && cars->fuel == fuel){
        return cars;
    }
    p_car tmp = cars;
    while (tmp != NULL && tmp->fuel != fuel){
        if (tmp->fuel > fuel){
            tmp = tmp->left;
        }else{
            tmp = tmp->right;
        }
    }
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
        if (tmp->fast_car == NULL){
            tmp->fast_car = last_car_added;
        }
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

//data una coppia di stazioni, pianificare il percorso con il minor numero di tappe tra di esse.
// Nel caso siano presenti più percorsi con lo stesso numero minimo di tappe (cioè a pari merito),
// deve essere scelto il percorso che predilige le tappe con distanza più breve dall’inizio dell’autostrada.
void plan_route_command(char* message){
    const char* prefix = "pianifica-percorso";
    const char delimiter[] = " ";
    char* param = strstr(message,prefix);
    if (param != NULL){
        param += strlen(prefix);
    }
    char* end_ptr;
    char* token = strtok(param, delimiter);
    int km1 = (int) strtol(token, &end_ptr, 10);
    token = strtok(NULL, delimiter);
    int km2 = (int) strtol(token,&end_ptr,10);
    if (km1 < km2){
        plan_route_forward(km1,km2);
    }else{
        plan_route_backward(km1,km2);
    }

}
void plan_route_forward(int km1, int km2){
    p_station start_station = find_station_iterative(stations,km1);
    p_station tmp = NULL;
    p_station* array = malloc(sizeof(p_station)*1);
    int i = 0;
    while (true){
        if (tmp == NULL){
            tmp = find_min_station(stations,start_station,km2);
        } else{
            tmp = find_min_station(stations,start_station,tmp->km);
        }

        if(tmp == NULL){
            printf("nessun percorso\n");
            free(array);
            return;
        }else{
            if (i != 0 && tmp->km == array[i-1]->km){
                printf("nessun percorso\n");
                free(array);
                return;
            }
            array[i] = tmp;
            if(tmp->km == km1){
                break;
            }
            i++;
            array = realloc(array,sizeof(p_station*)*(i+1));
        }
    }
    //array contiene tutte le stazioni intermedie tra km1 e km2 ma in ordine inverso. Stampo il percorso composto da km1, array, km2
    for (int j = i; j >= 0; --j) {
        if(j == 0){
            printf("%d",array[j]->km);
            break;
        }
        printf("%d ",array[j]->km);
    }
    printf("%d\n",km2);
    free(array);
}

//scorro tutte le stazioni tra km1 e km2 fino a quando non trovo una stazione tale che tmp->km > km1 e tmp->km < km2 e tmp->fast_car->fuel + tmp->km >= km2
p_station find_min_station(p_station highway, p_station km1_station, int km2){
    if (km1_station == NULL){
        return NULL;
    }
    int km1 = km1_station->km;
    p_station tmp = km1_station;
    p_station valid_station = NULL;
    while (tmp != NULL){
        if((tmp->km == km2 && tmp->left == NULL && tmp->right == NULL)
           || (tmp->km == km2 && tmp->left != NULL && tmp->left->id == id)){
            break;
        }
        if (tmp != NULL && tmp->fast_car != NULL && tmp->km + tmp->fast_car->fuel >= km2 && tmp->km >= km1 && tmp->km <= km2){
            if (valid_station == NULL || tmp->km < valid_station->km){
                valid_station = tmp;
            }
        }
        if (tmp->left != NULL && tmp->left->id != id && tmp->left->km >= km1 ){
            tmp = tmp->left;

        }else if (tmp->right != NULL && tmp->right->id != id && tmp->right->km >= km1){
            tmp = tmp->right;

        }else{
            tmp->id = id;
            if (tmp->parent == NULL){
                if (tmp->right != NULL && tmp->right->id != id){
                    tmp = tmp->right;
                }else{
                    break;
                }
            }else{
                tmp = tmp->parent;
            }
        }
    }
    id++;
    return valid_station;
}

//km1 > km2
void plan_route_backward(int km1,int km2){
    p_station start_station = find_station_iterative(stations,km1);
    p_station tmp = NULL;
    p_station* array = malloc(sizeof(p_station)*1);
    int i = 0;
    array[i] = find_station_iterative(stations,km2);
    i++;
    while (true){
        if (tmp == NULL){
            tmp = find_min_station_backward(stations,start_station,km2);
        } else{
            tmp = find_min_station_backward(stations,start_station,tmp->km);
        }

        if(tmp == NULL){
            printf("nessun percorso\n");
            free(array);
            return;
        }else{
            if (i != 0 && tmp->km == array[i-1]->km){
                printf("nessun percorso\n");
                free(array);
                return;
            }
            array = realloc(array,sizeof(p_station*)*(i+1));
            array[i] = tmp;
            if(tmp->km == km1){
                break;
            }
            i++;
            //array = realloc(array,sizeof(p_station*)*(i+1));
        }
    }

    for (int j = i; j >= 0; --j) {
        if (array[j] == NULL){
            continue;
        }
        int max = array[j]->km - array[j]->fast_car->fuel;
        for (int k = j; k >= 0 ; --k) {
            if (j != k){
                if(array[k] != NULL && max > array[k]->km){
                    //gli elemnti tra j e k possono essere rimossi dal percorso
                    for (int l = j-1; l > k+1; --l) {
                        array[l] = NULL;
                    }
                    break;
                }
                if (array[k] == array[0] && max < array[0]->km){
                    for (int l = j-1; l >= 1; --l) {
                        array[l] = NULL;
                    }
                    break;
                }
            }
        }

    }

    //array contiene tutte le stazioni intermedie tra km1 e km2 ma in ordine inverso. Stampo il percorso composto da km1, array, km2
    for (int j = i; j >= 0; --j) {
        if (array[j] != NULL){
            if(j == 0){
                printf("%d",array[j]->km);
                break;
            }
            printf("%d ",array[j]->km);
        }
    }
    printf("\n");
    free(array);

}
//km1 > km2 --> km1_station ha km maggiore di km2. Il percorso va da km1 a km2
//km1 partenza e km2 arrivo --> km1 grande ; km2 piccolo
p_station find_min_station_backward(p_station highway, p_station km1_station, int km2){
    int km1 = km1_station->km;
    p_station tmp = km1_station;
    p_station valid_station = NULL;
    while (tmp != NULL){
        if((tmp->km == km2 && tmp->left == NULL && tmp->right == NULL)
           || (tmp->km == km2 && tmp->right != NULL && tmp->right->id == id)){
            break;
        }
        //int porcamadonna = tmp->km - tmp->fast_car->fuel;
        if (tmp != NULL && tmp->fast_car != NULL && tmp->km - tmp->fast_car->fuel <= km2 && tmp->km <= km1 && tmp->km >= km2){
            if (valid_station == NULL || (valid_station != NULL && tmp->km < valid_station->km) || (valid_station != NULL && valid_station->km == km2)){
                valid_station = tmp;
            }
        }
        if (tmp->left != NULL && tmp->left->id != id && tmp->left->km <= km1 ){
            tmp = tmp->left;

        }else if (tmp->right != NULL && tmp->right->id != id && tmp->right->km <= km1){
            tmp = tmp->right;

        }else{
            tmp->id = id;
            if (tmp->parent == NULL){
                if (tmp->right != NULL && tmp->right->id != id){
                    tmp = tmp->left;
                }else{
                    break;
                }
            }else{
                tmp = tmp->parent;
            }
        }
    }
    id++;
    return valid_station;

}
void free_all_stations(p_station highway){
    if (highway == NULL){
        return;
    }
    free_all_stations(highway->left);
    free_all_stations(highway->right);
    in_order_free_cars_iterative(highway->cars);
    free(highway);
}

