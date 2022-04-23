#include "cmd_deal.h"

extern SQL_HANDLE sqlhandle;

char* packaging_room_info_json(int *len)
{
    cJSON *root=NULL, *obj=NULL;
    char *str = NULL;

    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root,"type", "room");

    obj = cJSON_CreateObject();
    // cJSON_AddNumberToObject(obj,"temp", 34);
    // cJSON_AddNumberToObject(obj,"humi", 75);
    cJSON_AddStringToObject(obj,"temp", "34");
    cJSON_AddStringToObject(obj,"humi", "53");
    cJSON_AddStringToObject(obj,"airquality", "良好");
    cJSON_AddItemToObject(root,"info", obj);

    str = cJSON_Print(root);
    strcat(str,"\r\n");
    *len = strlen(str);
    cJSON_Delete(root);

    return str;
}

char* packaging_patient_info_json(int *len, char *name)
{
    cJSON *root=NULL, *obj=NULL;
    char *str = NULL;
    SQL_TABLE_S tbInfo;
    int i=0,j=0, row=0;
    char cmd[64]={0};

    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root,"type", "patient");

    sprintf(cmd,"select * from manageHumanTb where name = '%s';", name);
    sqlQueryData(sqlhandle, cmd, &tbInfo);
    if(tbInfo.nrows > 0){
        // for(i=0; i<tbInfo.nrows; i++) {
        //     for(j=0; j<tbInfo.nfields; j++){
        //         printf(" %s |", tbInfo.table[i][j]);
        //     }
        //     printf("\r\n");
        // }
        obj = cJSON_CreateObject();
        cJSON_AddStringToObject(obj,"name", tbInfo.table[row][1]);
        cJSON_AddStringToObject(obj,"sex", tbInfo.table[row][5]);
        cJSON_AddStringToObject(obj,"age", tbInfo.table[row][6]);
        cJSON_AddStringToObject(obj,"work", tbInfo.table[row][7]);
        cJSON_AddStringToObject(obj,"sickStatus", tbInfo.table[row][8]);
        cJSON_AddStringToObject(obj,"treatTime", tbInfo.table[row][9]);
        cJSON_AddStringToObject(obj,"treatProgress", tbInfo.table[row][10]);
        cJSON_AddStringToObject(obj,"treatWay", tbInfo.table[row][11]);
        cJSON_AddStringToObject(obj,"oxygen", "65");
        cJSON_AddStringToObject(obj,"heartRate", "53");

        cJSON_AddItemToObject(root,"info", obj);
    }else{
        printf("return NULL\r\n");
    }    
    str = cJSON_Print(root);
    strcat(str,"\r\n");
    *len = strlen(str);
    cJSON_Delete(root);
    
    sqlTbQueryFree(sqlhandle, tbInfo);
    
    return str;
}

char* packaging_addpatient_info_json(int *len)
{
    cJSON *root=NULL, *array=NULL;
    cJSON *obj[4];
    char *str = NULL;
    int i=0,j=0;
    SQL_TABLE_S tbInfo;

    sqlQueryData(sqlhandle, "select * from manageHumanTb;", &tbInfo);
    // for(i=0; i<tbInfo.nrows; i++) {
    //     for(j=0; j<tbInfo.nfields; j++){
    //         printf(" %s |", tbInfo.table[i][j]);
    //     }
    //     printf("\r\n");
    // }
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root,"type", "refresh");

    array = cJSON_CreateArray();

    for(i=0; i<tbInfo.nrows; i++){
        obj[i] = cJSON_CreateObject();
        cJSON_AddStringToObject(obj[i],"name", tbInfo.table[i][1]);
        cJSON_AddStringToObject(obj[i],"cls", tbInfo.table[i][2]);
        cJSON_AddStringToObject(obj[i],"room", tbInfo.table[i][3]);
        cJSON_AddStringToObject(obj[i],"bed", tbInfo.table[i][4]);
        cJSON_AddItemToArray(array, obj[i]);
    }

    cJSON_AddItemToObject(root,"info", array);

    str = cJSON_Print(root);
    strcat(str,"\r\n");
    *len = strlen(str);
    cJSON_Delete(root);
    sqlTbQueryFree(sqlhandle, tbInfo);
    
    return str;
}


char* packaging_showline_info_json(int *len)
{
    cJSON *root=NULL, *array=NULL;
    cJSON *obj[2];
    char *str = NULL;
    char strId[16]={0};
    int i=0,j=0;

    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root,"type", "showline");

    array = cJSON_CreateArray();

    for(i=0; i<2; i++){
        obj[i] = cJSON_CreateObject();
        for(j=0;j<20;j++){
            sprintf(strId, "%d", j);
            cJSON_AddNumberToObject(obj[i], strId, j);
        }
        cJSON_AddItemToArray(array, obj[i]);
    }

    cJSON_AddItemToObject(root,"info", array);

    str = cJSON_Print(root);
    strcat(str,"\r\n");
    *len = strlen(str);
    cJSON_Delete(root);
    
    return str;
}

char* parse_json_data_name(char *buf)
{
    char *str = (char *)malloc(32);

    cJSON *root = cJSON_Parse(buf);	//解析
	if (!root){	//若root为nullptr指针，则解析失败
		printf("Parse Error");
		return NULL;
	}
    cJSON *info = cJSON_GetObjectItem(root, "info");
    cJSON *name = cJSON_GetObjectItem(info, "name");
    
    strcpy(str, name->valuestring);
    cJSON_Delete(root);

    return str;
}

enum CmdType parse_json_data(char *buf)
{
    enum CmdType TYPE;

    cJSON *root = cJSON_Parse(buf);	//解析 
	if (!root){	//若root为nullptr指针，则解析失败
		printf("Parse Error");
		return ERR;
	}
    cJSON *type = cJSON_GetObjectItem(root, "type");	//根据key值获取对应的value
    cJSON *info = cJSON_GetObjectItem(root, "info");
	printf("type=%s\r\n", type->valuestring);

    if(strcmp(type->valuestring, "add") == 0){
        TYPE = ADD;
    }else if(strcmp(type->valuestring, "showline") == 0){
        TYPE = SHOWLINE;
    }else if(strcmp(type->valuestring, "refresh") == 0){
        TYPE = REFRESH;
    }else if(strcmp(type->valuestring, "showroom") == 0){
        TYPE = SHOWROOM;
    }else if(strcmp(type->valuestring, "showpatient") == 0){
        TYPE = SHOWPATIENT;
    }else if(strcmp(type->valuestring, "delpatient") == 0){
        TYPE = DELPATIENT;
    }else if(strcmp(type->valuestring, "updateinfo") == 0){
        TYPE = UPDATEINFO;
    }else{
        TYPE = ERR;
    }
    cJSON_Delete(root);

    return TYPE;
}
