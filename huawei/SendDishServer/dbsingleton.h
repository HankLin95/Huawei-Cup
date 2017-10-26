#ifndef DBSINGLETON_H
#define DBSINGLETON_H	
#include <stdio.h>
#include "sqlite3.h"
#include<string>
class del_single;
class dbsingle
{
public:
	static dbsingle* getinstace(char*filename);
	static dbsingle* getinstace();
	void db_close();

    void gettable(char*sql,char ***result,int&col,int&row);

    int gettable_return(char*sql,char ***result,int&col,int&row);
private:
	static dbsingle* instance;
	friend class del_single;
	static del_single del;
	sqlite3 *Pdb;
	~dbsingle ();
	dbsingle();
};

class del_single
{
public:
	~del_single();
	
};
#endif
