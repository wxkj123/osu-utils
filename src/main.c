//
//  main.c
//  osu-utils
//
//  Created by Merry on 2023/9/9.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "util/Downloader.h"
#include "util/Getbplist.h"
#include "util/OAuth.h"

int main(void){
    int* sids;
    int uid,offset,limit,r;
    uint8_t mode;
    struct stat st={0};
    if(stat("Cache",&st)==-1){
        mkdir("Cache",0755);
    }
    curl_global_init(CURL_GLOBAL_ALL);
    char token[1000]={'\0'};
    read_token(token);
    // Get user info.

    printf("Type in the uid you want to get bplist from: ");
    r=scanf("%d",&uid);
    if(r!=1){
        fprintf(stderr,"uid format error.\n");
        exit(16);
    }
    printf("Type in the mode of the bplist you want to get(0 stands for std, 1 for taiko, 2 for ctb, 3 for mania): ");
    r=scanf("%d",&mode);
    if((r!=1)||(mode>3)){
        fprintf(stderr,"Mode format error.\n");
        exit(17);
    }
    printf("Type in the start position of the bplist you want to get(0 for default): ");
    r=scanf("%d",&offset);
    if((r!=1)||(offset>100)){
        fprintf(stderr,"Offset format error.\n");
        exit(18);
    }
    printf("Type in the end position of the bplist you want to get(100 for default): ");
    r=scanf("%d",&limit);
    if(r!=1||(limit>100)){
        fprintf(stderr,"Limit format error.\n");
        exit(19);
    }
    get_bplist(uid,mode,offset,limit,token);
    sids=getsids();
    mapdownloader(sids,offset,limit);
    curl_global_cleanup();
    return 0;
}
