#include "Types.h"
#include "CError.h"

using namespace my;

int main()
{
	CError::newError(ecGeneral, etGeneral, "E1") << "A1" << "A2-" << 3 << "-A3";
	CError::newError(ecGeneral, etGeneral, "E2") DETAILS("A1" << "A2-" << 3 << "-A3");
	TRACE("Trace1");
	TRACE("Trace2") DETAILS("Integer: " << 7 << " - OK");

	CError::printErrors();
	CError::clearErrors();

	return 0;
}
