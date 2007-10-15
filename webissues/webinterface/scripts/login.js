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

function onUnregSubmit ()
{
	uname = document.getElementById("uname");
	if ( uname.value == "" )
	{
		alert ("Enter please your name");
		uname.focus();
		return false
	}
}

function onRegisteredSubmit()
{
	document.getElementById ('messageLabel').innerHTML = "Logining...";
	login = document.getElementById ("login").value;
	password = document.getElementById ("password").value;
	xmlRoot = processRequest ("LOGIN"+" '"+login+"' '"+password+"'");
  	var html = '';
	errElement = xmlRoot.getElementsByTagName ("ERROR")[0];
  	if ( errElement )
	{
		errElement = xmlRoot.childNodes[0];
		codeElement = xmlRoot.getElementsByTagName("code")[0];
  		textElement = xmlRoot.getElementsByTagName("text")[0];
  		html = "ERROR "+codeElement.item(0).data + 
             ", " + textElement.item(0).data;
		document.getElementById ('messageLabel').innerHTML = html;
		return false;
	}
	return true;
}

function handleServerResponse ()
{
	if ( xmlHttp.readyState != 4 )
	{	
		return;
	}
	if ( xmlHttp.status != 200 )
	{
		alert ("Request to server failed");
		return;
	}
  	var xmlRoot = xmlHttp.responseXML.documentElement;
  	errElement = xmlRoot.childNodes[0];
  	var html = '';
  	if ( xmlRoot.firstChild.tagName == 'ERROR')
	{
		errElement = xmlRoot.childNodes[0];
		codeElement = errElement.getElementsByTagName("code")[0];
  		textElement = errElement.getElementsByTagName("text")[0];
  		html = "ERROR "+codeElement.item(0).data + 
             ", " + textElement.item(0).data;
	}
  	document.getElementById ('messageLabel').innerHTML = html;


}