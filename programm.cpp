#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "myVector.h"
#include "myString.h"
#include <ctype.h>
#include "city.h"
#include "distance.h"
#include "map.h"
#include "Neighboor.h"
#include "hashmap.h"

#define MEMORYBACKUP 10

/*Этот код представляет функцию sortNeighboor, которая сортирует массив структур Neighboor по возрастанию значения поля dist (расстояние).*/
void sortNeighboor(Neighboor* dist, int n) {
    Neighboor key;
    int j;
    for (int i = 1; i < n; i++) {
        key = dist[i];
        for (j = i - 1; j >= 0 && dist[j].dist > key.dist; j--) {
            dist[j + 1] = dist[j];
        }
        dist[j + 1] = key;
    }
}

/*Проверяет является ли полем*/
bool is_valid(int x, int y, Map& map) {
    return (x >= 0 && x < map.width&& y >= 0 && y < map.height&& map.map[y][x] == '#');
}

// Этот код представляет функцию bfs, которая реализует алгоритм поиска в ширину (BFS) для нахождения кратчайшего пути на карте.
// Funkcja BFS do szukania najkrótszej drogi na mapie
int bfs(Map& map, int start_x, int start_y, int end_x, int end_y, bool** visited) {

    // Przeszukiwanie BFS
    myVector* queue = new_vector();
    int front = 0, rear = -1;
    visited[start_y][start_x] = true;
    bool skip = false;

    Neighboor* start = (Neighboor*)malloc(sizeof(Neighboor));
    start->x = start_x;
    start->y = start_y;
    start->dist = 0;

    vector_push(queue, start);
    rear++;

    while (front <= rear) {
        int dist[4];
        int countDist = 0;
        Neighboor* current = (Neighboor*)vector_get(queue, front);
        if (current->x == end_x && current->y == end_y) return 0;
        front++;

        // Sprawdzanie węzłów sąsiednich
        int next_x, next_y;

        Neighboor neigh[4];

        // Węzeł po lewej
        next_x = current->x - 1;
        next_y = current->y;
        if (next_x == end_x && next_y == end_y) {
            return current->dist + 1;
        }
        if (is_valid(next_x, next_y, map) && visited[next_y][next_x] == false) {
            visited[next_y][next_x] = true;
            /*
            вычисляется расстояние до целевого узла dist[countDist],
            и структура Neighboor с данными соседнего узла добавляется в массив neigh и инкрементируется countDist
            */
            dist[countDist] = ((next_x - end_x) * (next_x - end_x) + (next_y - end_y) * (next_y - end_y));
            neigh[countDist] = { next_x, next_y, dist[countDist] };
            countDist++;
        }


        // Węzeł po prawej
        next_x = current->x + 1;
        next_y = current->y;
        if (next_x == end_x && next_y == end_y) {
            return current->dist + 1;
        }
        if (is_valid(next_x, next_y, map) && visited[next_y][next_x] == false) {
            visited[next_y][next_x] = true;
            /*
            вычисляется расстояние до целевого узла dist[countDist],
            и структура Neighboor с данными соседнего узла добавляется в массив neigh и инкрементируется countDist
            */
            dist[countDist] = ((next_x - end_x) * (next_x - end_x) + (next_y - end_y) * (next_y - end_y));
            neigh[countDist] = { next_x, next_y, dist[countDist] };
            countDist++;
        }

        // Węzeł u góry
        next_x = current->x;
        next_y = current->y - 1;
        if (next_x == end_x && next_y == end_y) {
            return current->dist + 1;
        }
        if (is_valid(next_x, next_y, map) && visited[next_y][next_x] == false) {
            visited[next_y][next_x] = true;
            /*
            вычисляется расстояние до целевого узла dist[countDist],
            и структура Neighboor с данными соседнего узла добавляется в массив neigh и инкрементируется countDist
            */
            dist[countDist] = ((next_x - end_x) * (next_x - end_x) + (next_y - end_y) * (next_y - end_y));
            neigh[countDist] = { next_x, next_y, dist[countDist] };
            countDist++;
        }

        // Węzeł na dole
        next_x = current->x;
        next_y = current->y + 1;
        if (next_x == end_x && next_y == end_y) {
            return current->dist + 1;
        }
        if (is_valid(next_x, next_y, map) && visited[next_y][next_x] == false) {
            visited[next_y][next_x] = true;
            /*
            вычисляется расстояние до целевого узла dist[countDist],
            и структура Neighboor с данными соседнего узла добавляется в массив neigh и инкрементируется countDist
            */
            dist[countDist] = ((next_x - end_x) * (next_x - end_x) + (next_y - end_y) * (next_y - end_y));
            neigh[countDist] = { next_x, next_y, dist[countDist] };
            countDist++;
        }

        sortNeighboor(neigh, countDist);
        for (int i = 0; i < countDist; i++) {
            rear++;
            Neighboor* temp = (Neighboor*)malloc(sizeof(Neighboor));
            temp->x = neigh[i].x;
            temp->y = neigh[i].y;
            temp->dist = current->dist + 1;
            vector_push(queue, temp);
        }
    }
    free(start);
    return -1; // Brak drogi do końcowego wierzchołka
    
}

/*Вчитывание карты, нахождение "*" и поиск вокруг неё соседей */
/*Записывание идёт в такой способ что вчитываем 2-3 первые строки, а потом начинаем заново вчитывать что бы найти звезду!*/
void loadFromConsole(Map& map, myVector* cityVect, HashMap* hmap)
{
    char c;
    bool skip;
    bool direction = false;
    // Wczytaj szerokość i wysokość mapy
    std::cin >> map.width >> map.height;

    // Alokuj pamięć na mapę
    map.map = new char* [map.height];
    for (int i = 0; i < map.height; i++)
    {
        map.map[i] = new char[map.width + 1];
    }

    // Wczytaj każdy wiersz mapy
    for (int i = 0; i < map.height + 2; i++)
    {
        for (int j = 0; j < map.width; j++)
        {
            if (i < map.height) {
                char ch = getchar();
                if (ch == '\n') {
                    j--;
                    continue;
                }
                map.map[i][j] = ch;
                map.map[i][map.width] = '\0';
            }
        }
        if (i > 1) {
            for (int j = 0; j < map.width; j++) {
                if (map.map[i - 2][j] == '*') {
                    City* city = new City;
                    MyString_init(&city->name);
                    skip = false;
                    direction = false;
                    city->x = j;
                    city->y = i - 2;
                    for (int k = i - 3; k < i; k++) {
                        if (k >= map.height || k < 0)
                        {
                            continue;
                        }
                        for (int g = j - 1; g < j + 2; g++) {
                            if (g >= map.width || g < 0) {
                                continue;
                            }
                            if (isalnum(map.map[k][g])) {
                                int q = 0;
                                if (g + q >= 1 && g + q < map.width - 1) {
                                    if (isalnum(map.map[k][g + q - 1]) && isalnum(map.map[k][g + q + 1])) continue;
                                    if (isalnum(map.map[k][g + q - 1])) direction = true;
                                }
                                if (g + q == map.width - 1) direction = true;
                                while (isalnum(map.map[k][g + q])) {
                                    MyString_append_char(&city->name, map.map[k][g + q]);
                                    if (direction) q--;
                                    else q++;
                                }
                                if (direction) {
                                    MyString_reverse(&city->name);
                                }
                                city->id = cityVect->size;
                                city->distance = new_vector();
                                //dodajemy do naszego vectora
                                vector_push(cityVect, city);
                                //dodejemy do naszej hashmapy
                                hashmap_put(hmap, city->name.data_, city);
                                skip = true;
                            }
                            if (skip) break;
                        }
                        if (skip) break;
                    }
                }
            }
        }
    }

    // Здесь просто вчитываем кол-во самолётов и команды которые мы должны сделать
    int numOfDist;
    std::cin >> numOfDist;

    for (int i = 0; i < numOfDist; i++)
    {
        Distance* distantion = new Distance;
        MyString_init(&distantion->start);
        MyString_init(&distantion->finish);
        while (true)
        {
            c = getchar();
            if (c == ' ') {
                break;
            }
            else if (c != '\n') {
                MyString_append_char(&distantion->start, c);
            }
        }
        while (true)
        {
            c = getchar();
            if (c == ' ') {
                break;
            }
            else if (c != '\n') {
                MyString_append_char(&distantion->finish, c);
            }
        }
        std::cin >> distantion->dist;
        City* tempA = hashmap_get(hmap, distantion->start.data_);
        vector_push(tempA->distance, distantion);
    }
}

// Этот код представляет функцию runCities, которая выполняет обход по городам и вычисляет расстояния между ними.
void runCities(myVector cityVect, bool** visited) {
    bool skip = true;
    for (int i = 0; i < cityVect.size; i++)
    {
        City* CityA = (City*)vector_get(&cityVect, i);
        int dx[4] = { -1, 0, 1, 0 };
        int dy[4] = { 0, -1, 0, 1 };
        for (int t = 0; t < 4; t++) {
            int x = CityA->x + dx[t];
            int y = CityA->y + dy[t];
            if (x < 0 || x >= map.width || y < 0 || y >= map.height) continue;
            if (map.map[y][x] == '*' || map.map[y][x] == '#') skip = false;
        }
        if (skip) continue;
        for (int j = i; j < cityVect.size; j++)
        {
            skip = true;
            for (int i = 0; i < map.height; i++)
            {
                for (int j = 0; j < map.width; j++) {
                    visited[i][j] = false;
                }
            }
            City* CityB = (City*)vector_get(&cityVect, j);
            for (int r = 0; r < 4; r++) {
                int x = CityB->x + dx[r];
                int y = CityB->y + dy[r];
                if (x < 0 || x >= map.width || y < 0 || y >= map.height) continue;
                if (map.map[y][x] == '*' || map.map[y][x] == '#') skip = false;
            }
            if (skip) continue;
            int timeDist = bfs(map, CityA->x, CityA->y, CityB->x, CityB->y, visited);
            Distance* dist = new Distance;
            dist->start = CityA->name;
            dist->finish = CityB->name;
            dist->dist = timeDist;
            if (timeDist != -1) {
                vector_push(CityA->distance, dist);
                Distance* distReverse = new Distance;
                distReverse->start = CityB->name;
                distReverse->finish = CityA->name;
                distReverse->dist = timeDist;
                vector_push(CityB->distance, distReverse);
            }
        }
    }
}

// Этот код представляет функцию dijkstra, которая реализует алгоритм Дейкстры для нахождения кратчайшего
// пути между двумя городами в графе.
void dijkstra(int CityStart, int CityEnd, myVector cityVect, int num, HashMap* hmap) {
    // Inicjalizacja zmiennych
    int* dist = (int*)malloc(cityVect.size * sizeof(int) + MEMORYBACKUP);// Tablica odległości od wierzchołka startowego
    int* prev = (int*)malloc(cityVect.size * sizeof(int) + MEMORYBACKUP);
    bool* visited = (bool*)malloc(cityVect.size * sizeof(bool) + MEMORYBACKUP);
    int front = 0; // Zmienna wskazująca na początek kolejki
    int rear = 0; // Zmienna wskazująca na koniec kolejki
    for (int i = 0; i < cityVect.size; i++) {
        dist[i] = INT_MAX; // Ustawienie początkowej wartości odległości na nieskończoność
        prev[i] = -1; // Ustawienie początkowego poprzednika na -1 (brak poprzednika)
        visited[i] = false; // Ustawienie początkowej wartości odwiedzenia na false
    }
    dist[CityStart] = 0; // Odległość od wierzchołka startowego do niego samego wynosi 0

    // Dodanie wierzchołka startowego do kolejki
    int* queue = (int*)malloc(cityVect.size * sizeof(int) + MEMORYBACKUP); // Kolejka z wierzchołkami do przetworzenia
    queue[rear++] = CityStart;
    visited[CityStart] = true;

    // Przetwarzanie wierzchołków w kolejce
    while (front != rear) { // Dopóki kolejka nie jest pusta
        int curr_index = queue[front++]; // Pobranie pierwszego wierzchołka z kolejki
        visited[curr_index] = false;

        // Przetwarzanie sąsiadów
        City* temp = (City*)vector_get(&cityVect, curr_index);
        for (int j = 0; j < temp->distance->size; j++) {
            Distance* curr_distance = (Distance*)vector_get(temp->distance, j);
            City* tempA = hashmap_get(hmap, curr_distance->start.data_);
            City* tempB = hashmap_get(hmap, curr_distance->finish.data_);
            int index_A = tempA->id;
            int index_B = tempB->id;
            int time = curr_distance->dist;
            if (dist[index_A] != INT_MAX && dist[index_A] + time < dist[index_B]) {
                dist[index_B] = dist[index_A] + time;
                prev[index_B] = index_A;
                if (!visited[index_B]) { // Jeśli wierzchołek nie był jeszcze przetwarzany
                    queue[rear++] = index_B; // Dodaj wierzchołek do kolejki
                    visited[index_B] = true;
                }
            }
        }
    }
    if (num == 0) {
        std::cout << dist[CityEnd] << std::endl;
        free(queue);
        free(visited);
        free(prev);
        free(dist);
        return;
    }
    if (num == 1) {
        std::cout << dist[CityEnd];
        int curr_city = CityEnd;
        myVector cityName = *new_vector();
        while (curr_city != -1) {
            City* city = (City*)vector_get(&cityVect, curr_city);
            if (curr_city != CityStart && curr_city != CityEnd) {
                vector_push(&cityName, city->name.data_);
            }
            curr_city = prev[curr_city];
        }
        for (int j = cityName.size - 1; j >= 0; j--) {
            char* name = (char*)vector_get(&cityName, j);
            std::cout << " " << name;
        }
        std::cout << std::endl;
        free(queue);
        free(visited);
        free(prev);
        free(dist);
        return;
        
    }
    free(queue);
    free(visited);
    free(prev);
    free(dist);
}

// Вчитывание комманд
void Commands(myVector cityVect, HashMap* hmap) {
    int numCommands;
    int num;
    MyString CityA;
    MyString CityB;
    char c;
    std::cin >> numCommands;
    for (int i = 0; i < numCommands; i++)
    {
        MyString_init(&CityA);
        MyString_init(&CityB);
        while (true)
        {
            c = getchar();
            if (c == ' ') {
                break;
            }
            else if (c != '\n') {
                MyString_append_char(&CityA, c);
            }

        }
        while (true)
        {
            c = getchar();
            if (c == ' ') {
                break;
            }
            else if (c != '\n') {
                MyString_append_char(&CityB, c);
            }
        }
        std::cin >> num;
        City* tempA = hashmap_get(hmap, CityA.data_);
        City* tempB = hashmap_get(hmap, CityB.data_);
        dijkstra(tempA->id, tempB->id, cityVect, num, hmap);
    }
    MyString_destroy(&CityA);
    MyString_destroy(&CityB);
}

int main() {
    myVector* cityVect = new_vector();
    HashMap* hmap = new_hashmap(500000);
    loadFromConsole(map, cityVect, hmap);
    bool** visited = new bool* [map.height];
    for (int i = 0; i < map.height; i++)
    {
        visited[i] = new bool[map.width];
        for (int j = 0; j < map.width; j++) {
            visited[i][j] = false;
        }
    }
    runCities(*cityVect, visited);
    Commands(*cityVect, hmap);
    return 0;
}