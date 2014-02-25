#ifndef _HG_CERROR
#define _HG_CERROR

#include "Types.h"

namespace my
{

	/** Enumeration for error codes
	*/
	enum tErrorCode
	{
		ecSuccess = 0,
		ecGeneral,
		ecErrorReport

	};

	/** Enumeration for error types
	*/
	enum tErrorType
	{
		etSuccess = 0x00,
		etGeneral = 0x01,
		etMemory = 0x02,

	};

	class CError
	{
	private:
		static CError * spLastErr;

		tErrorCode		mErrCode;
		tErrorType		mErrType;
		CError *		pPrevErr;
		tString 		mMessage;

		CError(tErrorCode errCode, tErrorType errType, const char * errMessage);

		~CError();

	public:

		/** Creates a new CError object and adds it to the errors linked list.
		*
		*	@param errCode		Code representation of the error.
		*	@param errType		Type(s) of the error.
		*	@param errMessage	Brief description of the error.
		*
		*	@return 			Reference to the CError object.
		*
		*	@warning			Should not be used directly! Use the ERROR( code, type, msg ) macro instead!
		*
		*/
		static CError& newError(tErrorCode errCode, tErrorType errType, const char * errMessage);


		/** Prints the errors.
		*
		*	The function prints the errors in the reverse order they occured.
		*	A printed error generally looks like this:
		*	<pre>
		*		`(error code)  [<error type 1>][(error type 2)]...[(error type n)]  (error message)`
		*		`  - on line: (line) in file: (file)`
		*		`  - details: (additional error message)`
		*	</pre>
		*	The last two lines are optional and can be turned off separately.
		*
		*	@todo				Not yet configurable.
		*
		*/
		static void printErrors();

		/** Deletes all error.
		 */
		static int clearErrors();


		/** Cast operator for tErrorCode type
		 *	@return				Code representation of the error.
		 */
		operator tErrorCode() const;


		/** Concatenates the right hand side string to the error message.
		 *
		 *	@param message	Input string to be concatenated.
		 *
		 *	@return 		Reference to the CError object.
		 *
		 */
		CError& operator << ( const char * message );

		//CError& operator << ( const tReal &num );


		/** Concatenates the right hand side int to the error message.
		*
		*	@param num		Integer to be concatenated.
		*
		*	@return 		Reference to the CError object.
		*
		*/
		CError& operator << ( const int &num );

	};

} // namespace: my

/**	Returns error code of success.
 */
#define SUCCESS 							return my::ecSuccess


/**	Defines the additional error message to help locate the occured error.
*/
#define ERROR_LOCATION						<< "\n - on line: " << (int)__LINE__ << " in file: " << __FILE__

/**	Creates a new error.
*/
#define ERROR( errCode, errType, errMsg )	return my::CError::newError( (errCode), (errType), (errMsg) ) ERROR_LOCATION
#define DETAILS( details )					<< "\n  - details: " << details

#define TRACE( traceMsg )					my::CError::newError( my::ecSuccess, my::etSuccess, (traceMsg) ) ERROR_LOCATION

#define ifPassed( func )					if( (func) == my::ecSuccess )
#define ifFailed( func )					if( (func) != my::ecSuccess )

#endif // _HG_CERROR
