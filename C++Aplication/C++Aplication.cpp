// C++Aplication.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <Windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

using namespace std;

void showSqlError(unsigned int handleType, const SQLHANDLE& handle)
{
	SQLCHAR SqlState[1024];
	SQLCHAR message[1024];
	if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SqlState, NULL, message, 1024, NULL))
	{
		cout << "Mensagem do driver do sql: " << message << "." << endl;
	}
}

int main()
{
	SQLHANDLE SqlEventHandle = NULL;
	SQLHANDLE SqlConnectionHandle = NULL;
	SQLHANDLE SqlStatementHandle = NULL;
	SQLRETURN retCode = 0;
	char SqlQuery[] = "SELECT * FROM PESSOA";

	do
	{
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SqlEventHandle))
			break;

		if (SQL_SUCCESS != SQLSetEnvAttr(SqlEventHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
			break;

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, SqlEventHandle, &SqlConnectionHandle))
			break;

		if (SQL_SUCCESS != SQLSetConnectAttr(SqlConnectionHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0))
			break;

		SQLCHAR retConString[1024];
		switch (SQLDriverConnect(SqlConnectionHandle, NULL, (SQLCHAR*)"DRIVER={ODBC Driver 17 for SQL Server};Server=localhost;Database=DAPPER;Trusted_Connection=Yes;", SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT))
		{
		case SQL_SUCCESS:
			break;
		case SQL_SUCCESS_WITH_INFO:
			break;
		case SQL_NO_DATA_FOUND:
			showSqlError(SQL_HANDLE_DBC, SqlConnectionHandle);
			retCode = -1;
			break;
		case SQL_INVALID_HANDLE:
			showSqlError(SQL_HANDLE_DBC, SqlConnectionHandle);
			retCode = -1;
			break;
		case SQL_ERROR:
			showSqlError(SQL_HANDLE_DBC, SqlConnectionHandle);
			retCode = -1;
			break;
		default:
			break;
		}
		if (retCode == -1)
			break;

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, SqlConnectionHandle, &SqlStatementHandle))
			break;

		if (SQL_SUCCESS != SQLExecDirect(SqlStatementHandle, (SQLCHAR*)SqlQuery, SQL_NTS))
		{
			showSqlError(SQL_HANDLE_STMT,SqlStatementHandle);
			break;
		}
		else
		{
			char nome[256];
			int idade;
			while (SQLFetch(SqlStatementHandle) == SQL_SUCCESS)
			{
				SQLGetData(SqlStatementHandle, 2, SQL_C_DEFAULT, &nome, sizeof(nome), NULL);
				SQLGetData(SqlStatementHandle, 3, SQL_C_DEFAULT, &idade, size(nome), NULL);
				cout << nome << "  " << idade << endl;
			}
		}
	} while (false);

	SQLFreeHandle(SQL_HANDLE_STMT, SqlStatementHandle);
	SQLDisconnect(SqlConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SqlConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SqlEventHandle);
	
	getchar();
	std::cout << "Hello World!\n";
}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
