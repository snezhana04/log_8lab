#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <queue>
#include <vector>
#include <list>

typedef struct QNode {
    int vertex;
    struct QNode* next;
} QNode;

typedef struct {
    QNode* head;
    QNode* tail;
    int size;
} QueueList;

// инициализируем очередь
void queueInit(QueueList* q) {
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

// добавляем новую вершину в конец очереди
int queuePush(QueueList* q, int v) {
    QNode* node = (QNode*)malloc(sizeof(QNode));
    if (node == NULL) return 0;
    node->vertex = v;
    node->next = NULL;
    if (q->tail == NULL) {
        q->head = q->tail = node;
    }
    else {
        q->tail->next = node;
        q->tail = node;
    }
    q->size++;
    return 1;
}
//извлекаем вершину из конца
int queuePop(QueueList* q, int* v) {
    if (q->head == NULL) return 0;
    QNode* tmp = q->head;
    *v = tmp->vertex;
    q->head = tmp->next;
    if (q->head == NULL) q->tail = NULL;
    free(tmp);
    q->size--;
    return 1;
}

int queueIsEmpty(QueueList* q) {
    return q->head == NULL;
}

void queueFree(QueueList* q) {
    int dummy;
    while (queuePop(q, &dummy));
}

void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//создаем матрицу
int** createMatrix(int n) {
    int** A = (int**)malloc(n * sizeof(int*));
    if (A == NULL) return NULL;
    for (int i = 0; i < n; i++) {
        A[i] = (int*)calloc(n, sizeof(int));
        if (A[i] == NULL) {
            for (int j = 0; j < i; j++) free(A[j]);
            free(A);
            return NULL;
        }
    }
    return A;
}

//освобождаем под нее память
void freeMatrix(int** A, int n) {
    if (A == NULL) return;
    for (int i = 0; i < n; i++) free(A[i]);
    free(A);
}


void printMatrix(int** A, int n) {
    printf("    ");
    for (int j = 0; j < n; j++) printf("x%-3d", j + 1);
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("x%-2d ", i + 1);
        for (int j = 0; j < n; j++) printf("%-4d", A[i][j]);
        printf("\n");
    }
}

void randomFill(int** A, int n, double p) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            double r = (double)rand() / (double)RAND_MAX;
            if (r < p) {
                A[i][j] = 1;
                A[j][i] = 1;
            }
        }
    }
}

//создаем списки смежности
std::vector<std::list<int>> createAdjacencyList(int** A, int n) {
    std::vector<std::list<int>> list(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (A[i][j] == 1) {
                list[i].push_back(j);
            }
        }
    }

    return list;
}

void printAdjacencyList(const std::vector<std::list<int>>& list, int n) {
    for (int i = 0; i < n; i++) {
        printf("x%d:", i + 1);

        if (list[i].empty()) {
            printf(" нет смежных вершин");
        }

        for (int vertex : list[i]) {
            printf(" x%d", vertex + 1);
        }

        printf("\\n");
    }
}

//матрица
double bfsMatrix(int** A, int n, int start, int showOrder) {
    int* visited = (int*)calloc(n, sizeof(int));

    if (visited == NULL) {
        printf("Не удалось выделить память.\n");
        return -1.0;
    }

    if (showOrder) printf("Порядок посещения: ");
    clock_t startTime = clock();

    for (int k = -1; k < n; k++) {
        int currentStart = (k == -1) ? start : k;
        if (visited[currentStart]) continue;

        std::queue<int> queue;
        visited[currentStart] = 1;
        queue.push(currentStart);
        if (showOrder) printf("x%d ", currentStart + 1);

        while (!queue.empty()) {
            int v = queue.front();
            queue.pop();
            for (int j = 0; j < n; j++) {
                if (A[v][j] == 1 && !visited[j]) {
                    visited[j] = 1;
                    queue.push(j);
                    if (showOrder) printf("x%d ", j + 1);
                }
            }
        }
    }

    clock_t endTime = clock();
    double time = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    if (showOrder) printf("\nВремя выполнения: %.6f секунд\n", time);

    free(visited);
    return time;
}

//список смежности, очередь
double bfsList(const std::vector<std::list<int>>& list, int n, int start, int showOrder) {
    int* visited = (int*)calloc(n, sizeof(int));

    if (visited == NULL) {
        printf("Не удалось выделить память.\n");
        return -1.0;
    }

    if (showOrder) printf("Порядок посещения: ");
    clock_t startTime = clock();

    for (int k = -1; k < n; k++) {
        int currentStart = (k == -1) ? start : k;
        if (visited[currentStart]) continue;

        std::queue<int> queue;
        visited[currentStart] = 1;
        queue.push(currentStart);
        if (showOrder) printf("x%d ", currentStart + 1);

        while (!queue.empty()) {
            int v = queue.front();
            queue.pop();
            for (int u : list[v]) {
                if (!visited[u]) {
                    visited[u] = 1;
                    queue.push(u);
                    if (showOrder) printf("x%d ", u + 1);
                }
            }
        }
    }

    clock_t endTime = clock();
    double time = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    if (showOrder) printf("\nВремя выполнения: %.6f секунд\n", time);

    free(visited);
    return time;
}

//матрица, собственная очередь
double bfsMatrixCustomQueue(int** A, int n, int start, int showOrder) {
    int* visited = (int*)calloc(n, sizeof(int));
    if (visited == NULL) {
        printf("Не удалось выделить память.\n");
        return -1.0;
    }

    QueueList q;
    queueInit(&q);

    if (showOrder) printf("Порядок посещения: ");
    clock_t startTime = clock();

    for (int k = -1; k < n; k++) {
        int currentStart = (k == -1) ? start : k;
        if (visited[currentStart]) continue;

        visited[currentStart] = 1;
        queuePush(&q, currentStart);
        if (showOrder) printf("x%d ", currentStart + 1);

        while (!queueIsEmpty(&q)) {
            int v;
            queuePop(&q, &v);
            for (int j = 0; j < n; j++) {
                if (A[v][j] == 1 && !visited[j]) {
                    visited[j] = 1;
                    queuePush(&q, j);
                    if (showOrder) printf("x%d ", j + 1);
                }
            }
        }
    }

    clock_t endTime = clock();
    double time = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    if (showOrder) printf("\nВремя выполнения: %.6f секунд\n", time);

    queueFree(&q);
    free(visited);
    return time;
}


void printGraphInfo(int** A, int n, const std::vector<std::list<int>>& list) {
    printf("Матрица смежности графа G:\n");
    printMatrix(A, n);
    printf("\nСписки смежности графа G:\n");
    printAdjacencyList(list, n);
    printf("\n");
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand((unsigned)time(NULL));

    int** A = NULL;
    int n = 0;
    int running = 1;

    while (running) {
        int choice;

        printf("\nГлавное меню:\n");
        printf("1 - Создать граф и вывести матрицу смежности\n");
        printf("2 - Обход в ширину по матрице (стандартная queue)\n");
        printf("3 - Обход в ширину по спискам смежности (стандартная queue)\n");
        printf("4 - Обход в ширину по матрице (своя очередь-список)\n");
        printf("5 - Сравнить время выполнения трех видов обхода BFS\n");
        printf("0 - Выход\n");
        printf("Ваш выбор: ");

        if (scanf_s("%d", &choice) != 1) {
            printf("Ошибка ввода.\n");
            clearInput();
            continue;
        }

        switch (choice) {

        case 1: {
            int newN;
            double p;

            printf("Введите количество вершин: ");
            if (scanf_s("%d", &newN) != 1) {
                printf("Ошибка ввода.\n");
                clearInput();
                break;
            }

            printf("Введите вероятность появления ребра (от 0 до 1): ");
            if (scanf_s("%lf", &p) != 1) {
                printf("Ошибка ввода.\n");
                clearInput();
                break;
            }

            if (newN <= 0 || p < 0 || p > 1) {
                printf("Неверные данные.\n");
                break;
            }

            int** B = createMatrix(newN);
            if (B == NULL) {
                printf("Не удалось выделить память.\n");
                break;
            }

            randomFill(B, newN, p);

            freeMatrix(A, n);
            A = B;
            n = newN;

            printf("\nМатрица смежности графа G:\n");
            printMatrix(A, n);

            break;
        }

        case 2:
        case 3:
        case 4: {
            system("cls");

            if (A == NULL) {
                printf("Сначала создайте граф с помощью пункта 1.\n");
                break;
            }

            std::vector<std::list<int>> list = createAdjacencyList(A, n);

            printGraphInfo(A, n, list);

            int start;
            printf("\nВведите начальную вершину (1-%d): ", n);

            if (scanf_s("%d", &start) != 1) {
                printf("Ошибка ввода.\n");
                clearInput();
                break;
            }

            if (start < 1 || start > n) {
                printf("Неверный номер вершины.\n");
                break;
            }

            if (choice == 2) {
                printf("\nОбход в ширину по матрице :\n");
                bfsMatrix(A, n, start - 1, 1);
            }
            else if (choice == 3) {
                printf("\nОбход в ширину по спискам смежности:\n");
                bfsList(list, n, start - 1, 1);
            }
            else if (choice == 4) {
                printf("\nОбход в ширину по матрице (своя очередь-список):\n");
                bfsMatrixCustomQueue(A, n, start - 1, 1);
            }
            break;
        }

        case 5: {
            if (A == NULL) {
                printf("Сначала создайте граф с помощью пункта 1.\n");
                break;
            }

            std::vector<std::list<int>> list = createAdjacencyList(A, n);

            int start;
            printf("Введите начальную вершину (1-%d): ", n);

            if (scanf_s("%d", &start) != 1) {
                printf("Ошибка ввода.\n");
                clearInput();
                break;
            }

            if (start < 1 || start > n) {
                printf("Неверный номер вершины.\n");
                break;
            }

            double t1 = bfsMatrix(A, n, start - 1, 0);
            double t2 = bfsList(list, n, start - 1, 0);
            double t3 = bfsMatrixCustomQueue(A, n, start - 1, 0);

            printf("\nСравнение времени выполнения трех видов обхода BFS:\n");
            printf("---------------------------------------------------------------\n");
            printf("| Способ обхода                         | Время (сек.)       |\n");
            printf("---------------------------------------------------------------\n");
            printf("| Матрица смежности + стандартная queue | %18.10f |\n", t1);
            printf("| Список смежности + стандартная queue  | %18.10f |\n", t2);
            printf("| Матрица + собственная очередь-список  | %18.10f |\n", t3);
            printf("---------------------------------------------------------------\n");
            break;
        }

        case 0:
            running = 0;
            break;

        default:
            printf("Неверный пункт меню.\n");
            break;
        }

        if (running) {
            printf("\nНажмите Enter для возврата в меню...");
            while (getchar() != '\n');
            getchar();
            system("cls");
        }
    }

    freeMatrix(A, n);
    printf("Программа завершена.\n");
    return 0;
}