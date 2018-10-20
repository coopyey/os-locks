#ifndef PROJECT_H
#define PROJECT_H

#define SIZE 5

typedef int buffer_item;

void initData();
void *producer(void *param);
void *consumer(void *param);
int insert_item(buffer_item item);
int remove_item(buffer_item *item);
int main();

#endif