/*function createMenu()
{
	generateMenu("treeview", "project", "Monkey studio");
	generateMenuItem ("projectOutline","bugs", "Bugs");
	generateMenuItem ("projectOutline","features", "Features");
	generateMenu ("treeview", "project2","Webissues web interface");
}
*/

function main ()
{
	wi = new Wi ();
	alert ('main');
	if ( !syncXH || !aSyncXH )
	{
		alert ("Can't create XmlHttp objects");
		return;
	}
	process_HELLO ();
	process_WHOAMI ();
	process_LIST_PROJECTS ();
}

function show_folder (folder_id)
{
	process_LIST_ISSUES (folder_id,0);
}