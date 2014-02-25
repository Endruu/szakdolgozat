#include "CError.h"
#include <iostream>

using namespace my;

CError * CError::spLastErr = NULL;


// ----------------------------------------------------------------------------------------------------------------------------------------------------

CError::CError(tErrorCode errCode, tErrorType errType, const char * errMessage)
{
	mErrCode = errCode;
	mErrType = errType;
	mMessage = errMessage;

	pPrevErr = spLastErr;
	spLastErr = this;
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------

CError::~CError()
{
	spLastErr = pPrevErr;
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------

CError& CError::newError(tErrorCode errCode, tErrorType errType, const char * errMessage)
{
	CError * err = new ( std::nothrow ) CError(errCode, errType, errMessage);

	if( err == NULL )
	{
		spLastErr->mErrCode = ecErrorReport;
		spLastErr->mErrType = etMemory;
	}

	return *err;
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------

void CError::printErrors()
{
	int num = 1;
	CError * actErr = spLastErr;

	while( actErr != NULL )
	{
		std::cout << num++ << ":\t";

		if( actErr->mErrType == etSuccess )	std::cout << "[OK]";
		if( actErr->mErrType & etGeneral )	std::cout << "[GEN]";
		if( actErr->mErrType & etMemory )	std::cout << "[MEM]";

		std::cout << "\t" << actErr->mMessage << std::endl << std::endl;

		actErr = actErr->pPrevErr;
	}
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------

int CError::clearErrors()
{
	int num = 0;

	while( spLastErr != NULL )
	{
		num++;
		delete spLastErr;
	}

	return num;
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------

CError::operator tErrorCode() const
{
	return mErrCode;
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------

CError& CError::operator << ( const char * message )
{
	mMessage += message;
	return *this;
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------

CError& CError::operator << ( const int &num )
{
	char message[20];
	sprintf(message, "%d", num);	//TODO: hibakezelés
	mMessage += message;
	return *this;
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------
