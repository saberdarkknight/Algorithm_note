#define ALPHABET_SIZE 26
typedef struct Trie {
    bool isKey;
    struct Trie * children[ALPHABET_SIZE];
} Trie;

Trie* trieCreate() {
    Trie* head = (Trie*) malloc(sizeof(Trie));
    head->isKey = false;
    for ( int i = 0; i < ALPHABET_SIZE; i++){
        head->children[i] = NULL;
    }
    return head;
}

void trieInsert(Trie* obj, char* word) {
    while (*word) {
        int idx = *word - 'a';
        if (obj->children[idx] == NULL) {
            obj->children[idx] = trieCreate();
        }
        obj = obj->children[idx];
        word++;
    }
    obj->isKey = true;
}

bool trieSearch(Trie* obj, char* word) {
    while (*word) {
        int idx = *word - 'a';
        if (obj->children[idx] == NULL) {
            return false;
        }
        obj = obj->children[idx];
        word++;
    }
    return obj->isKey;
}

bool trieStartsWith(Trie* obj, char* prefix) {
    while (*prefix) {
        int idx = *prefix - 'a';
        if (obj->children[idx] == NULL) {
            return false;
        }
        obj = obj->children[idx];
        prefix++;
    }
    return true;
}

void trieFree(Trie* obj) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (obj->children[i]) {
            trieFree(obj->children[i]);
        }
    }
    free(obj);
}