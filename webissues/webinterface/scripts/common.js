function createXmlHttpRequestObject ()
{
    var xmlHttp;
    try
    {
        xmlHttp = new XMLHttpRequest ();
    }
    catch (e)
    {
        var XmlHttpVersions = new Array ("MSXML2.XMLHTTP.6.0",
                                        "MSXML2.XMLHTTP.5.0",
                                        "MSXML2.XMLHTTP.4.0",
                                        "MSXML2.XMLHTTP.3.0",
                                        "MSXML2.XMLHTTP",
                                        "Microsoft.XMLHTTP" );
        for ( var i = 0; i <XmlHttpVersions.length && !xmlHttp; i++)
        {   
            try
            {
                xmlHttp = new ActiveXObject ( XmlHttpVersions[i]);
            }
            catch (e) {}
        }
    }
    return xmlHttp;
}