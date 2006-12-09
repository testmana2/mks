#ifndef MONKEY_H#define MONKEY_H#include <string>#include <vector>/**  * class Monkey  * This class herits from QApplication and contain all needed pointers for shared  * objects  */class Monkey{public:	// Constructors/Destructors	//  	/**	 * Empty Constructor	 */	Monkey ( );	/**	 * Empty Destructor	 */	virtual ~Monkey ( );	/**	 * @return Settings*	 */	Settings* settings ( );	/**	 * @return UISplashScreen*	 */	UISplashScreen* splashScreen ( );	/**	 * @return UIMain*	 */	UIMain* main ( );	/**	 * @return MenuTools*	 */	MenuTools* tools ( );protected:private:};#endif // MONKEY_H