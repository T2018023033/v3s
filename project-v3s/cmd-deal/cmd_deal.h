#ifndef __CMD_DEAL_H__
#define __CMD_DEAL_H__

#include "../cJSON/cJSON.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include "../sqlitedbms-api/sqlitedbms_client.h"


enum CmdType {
    ADD=0,
    SHOWLINE,
    REFRESH,
    SHOWROOM,
    SHOWPATIENT,
    DELPATIENT,
    UPDATEINFO,
    ERR
};

char* packaging_room_info_json(int *len);
char* packaging_patient_info_json(int *len, char *name);
char* packaging_addpatient_info_json(int *len);
char* packaging_showline_info_json(int *len);
enum CmdType parse_json_data(char *buf);
char* parse_json_data_name(char *buf);

#endif // !__CMD_DEAL_H__
