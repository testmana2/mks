#ifndef ABSTRACTCLASSVIEWER_H#define ABSTRACTCLASSVIEWER_H#include <string>#include <vector>/******************************* Abstract Class ****************************AbstractClassViewer does not have any pure virtual methods, but its author  defined it as an abstract class, so you should not use it directly.  Inherit from it instead and create only objects from the derived classes*****************************************************************************/class AbstractClassViewer{public:	// Constructors/Destructors	//  	/**	 * Empty Constructor	 */	AbstractClassViewer ( );	/**	 * Empty Destructor	 */	virtual ~AbstractClassViewer ( );protected:private:};#endif // ABSTRACTCLASSVIEWER_H