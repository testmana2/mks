#include "Monkey.h"
#include "UITestReport.h"
//
int main( int argc, char** argv )
{
	Monkey* a = Monkey::self( argc, argv );
	int i = a->exec();
#ifdef __COVERAGESCANNER__
	UITestReport::self()->exec();
#endif
	delete a;
	return i;
}
