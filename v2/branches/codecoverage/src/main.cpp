#include "Monkey.h"
#include "UITestReport.h"
//
int main( int argc, char** argv )
{
#ifdef __COVERAGESCANNER__
    // Installation of the default code coverage handler 
    // The code coverage filename report produced is:
    //     <monkeystudio_exepath>.csexe
    __coveragescanner_install(argv[0]);
    UITestReport::setTestTitle(QString());
#endif
	Monkey* a = Monkey::self( argc, argv );
	int a_ret = a->exec();
#ifdef __COVERAGESCANNER__
    UITestReport::self()->exec();
#endif
    return a_ret;
}
