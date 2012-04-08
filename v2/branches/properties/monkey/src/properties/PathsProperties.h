#ifndef PATHSPROPERTIES_H
#define PATHSPROPERTIES_H

#include <XmlObject.h>

class PathsProperties : public XmlObject
{
public:
    enum StorageLocation
    {
        SL_TEMPLATES,
        SL_TRANSLATIONS,
        SL_PLUGINS,
        SL_SCRIPTS,
        SL_APIS
    };
    
    PathsProperties();
    
    QStringList paths( PathsProperties::StorageLocation location ) const;
    void setPaths( PathsProperties::StorageLocation location, const QStringList& paths );
    
    static QString storageLocationToString( PathsProperties::StorageLocation location );

protected:
    QVariantMap mPaths; // string, stringlist

    virtual void processData( XmlObjectProcessor* container );
};

#endif // PATHSPROPERTIES_H
