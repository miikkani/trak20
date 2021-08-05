/* Count frequency of words.
 *
 * Binary search three for counting frequency.
 * Quicksort for sorting frequencies.
 *
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


#define MAX 40 /* max length of word */
#define TOPNUMBER 30

struct node {
    struct node* left;
    struct node* right;
    char* word;
    int count;
};


/* functions */
struct node* add(struct node*, char*);
int treesize(struct node* root);
void tprint(struct node*);
int toarray(struct node*, struct node*[], int); 
void pikasort(struct node*[], int, int);
void swap(struct node*[], int, int);




/* main program */
int main(int argc, char* argv[])
{
    int c,len,words;
    FILE* file;
    struct node* root;
    char word[MAX];
    words = 0;
    len = 0;
    int i = 0;
    
    if((file = fopen(*++argv, "r")) == NULL){
        printf("nullval");
        return 1;
    }

    root = NULL;

    c = getc(file);
    do {
        if( (c >= 'a'&& c <= 'z') || (c >= 'A'&& c <= 'Z')
             || (c == '\''))
        {
            word[i] = toupper(c);
            i++;
            len++;
        } else {
            if(len != 0){
                word[i] = '\0';

                /* add new node to tree */
                root = add(root, word);
                
                //printf(" %s\n",word);
                words++;
                len = 0;
                i = 0;
            }
        }
    } while((c = getc(file)) != EOF);
    fclose(file);
    tprint(root);
    int nodes = treesize(root);

    struct node** array;
    array = malloc(nodes * sizeof(struct node*));
    toarray(root, array, 0);
    pikasort(array, 0, nodes-1);


    //printf("\n===========================\n");
    //printf("=== WORDS.c (TRAK2021) ====\n");
    printf(" .-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
    printf(" .-` WORDS.c (TRAK2021)  `-.\n");
    printf(" .-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
    //printf("===========================\n");
    printf(" ~:   FILE: %s\n", argv[0]);
    printf(" ~: UNIQUE: %d\n", nodes);
    printf(" ~:  TOTAL: %d\n", words);
    printf(" .-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
    printf(" .-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
    //printf("\n");
    printf(" .-`    TOP %3d WORDS    `-. \n", TOPNUMBER);
    printf(" .                         .\n");
    printf("  .WORD.             .FREQ.\n");
    printf("  ------             ------\n");
    //printf("-------------------\n");
    //printf(" WORD\tFREQ(*)\n");
    //printf("-------------------\n");

    int toplist = nodes - TOPNUMBER;
    if(toplist <= 0) toplist = 0;

    int j = 1;
    for(int i = nodes-1; i >= toplist; --i, j++) {
        printf("%3d. %-15s%6d\n",j, array[i]->word,array[i]->count);
    }

    return 0;
}

/* */
void readchars(const char* file)
{
    int ccc;

}

/* add new node to tree */
struct node* add(struct node* node, char* word)
{
    if(node == NULL) {
        node = (struct node*) malloc(sizeof(struct node));
        char* temp = (char*) malloc(strlen(word)+1);
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
//        printf("%4d %s\n",root->count,root->word);
        tprint(root->right);
    }

}

/* copy tree to array */
int toarray(struct node* root, struct node* arr[], int i) {

    if(root == NULL) return i;

    i = toarray(root->left, arr, i);
    arr[i] = root;
    //char* temp = (char*) malloc(strlen(root->word)+1);
    //strcpy(temp,root->word);
    i++;

    i = toarray(root->right, arr, i);
    return i;
    
}

void pikasort(struct node* arr[], int left, int right) {
    int i, pivot;

    if(left >= right) return;
    swap(arr, left, (left+right)/2);
    pivot = left;
    for(i=left+1; i <= right; ++i) {
        if(arr[i]->count < arr[left]->count)
            swap(arr, ++pivot, i);
    }
    swap(arr, left, pivot);
    pikasort(arr, left, pivot-1);
    pikasort(arr, pivot+1, right);
}



void swap(struct node* a[], int i, int j) {
    struct node *temp;
    temp = a[i];
    a[i] = a[j];
    a[j] = temp;
//    printf(" %d_%d ",a[i]->count, a[j]->count);
}


