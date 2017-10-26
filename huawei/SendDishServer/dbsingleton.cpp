#include "dbsingleton.h"
dbsingle* dbsingle::instance=NULL;
del_single dbsingle::del;
dbsingle::dbsingle()
{
	printf("构造数据库单例类\n");
}
dbsingle* dbsingle::getinstace(char*filename)
{
	if(dbsingle::instance==NULL)
	{
		dbsingle::instance=new dbsingle;
		if(sqlite3_open(filename,&instance->Pdb)==SQLITE_OK)
		{
			printf("成功打开数据库%s\n",filename);
		}
		else
		{
			printf("打开数据库出错\n");
		    printf("错误代码%d,错误信息%s",sqlite3_errcode(instance->Pdb),sqlite3_errmsg(instance->Pdb));
		}
		return instance;
	}
	else
	{
		return dbsingle::instance;
	}
}
dbsingle* dbsingle::getinstace()
{
	if(dbsingle::instance==NULL)
	{
		dbsingle::instance=new dbsingle;
		return instance;
	}
	else
	{
		return dbsingle::instance;
	}
}
void dbsingle::db_close()
{
	if(instance->Pdb==NULL)
	{
		printf("还未打开数据库\n");
		return;
	}
	else
	{
		sqlite3_close(instance->Pdb);
		printf("关闭数据库\n");
	}
}


void dbsingle::gettable(char*sql,char ***result,int&col,int&row)
{

    if(result==NULL)
        result =new char**;
    if(instance->Pdb==NULL)
    {
        printf("还未打开数据库,请打开数据库后查询\n");
        return ;
    }
    int ret;
    char *errmsg;
    ret=sqlite3_get_table(instance->Pdb,sql,result,&row,&col,&errmsg);
    if(ret!=SQLITE_OK)
    {
        printf("huoqu数据库出错\n");
        printf("错误代码%d,错误信息%s",sqlite3_errcode(instance->Pdb),sqlite3_errmsg(instance->Pdb));
    }
    return ;
}

int dbsingle::gettable_return(char*sql,char ***result,int&col,int&row)
{

    if(result==NULL)
        result =new char**;
    if(instance->Pdb==NULL)
    {
        printf("还未打开数据库,请打开数据库后查询\n");
        return 0;
    }
    int ret;
    char *errmsg;
    ret=sqlite3_get_table(instance->Pdb,sql,result,&row,&col,&errmsg);
    if(ret!=SQLITE_OK)
    {
        printf("huoqu数据库出错\n");
        printf("错误代码%d,错误信息%s",sqlite3_errcode(instance->Pdb),sqlite3_errmsg(instance->Pdb));
        return 0;
    }
    return 1;
}




dbsingle::~dbsingle ()
{
	printf("数据库单例析构函数\n");
}
del_single::~del_single()
{
	delete dbsingle::instance;
}
