#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdbool.h"

//#############################################################
//-------------------------STRUTTURE DATI----------------------

//struttura di una macchina
typedef struct s_car{
    int fuel;
    int available;
    struct s_car* left;
    struct s_car* right;
    struct s_car* parent;
}t_car;
//tipo puntatore a macchina
typedef t_car* p_car;

//struttura di una stazione
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
//tipo puntatore a stazione
typedef t_station* p_station;

//struttura dati per una coda
typedef struct s_queue{
    int index;
    struct s_queue* next;
}t_queue;
//tipo puntatore a coda
typedef t_queue* p_queue;

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
p_car find_fast_car(p_car cars);
p_station* breadth_first_search(p_station* array, int array_size);
p_queue enqueue(p_queue pQueue, int index);
//#############################################################
//-------------------------MAIN-----

int main(void) {
    char input[20000];
    /*FILE *file;
    file = fopen("extra_test_cases/open_extra_gen.txt","r");
    if (file == NULL){
        printf("Errore apertura file\n");
        return -1;
    }*/
    while (true){
        if (fgets(input, 20000, stdin)){
            analyzeMessage(input);
        }else{
            break;
        }
    }
    //fclose(file);

    /*char c;
    char input[20000];
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
    //free_all_stations(stations);
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
    p_car new = (p_car) malloc(sizeof(t_car) * 1);
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
    p_station new = malloc(sizeof(t_station)*1);
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
            tmp = NULL;
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
            tmp = NULL;
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
            tmp = NULL;
            printf("demolita\n");
            return highway;
        }else{
            if (tmp->parent == NULL && tmp->right == NULL && tmp->left == NULL ){
                in_order_free_cars_iterative(tmp->cars);
                free(tmp);
                tmp = NULL;
                printf("demolita\n");
                return NULL;
            }
            if (tmp->parent == NULL && tmp ->right == NULL && tmp->left != NULL){
                tmp->left->parent = NULL;
                stations = tmp->left;
                in_order_free_cars_iterative(tmp->cars);
                free(tmp);
                tmp = NULL;
                printf("demolita\n");
                return stations;
            }
            if (tmp->parent == NULL && tmp ->right != NULL && tmp->left == NULL){
                tmp->right->parent = NULL;
                stations = tmp->right;
                in_order_free_cars_iterative(tmp->cars);
                free(tmp);
                tmp = NULL;
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
            tmp = NULL;
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
    if (cars_number == 0){
        return;
    }
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
    if (car_to_remove == NULL ){
        printf("non rottamata\n");
        return;
    }
    bool find_new_fast_car = false;
    if (car_to_remove == tmp_found_station->fast_car && car_to_remove->available == 1) {
        find_new_fast_car = true;
    }

    bool removed = false;
    tmp_found_station->cars = delete_car(tmp_found_station->cars, fuel,car_to_remove,&removed);
    if (removed){
        tmp_found_station->cars_number--;
        if (find_new_fast_car){
            tmp_found_station->fast_car = find_fast_car(tmp_found_station->cars);
        }
    }
}
//prende in input la root delle macchine e cerca la macchina con autonomia maggiore
p_car find_fast_car(p_car cars) {
    if (cars == NULL){
        return NULL;
    }
    p_car tmp = cars;
    while (tmp->right != NULL){
        tmp = tmp->right;
    }
    return tmp;
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
            tmp2 = NULL;
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
        tmp = NULL;
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
        tmp = NULL;
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
        tmp = NULL;
        return cars;
    }else{
        if (tmp->parent == NULL && tmp->right == NULL && tmp->left == NULL ){
            tmp->fuel = 0;
            tmp->available = 0;
            free(tmp);
            tmp = NULL;
            return NULL;
        }
        if (tmp->parent == NULL && tmp ->right == NULL && tmp->left != NULL){
            tmp->left->parent = NULL;
            cars = tmp->left;
            free(tmp);
            tmp = NULL;
            return cars;
        }
        if (tmp->parent == NULL && tmp ->right != NULL && tmp->left == NULL){
            tmp->right->parent = NULL;
            cars = tmp->right;
            free(tmp);
            tmp = NULL;
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
        tmp = NULL;
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
    if (km1 == km2){
        printf("%d",km1);
        return;
    }
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
            array = NULL;
            return;
        }else{
            if (i != 0 && tmp->km == array[i-1]->km){
                printf("nessun percorso\n");
                free(array);
                array = NULL;
                return;
            }
            array[i] = tmp;
            if(tmp->km == km1){
                break;
            }
            i++;
            p_station *tmp_array = realloc(array,sizeof(p_station)*(i+1));
            if (tmp_array != NULL){
                array = tmp_array;
            }

        }
    }
    //array contiene tutte le stazioni intermedie tra km1 e km2 ma in ordine inverso. Stampo il percorso composto da km1, array, km2
    for (int j = i; j >= 0; --j) {
        if(j == 0){
            printf("%d ",array[j]->km);
            break;
        }
        printf("%d ",array[j]->km);
    }
    printf("%d\n",km2);
    free(array);
    array = NULL;
}

//scorro tutte le stazioni tra km1 e km2 fino a quando non trovo una stazione tale che tmp->km > km1 e tmp->km < km2 e tmp->fast_car->fuel + tmp->km >= km2.
//km1_station è la stazione di partenza e km2 è la stazione che devo raggiungere con un l'auto più veloce di una stazione tra km1 e km2. Se non trovo nessuna stazione, ritorno NULL
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
        if (tmp->fast_car != NULL) {
            if (tmp->km + tmp->fast_car->fuel >= km2 && tmp->km >= km1 && tmp->km <= km2) {
                if (valid_station == NULL || tmp->km < valid_station->km) {
                    valid_station = tmp;
                }
            }
        }
        if (tmp->left != NULL && tmp->left->id != id && tmp->left->km >= km1 ){
            tmp = tmp->left;

        }else if (tmp->right != NULL && tmp->right->id != id && tmp->right->km >= km1){
            if (valid_station != NULL && tmp->right->km > valid_station->km){
                break;
            }
            tmp = tmp->right;

        }else{
            tmp->id = id;
            if (tmp->parent == NULL){
                if (tmp->right != NULL && tmp->right->id != id){
                    tmp = tmp->right;
                }else{
                    break;
                    //forse andrebbe return NULL
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
            array = NULL;
            return;
        }else{
            if (i != 0 && tmp->km == array[i-1]->km){
                printf("nessun percorso\n");
                free(array);
                array = NULL;
                return;
            }
            p_station *tmp_array = realloc(array,sizeof(p_station)*(i+1));
            if (tmp_array != NULL){
                array = tmp_array;
            }
            array[i] = tmp;
            if(tmp->km == km1){
                break;
            }
            i++;
        }
    }
    //printf("nessun percorso\n");
    p_station* path = breadth_first_search(array,i);


    //array contiene tutte le stazioni intermedie tra km1 e km2 ma in ordine inverso. Stampo il percorso composto da km1, array, km2
    for (int j = i; j >= 0; --j) {
        if (path[j] != NULL){
            if(j == 0){
                printf("%d",path[j]->km);
                break;
            }
            printf("%d ",path[j]->km);
        }
    }
    printf("\n");
    free(path);
    path = NULL;
    free(array);
    array = NULL;


}
//effettua una ricerca del percorso minimo partendo da array e calconando il nodo che una stazione può raggiungere con array[i]->km - array[i]->fast_car->fuel. Il nodo di partenza è array[i] e il nodo di arrivo è array[0]. L'array è ordinato in ordine crescente di km, ma il pianiicamento del percorso viene effettuato in ordine decrescente di km.
p_station* breadth_first_search(p_station* array, int array_size){
    id++;
    //creo un array di interi dove ogni indice corrisponde a una stazione di array, e dentro ogni cella c'è la distanza che separa la stazione di partenza da quella analizzata
    int* distances = malloc(sizeof(int)*(array_size+1));
    for (int i = array_size; i >= 0; --i) {
        if (i == array_size){
            distances[array_size] = 0;
        }else{
            distances[i] = -1;
        }
    }
    //creo un array di interi dove ogni indice corrisponde a una stazione di array, e dentro ogni cella c'è l'indice della stazione
    int* parents = malloc(sizeof(int)*(array_size+1));
    for (int i = array_size; i >= 0; --i) {
        parents[i] = -1;
    }
    //creo una coda di stazioni
    p_queue queue = NULL;
    //inserisco la stazione di partenza
    queue = enqueue(queue,array_size);



    while(queue != NULL){
        p_queue tmp = queue;
        queue = queue->next;
        p_station station_dequeued = array[tmp->index];
        //-- cerco l'indice della stazione appera dequeued
        int station_dequeued_index = tmp->index;
        /*for (int i = 0; i <= array_size; ++i) {
            if (array[i] != NULL && array[i]->km == station_dequeued->km){
                station_dequeued_index = i;
                break;
            }
        }*/
        free(tmp);
        tmp = NULL;
        p_station next_station;
        //--cerco la prima stazione raggiungibile dalla stazione dequeued
        int next_station_index = -1;
        next_station = NULL;
        for (int i = 0; i <= array_size; ++i) {
            if (array[i] != NULL && station_dequeued->fast_car != NULL && array[i]->km >= station_dequeued->km - station_dequeued->fast_car->fuel  && array[i] != station_dequeued){//&& array[i]->id != id
                next_station = array[i];
                next_station_index = i;
                //array[i]->id = id;
                break;
            }
        }
        //--
        do {
            if (next_station != NULL){
                if (distances[next_station_index] == - 1){
                    distances[next_station_index] = distances[station_dequeued_index] + 1;
                    parents[next_station_index] = station_dequeued->km;
                    queue = enqueue(queue,next_station_index);
                }else{
                    if (distances[station_dequeued_index] + 1 < distances[next_station_index]){
                        distances[next_station_index] = distances[station_dequeued_index] + 1;
                        parents[next_station_index] = station_dequeued->km;
                        queue = enqueue(queue,next_station_index);
                    }
                    if (distances[station_dequeued_index] + 1 == distances[next_station_index]){
                        if(array[station_dequeued_index]->km < parents[next_station_index]){
                            parents[next_station_index] = array[station_dequeued_index]->km;
                            //queue = enqueue(queue,next_station);
                        }
                    }
                }

                if (next_station == array[0]){
                    //ho trovato la stazione di arrivo
                    //printf("next station è l'arrivo\n");
                    //break;
                }
                if (next_station_index + 1 < array_size){
                    if (array[next_station_index + 1] != NULL && array[next_station_index +1]->km >= station_dequeued->km - station_dequeued->fast_car->fuel  && array[next_station_index + 1] != station_dequeued){//&& array[next_station_index +1]->id != id
                        next_station = array[next_station_index+1];
                        next_station_index++;
                        //array[next_station_index]->id = id;
                    }else{
                        next_station = NULL;
                    }
                }else{
                    next_station = NULL;
                }
            }
        } while (next_station != NULL);
        if (next_station == array[0]){
            //ho trovato la stazione di arrivo
            //break;
        }
    }
    //creo un array di stazioni che contiene il percorso minimo
    p_station* path = malloc(sizeof(p_station)*(array_size+1));
    int next_path_station = parents[0];
    path[0] = array[0];
    path[array_size] = array[array_size];
    for (int i = 1; i < array_size; ++i) {
        if (array[i] != NULL && array[i]->km == next_path_station){
            path[i] = array[i];
            next_path_station = parents[i];
            if (next_path_station == -1){
                break;
            }
        }else{
            path[i] = NULL;
        }
    }
    free(distances);
    distances = NULL;
    free(parents);
    parents = NULL;
    id++;
    return path;

}

//aggiunge alla coda una stazione
p_queue enqueue(p_queue pQueue, int index){
    p_queue tmp = pQueue;
    if (tmp == NULL){
        tmp = malloc(sizeof(t_queue)*1);
        tmp->index = index;
        tmp->next = NULL;
        pQueue = tmp;
        return pQueue;
    }
    while (tmp->next != NULL){
        tmp = tmp->next;
    }
    p_queue last = malloc(sizeof(t_queue)*1);
    tmp->next = last;
    last->index = index;
    last->next = NULL;
    return pQueue;
}


//esempio: pianifica-percorso km1 = 1000 , km2 = 10
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
        if (tmp->fast_car != NULL) {
            if (tmp->km - tmp->fast_car->fuel <= km2 && tmp->km <= km1 && tmp->km > km2) {
                if (valid_station == NULL || (valid_station != NULL && tmp->km < valid_station->km) ||
                    (valid_station != NULL && valid_station->km == km2)) {
                    valid_station = tmp;
                }
            }
        }
        if (tmp->left != NULL && tmp->left->id != id && tmp->left->km <= km1 && tmp->km > km2){
            tmp = tmp->left;

        }else if (tmp->right != NULL && tmp->right->id != id && tmp->right->km <= km1){
            tmp = tmp->right;

        }else{
            tmp->id = id;
            if (tmp->km == km2 && tmp->right != NULL && tmp->right->id == id){
                break;
            }
            if (tmp->parent == NULL){
                if (tmp->left != NULL && tmp->left->id != id){
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
    highway = NULL;
}