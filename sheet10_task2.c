#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
    char firstname[20];
    char lastname[30];
    int number;
    char address[100];
    int courses;
    struct student *next;
    struct student *prev;
};

struct student *createStudent(char firstname[20], char lastname[30], int number, char address[100], int courses, struct student *next, struct student *prev){
    struct student *s = malloc(sizeof(struct student));

    strncpy(s->firstname, firstname, 20);
    strncpy(s->lastname, lastname, 30);
    s->number = number;
    strncpy(s->address, address, 100);
    s->courses = courses;
    s->next = next;
    s->prev = prev;

    return s;
}

struct student *reverseList(struct student *s){
    struct student *temp;

    while(s) {
        temp = s->next;
        s->next = s->prev;
        s->prev = temp;
        if(!temp){
            return s;
        }
        s = temp;
    }
}

void printList(struct student *s){
    do{
        printf("{%s, %s, %d, %s, %d}\n", s->firstname, s->lastname, s->number, s->address, s->courses );
    } while(s = s->next);
    printf("----------------------------------------------\n");
}

int main(){
    struct student *s3 = createStudent("Lisa", "Lustig", 66666, "Denickestrasse 15", 8, NULL, NULL);
    struct student *s2 = createStudent("Hans", "Peter", 44444, "Kasernenstrasse 12", 2, s3, NULL);
    struct student *s1 = createStudent("Anna", "Musterfrau", 22222, "Am Schwarzenberg−Campus 3", 4, s2, NULL);

    s3->prev = s2;
    s2->prev = s1;

    struct student *s = s1;

    printList(s);

    // add 2 new structs
    while(s->next){
        s = s->next;
    };

    s->next = createStudent("Bob", "Bar", 0, "127.0.0.1", 1010, NULL, s);
    s = s->next;
    s->next = createStudent("Alice", "Foo", 1, "localhost", 101, NULL, s);

    s = s1;

    // delete third struct
    struct student *deletedStudent = s->next->next;
    deletedStudent->prev->next = deletedStudent->next;
    deletedStudent->next->prev = deletedStudent->prev;
    free(deletedStudent);

    printList(s);

    printList(reverseList(s));
}
