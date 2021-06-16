#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct tree
{
    char tictac[3][3];
    int level, weight, children;
    int trueOrFalse;
    struct tree *array[20];
} tree;

typedef struct parsing
{
    int flag;
    int val;
} parsing;

typedef struct queue
{
    struct tree *node;
    struct queue *next;
} queue;

queue *front, *rear;

// common queue functions

void initQ()
{
    front = NULL;
    rear = NULL;
}

int isEmpty()
{
    return front == NULL;
}

void enqueue(struct tree *node)
{
    queue *tmp = (queue *)malloc(sizeof(queue));
    tmp->next = NULL;
    tmp->node = node;
    if (front == NULL && rear == NULL)
    {
        front = tmp;
        rear = tmp;
    }
    rear->next = tmp;
    rear = tmp;
}

struct tree *dequeue()
{
    queue *tmp = front;
    tree *node;
    if (tmp == NULL)
    {
        return NULL;
    }
    node = tmp->node;
    if (front == rear)
    {
        front = NULL;
        rear = NULL;
    }
    else
    {
        front = front->next;
    }
    free(tmp);
    return node;
}

//  -------------------------
// | FUNCTIONS FOR PROBLEM 1 |
//  -------------------------

// read function
void read(char *input, int pb, char *turn, char tictac[][3])
{
    //printf("Read file %s\n", input);
    FILE *f;
    f = fopen(input, "r");
    if (!f)
    {
        printf("Could not open file!\n");
    }

    char s[8];
    int i, j;

    if (pb == 1)
    {
        fgets(s, 7, f);
        *turn = s[0];

        for (i = 0; i < 3; i++)
        {
            fgets(s, 8, f);
            for (j = 0; j < 3; j++)
            {
                tictac[i][j] = s[j * 2];
            }
        }
    }

    fclose(f);
}

// dealloc function
void freeTree(tree *node)
{
    int i = 0;

    while (node != NULL && node->array[i] != NULL)
    {
        freeTree(node->array[i++]);
    }
    free(node);
}

// write to file function
void printTicTac(char tictac[][3], int level, FILE *f)
{
    int i, j;
    for (i = 0; i < 3; i++)
    {

        // generate TAB's according to level
        for (j = 0; j < level; j++)
        {
            fprintf(f, "\t");
        }
        for (j = 0; j < 3; j++)
        {
            fprintf(f, "%c", tictac[i][j]);
            if (j != 2)
            {
                fprintf(f, " ");
            }
        }
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
}

void printTree(tree *node, FILE *f)
{
    int i = 0;

    printTicTac(node->tictac, node->level, f);

    while (node != NULL && node->array[i] != NULL)
    {
        printTree(node->array[i++], f);
    }
}

// checks current level by counting empty spaces
int checkLvl(char tictac[][3])
{
    int i, j, level = 0;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (tictac[i][j] == '-')
            {
                level++;
            }
        }
    }
    return level;
}

int checkWin(char tictac[][3])
{
    int i;
    for (i = 0; i < 3; i++)
    {
        if (tictac[i][0] != '-' && tictac[i][0] == tictac[i][1] && tictac[i][0] == tictac[i][2])
        {
            return 1;
        }
        if (tictac[0][i] != '-' && tictac[0][i] == tictac[1][i] && tictac[0][i] == tictac[2][i])
        {
            return 1;
        }
    }
    if (tictac[0][0] != '-' && tictac[0][0] == tictac[1][1] && tictac[0][0] == tictac[2][2])
    {
        return 1;
    }
    if (tictac[0][2] != '-' && tictac[0][2] == tictac[1][1] && tictac[0][2] == tictac[2][0])
    {
        return 1;
    }
    return 0;
}

// makes a move on the board at position pos, as player
// if pos = -1, it builds the root board
void tictacMaker(char where[][3], char from[][3], int pos, char player)
{
    // when applied to root, with poz = -1, it doesnt make a move

    int i, j, count = 0;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (from[i][j] == '-')
            {
                count++;
            }
            if (count == pos)
            {
                where[i][j] = player;
                count++;
            }
            else
            {
                where[i][j] = from[i][j];
            }
        }
    }
}

// allocates new tree node
struct tree *newNode()
{
    tree *temp = (tree *)malloc(sizeof(tree));
    int i;
    for (i = 0; i < 20; i++)
    {
        temp->array[i] = NULL;
    }
    return temp;
}

// builds game tree
struct tree *insert(char parentTictac[][3], int level, int pos, char player, int totalLvls)
{
    tree *temp = newNode();
    temp->level = totalLvls - level;
    int i;

    tictacMaker(temp->tictac, parentTictac, pos, player);

    if (checkWin(temp->tictac) == 1)
    {
        return temp;
    }

    player = (player == 'X') ? 'O' : 'X';

    for (i = 0; i < level; i++)
    {
        temp->array[i] = insert(temp->tictac, level - 1, i + 1, player, totalLvls);
    }

    return temp;
}

//  -------------------------
// | FUNCTIONS FOR PROBLEM 2 |
//  -------------------------

struct tree *insert2(char parentTictac[][3], int level, int pos, char player, int totalLvls, char winner)
{
    tree *temp = newNode();
    temp->level = totalLvls - level;

    int i;

    tictacMaker(temp->tictac, parentTictac, pos, player);

    player = (player == 'X') ? 'O' : 'X';

    if (winner == player)
    {
        temp->weight = 1;
    }
    else
    {
        temp->weight = 0;
    }

    if (checkWin(temp->tictac) == 1)
    {
        if (winner != player)
        {
            temp->trueOrFalse = 1;
        }
        else
        {
            temp->trueOrFalse = 0;
        }
        temp->children = 0;
        return temp;
    }
    else
    {
        temp->trueOrFalse = 0;
        temp->children = level;
    }

    for (i = 0; i < level; i++)
    {
        temp->array[i] = insert2(temp->tictac, level - 1, i + 1, player, totalLvls, winner);
    }

    return temp;
}

int TorF(struct tree **node)
{
    int i = 0, a, aux;

    if ((*node)->weight == 1)
    {
        aux = 0;
    }
    else
    {
        aux = 1;
    }

    while (*node != NULL && i < (*node)->children)
    {
        a = TorF(&(*node)->array[i++]);

        if ((*node)->weight == 1)
        {
            aux = aux || a;
        }
        else
        {
            aux = aux && a;
        }
    }
    if ((*node)->children != 0)
    {
        (*node)->trueOrFalse = aux;
    }

    return (*node)->trueOrFalse;
}

void printTorF(tree *node, FILE *f)
{
    int i;

    for (i = 0; i < node->level; i++)
    {
        fprintf(f, "\t");
    }
    // printf("level %d, weight %d\n", node->level, node->weight);

    if (node->trueOrFalse == 0)
    {
        fprintf(f, "F\n");
    }
    else
    {
        fprintf(f, "T\n");
    }

    i = 0;

    while (node != NULL && node->array[i] != NULL)
    {
        printTorF(node->array[i++], f);
    }
}

//  -------------------------
// | FUNCTIONS FOR PROBLEM 3 |
//  -------------------------

void freeTreeMin(tree *node)
{
    int i = 0;

    while (node != NULL && i < (*node).children)
    {
        freeTree(node->array[i++]);
    }
    free(node);
}

// parse text from input file
// flag = 1  =>  parsing number of children
// flag = 0  =>  parsing leaf value
struct parsing *parse(FILE *f)
{
    parsing *temp = (parsing *)malloc(sizeof(parsing));
    int number = 0, sign = 1;
    char str = '\0';

    while (1)
    {
        fscanf(f, "%c", &str);

        if (str == '-')
        {
            sign = -1;
        }
        if (str == '(')
        {
            temp->flag = 1;
        }
        if (str == '[')
        {
            temp->flag = 0;
        }
        if (str == ')' || str == ']')
        {
            fscanf(f, "%c", &str);
            break;
        }
        if (isdigit(str))
        {
            number = number * 10 + (str - 48);
        }
    }
    temp->val = sign * number;
    // printf("number: %d\n", temp->val);
    return temp;
}

// build tree and apply algorithm
struct tree *minimaxTreeBuilder(FILE *f)
{
    tree *root = (tree *)malloc(sizeof(tree));
    root->level = 0;
    parsing *read = parse(f);
    root->children = read->val;
    free(read);

    tree *tmp;
    int i;

    initQ();
    enqueue(root);

    while (isEmpty() == 0)
    {
        tmp = dequeue();
        for (i = 0; i < tmp->children; i++)
        {
            tmp->array[i] = (tree *)malloc(sizeof(tree));
            tmp->array[i]->level = tmp->level + 1;
            parsing *read = parse(f);

            if (read->flag == 1)
            {
                tmp->array[i]->weight = 0;
                tmp->array[i]->children = read->val;
            }
            else
            {
                tmp->array[i]->weight = read->val;
                tmp->array[i]->children = 0;
            }
            free(read);
            enqueue(tmp->array[i]);
        }
    }
    return root;
}

int minimax(struct tree **node)
{
    int i = 0, a, min = __INT16_MAX__, max = -min;

    while (*node != NULL && i < (*node)->children)
    {
        a = minimax(&(*node)->array[i++]);

        // if level is even, select minimum
        // else, select maximum
        if ((*node)->level % 2 == 1)
        {
            if (a < min)
            {
                min = a;
            }
        }
        else
        {
            if (a > max)
            {
                max = a;
            }
        }
    }
    if ((*node)->children != 0)
    {
        (*node)->weight = (max == -__INT16_MAX__) ? min : max;
        // printf("leaf: %d\n", (*node)->weight);
    }

    return (*node)->weight;
}

void printMinimax(tree *node, FILE *f)
{
    int i;

    for (i = 0; i < node->level; i++)
    {
        fprintf(f, "\t");
    }
    // printf("level %d, weight %d\n", node->level, node->weight);
    fprintf(f, "%d\n", node->weight);
    i = 0;

    while (node != NULL && i < (*node).children)
    {
        printMinimax(node->array[i++], f);
    }
}

//  -------------------------
// | MAIN PROBLEM FUNCTIONS  |
//  -------------------------

// pb no 1
void ProbOne(char *input, char *output)
{
    int level;
    char player, tictac[3][3];

    // read board and player from input file
    read(input, 1, &player, tictac);

    // choose other player, it will change back when root is initiated
    player = (player == 'X') ? 'O' : 'X';

    // check how many empty spaces
    level = checkLvl(tictac);

    // send -1 as pos so that tictac move maker doesnt add move on root
    tree *root = insert(tictac, level, -1, player, level);

    FILE *f;
    f = fopen(output, "w");

    printTree(root, f);

    fclose(f);
    freeTree(root);
}

// pb no 2
void ProbTwo(char *input, char *output)
{
    int level;
    char player, tictac[3][3], winner;

    // read board and player from input file
    read(input, 1, &player, tictac);

    winner = player;

    // choose other player, it will change back when root is initiated
    player = (player == 'X') ? 'O' : 'X';

    // check how many empty spaces
    level = checkLvl(tictac);

    // send -1 as pos so that tictac move maker doesnt add move on root
    tree *root = insert2(tictac, level, -1, player, level, winner);

    TorF(&root);

    FILE *f;
    f = fopen(output, "w");

    printTorF(root, f);
    //printTree(root, f);

    fclose(f);
    freeTree(root);
}

// pb no 3
void ProbThree(char *input, char *output)
{
    FILE *f = fopen(input, "r");

    char levels[3];
    fgets(levels, 3, f);

    tree *root = minimaxTreeBuilder(f);
    fclose(f);

    minimax(&root);

    f = fopen(output, "w");
    printMinimax(root, f);
    fclose(f);

    freeTreeMin(root);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Insufficient arguments! (%d)\n", argc);
        return 0;
    }

    // select problem
    {
        if (strcmp(argv[1], "-c1") == 0)
        {
            ProbOne(argv[2], argv[3]);
        }
        if (strcmp(argv[1], "-c2") == 0)
        {
            ProbTwo(argv[2], argv[3]);
        }
        if (strcmp(argv[1], "-c3") == 0)
        {
            ProbThree(argv[2], argv[3]);
        }
    }

    return 0;
}