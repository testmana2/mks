/*function createMenu()
{
	generateMenu("treeview", "project", "Monkey studio");
	generateMenuItem ("projectOutline","bugs", "Bugs");
	generateMenuItem ("projectOutline","features", "Features");
	generateMenu ("treeview", "project2","Webissues web interface");
}
*/

function initWi ()
{
	Wi = new Object(); //Webissues global
}
function main ()
{
	alert ('main');
	if ( !syncXH || !aSyncXH )
	{
		alert ("Can't create XmlHttp objects");
		return;
	}
	//createMenu();
 	resp = processRequest ("HELLO");
 	document.getElementById ("serverName").innerHTML = resp.getElementsByTagName("server_name")[0].firstChild.data;
 	resp = processRequest ("WHOAMI");
 	document.getElementById ("userName").innerHTML = "Logined as "+resp.getElementsByTagName("user_name")[0].firstChild.data;	
	listProjects ("treeview");
}

function listProjects ( where)
{
	initWi();
	resp = processRequest ("LIST PROJECTS");
	names = resp.getElementsByTagName ("project_name");
	ids = resp.getElementsByTagName ("project_id");
	Wi.projects = new Array (names.length);
	for (i = 0; i < names.length; i++)
	{
		Wi.projects[i] = new Object();
		Wi.projects[i].name = names[i].firstChild.data;
		Wi.projects[i].id = ids[i].firstChild.data;
	}
	for (i = 0; i< Wi.projects.length; i++)
		generateMenu (where, "project_"+Wi.projects[i].id,Wi.projects[i].name);
}