function dateToString(value) //date in seconds sinse 1 jan 1970
{
	date = new Date(parseInt(value)*1000);
	return date.getFullYear()+'-'+date.getMonth()+'-'+date.getDate()+' '+date.getHours()+':'+date.getMinutes();
}

function generateIssue (where, issue_id, issue_name, stamp_id, created, created_by, modified, modified_by, status, severity)
{
html = '<div id="'+'issue_'+issue_id+'"'+' class="issue">';
html +='<span class="issue_id">'+issue_id+'</span>';
html +='<span class="issue_name">'+issue_name+'</span>';
html +='<span class="issue_created">'+dateToString(created)+'</span>';
html +='<span class="issue_created_by">'+created_by+'</span>';
html +='<span class="issue_modified">'+dateToString(modified)+'</span>';
html +='<span class="issue_modified_by">'+modified_by+'</span>';
html +='<span class="issue_status">'+status+'</span>';
html +='<span class="issue_severity">'+severity+'</span>';
html += '</div>';
document.getElementById(where).innerHTML += html;
}