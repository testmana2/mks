/*
This menu script downloaded from
http://www.cerebuswebmaster.com/develop/scripts/javascript/folder-menu/
and was modified for site
*/
var Open = '';
var Closed = '';

function preload()
{
	if(document.images)
	{
		Open = new Image(16,13);   
		Closed = new Image(16,13);
		Open.src = 'img/open.png';
		Closed.src = 'img/closed.png';
	}
}

function showhide (what) {
if (document.getElementById(what + 'Outline').style.display=='none'){
    document.getElementById(what + 'Sign').src=Open.src;
    document.getElementById(what + 'Outline').style.display='';
}
else {
    document.getElementById(what + 'Sign').src=Closed.src;
    document.getElementById(what + 'Outline').style.display='none';
}

}

function oops(data)
{
    alert ('oops'+data.id+data.parentElement.id);
}


function generateMenu (where, menu_id, menu_text)
{
html = '<div id="'+menu_id+'" onClick="showhide(this.id)" class="treeview-toplevel">';
html +=	'<img id="'+menu_id+'Sign" src="img/closed.png">';
html +=	'<img src="img/project-16.png"> ';
html += menu_text;
html += '</div>';
html += '<div id="'+menu_id+'Outline" class="treeview-item" style="display:none"> '
html += '</div>';
document.getElementById(where).innerHTML += html;
}

function generateMenuItem (where, item_id, itemtext, on_click)
{
	html = '<div id="'+item_id+'" onclick='+on_click+'">'
	html +=	'<img src="img/folder-16.png"> ';
	html +=itemtext;
	html +='</div>';
	document.getElementById (where+"Outline").innerHTML += html;
}
document.onload=preload();