var serverAddress = "http://127.0.0.1/server/index.php";
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

function process_HELLO ()
{
 	resp = processRequest ("HELLO");
	
 	wi.setServerName (resp.getElementsByTagName("server_name")[0].firstChild.data)
}

function process_WHOAMI ()
{
 	resp = processRequest ("WHOAMI");
	wi.setUserName (resp.getElementsByTagName("user_name")[0].firstChild.data)
}

function process_LIST_PROJECTS ()
{
	resp = processRequest ("LIST PROJECTS");
	projects = resp.getElementsByTagName ('PROJECT');
	for (i = 0; i < projects.length; i++)
		wi.addProject( new Project(
						projects[i].childNodes.item(1).firstChild.data, //id
						projects[i].childNodes.item(3).firstChild.data) //name
					 );
	folders = resp.getElementsByTagName ('FOLDER');
	for (i = 0; i < folders.length; i++)
		wi.addFolder (new Folder(
						folders[i].childNodes.item(1).firstChild.data, //folder_id
						folders[i].childNodes.item(3).firstChild.data, //project_id
						folders[i].childNodes.item(5).firstChild.data, //folder_name
						folders[i].childNodes.item(7).firstChild.data, //type_id
						folders[i].childNodes.item(9).firstChild.data) //stamp_id
					 );
}

function process_LIST_ISSUES (folder_id,stamp_id)
{
	resp = processRequest ("LIST ISSUES "+folder_id+" "+stamp_id);
	issueXML = resp.getElementsByTagName ("ISSUE");
	newIssues = new Object();
	for (i = 0; i < issueXML.length; i++)
	{
		ni = new Issue(	issueXML[i].childNodes.item(1).firstChild.data, //issue_id
						issueXML[i].childNodes.item(3).firstChild.data, //issue_name
						issueXML[i].childNodes.item(5).firstChild.data, //stamp_id
						issueXML[i].childNodes.item(7).firstChild.data, //created
						issueXML[i].childNodes.item(9).firstChild.data, //created_by
						issueXML[i].childNodes.item(11).firstChild.data, //modified
						issueXML[i].childNodes.item(13).firstChild.data, //modified_by
						0, //status
						0  //sevetity
					);
		newIssues[issueXML[i].childNodes.item(1).firstChild.data] = ni;
	}
	issueValues = resp.getElementsByTagName ("ISSUE_VALUE");
	for (i = 0; i < issueValues.length; i++)
	{
		if ( issueValues[i].childNodes.item(3).firstChild.data == 1) //attr_id
			newIssues[issueValues[i].childNodes.item(1).firstChild.data].status = //issue_id
				issueValues[i].childNodes.item(5).firstChild.data //attr_value
		if ( issueValues[i].childNodes.item(3).firstChild.data == 1) //attr_id
			newIssues[issueValues[i].childNodes.item(1).firstChild.data].severity = //issue_id
				issueValues[i].childNodes.item(5).firstChild.data //attr_value
		while (newIssues.length) //not empty
		{
			is = newIssues.pop()
			wi.addIssue (is);
		}
	}
}