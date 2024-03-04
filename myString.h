#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    char* data_;
    size_t size_;
} MyString;

// Konstruktor bezargumentowy tworzący pusty napis
void MyString_init(MyString* str) {
    str->data_ = (char*)malloc(sizeof(char));
    str->data_[0] = '\0';
    str->size_ = 0;
}

// Konstruktor tworzący napis na podstawie innego napisu
void MyString_init_from_cstr(MyString* str, const char* other) {
    str->data_ = (char*)malloc(sizeof(char) * (strlen(other) + 1));
    strcpy(str->data_, other);
    str->size_ = strlen(other);
}

// Konstruktor kopiujący
void MyString_init_copy(MyString* str, const MyString* other) {
    str->data_ = (char*)malloc(sizeof(char) * (other->size_ + 1));
    strcpy(str->data_, other->data_);
    str->size_ = other->size_;
}

// Konstruktor przenoszący
void MyString_init_move(MyString* str, MyString* other) {
    str->data_ = other->data_;
    str->size_ = other->size_;
    other->data_ = NULL;
    other->size_ = 0;
}

// Destruktor zwalniający pamięć
void MyString_destroy(MyString* str) {
    free(str->data_);
    str->data_ = NULL;
    str->size_ = 0;
}

void MyString_copy(MyString* dest, const MyString* src) {
    if (dest != src) {  // Zabezpieczenie przed samoprzypisaniem
        dest->size_ = src->size_;
        dest->data_ = (char*)malloc(sizeof(char) * (src->size_ + 1));
        strcpy(dest->data_, src->data_);
    }
}

int MyString_equal(const MyString* str1, const MyString* str2) {
    if (str1->size_ != str2->size_) {
        return 0;
    }
    return strcmp(str1->data_, str2->data_) == 0;
}

// Przeciążenie operatora indeksowania
char MyString_at(const MyString* str, size_t index) {
    return str->data_[index];
}

// Przeciążenie operatora const indeksowania
const char MyString_at_const(const MyString* str, size_t index) {
    return str->data_[index];
}

void MyString_append_cstr(MyString* str, const char* other) {
    size_t len = strlen(other);
    char* tmp = (char*)malloc(sizeof(char) * (str->size_ + len + 1));
    strcpy(tmp, str->data_);
    strcat(tmp, other);
    free(str->data_);
    str->data_ = tmp;
    str->size_ += len;
}

void MyString_append_char(MyString* str, const char ch) {
    size_t len = strlen(str->data_);
    char* temp = (char*)malloc(sizeof(char) * (len + 2));
    strcpy(temp, str->data_);
    temp[len] = ch;
    temp[len + 1] = '\0';
    free(str->data_);
    str->data_ = temp;
    str->size_++;
}

void MyString_resize(MyString* str, size_t new_size) {
    char* tmp = (char*)realloc(str->data_, sizeof(char) * (new_size + 1));
    if (tmp == NULL) {
        printf("Error: Failed to resize MyString\n");
        return;
    }
    str->data_ = tmp;
    if (new_size > str->size_) {
        memset(str->data_ + str->size_, 0, new_size - str->size_);
    }
    str->size_ = new_size;
}

size_t MyString_length(const MyString* str) {
    return str->size_;
}

const char* MyString_c_str(const MyString* str) {
    return str->data_;
}

void MyString_reverse(MyString* str) {
    for (int i = 0; i < str->size_ / 2; i++) {
        char temp = str->data_[i];
        str->data_[i] = str->data_[str->size_ - i - 1];
        str->data_[str->size_ - i - 1] = temp;
    }
}

bool MyString_compare(const MyString* str1, const MyString* str2) {
    return strcmp(str1->data_, str2->data_);
}
