#include <stdio.h>
#include "event_timer.h"
#include "zlog.h"

event_timer_t* timer = NULL;

zlog_category_t *pzlog = NULL;

zlog_category_t * serverLog(const char* path){
	int rc;
	zlog_category_t *zlog_handler = NULL;

	rc = zlog_init(path);

	if (rc) {
		return NULL;
	}

	zlog_handler = zlog_get_category("webAdapterlog");

	if (!zlog_handler) {
		zlog_fini();
		return NULL;
	}

	return zlog_handler;
}

void closeServerLog(){
	zlog_fini();
}

void test1(ngx_event_t *ev) {
    int a = *((int*)ev->data);
    zlog_info(pzlog, "test1(), a = %d \n", a);
    //add_event_timer(ev, 5, timer);
}

void test2(ngx_event_t *ev) {
    double b = *((double*)ev->data);
    zlog_info(pzlog, "test2(), b = %f \n", b);
    //add_event_timer(ev, 10, timer);
}

void test3(ngx_event_t *ev) {
    int data = *((int*)ev->data);
    zlog_info(pzlog, "test3(), data = %d \n", data);
    free(ev);
}

void test4(ngx_event_t *ev){
    int data = *((int*)ev->data);
    zlog_info(pzlog, "test4()---- data : %d \n", data);
    free(ev);
}


void* tmp_add(void* args){
	pthread_detach(pthread_self());
    event_timer_t* timer = (event_timer_t*)args;
    int a[10] = {1,2,3,4,5,6,7,8,9,10};
    int cnt = 0;
    while(1){
        ngx_event_t *ev1 = (ngx_event_t*)malloc(sizeof(ngx_event_t));
        ev1->data = &a[cnt];
        ev1->handler = test4;
        add_event_timer(ev1, 10, timer);
        usleep(100000);
        cnt = (cnt + 1) % 10;
    }
}

void* tmp_add_1(void* args){
	pthread_detach(pthread_self());
    event_timer_t* timer = (event_timer_t*)args;
    int a[10] = {111,112,113,114,115,116,117,118,119,120};
    int cnt = 0;
    while(1){
        ngx_event_t *ev1 = (ngx_event_t*)malloc(sizeof(ngx_event_t));
        ev1->data = &a[cnt];
        ev1->handler = test3;
        add_event_timer(ev1, 5, timer);
        usleep(10000);
        cnt = (cnt + 1) % 10;
    }
}

void main(void){
	pzlog = serverLog("../conf/zlog_default.conf");

	zlog_info(pzlog,"start event timer process\n");

    int stat = ngx_event_timer_init(&timer, pzlog);
    if(stat != 0){
        zlog_info(pzlog, "ngx_event_timer_init error , stat = %d \n", stat);
        return ;
    }

    int a = 1;
    double b = 2;
    ngx_event_t *ev1 = (ngx_event_t*)malloc(sizeof(ngx_event_t));
    ev1->handler = test1;
    ev1->data = &a;
    ngx_event_t *ev2 = (ngx_event_t*)malloc(sizeof(ngx_event_t));
    ev2->handler = test2;
    ev2->data = &b;

    add_event_timer(ev1, 10, timer);
    add_event_timer(ev2, 100, timer);

    pthread_t   tmp_pid;
    int ret = pthread_create(&tmp_pid, NULL, tmp_add, (void*)(timer));
    if(ret != 0){
		return;
    }

    pthread_t   tmp_pid_1;
    ret = pthread_create(&tmp_pid_1, NULL, tmp_add_1, (void*)(timer));
    if(ret != 0){
		return;
    }


    while(1){
        sleep(1);
    }
}

