#ifndef ODBC_WRAPPER_H
#define ODBC_WRAPPER_H
// code copied from http://www.codeproject.com/KB/database/ODBC.aspx
// and modified
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
namespace ODBC
{
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <odbcinst.h>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
#pragma comment(lib,"odbc32.lib")
#pragma comment(lib,"odbcbcp.lib")
#pragma comment(lib,"OdbcCP32.Lib")
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
#define IS_SQL_ERR !IS_SQL_OK
#define IS_SQL_OK(res) (res==SQL_SUCCESS_WITH_INFO || res==SQL_SUCCESS)
#define SAFE_STR(str) ((str==NULL) ? _T("") : str)
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
enum sqlDataTypes
{
    sqlDataTypeUnknown=SQL_UNKNOWN_TYPE,
    sqlDataTypeChar=SQL_CHAR,
    sqlDataTypeNumeric=SQL_NUMERIC,
    sqlDataTypeDecimal=SQL_DECIMAL,
    sqlDataTypeInteger=SQL_INTEGER,
    sqlDataTypeSmallInt=SQL_SMALLINT,
    sqlDataTypeFloat=SQL_FLOAT,
    sqlDataTypeReal=SQL_REAL,
    sqlDataTypeDouble=SQL_DOUBLE,
#if (ODBCVER >= 0x0300)
    sqlDataTypeDateTime=SQL_DATETIME,
#endif
    sqlDataTypeVarChar=SQL_VARCHAR
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class DSNConnection
{
public:
    bool Connect(LPCTSTR svSource ,LPCTSTR szUsername, LPCTSTR szPassword);

    DSNConnection() : m_hDBC(NULL), m_hEnv(NULL), m_nReturn(NULL) { }
    virtual ~DSNConnection();
    void Disconnect();
public:
    operator HDBC() { return m_hDBC; }
private:
    SQLRETURN  m_nReturn; // Internal SQL Error code
    HENV       m_hEnv;    // Handle to environment
    HDBC       m_hDBC;    // Handle to database connection
};
 /////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class ODBCConnection
{
public:
    bool Connect(LPCTSTR svSource);
    ODBCConnection();
    virtual ~ODBCConnection();
    void Disconnect();
public:
  operator HDBC()   { return m_hDBC; }
 private:
  SQLRETURN       m_nReturn;      // Internal SQL Error code
  HENV            m_hEnv;         // Handle to environment
  HDBC            m_hDBC;         // Handle to database connection
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class MSSQLConnection : public ODBCConnection
{
public:
    enum enumProtocols
    {
        protoNamedPipes,
        protoWinSock,
        protoIPX,
        protoBanyan,
        protoRPC
    };
public:
    MSSQLConnection() { };
    virtual ~MSSQLConnection() { };
  BOOL Connect(LPCTSTR User=_T(""),LPCTSTR Pass=_T(""), LPCTSTR Host=_T("(local)"),BOOL Trusted=1, enumProtocols Proto=protoNamedPipes);
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class MDBConnection : public ODBCConnection
 {
 public:
  MDBConnection() {};
  virtual ~MDBConnection() {};
  int Connect(const LPCTSTR MDBPath, const LPCTSTR User = _T(""), const LPCTSTR Pass = _T(""),BOOL Exclusive=0);
 };
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class ODBCStmt
{
    HSTMT m_hStmt;
public:
    operator HSTMT() { return m_hStmt; }
    ODBCStmt(HDBC hDBCLink);
    virtual ~ODBCStmt()
    {
        if(m_hStmt!=INVALID_HANDLE_VALUE)
            SQLFreeHandle(SQL_HANDLE_STMT,m_hStmt);
    }
    BOOL IsValid()
    {
        return m_hStmt!=INVALID_HANDLE_VALUE;
    }
    USHORT GetColumnCount()
    {
        short nCols=0;
        if(!IS_SQL_OK(SQLNumResultCols(m_hStmt, &nCols)))
            return 0;
        return nCols;
    }
    DWORD GetChangedRowCount(void)
    {
        long nRows=0;
        if(!IS_SQL_OK(SQLRowCount(m_hStmt, &nRows)))
            return 0;
        return nRows;
    }
    BOOL Query(const LPCTSTR strSQL)
    {
        SQLRETURN nRet = SQLExecDirect(m_hStmt, (SQLTCHAR *)strSQL, SQL_NTS);
        return IS_SQL_OK(nRet);
    }
    BOOL Fetch()
    {
        SQLRETURN nRet = SQLFetch(m_hStmt);
        return IS_SQL_OK(nRet);
    }
    BOOL FecthRow(UINT nRow)
    {
        return IS_SQL_OK(SQLSetPos(m_hStmt, nRow, SQL_POSITION, SQL_LOCK_NO_CHANGE));
    }
    BOOL FetchPrevious()
    {
        SQLRETURN nRet = SQLFetchScroll(m_hStmt, SQL_FETCH_PRIOR, 0);
        return IS_SQL_OK(nRet);
    }
    BOOL FecthNext()
    {
        SQLRETURN nRet = SQLFetchScroll(m_hStmt, SQL_FETCH_NEXT, 0);
        return IS_SQL_OK(nRet);
    }
    BOOL FetchRow(ULONG nRow, BOOL Absolute = 1)
    {
        SQLRETURN nRet = SQLFetchScroll(m_hStmt, (Absolute ? SQL_FETCH_ABSOLUTE : SQL_FETCH_RELATIVE), nRow);
        return IS_SQL_OK(nRet);
    }
    BOOL FetchFirst()
    {
        SQLRETURN nRet = SQLFetchScroll(m_hStmt, SQL_FETCH_FIRST, 0);
        return IS_SQL_OK(nRet);
    }
    BOOL FetchLast()
    {
        SQLRETURN nRet = SQLFetchScroll(m_hStmt, SQL_FETCH_LAST, 0);
        return IS_SQL_OK(nRet);
    }
    BOOL Cancel()
    {
        SQLRETURN nRet = SQLCancel(m_hStmt);
        return IS_SQL_OK(nRet);
    }
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class ODBCRecord
{
    HSTMT m_hStmt;
public:
    ODBCRecord(HSTMT hStmt) { m_hStmt=hStmt; }
    ~ODBCRecord() { }
    USHORT GetColumnCount()
    {
        short nCols=0;
        if(!IS_SQL_OK(SQLNumResultCols(m_hStmt,&nCols)))
            return 0;
        return nCols;
    }
    BOOL BindColumn(USHORT Column, LPVOID pBuffer,
        ULONG pBufferSize, LONG * pReturnedBufferSize = NULL,
        USHORT nType = SQL_C_TCHAR)
    {
        LONG pReturnedSize = 0;
        SQLRETURN Ret = SQLBindCol(m_hStmt, Column, nType, pBuffer, pBufferSize, &pReturnedSize);
        if(*pReturnedBufferSize)
            *pReturnedBufferSize = pReturnedSize;
        return IS_SQL_OK(Ret);
    }
    USHORT GetColumnByName(LPCTSTR Column)
    {
        SHORT nCols = GetColumnCount();
        for(USHORT i=1; i<(nCols+1); i++)
        {
            TCHAR Name[256] = _T("");
            GetColumnName(i, Name, sizeof(Name));
            if(!_tcsicmp(Name, Column))
                return i;
        }
        return 0;
    }
    BOOL GetData(USHORT Column, LPVOID pBuffer,
        ULONG pBufLen, LONG * dataLen=NULL, int Type=SQL_C_DEFAULT)
    {
        SQLINTEGER od=0;
        int Err=SQLGetData(m_hStmt,Column,Type,pBuffer,pBufLen,&od);
        if(IS_SQL_ERR(Err))
        {
            return 0;
        }
        if(dataLen)
            *dataLen=od;
        return 1;
    }
    int GetColumnType(USHORT Column)
    {
       int nType=SQL_C_DEFAULT;
       SQLTCHAR svColName[ 256 ]=_T("");
       SWORD swCol=0,swType=0,swScale=0,swNull=0;
       UDWORD pcbColDef;
       SQLDescribeCol(m_hStmt,             // Statement handle
                       Column,             // ColumnNumber
                       svColName,          // ColumnName
                       sizeof( svColName), // BufferLength
                       &swCol,             // NameLengthPtr
                       &swType,            // DataTypePtr
                       &pcbColDef,         // ColumnSizePtr
                       &swScale,           // DecimalDigitsPtr
                       &swNull);           // NullablePtr
        nType = (int)swType;
        return(nType);
    }
    DWORD GetColumnSize(USHORT Column)
    {
       int nType = SQL_C_DEFAULT;
       SQLTCHAR svColName[256] = _T("");
       SWORD swCol = 0, swType = 0, swScale = 0, swNull = 0;
       DWORD pcbColDef = 0;
       SQLDescribeCol( m_hStmt,            // Statement handle
                       Column,             // ColumnNumber
                       svColName,          // ColumnName
                       sizeof( svColName), // BufferLength
                       &swCol,             // NameLengthPtr
                       &swType,            // DataTypePtr
                       &pcbColDef,         // ColumnSizePtr
                       &swScale,           // DecimalDigitsPtr
                       &swNull );          // NullablePtr
        return pcbColDef;
    }
    DWORD GetColumnScale(USHORT Column)
    {
       int nType = SQL_C_DEFAULT;
       SQLTCHAR svColName[256] = _T("");
       SWORD swCol = 0, swType = 0, swScale = 0, swNull = 0;
       DWORD pcbColDef = 0;
       SQLDescribeCol( m_hStmt,            // Statement handle
                       Column,             // ColumnNumber
                       svColName,          // ColumnName
                       sizeof( svColName), // BufferLength
                       &swCol,             // NameLengthPtr
                       &swType,            // DataTypePtr
                       &pcbColDef,         // ColumnSizePtr
                       &swScale,           // DecimalDigitsPtr
                       &swNull);           // NullablePtr
       return swScale;
    }
    BOOL GetColumnName(USHORT Column, LPTSTR Name, SHORT NameLen )
    {
       int nType = SQL_C_DEFAULT;
       SWORD swCol = 0, swType = 0, swScale = 0, swNull = 0;
       DWORD pcbColDef = 0;
       SQLRETURN Ret=
        SQLDescribeCol(m_hStmt,             // Statement handle
                       Column,              // ColumnNumber
                       (SQLTCHAR*)Name,     // ColumnName
                       NameLen,             // BufferLength
                       &swCol,              // NameLengthPtr
                       &swType,             // DataTypePtr
                       &pcbColDef,          // ColumnSizePtr
                       &swScale,            // DecimalDigitsPtr
                       &swNull);            // NullablePtr

       if(IS_SQL_ERR(Ret))
            return 0;
       return 1;
    }
    BOOL IsColumnNullable(USHORT Column)
    {
       int nType = SQL_C_DEFAULT;
       SQLTCHAR svColName[256] = _T("");
       SWORD swCol = 0, swType = 0, swScale = 0, swNull = 0;
       UDWORD pcbColDef;
       SQLDescribeCol( m_hStmt,            // Statement handle
                       Column,             // ColumnNumber
                       svColName,          // ColumnName
                       sizeof(svColName),  // BufferLength
                       &swCol,             // NameLengthPtr
                       &swType,            // DataTypePtr
                       &pcbColDef,         // ColumnSizePtr
                       &swScale,           // DecimalDigitsPtr
                       &swNull);           // NullablePtr

       return (swNull == SQL_NULLABLE);
    }
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
}; // namespace ODBC
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
#endif // ODBC_WRAPPER_H
