/****************************** Module Header ******************************\
* Module Name:  CppUseADONET.cpp
* Project:      CppUseADONET
* Copyright (c) Microsoft Corporation.
* 
* The CppUseADONET example demonstrates the Microsoft ADO.NET technology to 
* access databases using Visual C++ in both managed code and unmanaged code. 
* It shows the basic structure of connecting to a data source, issuing SQL 
* commands, using DataSet object and performing the cleanup. 
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/en-us/openness/resources/licenses.aspx#MPL.
* All other rights reserved.
* 
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\****************************************************************************/

#include "stdafx.h"

#include <comdef.h>
#include <gcroot.h>
#include <iostream>
using namespace std;

#using <System.dll>
#using <System.Data.dll>
using namespace System;
using namespace System::Data;
using namespace System::Data::SqlClient;
using namespace System::Runtime::InteropServices;

// Max row number of the DataSet
#define MAXROWS 100

// Managed codes to connect database via ADO.NET
#pragma managed
class DatabaseClass
{	
public:
    DatabaseClass()
	{
		// Create the database connection string
			conStr = "Data Source=DEEPTHOUGHT;Initial Catalog=SQLServer2005DB;Integrated Security=True";
	}

	DatabaseClass(const char* _conStr) 
	{
		conStr = gcnew String(_conStr);
	}
	/*
		gcroot<DataSet ^> ds;
	gcroot<String ^> conStr;
	gcroot<SqlConnection ^> con;
	gcroot<SqlCommand ^> cmd;
	gcroot<SqlCommand ^> resultcmd;
	gcroot<SqlCommand ^> environmentcmd;
	gcroot<SqlCommand ^> environmentselectcmd;
	gcroot<SqlCommand ^> experimentcmd;
	gcroot<SqlCommand ^> experimentselectcmd;
	gcroot<SqlDataAdapter ^> da;
	*/
	~DatabaseClass()
	{
		delete experimentcmd;
		delete experimentselectcmd;
		delete resultcmd;
		delete environmentcmd;
		delete environmentselectcmd;
		delete da;
		delete cmd;
		if (con) delete con;
		delete conStr;
		delete ds;
	}


	/*!
    * \brief
	* Create and open database connection.
	*/
	void CreateConnection()
	{
		if(con) delete con;
		con = gcnew SqlConnection(conStr);
		con->Open();
		PrepareResultCommand();
		PrepareEnvironment();
		PrepareExperiment();
		PrepareSelectEnvironment();
		PrepareSelectExperiment();
		PrepareSelectResult();
	}

	void PrepareResultCommand()
	{
		// 1. Inialize the SqlCommand object.
		cmd = gcnew SqlCommand();

		// 2. Assign the connection to the SqlCommand.
		cmd->Connection = con;

		// 3. Set the SQL command text.
		// SQL statement or the name of the stored procedure.
		cmd->CommandText = "INSERT INTO [Experiment].[dbo].[Results]([experimentid],[measureid],[algorithmid],[environmentid],[iteration],[sample],[value]) "
			+"VALUES(@experimentid,@measureid,@algorithmid,@environmentid,@iteration,@sample,@value)";

		// 4. Set the command type.
		// CommandType::Text for ordinary SQL statements;
		// CommandType::StoredProcedure for stored procedures.
		cmd->CommandType = CommandType::Text;

		// 5. Append the parameters.
		// DBNull::Value for SQL-Nullable fields.
		cmd->Parameters->Add("@experimentid", SqlDbType::Int);
		cmd->Parameters->Add("@measureid", SqlDbType::Int);
		cmd->Parameters->Add("@algorithmid", SqlDbType::Int);
		cmd->Parameters->Add("@environmentid", SqlDbType::Int);
		cmd->Parameters->Add("@iteration", SqlDbType::Int);
		cmd->Parameters->Add("@sample", SqlDbType::Int);
		cmd->Parameters->Add("@value", SqlDbType::Float);

		//Prepare
		cmd->Prepare();
	}

	void AddResultRow( int expId, int measId,  int algId, int envId,  int it, int sample, double value )
	{
		// 5. Append the parameters.
		// DBNull::Value for SQL-Nullable fields.
		cmd->Parameters[0]->Value = expId;
		cmd->Parameters[1]->Value = measId;
		cmd->Parameters[2]->Value = algId;
		cmd->Parameters[3]->Value = envId;
		cmd->Parameters[4]->Value = it;
		cmd->Parameters[5]->Value = sample;
		cmd->Parameters[6]->Value = value;

		// 6. Execute the command.
		cmd->ExecuteNonQuery();
	}

	void PrepareExperiment()
	{
			// 1. Inialize the SqlCommand object.
		experimentcmd = gcnew SqlCommand();

		// 2. Assign the connection to the SqlCommand.
		experimentcmd->Connection = con;

		// 3. Set the SQL command text.
		// SQL statement or the name of the stored procedure.
		experimentcmd->CommandText = "INSERT INTO [Experiment].[dbo].[Experiment]([division],[robots],[maxpath]) VALUES (@division,@robots,@maxpath)";

		// 4. Set the command type.
		// CommandType::Text for ordinary SQL statements;
		// CommandType::StoredProcedure for stored procedures.
		experimentcmd->CommandType = CommandType::Text;

		// 5. Append the parameters.
		// DBNull::Value for SQL-Nullable fields.
		experimentcmd->Parameters->Add("@division", SqlDbType::Float);
		experimentcmd->Parameters->Add("@robots", SqlDbType::Int);
		experimentcmd->Parameters->Add("@maxpath", SqlDbType::Int);

		//Prepare.
		experimentcmd->Prepare();
	}
	
	void PrepareSelectResult()
	{
		resultselectcmd = gcnew SqlCommand();
		resultselectcmd->Connection = con;
		resultselectcmd->CommandText = "SELECT [id] FROM [Experiment].[dbo].[Results] WHERE experimentid = @experimentid and environmentid = @environmentid and algorithmid = @algorithmid and measureId = @measureId;";
		resultselectcmd->CommandType = CommandType::Text;
		resultselectcmd->Parameters->Add("@experimentid", SqlDbType::Int);
		resultselectcmd->Parameters->Add("@environmentid", SqlDbType::Int);
		resultselectcmd->Parameters->Add("@algorithmid", SqlDbType::Int);
		resultselectcmd->Parameters->Add("@measureid", SqlDbType::Int);
		resultselectcmd->Prepare();
	}

	void PrepareSelectExperiment()
	{
		// 1. Inialize the SqlCommand object.
		experimentselectcmd = gcnew SqlCommand();

		// 2. Assign the connection to the SqlCommand.
		experimentselectcmd->Connection = con;

		// 3. Set the SQL command text.
		// SQL statement or the name of the stored procedure.
		experimentselectcmd->CommandText = "SELECT [id] FROM [Experiment].[dbo].[Experiment] WHERE ([division] between @division and @division2) and [robots] = @robots ";

		// 4. Set the command type.
		// CommandType::Text for ordinary SQL statements;
		// CommandType::StoredProcedure for stored procedures.
		experimentselectcmd->CommandType = CommandType::Text;

		// 5. Append the parameters.
		// DBNull::Value for SQL-Nullable fields.
		experimentselectcmd->Parameters->Add("@division", SqlDbType::Float);
		experimentselectcmd->Parameters->Add("@division2", SqlDbType::Float);
		experimentselectcmd->Parameters->Add("@robots", SqlDbType::Int);

		//Prepare.
		experimentselectcmd->Prepare();
	}	

	void PrepareSelectEnvironment()
	{
		// 1. Inialize the SqlCommand object.
		environmentselectcmd = gcnew SqlCommand();

		// 2. Assign the connection to the SqlCommand.
		environmentselectcmd->Connection = con;

		// 3. Set the SQL command text.
		// SQL statement or the name of the stored procedure.
		environmentselectcmd->CommandText = "SELECT [id] FROM [Experiment].[dbo].[Environment] WHERE [size] =@size and [objects] = @objects and [ratio] =@ratio and [type] = @type;";

		// 4. Set the command type.
		// CommandType::Text for ordinary SQL statements;
		// CommandType::StoredProcedure for stored procedures.
		environmentselectcmd->CommandType = CommandType::Text;

		// 5. Append the parameters.
		// DBNull::Value for SQL-Nullable fields.
		environmentselectcmd->Parameters->Add("@size", SqlDbType::Int);
		environmentselectcmd->Parameters->Add("@objects", SqlDbType::Int);
		environmentselectcmd->Parameters->Add("@ratio", SqlDbType::Float);
		environmentselectcmd->Parameters->Add("@type", SqlDbType::Int);

		//Prepare.
		environmentselectcmd->Prepare();
	}	

	void AddExperiment( double division, int robots, int maxpath)
	{
		// 5. Append the parameters.
		experimentcmd->Parameters[0]->Value = division;
		experimentcmd->Parameters[1]->Value = robots;
		experimentcmd->Parameters[2]->Value = maxpath;

		// 6. Execute the command.
		experimentcmd->ExecuteNonQuery();
	}

	void PrepareEnvironment()
	{
			// 1. Inialize the SqlCommand object.
		environmentcmd = gcnew SqlCommand();

		// 2. Assign the connection to the SqlCommand.
		environmentcmd->Connection = con;

		// 3. Set the SQL command text.
		// SQL statement or the name of the stored procedure.
		environmentcmd->CommandText = "INSERT INTO [Experiment].[dbo].[Environment]([size],[objects] ,[ratio],[type]) VALUES (@size , @objects, @ratio, @type)";

		// 4. Set the command type.
		// CommandType::Text for ordinary SQL statements;
		// CommandType::StoredProcedure for stored procedures.
		environmentcmd->CommandType = CommandType::Text;
		
		// 5. Append the parameters.
		// DBNull::Value for SQL-Nullable fields.
		environmentcmd->Parameters->Add("@size", SqlDbType::Int);
		environmentcmd->Parameters->Add("@objects", SqlDbType::Int);
		environmentcmd->Parameters->Add("@ratio", SqlDbType::Float);
		environmentcmd->Parameters->Add("@type", SqlDbType::Int);

		environmentcmd->Prepare();
	}

	void AddEnvironment( int size, int objects, double ratio, int type)
	{
		// 5. Append the parameters.
		// DBNull::Value for SQL-Nullable fields.
		environmentcmd->Parameters[0]->Value = size;
		environmentcmd->Parameters[1]->Value = objects;
		environmentcmd->Parameters[2]->Value = ratio;
		environmentcmd->Parameters[3]->Value = type;
	
		// 6. Execute the command.
		environmentcmd->ExecuteNonQuery();
	}

	int GetResultID( int environmentId, int experimentId, int algorithmid, int measureId)
	{
		// 1. Inialize the DataSet object.
		ds = gcnew DataSet();

		// 3. Inialize the SqlDataAdapter object.
		// SqlDataAdapter represents a set of data commands and a 
        // database connection that are used to fill the DataSet and 
        // update a SQL Server database.
		resultselectcmd->Parameters[0]->Value = experimentId;
		resultselectcmd->Parameters[1]->Value = environmentId;
		resultselectcmd->Parameters[2]->Value = algorithmid;
		resultselectcmd->Parameters[3]->Value = measureId;
	
		da = gcnew SqlDataAdapter(resultselectcmd);
		
		// 4. Fill the DataSet object.
		// Fill the DataTable in DataSet with the rows selected by the SQL 
		// command.
		da->Fill(ds);

		// Create an array of VARIANT to hold the ID column values.
		VARIANT valuesID[1];

		//Get column values
		int len = this->GetValueForColumn(L"id", valuesID, 1);
		if ( len > 0 )
		{
			return (valuesID[0].vt == VT_NULL) ? -1 :  valuesID[0].intVal;
		}
		return -1;
	}

	int GetEnvironmentID( int size, int objects, double ratio, int type)
	{
		// 1. Inialize the DataSet object.
		ds = gcnew DataSet();

		// 3. Inialize the SqlDataAdapter object.
		// SqlDataAdapter represents a set of data commands and a 
        // database connection that are used to fill the DataSet and 
        // update a SQL Server database.
		environmentselectcmd->Parameters[0]->Value = size;
		environmentselectcmd->Parameters[1]->Value = objects;
		environmentselectcmd->Parameters[2]->Value = ratio;
		environmentselectcmd->Parameters[3]->Value = type;
	
		da = gcnew SqlDataAdapter(environmentselectcmd);
		
		// 4. Fill the DataSet object.
		// Fill the DataTable in DataSet with the rows selected by the SQL 
		// command.

		da->Fill(ds);

		// Create an array of VARIANT to hold the ID column values.
		VARIANT valuesID[1];

		//Get column values
		int len = this->GetValueForColumn(L"id", valuesID, 1);
		if ( len > 0 )
		{
			return (valuesID[0].vt == VT_NULL) ? -1 :  valuesID[0].intVal;
		}
		return -1;

		delete ds;
		delete da;
	}

	bool CheckResultExists(int algorithmid, double division, double robots, int size, int objects, double ratio, int type )
	{
		int measureId = 1;
		//Check environment exists
		int environmentId = GetEnvironmentID(size, objects,ratio,type);
		if (environmentId < 0)
		{
			return false;
		}

		//Check experiment exists
		int experimentId = GetExperimentID(division,robots, 30);
		if (experimentId < 0)
		{
			return false;
		}

		//Check result exists
		int resultId = GetResultID(environmentId,experimentId, algorithmid, 1);
		if (resultId < 0)
		{
			return false;
		} else
		{
			return true;
		}
	}

	int GetExperimentID( double division, int robots, int maxpath)
	{
		// 1. Inialize the DataSet object.
		ds = gcnew DataSet();

		// 3. Inialize the SqlDataAdapter object.
		// SqlDataAdapter represents a set of data commands and a 
        // database connection that are used to fill the DataSet and 
        // update a SQL Server database. 
		// 5. Append the parameters.
		experimentselectcmd->Parameters[0]->Value = division;
		experimentselectcmd->Parameters[1]->Value = division + 0.01;
		experimentselectcmd->Parameters[2]->Value = robots;

		da = gcnew SqlDataAdapter(experimentselectcmd);

		// 4. Fill the DataSet object.
		// Fill the DataTable in DataSet with the rows selected by the SQL 
		// command.
		da->Fill(ds);

		// Create an array of VARIANT to hold the ID column values.
		VARIANT valuesID[1];

		//Get column values
		int len = this->GetValueForColumn(L"id", valuesID, 1);
		if ( len > 0 )
		{
			return (valuesID[0].vt == VT_NULL) ? -1 :  valuesID[0].intVal;
		}
		return -1;

		if (ds) delete ds;	
		if (da) delete da;

	}

	/*!
	* \brief
	* Add one new data row to data source using SQL command.
	* 
	* \param lastName
	* Last Name value for data table LastName column.
	* 
	* \param firstName
	* First Name value for data table FirstName column.
	* 
	* \param image
	* SAFEARRAY holding the array of bytes which represents an image
	*/
	void AddRow(wchar_t *lastName, wchar_t *firstName, SAFEARRAY *image)
	{
		// 1. Inialize the SqlCommand object.
		cmd = gcnew SqlCommand();

		// 2. Assign the connection to the SqlCommand.
		cmd->Connection = con;

		// 3. Set the SQL command text.
		// SQL statement or the name of the stored procedure.
		cmd->CommandText = "INSERT INTO Person(LastName, FirstName, " + 
			"HireDate, EnrollmentDate, Picture) VALUES (@LastName, " + 
			"@FirstName, @HireDate, @EnrollmentDate, @Picture)";

		// 4. Set the command type.
		// CommandType::Text for ordinary SQL statements;
		// CommandType::StoredProcedure for stored procedures.
		cmd->CommandType = CommandType::Text;

		// 5. Append the parameters.
		// DBNull::Value for SQL-Nullable fields.
		cmd->Parameters->Add("@LastName", SqlDbType::NVarChar, 50)->Value = 
			Marshal::PtrToStringUni((IntPtr)lastName);
		cmd->Parameters->Add("@FirstName", SqlDbType::NVarChar, 50)->Value = 
			Marshal::PtrToStringUni((IntPtr)firstName);
		cmd->Parameters->Add("@HireDate", SqlDbType::DateTime)->Value = 
			DBNull::Value;
		cmd->Parameters->Add("@EnrollmentDate", SqlDbType::DateTime)->Value
			= DateTime::Now;
		if (image == NULL)
		{
			cmd->Parameters->Add("@Picture", SqlDbType::Image)->Value = 
				DBNull::Value;
		}
		else
		{
			// Convert the SAFEARRAY to an array of bytes.
			int len = image->rgsabound[0].cElements;
			array<byte> ^arr = gcnew array<byte>(len);
			int *pData;
			SafeArrayAccessData(image, (void **)&pData);
			Marshal::Copy(IntPtr(pData), arr, 0, len);
			SafeArrayUnaccessData(image);
			cmd->Parameters->Add("@Picture", SqlDbType::Image)->Value = arr;
		}

		// 6. Execute the command.
		cmd->ExecuteNonQuery();
	}

	/*!
	* \brief
	* Inialize the DataSet object and the SqlDataAdapter object, 
	* then fill DataSet object with the query data table.
	* 
	* \param command
	* SQL command to query the data.
	*/
	void FillDataSet(wchar_t *command)
	{
		// 1. Inialize the DataSet object.
		ds = gcnew DataSet();

		// 2. Create a SELECT SQL command.
		String ^ strSelectCmd = Marshal::PtrToStringUni((IntPtr)command);
		
		// 3. Inialize the SqlDataAdapter object.
		// SqlDataAdapter represents a set of data commands and a 
        // database connection that are used to fill the DataSet and 
        // update a SQL Server database. 
		da = gcnew SqlDataAdapter(strSelectCmd, con);

		// 4. Fill the DataSet object.
		// Fill the DataTable in DataSet with the rows selected by the SQL 
		// command.
		da->Fill(ds);

	}

	/*!
	* \brief
	* Retrieve data under one DataTable column into an array of VARIANT
	* 
	* \param dataColumn
	* The DataTable column name.
	*
	* \param values
	* The array of VARIANT holding the query results.
	*
	* \param valuesLength
	* The max row number of the query results.
	*
	* \returns
	* The row number of the query results.
	*/
	int GetValueForColumn(wchar_t *dataColumn, VARIANT *values, 
		int valuesLength)
	{
		// Convert the column name to managed String object.
		String ^columnStr = Marshal::PtrToStringUni((IntPtr)dataColumn);
		
		// Create an array of DataRow which holds all the DataTable rows.
		array<DataRow ^> ^rows = ds->Tables[0]->Select();

		// Get the row number.
		int len = rows->Length;
		len = (len > valuesLength) ? valuesLength : len;

		// Copy each row of data under the column to the array of VARIANT.
		for (int i = 0; i < len; i++)
		{
			Marshal::GetNativeVariantForObject(rows[i][columnStr], 
				IntPtr(&values[i]));
		}

		return len;
	}

	/*!
	* \brief
	* Close the database connection.
	*/
	void CloseConnection()
	{
		con->Close();
		delete con;
	}

private:
    // Using gcroot, you can use a managed type in
    // a native class.
	gcroot<DataSet ^> ds;
	gcroot<String ^> conStr;
	gcroot<SqlConnection ^> con;
	gcroot<SqlCommand ^> cmd;
	gcroot<SqlCommand ^> resultcmd;
	gcroot<SqlCommand ^> environmentcmd;
	gcroot<SqlCommand ^> environmentselectcmd;
	gcroot<SqlCommand ^> experimentcmd;
	gcroot<SqlCommand ^> experimentselectcmd;
	gcroot<SqlCommand ^> resultselectcmd;
	gcroot<SqlDataAdapter ^> da;
};

// Unmanaged codes
#pragma unmanaged
DWORD ReadImage(PCTSTR pszImage, SAFEARRAY FAR **psaChunk);

/*int _tmain(int argc, _TCHAR* argv[])
{
	DatabaseClass *db = NULL;

	try
	{
		/////////////////////////////////////////////////////////////////////
		// Connect to the data source.
		// 

		_tprintf("Connecting to the database...\n");

		// Initialize the managed DatabaseClass object.
		db = new DatabaseClass();

		// Create and open the database connection.
		// (The data source is created in the sample SQLServer2005DB)
		db->CreateConnection();


		/////////////////////////////////////////////////////////////////////
		// Build and Execute and ADO.NET Command.
		// It can be a SQL statement (SELECT/UPDATE/INSERT/DELETE), or a
		// stored procedure call.  Here is the sample of an INSERT command.
		//

		_tprintf("Inserting a record to the Person table.\n");

		// Create a SAFEARRAY to hold the array of bytes.
        SAFEARRAY FAR *psaChunk = NULL;

		// Read the image file into an array of bytes.
		int cbChunkBytes = ReadImage(_T("MSDN.jpg"), &psaChunk);

		// Set the SAFEARRAY to NULL if the image loading fails.  
		if (cbChunkBytes == 0)
		{
			psaChunk = NULL;
		}

		// Add one row of new data to the data source.
		db->AddRow(L"Sun", L"Lingzhi", psaChunk);


		/////////////////////////////////////////////////////////////////////
		// Use the DataSet Object.
        // The DataSet, which is an in-memory cache of data retrieved from a 
		// data source, is a major component of the ADO.NET architecture. The 
		// DataSet consists of a collection of DataTable objects that you can 
		// relate to each other with DataRelation objects.
        // 

		_tprintf("Using DataSet to store the data query result.\n");

		// Create a SELECT SQL command.
		wchar_t *strSelectedCmd = L"SELECT * FROM Person";

		// Fill the DataTable in the DataSet with the rows selected by the
		// SQL command.
		db->FillDataSet(strSelectedCmd);

		// Create an array of VARIANT to hold the PersonID column values.
		VARIANT valuesID[MAXROWS];

		// Fill the array of VARIANT with the PersonID column values.
		int len = db->GetValueForColumn(L"PersonID", valuesID, MAXROWS);

		// Create an array of VARIANT to hold the LastName column values.
		VARIANT valuesLastName[MAXROWS];

		// Fill the array of VARIANT with the PersonID column values. 
		db->GetValueForColumn(L"LastName", valuesLastName, MAXROWS);

		// Create an array of VARIANT to hold the FirstName column values.
		VARIANT valuesFirstName[MAXROWS];

		// Fill the array of VARIANT with the FirstName column values. 
		db->GetValueForColumn(L"FirstName", valuesFirstName, MAXROWS);
		
		// Display the values in the three VARIANT arrays.
		for (int i = 0; i < len; i++)
		{
			// When dumping a SQL-Nullable field, you need to test it for
			// VT_NULL.
			_tprintf("%d\t%s %s\n", valuesID[i].intVal, 
				(valuesFirstName[i].vt == VT_NULL) ? L"(DBNULL)" : valuesFirstName[i].bstrVal,
				(valuesLastName[i].vt == VT_NULL) ? L"(DBNULL)" : valuesLastName[i].bstrVal);
		}
	}
	catch (_com_error &err)
	{
		_tprintf(_T("The application throws the error: %s\n"), 
			err.ErrorMessage());
		_tprintf(_T("Description = %s\n"), (LPCTSTR) err.Description());
	}


	/////////////////////////////////////////////////////////////////////////
	// Clean up objects before exit.
	//

	// Close the database connection.
	db->CloseConnection();

	// Delete the managed DatabaseClass object.
    delete db;

	return 0;
}*/

/*!
 * \brief
 * Read an image file to a safe array of bytes.
 * 
 * \param pszImage
 * The path of the image file.
 * 
 * \param ppsaChunk
 * The output of the safe array.
 * 
 * \returns
 * The number of bytes read from the image file. 0 means failure.
 */