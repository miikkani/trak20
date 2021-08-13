/* Count frequency of words.
 *
 * Binary search tree for counting word frequency.
 * Quicksort for sorting frequencies.
 *
 * Total complexity  O = n*lg(n), from sorting algorithm as
 * searching from tree is way faster O = lg(n).
 * 
 * Note: Using quicksort for lots of duplicate values gets easily 
 * to O(n^2). For now seems that partition gives mostly worst case
 * split for text material.
 * 
 * TODO: Use HOARE for qsort partition() to increase
 * performance with duplicate values
 *
 *
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100 /* max length of word */
#define TOPNUMBER 20 /* length of list printed */

/* functions */
struct node* add(struct node*, char*);
int treesize(struct node* root);
void tprint(struct node*);
int toarray(struct node*, struct node*[], int); 
void pikasort(struct node*[], int, int);
void three_pikasort(struct node*[], int, int);
void swap(struct node*[], int , int);


void recur(int x) {
    ++x;
    // printf("%d\n",x);
    recur(x);
}

/* node structure */
struct node {
    struct node* left;
    struct node* right;
    char* word;
    int count;
};

int recursion = 0;
int depth = 0;
int calls = 0;
int swaps = 0;

/* main program */
int main(int argc, char* argv[])
{
    // recur(0);
    
    int c;
    int len;
    int words;
    int longest;
    int toplist;
    int i, j, k, ia;
    char word[MAX];
    char lword[MAX];
    FILE* file;
    struct node* root;
    struct node** array;
    int nodes;

    words = 0;
    len = 0;
    longest = 0; /* length of longest word */
    


    printf(" .-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
    printf(" PROCESSING...");

    /* open file from argument */
    if((file = fopen(*++argv, "r")) == NULL){
        printf("Usage: program <filename>\n");
        return 1;
    }

    /* collect and count words */
    root = NULL;
    c = getc(file);
    i = 0;
    do {
        // if(c > 127 ) printf("\n\nchar lolz: %d\n\n", c);
        if( (c >= 'a'&& c <= 'z')
            || (c >= 'A'&& c <= 'Z')
            || (c == '\''))
        {
        if(c > 127 ) printf("\n\nchar lolz: %d\n\n", c);
            word[i] = toupper(c);
            i++;
            len++;
            if(len > longest){
                longest = len;
                for(k=0;k<longest;++k) {
                    lword[k] = word[k];
                }
                lword[i] = '\0';
            }
            if(i > MAX) {
                printf("Word limit reached[%d].",MAX);
                printf("Aborting...\n");
                fclose(file);
                return 1;
            }
        } else {
            if(len != 0){
                word[i] = '\0';
                root = add(root, word); /* add new node */
                words++;
                len = 0;
                i = 0;
            }
        }
    } while((c = getc(file)) != EOF);
    fclose(file);
    
    /*
    get node count, move node pointers
    to array and sort it.
    */
    nodes = treesize(root);

    printf("nodes: %d\n",nodes);
    // printf("size of array: %d bytes\n", nodes * sizeof(struct node));

    array = (struct node**) malloc(nodes * sizeof(struct node));
    if(array == NULL) {
        printf("toobig\n");
        return 0;
    }
    for(ia = 0; ia < nodes; ++ia) {
        array[ia] = NULL;
    }


    toarray(root, array, 0);


    printf("1. sort\n");
    clock_t begin = clock();
    three_pikasort(array, 0, nodes-1);
    clock_t end = clock();
    double time_spent = ((double)(end - begin)) * 1000 / CLOCKS_PER_SEC;
    printf("calls: %d depth: %d swaps: %d\n", calls, depth, swaps);
    printf("sorting time: %.0f ms\n", time_spent);

    calls = 0;
    depth = 0;
    swaps = 0;
    printf("\n2. sort for already sorted list\n");
    begin = clock();
    three_pikasort(array, 0, nodes-1);
    end = clock();
    time_spent = ((double)(end - begin)) * 1000 / CLOCKS_PER_SEC;
    printf("calls: %d depth: %d swaps: %d\n", calls, depth, swaps);
    printf("sorting time: %.0f ms\n", time_spent);
/*
    printf("\ncalls: %d depth: %d swaps: %d\n", calls, depth, swaps);
    calls = 0;
    depth = 0;
    swaps = 0;
    begin = clock();
    pikasort(array, 0, nodes-1);
    end = clock();
    time_spent = ((double)(end - begin)) * 1000 / CLOCKS_PER_SEC;
    // double time_spent = ((double)(end - begin));
    printf("\nsorting time: %f\n", time_spent);
*/

    // printf("\ncalls: %d depth: %d swaps: %d\n", calls, depth, swaps);
    /* print results */
    // printf(" .-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
    printf("\r .-` WORDS.c (TRAK2021)  `-.\n");
    printf(" .-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
    printf(" ~:   FILE: %s\n", argv[0]);
    printf(" ~: UNIQUE: %d\n", nodes);
    printf(" ~:  TOTAL: %d\n", words);
    printf(" ~:LONGEST: %2d LETTERS ->", longest);
    printf(" \" %s \" \n", lword);
    printf(" .-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
    printf(" .-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
    printf(" .-`    TOP %3d WORDS    `-. \n", TOPNUMBER);
    printf(" .                         .\n");
    printf("  .WORD.             .FREQ.\n");
    printf("  ------             ------\n");

    toplist = nodes - TOPNUMBER;
    if(toplist <= 0) toplist = 0;
// toplist = 0;
    j = 1;
    for(i = nodes-1; i >= toplist; --i, j++) {
        printf("%3d. %-15s%6d\n",j, array[i]->word,array[i]->count);
    }

// tprint(root);

    return 0;
}


/* add new node to tree */
struct node* add(struct node* node, char* word)
{
    char* temp;
    if(node == NULL) {
        node = (struct node*) malloc(sizeof(struct node));
        temp = (char*) malloc(strlen(word)+1);
        strcpy(temp,word);
        node->word = temp;
        node->count = 1;
        node->left = node->right = NULL;
    } else if(strcmp(word, node->word) == 0) {
        node->count++;
    } else if(strcmp(word, node->word) < 0) {
        node->left = add(node->left, word);
    } else {
        node->right = add(node->right, word);
   }
    return node;
}


/* size of tree */
int treesize(struct node* root) {
    if(root == NULL) return 0;
    return 1 + treesize(root->left) + treesize(root->right);
}

/* print tree */
void tprint(struct node* root)
{
    if(root != NULL) {
        tprint(root->left);
        printf("%4d %s\n",root->count,root->word);
        tprint(root->right);
    }
}

/* copy tree to array */
int toarray(struct node* root, struct node* arr[], int i)
{
    if(root == NULL) return i;
    i = toarray(root->left, arr, i);
    arr[i] = root;
    i++;
    i = toarray(root->right, arr, i);
    return i;
}

/* quicksort with embedded partition() procedure */
void pikasort(struct node* arr[], int left, int right)
{
    // printf("%d-%d\n",left,right);
    int i;
    int pivot;
    int a,b;
    // i = 0;
    // pivot = 0;
    ++calls;
    if(recursion > depth) depth = recursion;
    ++recursion;

    if(left >= right) return;
     swap(arr, left, (left+right)/2); 
    pivot = left;
    for(i=left+1; i <= right; i++) {
        a = arr[i]->count;
        b = arr[left]->count;
        // if(arr[i]->count < arr[left]->count)
        if(a < b)
            swap(arr, ++pivot, i);
    }
    swap(arr, left, pivot);
    pikasort(arr, left, pivot-1);
    pikasort(arr, pivot+1, right);
    --recursion;
}

/* quicksort using embedded three-way partition procedure */
void three_pikasort(struct node* arr[] , int left, int right)
{
    calls++;
    if(recursion > depth) depth = recursion;
    ++recursion;

    if(right <= left) return;

    /* three-way partition() */
    int i,j,p,q;
    i = left -1;
    j = right;
    p = left -1;
    q = right;

    while(1) {
        while(arr[++i]->count < arr[right]->count);
        while(arr[right]->count < arr[--j]->count)
            if(j == 1) break;
        if(i >= j) break;
        swap(arr, i, j);
        if(arr[i]->count == arr[right]->count) {
            p++;
            swap(arr,p,i);
        }
        if(arr[j]->count == arr[right]->count) {
            q--;
            swap(arr,j,q);
        }
    }
    swap(arr,i,right);
    j = i - 1;
    int k;
    for(k = left; k < p; k++, j--)
        swap(arr,k,j);
    i = i + 1;
    for(k = right -1; k > q; k--, i++)
        swap(arr,i,k);
    /* partition() ends */

    /* sort partitions */
    three_pikasort(arr, left, j);
    three_pikasort(arr, i, right);

    --recursion;
}

/* swap() implementation for quicksort */
void swap(struct node* a[], int i, int j) {
    swaps++;
    struct node *temp;
    temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}
