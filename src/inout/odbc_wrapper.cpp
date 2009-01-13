// code copied from http://www.codeproject.com/KB/database/ODBC.aspx
// and modified

#include "odbc_wrapper.h"


using namespace ODBC;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
bool DSNConnection::Connect(LPCTSTR svSource ,LPCTSTR szUsername, LPCTSTR szPassword)
{
    int nConnect = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
    if( nConnect == SQL_SUCCESS || nConnect == SQL_SUCCESS_WITH_INFO)
    {
        nConnect = SQLSetEnvAttr( m_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
        if( nConnect == SQL_SUCCESS || nConnect == SQL_SUCCESS_WITH_INFO )
        {
            nConnect = SQLAllocHandle( SQL_HANDLE_DBC, m_hEnv, &m_hDBC);
            if(nConnect == SQL_SUCCESS || nConnect == SQL_SUCCESS_WITH_INFO)
            {
                SQLSetConnectOption(m_hDBC, SQL_LOGIN_TIMEOUT, 5);
                nConnect = SQLConnect(m_hDBC, (SQLTCHAR*)svSource, SQL_NTS, (SQLTCHAR*)szUsername,
                                    SQL_NTS,  (SQLTCHAR*)szPassword, SQL_NTS);
                if(nConnect == SQL_SUCCESS || nConnect == SQL_SUCCESS_WITH_INFO)
                {
                    return 1;
                }
            }
        }
    }
    if(m_hDBC != NULL)
    {
        m_nReturn = SQLDisconnect(m_hDBC);
        m_nReturn = SQLFreeHandle(SQL_HANDLE_DBC, m_hDBC);
    }
    if(m_hEnv!=NULL)
        m_nReturn = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
    m_hDBC    = NULL;
    m_hEnv    = NULL;
    m_nReturn = SQL_ERROR;

    return 0;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
virtual DSNConnection::~DSNConnection()
{
    if(m_hDBC != NULL)
        m_nReturn = SQLFreeHandle(SQL_HANDLE_DBC,  m_hDBC);
    if(m_hEnv!=NULL)
        m_nReturn = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void DSNConnection::Disconnect()
{
    if(m_hDBC != NULL)
    {
        m_nReturn = SQLDisconnect(m_hDBC);
        m_hDBC = NULL;
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
ODBCConnection::ODBCConnection()
{
    m_hDBC    = NULL;
    m_hEnv    = NULL;
    m_nReturn = SQL_ERROR;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
virtual ODBCConnection::~ODBCConnection()
{
    if(m_hDBC != NULL)
        m_nReturn = SQLFreeHandle(SQL_HANDLE_DBC, m_hDBC);
    if(m_hEnv!=NULL)
        m_nReturn = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
bool ODBCConnection::Connect(LPCTSTR svSource)
{
    int nConnect = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
    if(nConnect == SQL_SUCCESS || nConnect == SQL_SUCCESS_WITH_INFO)
    {
        nConnect = SQLSetEnvAttr( m_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
        if(nConnect == SQL_SUCCESS || nConnect == SQL_SUCCESS_WITH_INFO)
        {
            nConnect = SQLAllocHandle( SQL_HANDLE_DBC, m_hEnv, &m_hDBC);
            if(nConnect == SQL_SUCCESS || nConnect == SQL_SUCCESS_WITH_INFO)
            {
                SQLSetConnectOption(m_hDBC,SQL_LOGIN_TIMEOUT,5);
                short shortResult = 0;
                SQLTCHAR szOutConnectString[1024];
                nConnect = SQLDriverConnect(m_hDBC,                // Connection Handle
                                          NULL,                           // Window Handle
                                          (SQLTCHAR*)svSource,  // InConnectionString
                                          _tcslen(svSource),             // StringLength1
                                          szOutConnectString,             // OutConnectionString
                                          sizeof(szOutConnectString),   // Buffer length
                                          &shortResult,                   // StringLength2Ptr
                                          SQL_DRIVER_NOPROMPT);          // no User prompt

                return IS_SQL_OK(nConnect);
            }
        }
    }
    if(m_hDBC != NULL)
    {
        m_nReturn = SQLDisconnect(m_hDBC);
        m_nReturn = SQLFreeHandle(SQL_HANDLE_DBC,  m_hDBC);
    }
    if(m_hEnv != NULL)
        m_nReturn = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
    m_hDBC    = NULL;
    m_hEnv    = NULL;
    m_nReturn = SQL_ERROR;
    return(IS_SQL_OK(nConnect));
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void ODBCConnection::Disconnect()
{
    if(m_hDBC != NULL)
    {
        m_nReturn = SQLDisconnect(m_hDBC);
        m_hDBC = NULL;
    }
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
BOOL MSSQLConnection::Connect(LPCTSTR User,LPCTSTR Pass, LPCTSTR Host, BOOL Trusted, enumProtocols Proto)
{
    TCHAR str[512] = _T("");
    _stprintf(str, _T("Driver={SQL Server};Server=%s;Uid=%s;Pwd=%s; Trusted_Connection=%s;Network=%s;"),
                SAFE_STR(Host),SAFE_STR(User),SAFE_STR(Pass),
                (Trusted ? _T("Yes") : _T("No")));

    switch(Proto)
    {
    case protoNamedPipes:
        _tcscat(str,_T("dbnmpntw"));
        break;
    case protoWinSock:
        _tcscat(str,_T("dbmssocn"));
        break;
    case protoIPX:
        _tcscat(str,_T("dbmsspxn"));
        break;
    case protoBanyan:
        _tcscat(str,_T("dbmsvinn"));
        break;
    case protoRPC:
        _tcscat(str,_T("dbmsrpcn"));
        break;
    default:
        _tcscat(str,_T("dbmssocn"));
        break;
    }
    _tcscat(str,_T(";"));
    return ODBCConnection::Connect(str);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
int MDBConnection::Connect(LPCTSTR MDBPath, LPCTSTR User, LPCTSTR Pass, BOOL Exclusive)
{
    TCHAR str[512]=_T("");
    _stprintf(_T("Driver={Microsoft Access Driver (*.mdb)}; Dbq=%s;Uid=%s;Pwd=%s;Exclusive=%s;"),
               SAFE_STR(MDBPath),SAFE_STR(User), SAFE_STR(Pass),(Exclusive ? _T("yes") : _T("no")));
    return ODBCConnection::Connect(str);
 };
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
ODBCStmt::ODBCStmt(HDBC hDBCLink)
{
    SQLRETURN m_nReturn;
    m_nReturn = SQLAllocHandle(SQL_HANDLE_STMT, hDBCLink, &m_hStmt);
    SQLSetStmtAttr(m_hStmt, SQL_ATTR_CONCURRENCY, (SQLPOINTER) SQL_CONCUR_ROWVER, 0);
    SQLSetStmtAttr(m_hStmt, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER) SQL_CURSOR_KEYSET_DRIVEN, 0);
    if(!IS_SQL_OK(m_nReturn))
        m_hStmt=INVALID_HANDLE_VALUE;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

