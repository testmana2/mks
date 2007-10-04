function createMenu()
{
	generateMenu("treeview", "project", "Monkey studio");
	generateMenuItem ("projectOutline","bugs", "Bugs");
	generateMenuItem ("projectOutline","features", "Features");
	generateMenu ("treeview", "project2","Webissues web interface");
}

function main ()
{
	if ( !syncXH || !aSyncXH )
	{
		alert ("Can't create XmlHttp objects");
		return;
	}
	//createMenu();
 	resp = processRequest ("HELLO");
 	document.getElementById ("serverName").innerHTML = resp.getElementsByTagName("server_name")[0].firstChild.data;
	listProjects ("treeview");
}

function listProjects ( where)
{
	resp = processRequest ("LIST PROJECTS");
	names = resp.getElementsByTagName ("project_name");
	ids = resp.getElementsByTagName ("project_id");
	for (i = 0; i< names.length; i++)
		generateMenu (where, "project_"+ids[i],names[i].firstChild.data);
}