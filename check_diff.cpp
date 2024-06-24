#include <toolbox.h>
using namespace std;
/*
-----------------   **
*   Author jcg       *
*   date 12 Nov 2013 *
-----------------   **
*/

class CDiff : public CCheck
{
    public :
    int StoreInfo(string,string);                      // Overwrite of the virtual function
    CDiff(string, string);                           // Constructor
    void post_calculation(string);
    void pre_calculation(string);
};



CDiff::CDiff(string a, string b)
{
    cmd_mysql = a;
    cmd_mssql = b;
}
void CDiff::post_calculation(string id)
{
    if (con)
    {
		sql::PreparedStatement *pstmt;
        pstmt = con->prepareStatement("delete from dblist where flag=0 and id=?");
		pstmt->setString(1,id);
		
		try {
			pstmt->executeUpdate();
		} catch (sql::SQLException &e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
		delete pstmt;
	}
}

void CDiff::pre_calculation(string id)
{
    if (con)
    {
		sql::PreparedStatement *pstmt;
        pstmt = con->prepareStatement("update dblist set flag=0 where id=?");
		pstmt->setString(1,id);
		try {
			pstmt->executeUpdate();
		} catch (sql::SQLException &e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
		delete pstmt;
	}
    
}


int CDiff::StoreInfo(string id, string name)
{
    DBCHAR servername[256];
    DBCHAR dte[20];
    DBCHAR namedb[256];
    DBINT compatibility;

    sql::PreparedStatement *pstmt;

    if (SUCCEED != dbbind(dbproc, 1, STRINGBIND, 0, (BYTE *) &servername))
    {
        fprintf(stdout, "Had problem with bind\n");
        return(WARNING);
    }
    if (SUCCEED != dbbind(dbproc, 2, STRINGBIND, 0, (BYTE *) &dte))
    {
        fprintf(stdout, "Had problem with bind\n");
        return(WARNING);
    }
    if (SUCCEED != dbbind(dbproc, 3, STRINGBIND, 0, (BYTE *) &namedb))
    {
        fprintf(stdout, "Had problem with bind\n");
        return(WARNING);
    }
    if (SUCCEED != dbbind(dbproc, 4, INTBIND, 0, (BYTE *) &compatibility))
    {
        fprintf(stdout, "Had problem with bind\n");
        return(WARNING);
    }
	pstmt = con->prepareStatement("insert into dblist(id,dtcreation,dbname,compatlevel,flag) values (?,?,?,?,1)  ON DUPLICATE KEY UPDATE compatlevel = ?  , flag =1");
    while (dbnextrow(dbproc) != NO_MORE_ROWS)
    {
		pstmt->setString(1,id);
		pstmt->setString(2,dte);
		pstmt->setString(3,namedb);
		pstmt->setInt(4,compatibility);
		pstmt->setInt(5,compatibility);
		// insert current line
		try {
				pstmt->executeUpdate();
				cout << ".";
		} catch (sql::SQLException &e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
    }// End while
	delete pstmt;
    return (OK);
}

int
main(int argc, char **argv)
{
   CDiff *mycheck;
   // Do not take into account the sql 2000 version because mirroring was not existing.
   CDiff mycdiff((string) MYSQLREQ ,
                       (string)  "select @@SERVERNAME as servername,convert(nvarchar, crdate,112), name, cmptlevel from sysdatabases where name not in ('master', 'model', 'tempdb','msdb')");

    mycheck = &mycdiff;
    mycheck->PopulateTarget();
    mycheck->GetInfo();
}

