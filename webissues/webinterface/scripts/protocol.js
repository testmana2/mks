var serverAddress = "http://localhost/server/index.php";
var commandPrefix = serverAddress+"?command=";
var tryingInterral
var syncXH = createXmlHttpRequestObject ();  // for synchronous queryes
var aSyncXH = createXmlHttpRequestObject ();  // for periodical queryes

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

function processRequest (command)
{
    if (syncXH.readyState != 0 && syncXH.readyState != 4)
    {
        alert ("Script error. Not right XmlHttp request state");
        return;
	}
	syncXH.open ("GET", commandPrefix+command, false);
	syncXH.send (null);
	if ( syncXH.readyState != 4 )
	{	
		alert ("Request to server failed");
		return;
	}
	if ( syncXH.status != 200 )
	{
		alert ("Request to server failed. Status:"+syncXH.status+ " "+syncXH.statusText);
		return;
	}
  	var xmlRoot = syncXH.responseXML.documentElement;
  	if (!xmlRoot)
  	{
		alert ("Not right sever response");
		return;
	}
	alert ( syncXH.responseText );
	return xmlRoot;
}
